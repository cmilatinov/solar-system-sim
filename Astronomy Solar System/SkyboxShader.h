#pragma once
#include "Shader.h"
class SkyboxShader :
	public Shader
{
public:
	unsigned int location_projectionMatrix;
	unsigned int location_viewMatrix;
	unsigned int location_modelMatrix;

	SkyboxShader(const char* vertexPath, const char* fragmentPath);
	~SkyboxShader();

	virtual void bindAttributes();
	virtual void getUniformLocations();
};

