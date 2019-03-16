#include "VectorMatrix.h"
#include "Mesh.h"
#include "Loader.h"
#pragma once

#define SIZE 1000.0f
#define VERTEX_COUNT 200 

class Water
{
private:
	Mesh *m;
	Loader * l;
	int height;
	Mesh * generateWaterMesh();

public:
	Water(int height, Loader * loader);
	~Water();

	Mesh * getWaterMesh();
};

