#include "StaticShader.h"
#include <iostream>

StaticShader::StaticShader(const char* vertexPath, const char* fragmentPath) : Shader(vertexPath, fragmentPath) {
	bindAttributes();
	glLinkProgram(ID);
	glValidateProgram(ID);
	if (!checkCompileErrors(ID, "PROGRAM"));// exit(-3);
	getUniformLocations();
};

StaticShader::~StaticShader(){

}

void StaticShader::bindAttributes() {
	bindAttribute(0, "pos");
	bindAttribute(1, "normal");
	bindAttribute(2, "uv");
	bindAttribute(3, "color");
	bindAttribute(4, "modelMatrix");
}

void StaticShader::getUniformLocations() {
	location_projectionMatrix = getUniformLocation("projectionMatrix");
	location_viewMatrix = getUniformLocation("viewMatrix");
	location_modelMatrix = getUniformLocation("modelMatrix");
	location_texture = getUniformLocation("tex");
	location_clipPlane = getUniformLocation("clipPlane");
	location_needsTexture = getUniformLocation("needsTexture");
	for (int i = 0; i < MAX_LIGHTS; i++) {
		char * num = new char[2];
		sprintf(num, "%d", i);
		std::string pos = std::string("lightPosition[").append(num).append("]");
		char * strPos = (char*)pos.c_str();
		std::string color = std::string("lightColor[").append(num).append("]");
		char * strColor = (char*)color.c_str();
		location_lightPosition[i] = getUniformLocation(strPos);
		location_lightColor[i] = getUniformLocation(strColor);
	}
}

void StaticShader::loadLight(int i, Light * light) {
	this->loadVector(location_lightPosition[i], &light->getPosition());
	this->loadVector(location_lightColor[i], &light->getColor());
}