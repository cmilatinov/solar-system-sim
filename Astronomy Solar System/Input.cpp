#include "Input.h"
#include <ctime>
#include <iostream>
#include <string>

#define ONE_PRESS_TMR 0.5f
#define MIN_YEARS_PER_SEC 0.0015f
#define MAX_YEARS_PER_SEC 10.0f

static bool toScale = false;

static bool grabCursor = false;

bool wireframe = false;
long lastPlusMinus = clock();

bool creatingAsteroid = false;
bool asteroidPositionSet = false;

bool cameraSnapped = false;

Entity * tempEnt;
Orbit * tempOrbit;
float tempMeanAnomaly = 0.0f;
Shape * velocityShape;
Vector3f velocityDir;
Vector3f * entPos;

static std::vector<Asteroid*> addedAsteroids;

Input::Input(GLFWwindow * window, Engine * engine, Camera * camera)
{
	this->window = window;
	this->engine = engine;
	this->camera = camera;
}

Input::~Input()
{
}

void Input::processInput(int frameWidth, int frameHeight, Engine * engine)
{
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	//if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	//	glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) {
		float decrement = -0.01f;
		if (Engine::getYearsPerSecond() + decrement > MIN_YEARS_PER_SEC)
			engine->setYearsPerSecond(Engine::getYearsPerSecond() + decrement);
		else
			engine->setYearsPerSecond(MIN_YEARS_PER_SEC);
	}

	if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS) {
		float increment = 0.01f;
		if (Engine::getYearsPerSecond() + increment < MAX_YEARS_PER_SEC)
			engine->setYearsPerSecond(Engine::getYearsPerSecond() + increment);
		else
			engine->setYearsPerSecond(MAX_YEARS_PER_SEC);
	}

	if (creatingAsteroid && asteroidPositionSet) {
		glBindBuffer(GL_ARRAY_BUFFER, velocityShape->shapeVBO);
		glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
		if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
			velocityDir.x += 0.03f;
		}if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
			velocityDir.x -= 0.03f;
		}if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
			velocityDir.y += 0.03f;
		}if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
			velocityDir.y -= 0.03f;
		}if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
			velocityDir.z += 0.03f;
		}if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
			velocityDir.z -= 0.03f;
		}
		
		if (velocityDir.length() > 1.5f) {
			velocityDir = 1.5f * velocityDir.Normalize();
		}
		if (velocityDir.length() < 0.5f) {
			velocityDir = 0.5f * velocityDir.Normalize();
		}

		float data[] = { 0, 0, 0, velocityDir.x, velocityDir.y, velocityDir.z };
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(data), data);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	
	//GRAB CURSOR
	if (grabCursor)
		glfwSetCursorPos(window, frameWidth / 2, frameHeight / 2);
}

