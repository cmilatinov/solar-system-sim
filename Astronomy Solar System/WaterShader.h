#pragma once
#include "Shader.h"
#include "Light.h"
#define MAX_LIGHTS 1
class WaterShader :
	public Shader
{
public:
	unsigned int location_projectionMatrix;
	unsigned int location_viewMatrix;
	unsigned int location_modelMatrix;
	unsigned int location_texture;
	unsigned int location_time;
	unsigned int location_lightPosition[MAX_LIGHTS];
	unsigned int location_lightColor[MAX_LIGHTS];
	unsigned int location_perspectiveProjectionMatrix;
	unsigned int location_cameraViewMatrix;

	unsigned int location_reflectionTexture;
	unsigned int location_refractionTexture;
	unsigned int location_depthTexture;

	WaterShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath);
	WaterShader(const char* vertexPath, const char* fragmentPath);
	~WaterShader();

	virtual void bindAttributes();
	virtual void getUniformLocations();
	
	void loadLight(int i, Light * light);
};

