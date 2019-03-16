#include "TextRenderer.h"
#include "GuiManager.h"

#include <glad/glad.h>



TextRenderer::TextRenderer(Mesh * backgroundMesh){
	this->shader = new TextShader("textVertexShader.glsl", "textFragmentShader.glsl");
	this->guiMesh = backgroundMesh;
}


TextRenderer::~TextRenderer(){
	delete shader;
}

void TextRenderer::StartRender() {
	shader->use();
	glDisable(GL_DEPTH_TEST);
}

void TextRenderer::RenderTexts(std::vector<TextMesh*> texts){
	if (texts.size() == 0)
		return;
	
	StartRender();
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texts.at(0)->getBackground()->getTextureID());
	glBindVertexArray(guiMesh->GetVaoId());
	for (TextMesh * text : texts)
		RenderBackground(text);

	for (TextMesh * text : texts)
		RenderText(text);

	StopRender();
}

void TextRenderer::StopRender() {
	glBindVertexArray(0);
	glEnable(GL_DEPTH_TEST);
	shader->stop();
}



void TextRenderer::RenderText(TextMesh * text){
	if (!text->render) return;
	else if (!text->isOnScreen) return;

	Matrix4f * mat = text->getModelMatrix();
	shader->loadMatrix(shader->location_modelMatrix, mat);

	glBindVertexArray(text->getMesh()->GetVaoId());
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, text->getFont()->getTextureID());
	glDrawElements(GL_TRIANGLES, text->getMesh()->GetVertexCount(), GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindVertexArray(0);
}

void TextRenderer::RenderBackground(TextMesh * text){
	if (!text->render) return;
	else if (!text->isOnScreen) return;

	shader->loadMatrix(shader->location_modelMatrix, text->getBackground()->getModelMatrix());
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glDrawElements(GL_TRIANGLES, guiMesh->GetVertexCount(), GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

