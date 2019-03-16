#include "VectorMatrix.h"
#pragma once
class Camera
{
protected:
	Vector3f position;
	float pitch;
	float yaw;
	float roll;

public:
	Camera();

	Camera(Vector3f pos);

	~Camera();

	virtual void moveCamera(float deltaSec) = 0;
	virtual Matrix4f * createViewMatrix() = 0;
	virtual Matrix4f * createRotationMatrix() = 0;

	virtual Vector3f getForwardCameraVector() = 0;
	virtual Vector3f getLeftCameraVector() = 0;
	virtual Vector3f getRightCameraVector() = 0;
	virtual Vector3f getUpwardCameraVector() = 0;
	virtual Vector3f getDownwardCameraVector() = 0;

	virtual void SetIsLocked(bool locked) = 0;

	void LookAt(Vector3f point);

	float getPitch();
	float getYaw();
	float getRoll();
	float * getCameraRotation();
	void setPosition(Vector3f position);
	void setCameraRotation(Vector3f rotation);
	void setCameraRotation(float rx, float ry, float rz);
	void setCameraRotation(float rot[]);
	Vector3f getPosition();

	void invertPitch();
	void invertRoll();
};

