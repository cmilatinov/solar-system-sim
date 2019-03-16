#include "Light.h"



Light::Light()
{
}


Light::~Light()
{
}

Light::Light(Vector3f pos, Vector3f color) {
	this->pos = pos;
	this->color = color;
}

void Light::setPosition(Vector3f newPos) {
	this->pos = newPos;
}

void Light::move(Vector3f translation) {
	this->pos = this->pos + translation;
}

void Light::setColor(Vector3f newColor) {
	this->color = newColor;
}

Vector3f Light::getPosition() {
	return this->pos;
}

Vector3f Light::getColor() {
	return this->color;
}