#include "VectorMatrix.h"
#pragma once
class Light
{
private:
	Vector3f pos, color;

public:
	Light();
	Light(Vector3f pos, Vector3f color);
	~Light();

	void setPosition(Vector3f newPos);
	void move(Vector3f translation);
	void setColor(Vector3f newColor);

	Vector3f getPosition();
	Vector3f getColor();
};

