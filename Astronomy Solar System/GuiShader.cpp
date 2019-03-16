#include "GuiShader.h"
#include <iostream>

GuiShader::GuiShader(const char* vertexPath, const char* fragmentPath) : Shader(vertexPath, fragmentPath) {
	bindAttributes();
	glLinkProgram(ID);
	glValidateProgram(ID);
	if (!checkCompileErrors(ID, "PROGRAM"));
	getUniformLocations();
};

GuiShader::~GuiShader() {

}

void GuiShader::bindAttributes() {
	bindAttribute(0, "pos");
	bindAttribute(1, "uv");
}

void GuiShader::getUniformLocations() {
	location_modelMatrix = getUniformLocation("modelMatrix");
	location_texture = getUniformLocation("tex");
}