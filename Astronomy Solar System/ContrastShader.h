#pragma once
#include "Shader.h"
#include "Light.h"

#define MAX_LIGHTS 1

class ContrastShader :
	public Shader
{	
public:
	unsigned int location_contrast;

	ContrastShader(const char* vertexPath, const char* fragmentPath);
	~ContrastShader();

	virtual void bindAttributes();
	virtual void getUniformLocations();
};

