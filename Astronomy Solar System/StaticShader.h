#pragma once
#include "Shader.h"
#include "Light.h"

#define MAX_LIGHTS 1

class StaticShader :
	public Shader
{
public:
	unsigned int location_projectionMatrix;
	unsigned int location_viewMatrix;
	unsigned int location_modelMatrix;
	unsigned int location_texture;
	unsigned int location_needsTexture;
	unsigned int location_lightPosition[MAX_LIGHTS];
	unsigned int location_lightColor[MAX_LIGHTS];

	unsigned int location_clipPlane;

	StaticShader(const char* vertexPath, const char* fragmentPath);
	~StaticShader();

	virtual void bindAttributes();
	virtual void getUniformLocations();

	void loadLight(int i, Light * light);
};

