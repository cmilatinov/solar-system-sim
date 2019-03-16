#pragma once
#include "Shader.h"
class TextShader :
	public Shader
{
public:
	unsigned int location_modelMatrix;
	unsigned int location_texture;

	TextShader(const char* vertexPath, const char* fragmentPath);
	~TextShader();

	virtual void bindAttributes();
	virtual void getUniformLocations();
};

