#include "Camera.h"

Camera::Camera() {

}

Camera::Camera(Vector3f pos) {
	this->position = pos;
}

Camera::~Camera() {
}

float Camera::getPitch() {
	return pitch;
}

float Camera::getYaw() {
	return yaw;
}

float Camera::getRoll() {
	return roll;
}

float * Camera::getCameraRotation() {
	float rot[] = { pitch, yaw, roll };
	return rot;
}

void Camera::setPosition(Vector3f position) {
	this->position = position;
}

void Camera::setCameraRotation(Vector3f rotation){
	this->pitch = rotation.x;
	this->yaw = rotation.y;
	this->roll = rotation.z;
}

void Camera::setCameraRotation(float rx, float ry, float rz) {
	this->pitch = rx;
	this->yaw = ry;
	this->roll = rz;
}

void Camera::setCameraRotation(float rot[]) {
	this->pitch = rot[0];
	this->yaw = rot[1];
	this->roll = rot[2];
}

Vector3f Camera::getPosition() {
	return position;
}

void Camera::invertPitch() {
	this->pitch = -this->pitch;
}

void Camera::invertRoll() {
	this->roll = -this->roll;
}

void Camera::LookAt(Vector3f point) {
	Vector3f toPoint = position - point;
	
	this->yaw = atan(toPoint.x / (-1.0f * toPoint.z)) *  180.0f / PI;
	this->pitch = 90.0f - atan(sqrt(pow(toPoint.x, 2) + pow(toPoint.z, 2)) / toPoint.y) * 180.0f / PI;

	if (toPoint.z < 0)
		yaw += 180.0f;

	if (toPoint.y < 0)
		pitch -= 180.0f;
}