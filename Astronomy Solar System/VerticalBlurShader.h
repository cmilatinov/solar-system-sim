#pragma once
#include "Shader.h"
class VerticalBlurShader :
	public Shader
{
public:
	unsigned int location_modelMatrix;
	unsigned int location_texture;
	unsigned int location_targetHeight;

	VerticalBlurShader(const char* vertexPath, const char* fragmentPath);
	~VerticalBlurShader();

	virtual void bindAttributes();
	virtual void getUniformLocations();
};

