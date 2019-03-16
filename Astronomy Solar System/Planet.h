#include "VectorMatrix.h"
#include "Orbit.h"
#include "Entity.h"
#include "Time.h"
#include "FadedShape.h"
#include "Point.h"
#include "TextMesh.h" 

#pragma once
class Planet : 
	public Entity
{
private:
	Vector3f rotationalAxis;

	float rotationAtTime;
	Time * referenceTime;

	float currentRotation;
	

public:
	Planet(Mesh * model, float alpha, float delta, float epsilon, float rotationalPeriodInYears, Time * referenceTime, float rotationAtTime);
	~Planet();

	FadedShape * trail;
	Point * point;
	TextMesh * text;
	Orbit * orbit;
	Shape * axis;
	float alpha, delta, epsilon;
	float rotationalPeriodInYears;

	void tick(float deltaSec);
	void setToRotationAtTime(Time * t);

	Matrix4f * getModelMatrix();
	Matrix4f * getRotationMatrix();

	static Matrix4f * getRotationalMatrix(float alpha, float delta, float epsilon);
};

