#include "BrightnessGateShader.h"
#include <iostream>

BrightnessGateShader::BrightnessGateShader(const char* vertexPath, const char* fragmentPath) : Shader(vertexPath, fragmentPath) {
	bindAttributes();
	glLinkProgram(ID);
	glValidateProgram(ID);
	if (!checkCompileErrors(ID, "PROGRAM"));
	getUniformLocations();
};

BrightnessGateShader::~BrightnessGateShader() {

}

void BrightnessGateShader::bindAttributes() {
	bindAttribute(0, "pos");
	bindAttribute(1, "uv");
}

void BrightnessGateShader::getUniformLocations() {
	location_texture = getUniformLocation("tex");
}