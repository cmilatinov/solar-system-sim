#pragma once
#include "Shader.h"

class ShapeShader :
	public Shader
{
public:
	unsigned int location_projectionMatrix;
	unsigned int location_viewMatrix;
	unsigned int location_modelMatrix;
	unsigned int location_color;
	unsigned int location_opacity;
	unsigned int location_trailValue;

	ShapeShader(const char* vertexPath, const char* fragmentPath);
	~ShapeShader();

	virtual void bindAttributes();
	virtual void getUniformLocations();
};
