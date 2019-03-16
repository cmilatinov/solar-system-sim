#include "VectorMatrix.h"
#include "WaterShader.h"
#include "Water.h"
#include "Camera.h"
#include "Mesh.h"
#include "Terrain.h"

#include <vector>
#include <time.h>
#pragma once
class WaterRenderer
{
private:
	WaterShader * shader;
	long lastWaterTime;
	void StartRender(Camera * cam);

	void RenderWater(Matrix4f * modelMatrix, Mesh * waterMesh);

	void StopRender();
	

public:
	WaterRenderer(Matrix4f * projection);
	~WaterRenderer();

	void RenderWater(Camera * cam, std::vector<Terrain*> terrains);
	
	void TickShader();
};

