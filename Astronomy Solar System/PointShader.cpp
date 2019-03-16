#include "PointShader.h"
#include <iostream>

PointShader::PointShader(const char* vertexPath, const char* fragmentPath) : Shader(vertexPath, fragmentPath) {
	bindAttributes();
	glLinkProgram(ID);
	glValidateProgram(ID);
	if (!checkCompileErrors(ID, "PROGRAM"));// exit(-3);
	getUniformLocations();
};

PointShader::~PointShader() {

}

void PointShader::bindAttributes() {
	bindAttribute(0, "pos");
	bindAttribute(1, "color");
}

void PointShader::getUniformLocations() {
	location_projectionMatrix = getUniformLocation("projectionMatrix");
	location_viewMatrix = getUniformLocation("viewMatrix");
}