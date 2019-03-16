#include "Entity.h"

Entity::Entity(Mesh * m, Vector3f position, Vector3f rotation, float scale){
	this->model = m;
	this->pos = position;
	this->rot = rotation;
	this->scale = scale;
	this->texID = -1;
}

Entity::Entity(Mesh * m){
	this->model = m;
	this->pos = Vector3f(0, 0, 0);
	this->rot = Vector3f(0, 0, 0);
	this->scale = 1;
	this->texID = -1;
}

Entity::~Entity(){
	//delete model;
}

Mesh * Entity::getModel() {
	return model;
}

void Entity::setPosition(Vector3f position) {
	this->pos = position;
}

Vector3f Entity::getPosition() {
	return this->pos;
}

Vector3f * Entity::getPositionPointer(){
	return &this->pos;
}

void Entity::setRotation(Vector3f rotation) {
	this->rot = rotation;
}

Vector3f Entity::getRotation() {
	return this->rot;
}

void Entity::setScale(float scale) {
	this->scale = scale;
}

float Entity::getScale() {
	return this->scale;
}

void Entity::moveEntity(float dx, float dy, float dz) {
	pos = Vector3f(pos.x + dx, pos.y + dy, pos.z + dz);
	pos.Print();
}

void Entity::setAffectedByGravity(bool affectedByGravity){
	this->affectedByGravity = affectedByGravity;
}

bool Entity::isAffectedByGravity(){
	return this->affectedByGravity;
}

void Entity::rotateEntity(float rx, float ry, float rz) {
	rot = Vector3f(rot.x + rx, rot.y + ry, rot.z + rz);
}

void Entity::setTextureID(unsigned int tex) {
	this->texID = tex;
}

unsigned int Entity::getTextureID() {
	return this->texID;
}

Matrix4f * Entity::getModelMatrix() {
	Matrix4f * mat = new Matrix4f();
	mat->SetIdentity();
	mat->Translate(this->pos);
	mat->Rotate(this->rot);
	Vector3f v = Vector3f(this->scale, this->scale, this->scale);
	mat->Scale(v);
	return mat;
}
