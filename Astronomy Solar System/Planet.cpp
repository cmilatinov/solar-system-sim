#include "Planet.h"
#include "Engine.h"

Planet::Planet(Mesh * model, float alpha, float delta, float epsilon, float rotationalPeriodInYears, Time * referenceTime, float rotationAtTime) : Entity(model){
	this->alpha = alpha;
	this->delta = delta;
	this->epsilon = epsilon;
	
	this->rotationalAxis = Vector3f(0, 1, 0);
	Matrix4f * mat = getRotationalMatrix(alpha, delta, epsilon);
	rotationalAxis.Transform(mat);
	delete mat;

	this->rotationalPeriodInYears = rotationalPeriodInYears;
	this->referenceTime = referenceTime;
	this->rotationAtTime = rotationAtTime;
}

Planet::~Planet(){

}

void Planet::setToRotationAtTime(Time * t) {
	float years = t->difference(*referenceTime) / (365.25f * 24.0f * 3600.0f);
	currentRotation = (years * 360.0f) / rotationalPeriodInYears;
	currentRotation = fmod(currentRotation, 360.0f);
}

void Planet::tick(float deltaSec){
	currentRotation += (Engine::getYearsPerSecond() * deltaSec * 360.0f) / rotationalPeriodInYears;
	currentRotation = fmod(currentRotation, 360.0f);
}

Matrix4f * Planet::getModelMatrix() {
	Matrix4f * mat = new Matrix4f();
	mat->SetIdentity();
	mat->Translate(this->pos);
	
	mat->Rotate(Vector3f(-epsilon, 0, 0));
	mat->Rotate(Vector3f(0, 0, 90.0f - delta));
	mat->Rotate(Vector3f(0, -alpha, 0));
	
	mat->Rotate(Vector3f(0, currentRotation, 0));
	
	
	Vector3f v = Vector3f(this->scale, this->scale, this->scale);
	mat->Scale(v);
	return mat;
}

Matrix4f * Planet::getRotationMatrix() {
	Matrix4f * mat = new Matrix4f();
	mat->SetIdentity();
	mat->Translate(this->pos);

	mat->Rotate(Vector3f(0, alpha, 0));
	mat->Rotate(Vector3f(0, 0, delta - 90.0f));
	mat->Rotate(Vector3f(epsilon, 0, 0));

	return mat;
}
Matrix4f * Planet::getRotationalMatrix(float alpha, float delta, float epsilon) {
	Matrix4f * mat = new Matrix4f();
	mat->SetIdentity();
	mat->Rotate(Vector3f(0, alpha, delta - 90.0f));
	mat->Rotate(Vector3f(epsilon, 0, 0));
	return mat;
}