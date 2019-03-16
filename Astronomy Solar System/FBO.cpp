#include "FBO.h"
#include "Display.h"
#include <iostream>

FBO::FBO(int width, int height, bool needsDepthTexture){
	this->width = width;
	this->height = height;
	this->samples = 1;
	this->fboID = createFBO();
	this->addColorAttachment();
	if (needsDepthTexture)
		this->addDepthAttachment();
	else
		this->addDepthBuffer();
	this->unbind();
}

FBO::FBO(int width, int height, int samples) {
	this->width = width;
	this->height = height;
	this->samples = samples;
	this->fboID = createFBO();
	this->addColorBuffer();
	this->addDepthBuffer();
	this->unbind();
}

FBO::~FBO(){
	glDeleteFramebuffers(1, &fboID);
	if (depthBuffer)
		glDeleteRenderbuffers(1, &depthBuffer);
	if (colorBuffer)
		glDeleteRenderbuffers(1, &colorBuffer);
	for(unsigned int tex : textures)
		glDeleteTextures(1, &tex);
}

void FBO::resolve(FBO * fbo){
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo->getID());
	glBindFramebuffer(GL_READ_FRAMEBUFFER, this->fboID);
	glBlitFramebuffer(0, 0, this->width, this->height,
		0, 0, fbo->getTextureWidth(), fbo->getTextureHeight(),
		GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	GLenum err = glGetError();
	if (err != GL_NO_ERROR){
		std::cout << "Error while blitting (" << err << ")" << std::endl;
	}
}

void FBO::addDepthAttachment(){
	unsigned int tex = createTextureAttachment("DEPTH");
	this->textures.push_back(tex);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, tex, 0);
}

void FBO::addColorAttachment(){
	unsigned int tex = createTextureAttachment("RGB");
	this->textures.push_back(tex);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex, 0);
}

void FBO::addColorBuffer() {
	this->colorBuffer = createColorBufferAttachment();
}

void FBO::addDepthBuffer(){
	this->depthBuffer = createDepthBufferAttachment();
}

unsigned int FBO::createFBO() {
	unsigned int ID;
	glGenFramebuffers(1, &ID);
	glBindFramebuffer(GL_FRAMEBUFFER, ID);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	return ID;
}

unsigned int FBO::createDepthBufferAttachment() {
	unsigned int depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	if(samples <= 1)
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	else
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
	return depthBuffer;
}

unsigned int FBO::createColorBufferAttachment() {
	unsigned int colorBuffer;
	glGenRenderbuffers(1, &colorBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, colorBuffer);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_RGBA8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorBuffer);
	return colorBuffer;
}

unsigned int FBO::createTextureAttachment(std::string type) {
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	if (type == "RGB") 
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	else if (type == "DEPTH")
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	return texture;
}

void FBO::bind() {
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, fboID);
	glViewport(0, 0, width, height);
}

void FBO::unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Display::getWidth(), Display::getHeight());
}

std::vector<unsigned int> FBO::getTextureIDs() {
	return this->textures;
}

int FBO::getTextureHeight() {
	return height;
}

unsigned int FBO::getID(){
	return fboID;
}

int FBO::getTextureWidth() {
	return width;
}