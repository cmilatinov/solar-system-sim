#include <ctime>

#include "VectorMatrix.h"
#include "Shape.h"
#include "Time.h"

#pragma once
class Orbit 
{
public:
	float inclination, perihelionArg, ascendingNode;
	float semiMajorAxis, eccentricity, c;
	float meanMotion, orbitalPeriodInDays;
	Time * timeOfPeriapsis;
	Shape * shape;

	Orbit(float semiMajorAxis, float eccentricity, float inclination, float perihelionArg, float ascendingLongitude, float meanMotion, Time * time);
	~Orbit();

	Matrix4f * getConversionMatrix();

	Vector3f * getOrbitalPosition(float meanAnomaly);

	Vector3f * getOrbitalVelocity(float meanAnomaly);
	
	void updateMeanMotion();

};

