#include "Display.h"
#include <iostream>

static int SCR_WIDTH, SCR_HEIGHT, frameWidth, frameHeight;
static bool resizable = false, fullscreen = false, isInFocus = true;

Display::Display(){
	//Defaults
	frameWidth = 1600;
	frameHeight = 900;
	
	glfwGetMonitorPhysicalSize(glfwGetPrimaryMonitor(), &SCR_WIDTH, &SCR_HEIGHT);

}

Display::~Display(){

}

void Display::resizeViewport(GLFWwindow * window, int width, int height) {
	glViewport(0, 0, width, height);
	frameHeight = height;
	frameWidth = width;
}

void Display::focusCallback(GLFWwindow * window, int focus) {
	if (focus == GLFW_TRUE) {
		glfwSetCursorPos(window, frameWidth / 2, frameHeight / 2);
		isInFocus = true;
	}else
		isInFocus = false;
}

void Display::setResizable(bool resize) {
	resizable = resize;
}

void Display::setFullscreen(bool fullscr) {
	fullscreen = fullscr;
}

void Display::setSize(int width, int height) {
	frameWidth = width;
	frameHeight = height;
}

int Display::getWidth() {
	return frameWidth;
}

int Display::getHeight() {
	return frameHeight;
}

bool Display::getIsInFocus() {
	return isInFocus;
}

GLFWwindow * Display::createDisplay(char title[]) {
	GLFWwindow * window = glfwCreateWindow(frameWidth, frameHeight, title, NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(window);
	if (fullscreen)
		glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, SCR_WIDTH, SCR_HEIGHT, GLFW_DONT_CARE);
	if (resizable)
		glfwSetWindowSizeLimits(window, frameWidth, frameHeight, frameWidth, frameHeight);
	glfwSetWindowFocusCallback(window, focusCallback);
	glfwSetWindowAspectRatio(window, 16, 9);
	return window;
}