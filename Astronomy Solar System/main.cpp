#define _WIN32_WINNT 0x0601

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <time.h>
#include <iostream>
#include <chrono>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

#include "Display.h"
#include "Engine.h"
#include "Loader.h"
#include "Input.h"
#include "FBO.h"
#include "Color.h"
#include "GuiManager.h"
#include "INIReader.h"

void ReadConfig(const char * config);
void initGLFW();
void initGLAD();
void resize(GLFWwindow * window, int width, int height);
void processKey(GLFWwindow * window, int key, int scancode, int action, int mods);
void processClick(GLFWwindow * window, int button, int action, int mods);
void scrollCallback(GLFWwindow * window, double xOffset, double yOffset);
void gotoxy(int x, int y);

// settings
const float timePerTick = (1.0f / 70.0f);
const bool hideConsoleWindow = false;
static bool cameraSnapped = false;
static int scrollFactor = 0;
static float lastScale = 0.0f;
float fov;
double asteroidMass;
int msaaSamples;
int windowWidth;
int windowHeight;


Display * display;
GLFWwindow * window;

Engine * engine;
Loader * loader;
Input * input;
Camera * camera;

Vector3f camPos;
Vector3f camRot;

DynamicTextMesh * fpsCounter;

int main()
{	
	//HIDE CONSOLE
	if (hideConsoleWindow) 
		FreeConsole();

	//INIT GLFW
	initGLFW();

	//READ CONFIG
	ReadConfig("config.ini");
	//std::cout << "FOV : " << fov << std::endl;
	//std::cout << "MSAA : x" << msaaSamples << std::endl;
	//std::cout << "Asteroid Mass : " << asteroidMass << " kg" << std::endl;

	//CREATE DISPLAY
	display = new Display();
	display->setResizable(false);
	display->setFullscreen(false);
	display->setSize(windowWidth, windowHeight);
	window = display->createDisplay("Solar System Simulation");
	glfwSetFramebufferSizeCallback(window, resize);
	
	//INIT GLAD
	initGLAD();
	loader = new Loader();
	camera = new CameraFirstPersonNoclip(window);
	camera->setCameraRotation(90, 0, 0);
	camera->setPosition(Vector3f(00, 50, 0));
	engine = (new Engine(window, loader, (float)display->getWidth(), (float)display->getHeight(), fov, msaaSamples))->SetContrast(0.0f);
	input = new Input(window, engine, camera);
	input->setGrabCursor(true);
	input->astMass = asteroidMass;

	fpsCounter = new DynamicTextMesh(engine->fLoader->loadString("1", engine->f, engine->textSize, Vector2f()));
	engine->textManager->addText(fpsCounter);


	glfwSetKeyCallback(window, processKey);
	glfwSetMouseButtonCallback(window, processClick);
	glfwSetScrollCallback(window, scrollCallback);

	gotoxy(0, 3);
	printf("OpenGL Version : %s", glGetString(GL_VERSION));

	int fps = 0, ticksPerSec = 0;
	long lastTime = clock();
	long lastUpdate = clock();
	auto lastFrame = std::chrono::high_resolution_clock::now();
	while (!glfwWindowShouldClose(window)){
		long currentTime = clock();
		if (currentTime - lastTime >= timePerTick * CLOCKS_PER_SEC) {
			
			//INPUT
			input->processInput(display->getWidth(), display->getHeight(), engine);

			ticksPerSec++;
			lastTime = clock();
		}
		//INPUT
		input->tick();

		//INFO UPDATE
		long currentUpdate = clock();
		if (currentUpdate - lastUpdate >= CLOCKS_PER_SEC) {
			gotoxy(0, 0);
			printf("FPS :  %d\n", fps);
			printf("TICKS :  %d\n", ticksPerSec);

			if (fps != stoi(fpsCounter->textString)) {
				fpsCounter->setText(" " + std::to_string(fps) + " ", engine->fLoader);
				fpsCounter->setScreenPosition(Vector2f());
			}

			ticksPerSec = 0;
			fps = 0;
			lastUpdate = clock();
		}
		
		//DELTA TIME CALCULATION
		auto currentFrame = std::chrono::high_resolution_clock::now();
		float deltaSec = std::chrono::duration<double>(currentFrame - lastFrame).count();
		gotoxy(0, 2);
		printf("Delta Time :  %f\n\n", deltaSec);
		
		//CAMERA
		if (display->getIsInFocus())
			camera->moveCamera(deltaSec);
		
		engine->RenderScene(camera, deltaSec);

		engine->UpdateDisplay();

		lastFrame = currentFrame;

		fps++;
	}

	delete input; 
	//delete mr; 
	delete loader; 
	delete camera;
	glfwTerminate();
	return 0;
}

void initGLAD() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		exit(-1);
	}
}

void initGLFW() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
}

void resize(GLFWwindow * window, int width, int height) {
	Display::resizeViewport(window, width, height);
	engine->eManager->resizeTexts(engine->fLoader);
	engine->generalInfo->setText(engine->generalInfo->textString, engine->fLoader);
	engine->generalInfo->setLowerRightCornerPos(Vector2f(Display::getWidth(), Display::getHeight()));
}

void processClick(GLFWwindow * window, int button, int action, int mods) {
	input->processMouseEvent(window, button, action, mods);
}

