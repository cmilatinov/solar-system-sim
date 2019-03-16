#include "CameraFirstPersonNoclip.h"

#include "Input.h"

#include <math.h>
#include <iostream>



CameraFirstPersonNoclip::CameraFirstPersonNoclip(GLFWwindow * window) : Camera(){
	this->window = window;
}

CameraFirstPersonNoclip::~CameraFirstPersonNoclip() {

}

CameraFirstPersonNoclip::CameraFirstPersonNoclip(GLFWwindow * window, Vector3f pos) : Camera(pos){
	this->window = window;
}

void CameraFirstPersonNoclip::moveCamera(float deltaSec) {
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) 
		position = addVectors(position, deltaSec * calculateCameraMovementVector());
	
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) 
		position = addVectors(position, getRightPerpendicularVector(applyYawToVector(yaw, Vector3f(0, 0, deltaSec * CAM_SPEED))));
	
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) 
		position = addVectors(position, getLeftPerpendicularVector(applyYawToVector(yaw, Vector3f(0, 0, deltaSec * CAM_SPEED))));
	
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) 
		position = substractVectors(position, deltaSec * calculateCameraMovementVector());
	
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) 
		position.y += deltaSec * CAM_SPEED;
	
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) 
		position.y -= deltaSec * CAM_SPEED;
	
	if (locked) 
		return;

	if (!Input::isGrabCursor())
		return;

	int sizeX = Display::getWidth(), sizeY = Display::getHeight();
	sizeX /= 2;
	sizeY /= 2;
	double dx, dy;
	glfwGetCursorPos(window, &dx, &dy);
	dx -= sizeX;
	dy -= sizeY;
	dy = -dy;

	if (dx != 0) {
		yaw += ROT_SPEED * dx;
	}

	if (dy < 0) {
		if (pitch - (ROT_SPEED * dy)  > -MIN_PITCH)
			pitch = -MIN_PITCH;
		else
			pitch += -(ROT_SPEED * dy);
	}
	if (dy > 0) {
		if (pitch - (ROT_SPEED * dy)  < -MAX_PITCH)
			pitch = -MAX_PITCH;
		else
			pitch += -(ROT_SPEED * dy);
	}

	yaw = fmod(yaw, 360);
	pitch = fmod(pitch, 360);
}

Vector3f CameraFirstPersonNoclip::getRightPerpendicularVector(Vector3f vector) {
	float vecX = vector.x;
	float vecZ = vector.z;
	Vector3f parallel = Vector3f(-vecZ, 0, vecX);
	return parallel;
}

Vector3f CameraFirstPersonNoclip::getLeftPerpendicularVector(Vector3f vector) {
	float vecX = vector.x;
	float vecZ = vector.z;
	Vector3f parallel = Vector3f(vecZ, 0, -vecX);
	return parallel;
}

Vector3f CameraFirstPersonNoclip::calculateCameraMovementVector() {
	Vector3f movement = applyPitchToVector(-pitch, applyYawToVector(yaw, Vector3f(0, 0, CAM_SPEED)));
	return movement;
}

