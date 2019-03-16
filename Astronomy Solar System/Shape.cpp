#include "Shape.h"
#include "glad/glad.h"


Shape::Shape(Mesh * mesh, Vector3f color, Vector3f * pos){
	this->mesh = mesh;
	this->color = color;
	this->pos = pos;
	this->dashed = false;
}

Shape::~Shape(){

}

Mesh * Shape::getModel(){
	return this->mesh;
}

Vector3f Shape::getColor(){
	return this->color;
}

Matrix4f * Shape::getModelMatrix() {
	Matrix4f * mat = new Matrix4f();
	mat->SetIdentity();
	mat->Translate(*pos);
	return mat;
}

void Shape::setDashed(bool value){
	this->dashed = value;
}

void Shape::setOpacity(float value){
	this->opacity = value;
}

bool Shape::isDashed(){
	return this->dashed;
}

float Shape::getOpacity(){
	return this->opacity;
}
