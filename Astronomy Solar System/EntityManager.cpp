#include "EntityManager.h"
#include "Engine.h"
#include "utils.h"

#include <map>
#include <utility>

static std::map<Mesh *, std::vector<Entity*>> entities;
static std::map<Mesh *, int> numEntitiesToRender;
static std::map<Mesh *, unsigned int> vbos;

static bool bTickEntities = false;

EntityManager::EntityManager(Loader * loader, Matrix4f * projection){
	this->projectionMatrix = projection;
	this->loader = loader;
}

EntityManager::~EntityManager(){
	for (std::map<Mesh*, std::vector<Entity*>>::iterator it = entities.begin(); it != entities.end(); ++it) {
		for (Entity * ent : it->second) {
			delete ent;
		}
	}
}

std::vector<Entity*> EntityManager::getAllEntities(){
	std::vector<Entity*> entVector;
	for (std::map<Mesh*, std::vector<Entity*>>::iterator it = entities.begin(); it != entities.end(); ++it) {
		for (Entity * ent : it->second) {
			entVector.push_back(ent);
		}
	}
	return entVector;
}

std::map<Mesh*, std::vector<Entity*>> EntityManager::getEntityHashmap(){
	return entities;
}

void EntityManager::addEntity(Entity * ent) {
	std::map<Mesh*, std::vector<Entity*>>::iterator i = entities.find(ent->getModel());
	if (i != entities.end() && i->second.size() < MAX_INSTANCES) {
		i->second.push_back(ent);
	}else {
		std::vector<Entity*> entList;
		entList.push_back(ent);
		entities.insert(std::make_pair(ent->getModel(), entList));
		numEntitiesToRender.insert(std::make_pair(ent->getModel(), 1));
		unsigned int vbo = loader->createEmptyVBO(MAX_INSTANCES * DATA_LENGTH);
		vbos.insert(std::make_pair(ent->getModel(), vbo));
		loader->addInstancedAttribute(ent->getModel()->GetVaoId(), vbo, 4, 4, DATA_LENGTH, 0);
		loader->addInstancedAttribute(ent->getModel()->GetVaoId(), vbo, 5, 4, DATA_LENGTH, 4);
		loader->addInstancedAttribute(ent->getModel()->GetVaoId(), vbo, 6, 4, DATA_LENGTH, 8);
		loader->addInstancedAttribute(ent->getModel()->GetVaoId(), vbo, 7, 4, DATA_LENGTH, 12);
	}
}

void EntityManager::removeEntity(Entity * ent){
	std::map<Mesh*, std::vector<Entity*>>::iterator it = entities.find(ent->getModel());
	for (std::vector<Entity*>::iterator entIt = it->second.begin(); entIt != it->second.end(); ++entIt) {
		if (*entIt == ent) {
			std::cout << *entIt << std::endl;
			it->second.erase(entIt);
			//if (it->second.size() == 0)
				//entities.erase(it);

			return;
		}
	}
}

Entity * EntityManager::createEntity(Mesh * mesh, unsigned int texID, Vector3f pos, Vector3f rot, float scale) {
	Entity * e = new Entity(mesh, pos, rot, scale);
	e->setTextureID(texID);
	addEntity(e);
	return e;
}

