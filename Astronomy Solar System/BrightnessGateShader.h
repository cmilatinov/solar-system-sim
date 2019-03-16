#pragma once
#include "Shader.h"
class BrightnessGateShader :
	public Shader
{
public:
	unsigned int location_texture;

	BrightnessGateShader(const char* vertexPath, const char* fragmentPath);
	~BrightnessGateShader();

	virtual void bindAttributes();
	virtual void getUniformLocations();
};

