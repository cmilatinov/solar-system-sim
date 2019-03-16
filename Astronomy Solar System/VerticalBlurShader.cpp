#include "VerticalBlurShader.h"
#include <iostream>

VerticalBlurShader::VerticalBlurShader(const char* vertexPath, const char* fragmentPath) : Shader(vertexPath, fragmentPath) {
	bindAttributes();
	glLinkProgram(ID);
	glValidateProgram(ID);
	if (!checkCompileErrors(ID, "PROGRAM"));
	getUniformLocations();
};

VerticalBlurShader::~VerticalBlurShader() {

}
	
void VerticalBlurShader::bindAttributes() {
	bindAttribute(0, "pos");
}

void VerticalBlurShader::getUniformLocations() {
	location_modelMatrix = getUniformLocation("modelMatrix");
	location_texture = getUniformLocation("tex");
	location_targetHeight = getUniformLocation("targetHeight");
}