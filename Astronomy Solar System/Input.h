#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Engine.h"
#include "Asteroid.h"
#pragma once
class Input
{
private:
	
	GLFWwindow * window;
	Engine * engine;
	Camera * camera;

	bool checkPointIsInFrustum(Camera * cam, Vector3f point);

public:
	Input(GLFWwindow * window, Engine * engine, Camera * camera);
	~Input();

	static bool isGrabCursor();
	void setGrabCursor(bool bGrab);
	void processInput(int w, int h, Engine * engine);

	void processKeyEvent(GLFWwindow * window, int key, int scancode, int action, int mods);
	void processMouseEvent(GLFWwindow* window, int button, int action, int mods);

	void setCamera(Camera * camera);

	void tick();
	void setRenderOrbits(bool value);

	void createOrbit(Vector3f pos, Vector3f vel, float mass);
	Asteroid * createAsteroid(Entity * ent, Orbit * orbit, float meanAnomaly, std::string name);

	static bool isToScale();

	double astMass = 0.0;
};

