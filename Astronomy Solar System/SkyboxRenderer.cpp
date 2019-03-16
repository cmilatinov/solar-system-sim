#include "SkyboxRenderer.h"



SkyboxRenderer::SkyboxRenderer(Matrix4f * projection, Mesh * mesh){
	this->cube = mesh;
	this->shader = new SkyboxShader("skyboxVertexShader.glsl", "skyboxFragmentShader.glsl");
	this->shader->use();
	this->shader->loadMatrix(shader->location_projectionMatrix, projection);
	this->shader->stop();
}


SkyboxRenderer::~SkyboxRenderer(){
	delete shader;
}

void SkyboxRenderer::RenderSkyBox(Camera * cam, Skybox * box){
	StartRender(cam);

	Matrix4f * modelMatrix = new Matrix4f();
	modelMatrix->SetIdentity();
	modelMatrix->Scale(Vector3f(box->getSize(), box->getSize(), box->getSize()));
	shader->loadMatrix(shader->location_modelMatrix, modelMatrix);
	delete modelMatrix;

	glBindVertexArray(cube->GetVaoId());
	glEnableVertexAttribArray(0); 
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, box->getCubeMapTextureID());
	glDrawElements(GL_TRIANGLES, cube->GetVertexCount(), GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);

	StopRender();
}

void SkyboxRenderer::StartRender(Camera * cam) {
	shader->use();
	Matrix4f *m = cam->createRotationMatrix();
	shader->loadMatrix(shader->location_viewMatrix, m);
	delete m;
}

void SkyboxRenderer::StopRender() {
	glBindVertexArray(0);
	shader->stop();
}
