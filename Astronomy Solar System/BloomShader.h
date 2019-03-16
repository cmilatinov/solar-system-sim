#pragma once
#include "Shader.h"
class BloomShader :
	public Shader
{
public:
	unsigned int location_sceneTexture;
	unsigned int location_bloomTexture;

	BloomShader(const char* vertexPath, const char* fragmentPath);
	~BloomShader();

	virtual void bindAttributes();
	virtual void getUniformLocations();
};

