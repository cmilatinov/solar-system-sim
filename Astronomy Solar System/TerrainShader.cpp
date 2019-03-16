#include "TerrainShader.h"
#include <iostream>

TerrainShader::TerrainShader(const char* vertexPath, const char* fragmentPath) : Shader(vertexPath, fragmentPath) {
	bindAttributes();
	glLinkProgram(ID);
	glValidateProgram(ID);
	if (!checkCompileErrors(ID, "PROGRAM"));
	getUniformLocations();
};

TerrainShader::~TerrainShader() {

}

void TerrainShader::bindAttributes() {
	bindAttribute(0, "pos");
	bindAttribute(1, "normal");
	bindAttribute(2, "color"); 
	bindAttribute(3, "reflectivity");
}

void TerrainShader::getUniformLocations() {
	location_projectionMatrix = getUniformLocation("projectionMatrix");
	location_viewMatrix = getUniformLocation("viewMatrix");
	location_modelMatrix = getUniformLocation("modelMatrix");
	location_clipPlane = getUniformLocation("clipPlane");
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

void TerrainShader::loadLight(int i, Light * light) {
	this->loadVector(location_lightPosition[i], &light->getPosition());
	this->loadVector(location_lightColor[i], &light->getColor());
}