#include "VectorMatrix.h"

#pragma once
class Point
{
public:
	Vector3f color;
	Vector3f * position;

	float opaqueDistance = 0.0f;

	Point(Vector3f * pos, Vector3f color);
	~Point();
};

