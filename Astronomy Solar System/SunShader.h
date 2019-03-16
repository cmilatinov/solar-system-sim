#pragma once
#include "Shader.h"

class SunShader :
	public Shader
{
public:
	unsigned int location_projectionMatrix;
	unsigned int location_viewMatrix;
	unsigned int location_modelMatrix;
	unsigned int location_cameraDirection;

	SunShader(const char* vertexPath, const char* fragmentPath);
	~SunShader();

	virtual void bindAttributes();
	virtual void getUniformLocations();
};
