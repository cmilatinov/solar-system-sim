#include "GuiRenderer.h"

GuiRenderer::GuiRenderer(Mesh * guiMesh){
	this->guiMesh = guiMesh;
	this->shader = new GuiShader("guiVertexShader.glsl", "guiFragmentShader.glsl");
	this->shader->use();
	this->shader->loadInt(shader->location_texture, 0);
	this->shader->stop();
}


GuiRenderer::~GuiRenderer(){
	delete shader;
}

void GuiRenderer::StartRender() {
	shader->use();
}

void GuiRenderer::StopRender() {
	shader->stop();
}

void GuiRenderer::RenderGUI(std::vector<Gui*> guis){
	StartRender();

	for (Gui * g : guis)
		RenderGui(g);

	StopRender();
}

void GuiRenderer::RenderGui(Gui * gui) {
	shader->loadMatrix(shader->location_modelMatrix, gui->getModelMatrix());
	glBindVertexArray(guiMesh->GetVaoId());
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gui->getTextureID());
	glDrawElements(GL_TRIANGLES, guiMesh->GetVertexCount(), GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindVertexArray(0);
}