void Input::processKeyEvent(GLFWwindow * window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) {
		setGrabCursor(false);
	}

	if (key == GLFW_KEY_R && action == GLFW_RELEASE) {
		Time * resetTime = Time::J2000();
		engine->Reset(resetTime);
		delete resetTime;
		engine->timeElapsed = 0.0f;
	}
	if (key == GLFW_KEY_Z && action == GLFW_RELEASE) {
		EntityManager::setTickEntities(!EntityManager::getTickEntities());
		engine->bTickEntities = !engine->bTickEntities;
	}

	if (key == GLFW_KEY_X && action == GLFW_RELEASE) {
		for (Asteroid * ast : addedAsteroids) {
			engine->sManager->removeShape(ast->orbit->shape);
			engine->sManager->removeShape(ast->trail);
			engine->pManager->removePoint(ast->point);
			engine->eManager->removeEntity(ast->ent);
			engine->textManager->removeText(ast->text);
			
			delete ast;
		}
		addedAsteroids.clear();

	}

	if (key == GLFW_KEY_C && action == GLFW_RELEASE && !creatingAsteroid) {
		creatingAsteroid = true;
		asteroidPositionSet = false;
		tempEnt = engine->CreateEntity(engine->asteroid, engine->asteroidTexID, camera->getPosition() + 2.0f * camera->getForwardCameraVector().Normalize(), Vector3f(), 0.1f);
		tempEnt->setAffectedByGravity(false);
	}

	if (key == GLFW_KEY_M && action == GLFW_RELEASE) {
		engine->ToggleShowPlanetOrbits();
		setRenderOrbits(engine->showOrbits);
	}

	if (key == GLFW_KEY_T && action == GLFW_RELEASE) {
		engine->ToggleRenderText();
	}

	if (key == GLFW_KEY_H && action == GLFW_RELEASE) {
		engine->ToggleRenderPlanetNames();
		for (Asteroid * ast : addedAsteroids) {
			ast->text->render = !ast->text->render;
		}
	}

	if (key == GLFW_KEY_G && action == GLFW_RELEASE) {
		engine->generalInfo->render = !engine->generalInfo->render;
		for (TextMesh * text : engine->planetInformation) {
			text->render = !text->render;
		}
	}
	
	if (key == GLFW_KEY_Q && action == GLFW_RELEASE) {
		for (Entity * ent : engine->planets) {
			Planet * planet = (Planet *)ent;
			planet->axis->render = !planet->axis->render;
		}
	}

	if (key == GLFW_KEY_F && action == GLFW_RELEASE) {
		toScale = !toScale;
		float oldScale = Engine::getGlobalScale();
		if (toScale) {
			for (Asteroid * ast : addedAsteroids) {
				ast->ent->setScale(ast->ent->getScale() * 1.0f / oldScale);
				ast->point->opaqueDistance *= 1.0f / oldScale;
				ast->ent->maxDist = ast->point->opaqueDistance;
				ast->trail->minDist *= 1.0f / oldScale;
			}
			engine->setGlobalScale(1.0f);
			engine->sunEntity->setScale(0.025f);
		}else {
			for (Asteroid * ast : addedAsteroids){
				ast->ent->setScale(ast->ent->getScale() * (1.0f / 4.25875e-4f) / oldScale);
				ast->point->opaqueDistance *= (1.0f / 4.25875e-4f) / oldScale;
				ast->ent->maxDist = ast->point->opaqueDistance;
				ast->trail->minDist *= (1.0f / 4.25875e-4f) / oldScale;
			}
			engine->setGlobalScale(1.0f / 4.25875e-4f);
			engine->sunEntity->setScale(0.4f);
		}
	}

	if (asteroidPositionSet && creatingAsteroid && key == GLFW_KEY_ENTER && action == GLFW_RELEASE) {
		engine->sManager->removeShape(velocityShape);
		delete velocityShape;
		tempEnt->setAffectedByGravity(true);
		tempEnt->mass = 11e+27;
		tempEnt->velocity = tempEnt->velocity.length() * velocityDir;
		createOrbit(1.5e+10 * tempEnt->getPosition(), (4740.57172f / (10.0f *Engine::getYearsPerSecond())) * tempEnt->velocity, tempEnt->mass);
		Asteroid * ast = createAsteroid(tempEnt, tempOrbit, tempMeanAnomaly, "Asteroid " + std::to_string(addedAsteroids.size() + 1) + " ");
		addedAsteroids.push_back(ast);
		creatingAsteroid = false;
		asteroidPositionSet = false;
	}else if (creatingAsteroid && key == GLFW_KEY_ENTER && action == GLFW_RELEASE) {
		asteroidPositionSet = true;
		float vel = sqrt((6.67e-11 * 1.99e+30) / (tempEnt->getPosition().length() * 1.5e+10)) * 0.00210945021f;
		Vector3f dir = Vector3f::Cross(Vector3f(0, 1, 0), tempEnt->getPosition().Normalize()).Normalize();
		tempEnt->velocity = Engine::getYearsPerSecond() * vel * dir;
		entPos = new Vector3f();
		*entPos = tempEnt->getPosition();
		velocityDir = dir;
		float vertices[6] = { 0, 0, 0, velocityDir.x, velocityDir.y, velocityDir.z };
		velocityShape = engine->loader->createDynamicLine(vertices, sizeof(vertices), false, Vector3f(1, 1, 1), entPos);
		engine->sManager->addShape(velocityShape);
	}
}

void Input::processMouseEvent(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		setGrabCursor(true);
}

void Input::setCamera(Camera * camera){
	this->camera = camera;
}

void Input::tick(){
	if (!asteroidPositionSet && creatingAsteroid && tempEnt != nullptr) {
		tempEnt->setPosition(camera->getPosition() + 2.0f * camera->getForwardCameraVector().Normalize());
	}
	for (Asteroid * ast : addedAsteroids) {
		if (ast != nullptr) {
			ast->trail->tick();
			bool flag = checkPointIsInFrustum(camera, ast->ent->getPosition());

			if (flag) {
				Vector2f screenPos = MathUtils::Calculate2DScreenPosition(*engine->projectionMatrix, ast->ent->getPosition(), camera);
				ast->text->setScreenPosition(Vector2f(screenPos.x * Display::getWidth(), screenPos.y * Display::getHeight()));
				ast->text->isOnScreen = true;
			}
			else
				ast->text->isOnScreen = false;
		}
	}
}

