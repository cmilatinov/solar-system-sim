#include "PostProcessing.h"
#include "Display.h"

PostProcessing::PostProcessing(Mesh * quad, FBO * fbo){
	this->quad = quad;

	//CONTRAST
	this->contrastShader = new ContrastShader("contrastVertexShader.glsl", "contrastFragmentShader.glsl");
	contrastShader->use();
	contrastShader->loadFloat(contrastShader->location_contrast, 0.0f);
	contrastShader->stop();

	//GAUSSIAN BLUR
	this->hBlur = new  HorizontalBlurShader("horizontalBlurVertexShader.glsl", "horizontalBlurFragmentShader.glsl");
	this->vBlur = new  VerticalBlurShader("verticalBlurVertexShader.glsl", "verticalBlurFragmentShader.glsl");
	
	//BLOOM EFFECT
	this->brightnessShader = new BrightnessGateShader("brightnessGateVertexShader.glsl", "brightnessGateFragmentShader.glsl");
	this->bloomShader = new BloomShader("bloomVertexShader.glsl", "bloomFragmentShader.glsl");

	this->bloomShader->use();
	this->bloomShader->loadInt(bloomShader->location_sceneTexture, 0);
	this->bloomShader->loadInt(bloomShader->location_bloomTexture, 1);
	this->bloomShader->stop();

	this->hBlur->use();
	this->hBlur->loadFloat(hBlur->location_targetWidth, fbo->getTextureWidth() / 4.0f);
	this->vBlur->use();
	this->vBlur->loadFloat(vBlur->location_targetHeight, fbo->getTextureHeight() / 4.0f);
	this->vBlur->stop();
	

	this->scene = fbo;
	this->blurTemp = new FBO(fbo->getTextureWidth() / 2.0f, fbo->getTextureHeight() / 2.0f, false);

	this->temp = new FBO(fbo->getTextureWidth(), fbo->getTextureHeight(), false);
	this->temp2 = new FBO(fbo->getTextureWidth(), fbo->getTextureHeight(), false);
	this->temp3 = new FBO(fbo->getTextureWidth() / 2.0f, fbo->getTextureHeight() / 2.0f, false);
	this->temp4 = new FBO(fbo->getTextureWidth() / 4.0f, fbo->getTextureHeight() / 4.0f, false);
	this->temp5 = new FBO(fbo->getTextureWidth() / 8.0f, fbo->getTextureHeight() / 8.0f, false);
}

PostProcessing::~PostProcessing(){
}

void PostProcessing::SetContrast(float contrast){
	contrastShader->use();
	contrastShader->loadFloat(contrastShader->location_contrast, contrast);
	contrastShader->stop();
}

void PostProcessing::DoPostProcessing(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	//===========================================

	temp->bind();
	ChangeContrast(scene);
	temp->unbind();
	
	temp2->bind();
	BrightnessGate(temp);
	temp2->unbind();

	Blur(temp2, temp3, 1.0f / 2.0f);
	Blur(temp3, temp4, 1.0f / 4.0f);
	Blur(temp4, temp5, 1.0f / 8.0f);

	Bloom(temp, temp5);
	
	/*
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, temp5->getID());
	glBlitFramebuffer(0, 0, temp5->getTextureWidth(), temp5->getTextureHeight(),
		0, 0, Display::getWidth(), Display::getHeight(),
		GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	*/

	//===========================================
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
}

void PostProcessing::ChangeContrast(FBO * fbo) {
	contrastShader->use();
	glBindVertexArray(quad->GetVaoId());
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fbo->getTextureIDs().at(0));
	glDrawElements(GL_TRIANGLES, quad->GetVertexCount(), GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindVertexArray(0);
	contrastShader->stop();
}

void PostProcessing::Blur(FBO * fbo, FBO * out, float scale) {
	hBlur->use();
	hBlur->loadFloat(hBlur->location_targetWidth, fbo->getTextureWidth() * scale);
	vBlur->use();
	vBlur->loadFloat(vBlur->location_targetHeight, fbo->getTextureHeight() * scale);
	vBlur->stop();

	blurTemp->bind();
	hBlur->use();
	glBindVertexArray(quad->GetVaoId());
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fbo->getTextureIDs().at(0));
	glDrawElements(GL_TRIANGLES, quad->GetVertexCount(), GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindVertexArray(0);
	hBlur->stop();
	blurTemp->unbind();

	out->bind();
	vBlur->use();
	glBindVertexArray(quad->GetVaoId());
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, blurTemp->getTextureIDs().at(0));
	glDrawElements(GL_TRIANGLES, quad->GetVertexCount(), GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindVertexArray(0);
	hBlur->stop();
	out->unbind();
}

void PostProcessing::BrightnessGate(FBO * fbo) {
	brightnessShader->use();
	glBindVertexArray(quad->GetVaoId());
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fbo->getTextureIDs().at(0));
	glDrawElements(GL_TRIANGLES, quad->GetVertexCount(), GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindVertexArray(0);
	brightnessShader->stop();
}

void PostProcessing::Bloom(FBO * scene, FBO * bloom){
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, scene->getTextureIDs().at(0));
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, bloom->getTextureIDs().at(0));
	bloomShader->use();
	glBindVertexArray(quad->GetVaoId());
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glDrawElements(GL_TRIANGLES, quad->GetVertexCount(), GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindVertexArray(0);
	bloomShader->stop();
}
