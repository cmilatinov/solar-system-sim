#include "WaterShader.h"
#include <iostream>

WaterShader::WaterShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath) : Shader(vertexPath, fragmentPath, geometryPath) {
	bindAttributes();
	glLinkProgram(ID);
	glValidateProgram(ID);
	if (!checkCompileErrors(ID, "PROGRAM"));
	getUniformLocations();
};

WaterShader::WaterShader(const char* vertexPath, const char* fragmentPath) : Shader(vertexPath, fragmentPath) {
	bindAttributes();
	glLinkProgram(ID);
	glValidateProgram(ID);
	if (!checkCompileErrors(ID, "PROGRAM"));
	getUniformLocations();
};

WaterShader::~WaterShader() {

}

void WaterShader::bindAttributes() {
	bindAttribute(0, "pos");
}

void WaterShader::getUniformLocations() {
	location_projectionMatrix = getUniformLocation("projectionMatrix");
	location_viewMatrix = getUniformLocation("viewMatrix");
	location_modelMatrix = getUniformLocation("modelMatrix");
	location_time = getUniformLocation("time");
	location_reflectionTexture = getUniformLocation("reflectionTexture");
	location_refractionTexture = getUniformLocation("refractionTexture");
	location_depthTexture = getUniformLocation("depthTexture");
	location_perspectiveProjectionMatrix = getUniformLocation("perspectiveProjectionMatrix");
	location_cameraViewMatrix = getUniformLocation("cameraViewMatrix");
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

void WaterShader::loadLight(int i, Light * light) {
	this->loadVector(location_lightPosition[i], &light->getPosition());
	this->loadVector(location_lightColor[i], &light->getColor());
}