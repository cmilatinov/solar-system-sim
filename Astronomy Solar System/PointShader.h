#pragma once
#include "Shader.h"

class PointShader :
	public Shader
{
public:
	unsigned int location_projectionMatrix;
	unsigned int location_viewMatrix;

	PointShader(const char* vertexPath, const char* fragmentPath);
	~PointShader();

	virtual void bindAttributes();
	virtual void getUniformLocations();
};
