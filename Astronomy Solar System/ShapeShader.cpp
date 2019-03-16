#include "ShapeShader.h"
#include <iostream>

ShapeShader::ShapeShader(const char* vertexPath, const char* fragmentPath) : Shader(vertexPath, fragmentPath) {
	bindAttributes();
	glLinkProgram(ID);
	glValidateProgram(ID);
	if (!checkCompileErrors(ID, "PROGRAM"));// exit(-3);
	getUniformLocations();
};

ShapeShader::~ShapeShader() {

}

void ShapeShader::bindAttributes() {
	bindAttribute(0, "pos");
	bindAttribute(1, "trailValue");
}

void ShapeShader::getUniformLocations() {
	location_projectionMatrix = getUniformLocation("projectionMatrix");
	location_viewMatrix = getUniformLocation("viewMatrix");
	location_modelMatrix = getUniformLocation("modelMatrix");
	location_color = getUniformLocation("color");
	location_opacity = getUniformLocation("opacity");
	location_trailValue = getUniformLocation("usesTrailValues");
}