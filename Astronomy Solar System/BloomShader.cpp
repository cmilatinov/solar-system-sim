#include "BloomShader.h"
#include <iostream>

BloomShader::BloomShader(const char* vertexPath, const char* fragmentPath) : Shader(vertexPath, fragmentPath) {
	bindAttributes();
	glLinkProgram(ID);
	glValidateProgram(ID);
	if (!checkCompileErrors(ID, "PROGRAM"));
	getUniformLocations();
};

BloomShader::~BloomShader() {

}

void BloomShader::bindAttributes() {
	bindAttribute(0, "pos");
	bindAttribute(1, "uv");
}

void BloomShader::getUniformLocations() {
	location_sceneTexture = getUniformLocation("sceneTexture");
	location_bloomTexture = getUniformLocation("bloomTexture");
}