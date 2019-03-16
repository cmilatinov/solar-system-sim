#pragma once
#include "Shader.h"
class GuiShader :
	public Shader
{
public:
	unsigned int location_modelMatrix;
	unsigned int location_texture;

	GuiShader(const char* vertexPath, const char* fragmentPath);
	~GuiShader();

	virtual void bindAttributes();
	virtual void getUniformLocations();
};

