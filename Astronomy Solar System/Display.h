#include <glad/glad.h>
#include <glfw/glfw3.h>

#pragma once
class Display
{
public:
	Display();
	~Display();

	void setSize(int width, int height);
	static void resizeViewport(GLFWwindow * window, int width, int height);
	static void focusCallback(GLFWwindow * window, int focus);
	void setResizable(bool resizable);
	void setFullscreen(bool resizable);
	GLFWwindow * createDisplay(char title[]);
	bool getIsInFocus();

	static int getWidth();
	static int getHeight();
};