void EntityManager::tickEntities(Matrix4f projectionMatrix, Camera * cam, float deltaSec){
	int vboIndex = 0;
	
	for (std::map<Mesh*, std::vector<Entity*>>::iterator it = entities.begin(); it != entities.end(); ++it) {
		std::vector<int> entitiesToRender;
		for (int i = 0; i < it->second.size(); i++) {
			bool render = checkEntityIsInFrustum(cam, it->second.at(i));
			if (render) {
				entitiesToRender.push_back(i);
			}
		}
		std::map<Mesh*, int>::iterator entIterator = numEntitiesToRender.find(it->first);
		entIterator->second = entitiesToRender.size();

		int dataSize = DATA_LENGTH * entitiesToRender.size();
		float * data = new float[dataSize];
		int j = 0;
		for (int i : entitiesToRender) {
			Entity * e = it->second.at(i);
			Matrix4f * mat;
			
			if (!ClassUtils::instanceof<Planet>(e))
				mat = getModelMatrix(e);
			else
				mat = ((Planet *)e)->getModelMatrix();
			
			data[(DATA_LENGTH * j)] = mat->matrix[0][0];
			data[(DATA_LENGTH * j) + 1] = mat->matrix[0][1];
			data[(DATA_LENGTH * j) + 2] = mat->matrix[0][2];
			data[(DATA_LENGTH * j) + 3] = mat->matrix[0][3];
			data[(DATA_LENGTH * j) + 4] = mat->matrix[1][0];
			data[(DATA_LENGTH * j) + 5] = mat->matrix[1][1];
			data[(DATA_LENGTH * j) + 6] = mat->matrix[1][2];
			data[(DATA_LENGTH * j) + 7] = mat->matrix[1][3];
			data[(DATA_LENGTH * j) + 8] = mat->matrix[2][0];
			data[(DATA_LENGTH * j) + 9] = mat->matrix[2][1];
			data[(DATA_LENGTH * j) + 10] = mat->matrix[2][2];
			data[(DATA_LENGTH * j) + 11] = mat->matrix[2][3];
			data[(DATA_LENGTH * j) + 12] = mat->matrix[3][0];
			data[(DATA_LENGTH * j) + 13] = mat->matrix[3][1];
			data[(DATA_LENGTH * j) + 14] = mat->matrix[3][2];
			data[(DATA_LENGTH * j) + 15] = mat->matrix[3][3];
			delete mat;
			j++;
		}
		glBindBuffer(GL_ARRAY_BUFFER, vbos.find(it->first)->second);
		glBufferData(GL_ARRAY_BUFFER, MAX_INSTANCES * DATA_LENGTH * sizeof(float), NULL, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, dataSize * sizeof(float), data);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		delete[] data;
		vboIndex++;
		
		for (Entity * ent : it->second) {
			if (bTickEntities && ent->isAffectedByGravity()) {
				Vector3f pos = ent->getPosition();
				double force = ((6.67e-11 * 1.99e+30) / pow(ent->getPosition().length() * 1.5e+10, 2)) * pow(86400 * 365.25 * Engine::getYearsPerSecond(), 2) / 1.5e+10; //acceleration in au/year^2
				ent->acceleration = (-1.0 * force) * Vector3f(pos.x, pos.y, pos.z).Normalize(); //direction of acceleration
				
				Entity * closest = getClosestEntity(ent);
				if (closest != nullptr) {
					double entForce = ((6.67e-11 * closest->mass) / pow((closest->getPosition() - ent->getPosition()).length() * 1.5e+10, 2) * pow(86400 * 365.25 * Engine::getYearsPerSecond(), 2) / 1.5e+10);
					ent->acceleration = ent->acceleration + entForce * (closest->getPosition() - ent->getPosition()).Normalize();
				}
				
				ent->velocity = ent->velocity + deltaSec * ent->acceleration; //velocity in au/year
				ent->setPosition(ent->getPosition() + (deltaSec * ent->velocity));

				if (ClassUtils::instanceof<Planet>(ent)) {
					((Planet *)ent)->tick(deltaSec);
					((Planet *)ent)->trail->tick();
					
				}
			}
			
			if (ClassUtils::instanceof<Planet>(ent) && Engine::RenderText()) {
				bool flag = checkPointIsInFrustum(cam, ent->getPosition());

				if (flag) {
					Vector2f screenPos = MathUtils::Calculate2DScreenPosition(projectionMatrix, ent->getPosition(), cam);
					((Planet *)ent)->text->setScreenPosition(Vector2f(screenPos.x * Display::getWidth(), screenPos.y * Display::getHeight()));
					((Planet *)ent)->text->isOnScreen = true;
				}else
					((Planet *)ent)->text->isOnScreen = false;
			}
		}
	}
}

Entity * EntityManager::getClosestEntity(Entity * ent){
	float shortestDist = std::numeric_limits<float>::max();
	Entity * closest = nullptr;
	for (Entity * entity : getAllEntities()) {
		if (entity != ent) {
			Vector3f distanceVec = ent->getPosition() - entity->getPosition();
			if (distanceVec.length() < shortestDist) {
				closest = entity;
				shortestDist = distanceVec.length();
			}
		}
	}
	return closest;
}

int EntityManager::getNumEntitiesToRender(Mesh * model){
	return numEntitiesToRender.find(model)->second;
}

bool EntityManager::checkEntityIsInFrustum(Camera * cam, Entity * e) {
	if (e == nullptr)
		return false;
	if ((cam->getPosition() - e->getPosition()).length() > e->maxDist && e->maxDist != 0.0f)
		return false;

	Matrix4f * viewMatrix = cam->createViewMatrix();
	Matrix4f * modelMatrix = getModelMatrix(e);
	Matrix4f mvpm = *viewMatrix * *projectionMatrix;
	
	Vector4f leftPlane = Vector4f(mvpm.matrix[0][0] + mvpm.matrix[0][3], mvpm.matrix[1][0] + mvpm.matrix[1][3], mvpm.matrix[2][0] + mvpm.matrix[2][3], mvpm.matrix[3][0] + mvpm.matrix[3][3]).NormalizePlane();
	Vector4f rightPlane = Vector4f(-mvpm.matrix[0][0] + mvpm.matrix[0][3], -mvpm.matrix[1][0] + mvpm.matrix[1][3], -mvpm.matrix[2][0] + mvpm.matrix[2][3], -mvpm.matrix[3][0] + mvpm.matrix[3][3]).NormalizePlane();
	Vector4f bottomPlane = Vector4f(mvpm.matrix[0][1] + mvpm.matrix[0][3], mvpm.matrix[1][1] + mvpm.matrix[1][3], mvpm.matrix[2][1] + mvpm.matrix[2][3], mvpm.matrix[3][1] + mvpm.matrix[3][3]).NormalizePlane();
	Vector4f topPlane = Vector4f(-mvpm.matrix[0][1] + mvpm.matrix[0][3], -mvpm.matrix[1][1] + mvpm.matrix[1][3], -mvpm.matrix[2][1] + mvpm.matrix[2][3], -mvpm.matrix[3][1] + mvpm.matrix[3][3]).NormalizePlane();
	Vector4f nearPlane = Vector4f(mvpm.matrix[0][2] + mvpm.matrix[0][3], mvpm.matrix[1][2] + mvpm.matrix[1][3], mvpm.matrix[2][2] + mvpm.matrix[2][3], mvpm.matrix[3][2] + mvpm.matrix[3][3]).NormalizePlane();
	Vector4f farPlane = Vector4f(-mvpm.matrix[0][2] + mvpm.matrix[0][3], -mvpm.matrix[1][2] + mvpm.matrix[1][3], -mvpm.matrix[2][2] + mvpm.matrix[2][3], -mvpm.matrix[3][2] + mvpm.matrix[3][3]).NormalizePlane();
	
	Vector3f center = *modelMatrix * (e->getModel()->getCenter());
	float radius = e->getModel()->getRadius() * e->getScale();
	
	//clean up
	delete modelMatrix;
	delete viewMatrix;

	Vector4f planes[] = { nearPlane, farPlane, leftPlane, rightPlane, bottomPlane, topPlane };
	
	for (Vector4f plane : planes) {
		float dist = Vector4f::Dot(Vector4f(center.x, center.y, center.z, 1.0f), plane);
		if (dist < -radius) {
			return false;
		}
	}
	
	return true;
}

