#include "ContrastShader.h"
#include <iostream>

ContrastShader::ContrastShader(const char* vertexPath, const char* fragmentPath) : Shader(vertexPath, fragmentPath) {
	bindAttributes();
	glLinkProgram(ID);
	glValidateProgram(ID);
	if (!checkCompileErrors(ID, "PROGRAM")) exit(-3);
	getUniformLocations();
};

ContrastShader::~ContrastShader() {

}

void ContrastShader::bindAttributes() {
	bindAttribute(0, "pos");
	bindAttribute(1, "uv");
}

void ContrastShader::getUniformLocations() {
	location_contrast = getUniformLocation("contrast");
}
