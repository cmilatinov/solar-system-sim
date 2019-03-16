#include "VectorMatrix.h"
#include "Camera.h"
#include "Mesh.h"
#include "Skybox.h"
#include "SkyboxShader.h"
#pragma once
class SkyboxRenderer
{
private:
	SkyboxShader * shader;
	Mesh * cube;

	void StartRender(Camera * cam);

	void StopRender();

public:
	SkyboxRenderer(Matrix4f * projection, Mesh * cube);
	~SkyboxRenderer();

	void RenderSkyBox(Camera * cam, Skybox * box);

};

