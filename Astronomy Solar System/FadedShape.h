#include "Shape.h"
#include "Entity.h"
#include "glad/glad.h"

#include <vector>
#pragma once
class FadedShape : 
	public Shape
{
private:
	std::vector<Vector3f> points;
	Vector3f lastPoint;

	Entity * ent;

	unsigned int vbo;

public:
	FadedShape(Shape * initial, unsigned int vbo, std::vector<Vector3f> points, Entity * target, float minDist);
	~FadedShape();

	float minDist;

	void tick();

	void loadDataToBuffer();
};