Vector3f CameraFirstPersonNoclip::addVectors(Vector3f v1, Vector3f v2) {
	return Vector3f(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

Vector3f CameraFirstPersonNoclip::substractVectors(Vector3f v1, Vector3f v2) {
	Vector3f vec = Vector3f(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
	return vec;
}


Vector3f CameraFirstPersonNoclip::applyYawToVector(float yaw, Vector3f vector) {
	yaw = fmod(yaw, 360);
	if (yaw < 0)
		yaw += 360;
	int quarter = (int)floor(yaw / 90);
	float relativeYaw = fmod(yaw, 90);
	float *sidesHoriz = solveRectTriangle(relativeYaw, vector.length());
	if (quarter == 0) {
		vector = Vector3f(sidesHoriz[0], 0, -sidesHoriz[1]);
	}
	else if (quarter == 1) {
		vector = Vector3f(sidesHoriz[1], 0, sidesHoriz[0]);
	}
	else if (quarter == 2) {
		vector = Vector3f(-sidesHoriz[0], 0, sidesHoriz[1]);
	}
	else if (quarter == 3) {
		vector = Vector3f(-sidesHoriz[1], 0, -sidesHoriz[0]);
	}
	return vector;
}

Vector3f CameraFirstPersonNoclip::applyPitchToVector(float pitch, Vector3f vector) {
	float length = vector.length();
	float *sidesVertical = solveRectTriangle(abs(pitch), length);
	if (pitch > 0) {
		vector.y = sidesVertical[0];
		float proportion = sidesVertical[1] / length;
		vector.x *= proportion;
		vector.z *= proportion;
	}
	else if (pitch < 0) {
		vector.y = -sidesVertical[0];
		float proportion = sidesVertical[1] / length;
		vector.x *= proportion;
		vector.z *= proportion;
	}
	return vector;
}

float * CameraFirstPersonNoclip::solveRectTriangle(float angle, float hypotenuse) {
	float oppositeSide = 0;
	float adjacentSide = 0;
	oppositeSide = hypotenuse * sin(toRadians(angle));
	adjacentSide = hypotenuse * cos(toRadians(angle));
	float answer[] = { oppositeSide, adjacentSide };
	return answer;
}

float CameraFirstPersonNoclip::toRadians(float angle) {
	return angle * PI / 180;
}

Matrix4f * CameraFirstPersonNoclip::createViewMatrix() {
	Matrix4f * viewMatrix = new Matrix4f();
	viewMatrix->SetIdentity();
	viewMatrix->Rotate(pitch, Vector3f(1, 0, 0));
	viewMatrix->Rotate(yaw, Vector3f(0, 1, 0));
	viewMatrix->Rotate(roll, Vector3f(0, 0, 1));
	Vector3f negativeCameraPos = Vector3f(-position.x, -position.y, -position.z);
	viewMatrix->Translate(negativeCameraPos);
	return viewMatrix;
}

Matrix4f * CameraFirstPersonNoclip::createRotationMatrix()
{
	Matrix4f * viewMatrix = new Matrix4f();
	viewMatrix->SetIdentity();
	viewMatrix->Rotate(pitch, Vector3f(1, 0, 0));
	viewMatrix->Rotate(yaw, Vector3f(0, 1, 0));
	viewMatrix->Rotate(roll, Vector3f(0, 0, 1));
	return viewMatrix;
}

Vector3f CameraFirstPersonNoclip::getForwardCameraVector() {
	return calculateCameraMovementVector();
}

Vector3f CameraFirstPersonNoclip::getLeftCameraVector() {
	return getLeftPerpendicularVector(getForwardCameraVector());
}

Vector3f CameraFirstPersonNoclip::getRightCameraVector() {
	return getRightPerpendicularVector(getForwardCameraVector());
}

Vector3f CameraFirstPersonNoclip::getUpwardCameraVector() {
	Matrix4f viewMatrix = Matrix4f();
	viewMatrix.SetIdentity();
	viewMatrix.Rotate(pitch, Vector3f(1, 0, 0));
	viewMatrix.Rotate(yaw, Vector3f(0, 1, 0));
	return viewMatrix * Vector3f(0, 1, 0);
}

Vector3f CameraFirstPersonNoclip::getDownwardCameraVector() {
	Matrix4f viewMatrix = Matrix4f();
	viewMatrix.SetIdentity();
	viewMatrix.Rotate(pitch, Vector3f(1, 0, 0));
	viewMatrix.Rotate(yaw, Vector3f(0, 1, 0));
	return viewMatrix * Vector3f(0, -1, 0);
}

void CameraFirstPersonNoclip::SetIsLocked(bool locked){
	this->locked = locked;
}
