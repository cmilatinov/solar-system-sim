#include "EntityRenderer.h"



EntityRenderer::EntityRenderer(Matrix4f * projection){
	shader = new StaticShader("staticVertexShader.glsl", "staticFragmentShader.glsl");
	shader->use();
	shader->loadMatrix(shader->location_projectionMatrix, projection);
	shader->stop();
}


EntityRenderer::~EntityRenderer(){
	delete shader;
}

void EntityRenderer::StartRender(Camera * cam) {
	shader->use();
	Matrix4f * m = cam->createViewMatrix();
	shader->loadMatrix(shader->location_viewMatrix, m);
	delete m;
}

void EntityRenderer::RenderEntities(Camera * cam, std::vector<Light*> lights, std::vector<Entity*> entities){
	StartRender(cam);

	int i = 0;
	for (Light * light : lights) {
		shader->loadLight(i, light);
		i++;
	}

	for (Entity * ent : entities) {
		RenderEntity(ent);
	}

	StopRender();
}

void EntityRenderer::RenderEntities(Camera * cam, std::vector<Light*> lights, EntityManager * eManager){
	StartRender(cam);

	int i = 0;
	for (Light * light : lights) {
		shader->loadLight(i, light);
		i++;
	}

	std::map<Mesh*, std::vector<Entity*>> entities = eManager->getEntityHashmap();

	for (std::map<Mesh*, std::vector<Entity*>>::iterator it = entities.begin(); it != entities.end(); ++it) {
		int size = eManager->getNumEntitiesToRender(it->first);
		if (size > 0) {
			glBindVertexArray(it->first->GetVaoId());
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);
			glEnableVertexAttribArray(3);
			glEnableVertexAttribArray(4);
			glEnableVertexAttribArray(5);
			glEnableVertexAttribArray(6);
			glEnableVertexAttribArray(7);
			glEnableVertexAttribArray(8);
			if (it->second.at(0)->getTextureID() != -1) {
				shader->loadBoolean(shader->location_needsTexture, true);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, it->second.at(0)->getTextureID());
			}
			else
				shader->loadBoolean(shader->location_needsTexture, false);
			glDrawElementsInstanced(GL_TRIANGLES, it->first->GetVertexCount(), GL_UNSIGNED_INT, 0, size);
			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(2);
			glDisableVertexAttribArray(3);
			glDisableVertexAttribArray(4);
			glDisableVertexAttribArray(5);
			glDisableVertexAttribArray(6);
			glDisableVertexAttribArray(7);
			glDisableVertexAttribArray(8);
		}
	}

	StopRender();
}

void EntityRenderer::StopRender() {
	glBindVertexArray(0);
	shader->stop();
}

Matrix4f * EntityRenderer::getModelMatrix(Entity * e) {
	Matrix4f * mat = new Matrix4f();
	mat->SetIdentity();
	mat->Translate(e->getPosition());
	mat->Rotate(e->getRotation());
	Vector3f v = Vector3f(e->getScale(), e->getScale(), e->getScale());
	mat->Scale(v);
	return mat;
}

void EntityRenderer::RenderEntity(Entity * ent){
	Matrix4f * m = getModelMatrix(ent);
	shader->loadMatrix(shader->location_modelMatrix, m);
	delete m;
	glBindVertexArray(ent->getModel()->GetVaoId());
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	if (ent->getTextureID() != -1) {
		shader->loadBoolean(shader->location_needsTexture, true);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ent->getTextureID());
	}
	else
		shader->loadBoolean(shader->location_needsTexture, false);
	glDrawElements(GL_TRIANGLES, ent->getModel()->GetVertexCount(), GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
}


void EntityRenderer::ClipPlane(Vector4f * plane) {
	shader->use();
	shader->loadVector(shader->location_clipPlane, plane);
	shader->stop();
}