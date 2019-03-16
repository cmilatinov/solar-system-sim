#include "Terrain.h"
#include <vector>
#pragma once
class TerrainManager
{
private: 
	Loader * l;
	void addTerrain(Terrain * terrain);

public:
	TerrainManager(Loader * l);
	~TerrainManager();

	
	void createTerrain(int gridX, int gridZ);

	std::vector<Terrain*> getTerrains();
};

