#include "Camera.h"
#include "Display.h"

#pragma once
class CameraFirstPersonNoclip : public Camera {
private:
	float MAX_PITCH = 90, MIN_PITCH = -90;
	float CAM_SPEED = 10.0f;
	float ROT_SPEED = 0.05f;
	GLFWwindow * window;
	bool locked = false;

public:
	CameraFirstPersonNoclip(GLFWwindow * window);
	CameraFirstPersonNoclip(GLFWwindow * window, Vector3f pos);
	~CameraFirstPersonNoclip();

	void moveCamera(float deltaSec);
	Matrix4f * createViewMatrix();
	Matrix4f * createRotationMatrix();

	Vector3f getForwardCameraVector();
	Vector3f getLeftCameraVector();
	Vector3f getRightCameraVector();
	Vector3f getUpwardCameraVector();
	Vector3f getDownwardCameraVector();

	void SetIsLocked(bool locked);

private:
	Vector3f getRightPerpendicularVector(Vector3f vector);
	Vector3f getLeftPerpendicularVector(Vector3f vector);
	Vector3f calculateCameraMovementVector();
	Vector3f addVectors(Vector3f v1, Vector3f v2);
	Vector3f substractVectors(Vector3f v1, Vector3f v2);
	Vector3f applyYawToVector(float yaw, Vector3f vector);
	Vector3f applyPitchToVector(float pitch, Vector3f vector);
	float * solveRectTriangle(float angle, float hypotenuse);
	float toRadians(float angle);

};