#include "Camera.h"
#include "Entity.h"
#include "Display.h"

#pragma once
#include <glfw/glfw3.h>
class CameraThirdPerson : 
	public Camera
{
private:
	float ROT_SPEED = 0.05f;
	
	GLFWwindow * window;

	Vector3f getRightPerpendicularVector(Vector3f vector);
	Vector3f getLeftPerpendicularVector(Vector3f vector);
	Vector3f calculateCameraMovementVector();
	Vector3f applyYawToVector(float yaw, Vector3f vector);
	Vector3f applyPitchToVector(float pitch, Vector3f vector);
	float * solveRectTriangle(float angle, float hypotenuse);
	float toRadians(float angle);

public:
	CameraThirdPerson(GLFWwindow * window, Entity * entityToFollow, float radius);
	~CameraThirdPerson();

	Entity * keyEntity;
	float radius;

	void SetIsLocked(bool locked);

	void moveCamera(float deltaSec);

	Matrix4f * createViewMatrix();
	Matrix4f * createRotationMatrix();

	Vector3f getForwardCameraVector();
	Vector3f getLeftCameraVector();
	Vector3f getRightCameraVector();
	Vector3f getUpwardCameraVector();
	Vector3f getDownwardCameraVector();

	void setEntityToFollow(Entity * ent);
};

