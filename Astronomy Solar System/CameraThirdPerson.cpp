#include "CameraThirdPerson.h"
#include "Input.h"


CameraThirdPerson::CameraThirdPerson(GLFWwindow * window, Entity * entityToFollow, float radius) : Camera(){
	this->window = window;
	this->keyEntity = entityToFollow;
	this->radius = radius;	
}

CameraThirdPerson::~CameraThirdPerson(){
}

void CameraThirdPerson::SetIsLocked(bool locked){
}

void CameraThirdPerson::setEntityToFollow(Entity * ent) {
	this->keyEntity = ent;
}

void CameraThirdPerson::moveCamera(float deltaSec){

	if (Input::isGrabCursor()) {
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
			pitch -= (ROT_SPEED * dy);
		}
		if (dy > 0) {
			pitch -= (ROT_SPEED * dy);
		}
	}
	Vector3f dir = getForwardCameraVector().Normalize();
	position = keyEntity->getPosition() - radius * dir;

	yaw = fmod(yaw, 360);
	pitch = fmod(pitch, 360);
}

Matrix4f * CameraThirdPerson::createViewMatrix(){
	Matrix4f * viewMatrix = new Matrix4f();
	viewMatrix->SetIdentity();
	viewMatrix->Rotate(pitch, Vector3f(1, 0, 0));
	viewMatrix->Rotate(yaw, Vector3f(0, 1, 0));
	viewMatrix->Rotate(roll, Vector3f(0, 0, 1));
	Vector3f negativeCameraPos = Vector3f(-position.x, -position.y, -position.z);
	viewMatrix->Translate(negativeCameraPos);
	return viewMatrix;
}

Matrix4f * CameraThirdPerson::createRotationMatrix(){
	Matrix4f * viewMatrix = new Matrix4f();
	viewMatrix->SetIdentity();
	viewMatrix->Rotate(pitch, Vector3f(1, 0, 0));
	viewMatrix->Rotate(yaw, Vector3f(0, 1, 0));
	viewMatrix->Rotate(roll, Vector3f(0, 0, 1));
	return viewMatrix;
}

Vector3f CameraThirdPerson::getForwardCameraVector() {
	return calculateCameraMovementVector();
}

Vector3f CameraThirdPerson::getLeftCameraVector() {
	return getLeftPerpendicularVector(getForwardCameraVector());
}

Vector3f CameraThirdPerson::getRightCameraVector() {
	return getRightPerpendicularVector(getForwardCameraVector());
}

Vector3f CameraThirdPerson::getUpwardCameraVector() {
	Matrix4f viewMatrix = Matrix4f();
	viewMatrix.SetIdentity();
	viewMatrix.Rotate(pitch, Vector3f(1, 0, 0));
	viewMatrix.Rotate(yaw, Vector3f(0, 1, 0));
	return viewMatrix * Vector3f(0, 1, 0);
}

Vector3f CameraThirdPerson::getDownwardCameraVector() {
	Matrix4f viewMatrix = Matrix4f();
	viewMatrix.SetIdentity();
	viewMatrix.Rotate(pitch, Vector3f(1, 0, 0));
	viewMatrix.Rotate(yaw, Vector3f(0, 1, 0));
	return viewMatrix * Vector3f(0, -1, 0);
}

Vector3f CameraThirdPerson::getRightPerpendicularVector(Vector3f vector) {
	float vecX = vector.x;
	float vecZ = vector.z;
	Vector3f parallel = Vector3f(-vecZ, 0, vecX);
	return parallel;
}

Vector3f CameraThirdPerson::getLeftPerpendicularVector(Vector3f vector) {
	float vecX = vector.x;
	float vecZ = vector.z;
	Vector3f parallel = Vector3f(vecZ, 0, -vecX);
	return parallel;
}

Vector3f CameraThirdPerson::applyYawToVector(float yaw, Vector3f vector) {
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

Vector3f CameraThirdPerson::applyPitchToVector(float pitch, Vector3f vector) {
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

float * CameraThirdPerson::solveRectTriangle(float angle, float hypotenuse) {
	float oppositeSide = 0;
	float adjacentSide = 0;
	oppositeSide = hypotenuse * sin(toRadians(angle));
	adjacentSide = hypotenuse * cos(toRadians(angle));
	float answer[] = { oppositeSide, adjacentSide };
	return answer;
}

float CameraThirdPerson::toRadians(float angle) {
	return angle * PI / 180;
}

Vector3f CameraThirdPerson::calculateCameraMovementVector() {
	Vector3f movement = applyPitchToVector(-pitch, applyYawToVector(yaw, Vector3f(0, 0, 1.0f)));
	return movement;
}