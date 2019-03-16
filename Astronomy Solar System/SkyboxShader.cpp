#include "SkyboxShader.h"
#include <iostream>

SkyboxShader::SkyboxShader(const char* vertexPath, const char* fragmentPath) : Shader(vertexPath, fragmentPath) {
	bindAttributes();
	glLinkProgram(ID);
	glValidateProgram(ID);
	if (!checkCompileErrors(ID, "PROGRAM"));
	getUniformLocations();
};

SkyboxShader::~SkyboxShader() {

}

void SkyboxShader::bindAttributes() {
	bindAttribute(0, "pos");
}

void SkyboxShader::getUniformLocations() {
	location_projectionMatrix = getUniformLocation("projectionMatrix");
	location_viewMatrix = getUniformLocation("viewMatrix");
	location_modelMatrix = getUniformLocation("modelMatrix");
}