void processKey(GLFWwindow * window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_G && action == GLFW_RELEASE)
		fpsCounter->isOnScreen = !fpsCounter->isOnScreen;
	
	if (!cameraSnapped) {
		camPos = camera->getPosition();
		float * rot = camera->getCameraRotation();
		camRot = Vector3f(rot[0], rot[1], rot[2]);
	}
	if (key == GLFW_KEY_F1 && action == GLFW_RELEASE) {
		scrollFactor = 0;
		cameraSnapped = true;
		delete camera;
		lastScale = engine->mercuryEntity->getScale() * 20.0f;
		camera = new CameraThirdPerson(window, engine->mercuryEntity, lastScale);
		input->setCamera(camera);
		engine->activePlanetInfo = 0;
	}else if (key == GLFW_KEY_F2 && action == GLFW_RELEASE) {
		scrollFactor = 0;
		cameraSnapped = true;
		delete camera;
		lastScale = engine->venusEntity->getScale() * 20.0f;
		camera = new CameraThirdPerson(window, engine->venusEntity, lastScale);
		input->setCamera(camera);
		engine->activePlanetInfo = 1;
	}else if (key == GLFW_KEY_F3 && action == GLFW_RELEASE) {
		scrollFactor = 0;
		cameraSnapped = true;
		delete camera;
		lastScale = engine->earthEntity->getScale() * 20.0f;
		camera = new CameraThirdPerson(window, engine->earthEntity, lastScale);
		input->setCamera(camera);
		engine->activePlanetInfo = 2;
	}else if (key == GLFW_KEY_F4 && action == GLFW_RELEASE) {
		scrollFactor = 0;
		cameraSnapped = true;
		delete camera;
		lastScale = engine->marsEntity->getScale() * 20.0f;
		camera = new CameraThirdPerson(window, engine->marsEntity, lastScale);
		input->setCamera(camera);
		engine->activePlanetInfo = 3;
	}else if (key == GLFW_KEY_F5 && action == GLFW_RELEASE) {
		scrollFactor = 0;
		cameraSnapped = true;
		delete camera;
		lastScale = engine->jupiterEntity->getScale() * 20.0f;
		camera = new CameraThirdPerson(window, engine->jupiterEntity, lastScale);
		input->setCamera(camera);
		engine->activePlanetInfo = 4;
	}else if (key == GLFW_KEY_F6 && action == GLFW_RELEASE) {
		scrollFactor = 0;
		cameraSnapped = true;
		delete camera;
		lastScale = engine->saturnEntity->getScale() * 20.0f;
		camera = new CameraThirdPerson(window, engine->saturnEntity, lastScale);
		input->setCamera(camera);
		engine->activePlanetInfo = 5;
	}else if (key == GLFW_KEY_F7 && action == GLFW_RELEASE) {
		scrollFactor = 0;
		cameraSnapped = true;
		delete camera;
		lastScale = engine->uranusEntity->getScale() * 20.0f;
		camera = new CameraThirdPerson(window, engine->uranusEntity, lastScale);
		input->setCamera(camera);
		engine->activePlanetInfo = 6;
	}else if (key == GLFW_KEY_F8 && action == GLFW_RELEASE) {
		scrollFactor = 0;
		cameraSnapped = true;
		delete camera;
		lastScale = engine->neptuneEntity->getScale() * 20.0f;
		camera = new CameraThirdPerson(window, engine->neptuneEntity, lastScale);
		input->setCamera(camera);
		engine->activePlanetInfo = 7;
	}else if (key == GLFW_KEY_F9 && action == GLFW_RELEASE) {
		cameraSnapped = false;
		delete camera;
		camera = new CameraFirstPersonNoclip(window);
		camera->setCameraRotation(camRot);
		camera->setPosition(camPos);
		input->setCamera(camera);
		engine->activePlanetInfo = -1;
	}else
		input->processKeyEvent(window, key, scancode, action, mods);

	if (key == GLFW_KEY_F && action == GLFW_RELEASE) {
		if(engine->activePlanetInfo >= 0)
			lastScale = engine->planets.at(engine->activePlanetInfo)->getScale() * 20.0f;
		scrollCallback(window, 0, 0);
	}
}

void scrollCallback(GLFWwindow * window, double xOffset, double yOffset) {
	scrollFactor -= yOffset;
	if (!Input::isToScale()) {
		if (scrollFactor < -2)
			scrollFactor = -2;
	}else
		if (scrollFactor < 0)
			scrollFactor = 0;

	if (ClassUtils::instanceof<CameraThirdPerson>(camera)) {
		((CameraThirdPerson*)camera)->radius = lastScale + scrollFactor * 5.0f * ((CameraThirdPerson*)camera)->keyEntity->getScale();
		((CameraThirdPerson*)camera)->moveCamera(0.0f);
	}
}

void ReadConfig(const char * config) {
	INIReader reader(config);
	if (reader.ParseError() < 0) {
		std::cout << "Can't load settings from : " << "\'" << config << "\'" << std::endl;
		std::cout << "Using defaults" << std::endl;
		fov = 70.0f;
		msaaSamples =  0;
		asteroidMass = 11e+27;
		windowWidth = 1280;
		windowHeight = 720;
		return;
	}else {
		fov = reader.GetReal("Settings", "fov", 70.0);
		msaaSamples = reader.GetInteger("Settings", "msaa", 0);
		asteroidMass = reader.GetReal("Settings", "asteroidMass", 11e+27);
		windowWidth = reader.GetInteger("Frame", "width", 1280);
		windowHeight = reader.GetInteger("Frame", "height", 720);
	}
}

void gotoxy(int x, int y) {
	COORD coord;
	coord.X = x; coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	return;
}
