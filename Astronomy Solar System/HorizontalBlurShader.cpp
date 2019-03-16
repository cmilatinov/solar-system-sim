#include "HorizontalBlurShader.h"
#include <iostream>

HorizontalBlurShader::HorizontalBlurShader(const char* vertexPath, const char* fragmentPath) : Shader(vertexPath, fragmentPath) {
	bindAttributes();
	glLinkProgram(ID);
	glValidateProgram(ID);
	if (!checkCompileErrors(ID, "PROGRAM"));
	getUniformLocations();
};

HorizontalBlurShader::~HorizontalBlurShader() {

}

void HorizontalBlurShader::bindAttributes() {
	bindAttribute(0, "pos");
}

void HorizontalBlurShader::getUniformLocations() {
	location_modelMatrix = getUniformLocation("modelMatrix");
	location_texture = getUniformLocation("tex");
	location_targetWidth = getUniformLocation("targetWidth");
}