bool EntityManager::checkPointIsInFrustum(Camera * cam, Vector3f point){

	Matrix4f * viewMatrix = cam->createViewMatrix();
	Matrix4f mvpm = *viewMatrix * *projectionMatrix;

	Vector4f leftPlane = Vector4f(mvpm.matrix[0][0] + mvpm.matrix[0][3], mvpm.matrix[1][0] + mvpm.matrix[1][3], mvpm.matrix[2][0] + mvpm.matrix[2][3], mvpm.matrix[3][0] + mvpm.matrix[3][3]).NormalizePlane();
	Vector4f rightPlane = Vector4f(-mvpm.matrix[0][0] + mvpm.matrix[0][3], -mvpm.matrix[1][0] + mvpm.matrix[1][3], -mvpm.matrix[2][0] + mvpm.matrix[2][3], -mvpm.matrix[3][0] + mvpm.matrix[3][3]).NormalizePlane();
	Vector4f bottomPlane = Vector4f(mvpm.matrix[0][1] + mvpm.matrix[0][3], mvpm.matrix[1][1] + mvpm.matrix[1][3], mvpm.matrix[2][1] + mvpm.matrix[2][3], mvpm.matrix[3][1] + mvpm.matrix[3][3]).NormalizePlane();
	Vector4f topPlane = Vector4f(-mvpm.matrix[0][1] + mvpm.matrix[0][3], -mvpm.matrix[1][1] + mvpm.matrix[1][3], -mvpm.matrix[2][1] + mvpm.matrix[2][3], -mvpm.matrix[3][1] + mvpm.matrix[3][3]).NormalizePlane();
	Vector4f nearPlane = Vector4f(mvpm.matrix[0][2] + mvpm.matrix[0][3], mvpm.matrix[1][2] + mvpm.matrix[1][3], mvpm.matrix[2][2] + mvpm.matrix[2][3], mvpm.matrix[3][2] + mvpm.matrix[3][3]).NormalizePlane();
	Vector4f farPlane = Vector4f(-mvpm.matrix[0][2] + mvpm.matrix[0][3], -mvpm.matrix[1][2] + mvpm.matrix[1][3], -mvpm.matrix[2][2] + mvpm.matrix[2][3], -mvpm.matrix[3][2] + mvpm.matrix[3][3]).NormalizePlane();

	Vector3f center = point;

	delete viewMatrix;

	Vector4f planes[] = { nearPlane, farPlane, leftPlane, rightPlane, bottomPlane, topPlane };

	for (Vector4f plane : planes) {
		float dist = Vector4f::Dot(Vector4f(center.x, center.y, center.z, 1.0f), plane);
		if (dist < 0) {
			return false;
		}
	}

	return true;
}

Matrix4f * EntityManager::getModelMatrix(Entity * e) {
	Matrix4f * mat = new Matrix4f();
	mat->SetIdentity();
	mat->Translate(e->getPosition());
	mat->Rotate(e->getRotation());
	Vector3f v = Vector3f(e->getScale(), e->getScale(), e->getScale());
	mat->Scale(v);
	return mat;
}

void EntityManager::setTickEntities(bool value) {
	bTickEntities = value;
}

bool EntityManager::getTickEntities() {
	return bTickEntities;
}

void EntityManager::resizeTexts(FontLoader * loader){
	for (std::map<Mesh*, std::vector<Entity*>>::iterator it = entities.begin(); it != entities.end(); ++it) {
		for (Entity * ent : it->second) {
			if (ClassUtils::instanceof<Planet>(ent)) {
				((DynamicTextMesh *)((Planet *)ent)->text)->setText(((Planet *)ent)->text->textString, loader);
			}
		}
	}
}