void Input::setRenderOrbits(bool value){
	for (Asteroid * ast : addedAsteroids)
		ast->orbit->shape->render = value;
}

void Input::createOrbit(Vector3f pos, Vector3f vel, float mass){
	float dist = pos.length();
	float velocity = vel.length();
	float mu = 6.67e-11 * (mass + 2e+30);

	Vector3f hVec = Vector3f::Cross(pos, vel);
	Vector3f node = Vector3f::Cross(Vector3f(0, 1, 0), hVec);
	
	Vector3f eVec = (1.0f / mu) * ( (pow(velocity, 2) - (mu / dist)) * pos - (Vector3f::Dot(pos, vel) * vel));
	float e = eVec.length();

	float mechanicalEnergy = (pow(velocity, 2) / 2.0f) - (mu / dist);
	float a = -mu / (2.0f * mechanicalEnergy);

	if (e < 1) {
		float i = acos(hVec.y / hVec.length()) * 180.0 / PI;
		float ascendingNode = acos(node.x / node.length()) * 180.0 / PI;
		float periapsisArg = acos(Vector3f::Dot(node, eVec) / (node.length() * e)) * 180.0 / PI;
		float meanAnomaly = acos(Vector3f::Dot(eVec, pos) / (e * dist)) * 180.0f / PI;

		if (meanAnomaly != meanAnomaly)
			meanAnomaly = 180.0f;

		float period = sqrt((4 * pow(PI, 2) * pow(a, 3)) / mu) / (24 * 3600);

		if (eVec.y < 0)
			periapsisArg = 360.0f - periapsisArg;

		if (Vector3f::Dot(pos, vel) < 0)
			meanAnomaly = 360.0f - meanAnomaly;

		if (hVec.x < 0)
			ascendingNode = -ascendingNode;

		std::cout << "Semi-Major Axis : " << a / 1.5e+11 << " AU" << std::endl;
		std::cout << "Eccentricity : " << e << std::endl;
		std::cout << "Inclination : " << i << std::endl;
		std::cout << "Mean Anomaly : " << meanAnomaly << std::endl;
		std::cout << "Ascending Node : " << ascendingNode << std::endl;
		std::cout << "Argument of Periapsis : " << periapsisArg << std::endl;
		std::cout << "Orbital Period : " << period << " days" << std::endl;

		tempOrbit = engine->CreateOrbit(Vector3f(1, 1, 1), engine->origin, a / 1.5e+10, e, i, periapsisArg, ascendingNode, period, new Time(1, 1, 2000, 12, 0));

		tempMeanAnomaly = meanAnomaly;
	}
}

Asteroid * Input::createAsteroid(Entity * planet, Orbit * orbit, float meanAnomaly, std::string name){
	FadedShape * fShape = engine->loader->createTrail(orbit, planet, meanAnomaly, Color::purple().get(), 200, (PI * orbit->semiMajorAxis) / 250.0f);
	engine->sManager->addShape(fShape);

	Point * point = new Point(planet->getPositionPointer(), Color::purple().get());
	point->opaqueDistance = 75.0f;
	planet->maxDist = 75.0f;
	engine->pManager->addPoint(point);

	TextMesh * text = engine->fLoader->loadString(name, engine->f, engine->textSize, Vector2f());
	text->render = ((Planet *)engine->planets.at(0))->text->render;
	engine->textManager->addText(text);

	return new Asteroid(planet, orbit, point, fShape, text);
}

bool Input::isToScale(){
	return toScale;
}

bool Input::isGrabCursor() {
	return grabCursor;	
}

void Input::setGrabCursor(bool bGrab) {
	if (bGrab == grabCursor) return;

	grabCursor = bGrab;
	int w, h;
	glfwGetWindowSize(window, &w, &h);
	glfwSetCursorPos(window, w / 2, h / 2);
	if(bGrab)
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	else 
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

bool Input::checkPointIsInFrustum(Camera * cam, Vector3f point) {

	Matrix4f * viewMatrix = cam->createViewMatrix();
	Matrix4f mvpm = *viewMatrix * *engine->projectionMatrix;

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