#include "Orbit.h"
#include "Engine.h"

Orbit::Orbit(float semiMajorAxis, float eccentricity, float inclination, float perihelionArg, float ascendingLongitude, float orbitalPeriodInDays, Time * time){
	this->semiMajorAxis = semiMajorAxis;
	this->eccentricity = eccentricity;
	this->inclination = inclination;
	this->perihelionArg = perihelionArg;
	this->ascendingNode = ascendingLongitude;
	this->orbitalPeriodInDays = orbitalPeriodInDays;
	this->meanMotion = -1.0f * Engine::getYearsPerSecond() * 2.0f * PI / (orbitalPeriodInDays / 365.25f);
	this->timeOfPeriapsis = time;
}

Orbit::~Orbit(){
	delete shape;
}

Matrix4f * Orbit::getConversionMatrix() {
	Matrix4f * mat = new Matrix4f();
	mat->SetIdentity();

	mat->Rotate(-perihelionArg, Vector3f(0, 1, 0));
	mat->Rotate(-inclination, Vector3f(1, 0, 0));
	mat->Rotate(-ascendingNode, Vector3f(0, 1, 0));

	return mat;
}

Vector3f * Orbit::getOrbitalPosition(float meanAnomaly) {
	float M = meanAnomaly * PI / 180.0f;
	float E = M;
	while (true) {
		float dE = (E - this->eccentricity * sin(E) - M) / (1 - this->eccentricity * cos(E));
		E -= dE;
		if (abs(dE) < 1e-4) 
			break;
	}

	float P = this->semiMajorAxis * (cos(E) - this->eccentricity);
	float Q = this->semiMajorAxis * sin(E) * sqrt(1 - pow(this->eccentricity, 2));
	
	return new Vector3f(P, 0.0f, Q);
}

Vector3f * Orbit::getOrbitalVelocity(float meanAnomaly) {
	float Ldot = meanMotion;
	float M = meanAnomaly * PI / 180.0f;
	float E = M;
	while (true) {
		float dE = (E - this->eccentricity * sin(E) - M) / (1 - this->eccentricity * cos(E));
		E -= dE;
		if (abs(dE) < 1e-4)
			break;
	}

	float vP = -semiMajorAxis * sin(E) * Ldot / (1 - eccentricity * cos(E));
	float vQ = semiMajorAxis * cos(E) * sqrt(1 - eccentricity * eccentricity) * Ldot / (1 - eccentricity * cos(E));

	return new Vector3f(vP, 0.0f, vQ);
}

void Orbit::updateMeanMotion() {
	meanMotion = -1.0f * Engine::getYearsPerSecond() * 2.0f * PI / (orbitalPeriodInDays / 365.25f);
}