#pragma once
#include "Shader.h"
class HorizontalBlurShader :
	public Shader
{
public:
	unsigned int location_modelMatrix;
	unsigned int location_texture;
	unsigned int location_targetWidth;

	HorizontalBlurShader(const char* vertexPath, const char* fragmentPath);
	~HorizontalBlurShader();

	virtual void bindAttributes();
	virtual void getUniformLocations();
};

