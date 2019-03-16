#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <glad\glad.h>
#include <GLFW/glfw3.h>
#include "VectorMatrix.h"
#pragma once

class Shader
{
public:
	unsigned int ID, vertex, fragment, geometry;

	Shader(const char* vertexPath, const char* fragmentPath);
	Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath);
	~Shader();

	void use();
	void stop();
	void bindAttribute(int attribute, char varName[]);
	unsigned int getUniformLocation(char name[]);

	virtual void bindAttributes() {}
	virtual void getUniformLocations() {};

	void loadMatrix(unsigned int location, Matrix4f *mat);
	void loadVector(unsigned int location, Vector3f *vec);
	void loadVector(unsigned int location, Vector4f *vec);
	void loadBoolean(unsigned int location, bool boolean);
	void loadInt(unsigned int location, int value);
	void loadFloat(unsigned int location, float value);
	void loadDouble(unsigned int location, double value);

protected:
	bool checkCompileErrors(unsigned int shader, std::string type);
};