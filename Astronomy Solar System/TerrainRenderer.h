#include "VectorMatrix.h"
#include "TerrainManager.h"
#include "TerrainShader.h"
#include "Light.h"
#include "Camera.h"

#include <vector>
#pragma once
class TerrainRenderer
{
private:
	TerrainShader * shader;

	void StartRender(Camera * cam);

	void StopRender();

	void RenderTerrain(Terrain * terrain);

public:
	TerrainRenderer(Matrix4f * projection);
	~TerrainRenderer();

	void RenderTerrain(Camera * cam, std::vector<Light*> lights, std::vector<Terrain*> terrains);

	void ClipPlane(Vector4f * plane);
};

