#include "SunShader.h"
#include <iostream>

SunShader::SunShader(const char* vertexPath, const char* fragmentPath) : Shader(vertexPath, fragmentPath) {
	bindAttributes();
	glLinkProgram(ID);
	glValidateProgram(ID);
	if (!checkCompileErrors(ID, "PROGRAM"));// exit(-3);
	getUniformLocations();
};

SunShader::~SunShader() {

}

void SunShader::bindAttributes() {
	bindAttribute(0, "pos");
	bindAttribute(1, "normal");
	bindAttribute(2, "uv");
}

void SunShader::getUniformLocations() {
	location_projectionMatrix = getUniformLocation("projectionMatrix");
	location_viewMatrix = getUniformLocation("viewMatrix");
	location_modelMatrix = getUniformLocation("modelMatrix");
	location_cameraDirection = getUniformLocation("cameraDirection");
}