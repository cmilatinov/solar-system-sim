#include "TextShader.h"
#include <iostream>

TextShader::TextShader(const char* vertexPath, const char* fragmentPath) : Shader(vertexPath, fragmentPath) {
	bindAttributes();
	glLinkProgram(ID);
	glValidateProgram(ID);
	if (!checkCompileErrors(ID, "PROGRAM"));
	getUniformLocations();
};

TextShader::~TextShader() {

}

void TextShader::bindAttributes() {
	bindAttribute(0, "pos");
	bindAttribute(1, "uv");
}

void TextShader::getUniformLocations() {
	location_modelMatrix = getUniformLocation("modelMatrix");
	location_texture = getUniformLocation("tex");
}