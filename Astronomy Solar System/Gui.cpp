#include "Gui.h"
#include "Display.h"


Gui::Gui(unsigned int texID, int x, int y, int width, int height){
	this->texID = texID;
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->mat = new Matrix4f();
	this->mat->SetIdentity();
	float scaleX = (float)width / (float)Display::getWidth();
	float scaleY = (float)height / (float)Display::getHeight();
	this->mat->Translate(Vector3f(((scaleX - 1.0f) + ((float)(x * 2) / (float)Display::getWidth())), 
									((1.0f - scaleY) - ((float)(y * 2) /(float)Display::getHeight())), 
									0));
	this->mat->Scale(Vector3f(scaleX, scaleY, 1.0f));
}

Gui::~Gui(){
	delete mat;
}

void Gui::setPosition(Vector2f pos){
	mat->SetIdentity();
	this->x = pos.x;
	this->y = pos.y;
	float scaleX = (float)width / (float)Display::getWidth();
	float scaleY = (float)height / (float)Display::getHeight();
	mat->Translate(Vector3f(((scaleX - 1.0f) + ((float)(x * 2) / (float)Display::getWidth())),
		((1.0f - scaleY) - ((float)(y * 2) / (float)Display::getHeight())),
		0));
	mat->Scale(Vector3f(scaleX, scaleY, 1.0f));
}

void Gui::setBounds(Vector2f bounds){
	this->width = bounds.x;
	this->height = bounds.y;
	setPosition(Vector2f(x, y));
}

Matrix4f * Gui::getModelMatrix(){
	return mat;
}

unsigned int Gui::getTextureID() {
	return texID;
}