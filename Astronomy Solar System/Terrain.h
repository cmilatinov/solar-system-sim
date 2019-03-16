#include "Mesh.h"
#include "VectorMatrix.h"
#include "Loader.h"
#include "HeightsGenerator.h"
#include "Color.h"
#include "Water.h"
#include <time.h>

#define SIZE 1000.0f
#define VERTEX_COUNT 200
#define AMPLITUDE_COLOR_FACTOR 1.0f / 8.0f
#define WATER_HEIGHT -6.0f

#pragma once
class Terrain
{
public:
	Terrain(Loader* l, int x, int z);
	~Terrain();

private:
	int x, z;
	Mesh * m;
	Water * water;
	Matrix4f transform;
	Loader * l;
	bool bNeedsWater = false;
	float ** heights;

	Mesh * generateTerrain();	
	float average(float values[], int size);
	void applyColors(float colors[], int i, Color c1, Color c2);	

	float min(float values[], int size);
	float max(float values[], int size);
	float clamp(float n, float lower, float upper);

public:
	Matrix4f getTransform();
	Mesh * getTerrainModel();
	int getGridX();
	int getGridZ();
	bool needsWater();
	Water * getWater();

	float getTerrainHeight(float x, float z);

	static HeightsGenerator * getHeightsGenerator();

	static float getWaterHeight() { 
		return WATER_HEIGHT; 
	}

	static float getTerrainSize() {
		return SIZE;
	}

	static float getScale() {
		return 0.1f;
	}
};

