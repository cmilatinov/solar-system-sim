#include "VectorMatrix.h"
#include "Mesh.h"

#define MAX_GRAVITY_INFLUENCES 3

#pragma once
class Entity
{
protected:
	Vector3f pos;
	Vector3f rot;
	float scale;
	Mesh * model;
	unsigned int texID;
	bool affectedByGravity = true;

public:
	double mass = 0.0;
	Vector3f acceleration = Vector3f();
	Vector3f velocity = Vector3f();
	float maxDist = 0.0f;

	Entity(Mesh *m);
	Entity(Mesh *m, Vector3f position, Vector3f rotation, float scale);
	~Entity();

	void setTextureID(unsigned int tex);
	unsigned int getTextureID();

	void setPosition(Vector3f position);
	Vector3f getPosition();
	Vector3f * getPositionPointer();
	void setRotation(Vector3f rotation);
	Vector3f getRotation();
	void setScale(float scale);
	float getScale();
	void rotateEntity(float x, float y, float z);
	void moveEntity(float x, float y, float z);

	void setAffectedByGravity(bool affectedByGravity);
	bool isAffectedByGravity();
	
	virtual Matrix4f * getModelMatrix();

	Mesh * getModel();
};

