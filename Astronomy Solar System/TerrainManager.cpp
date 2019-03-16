#include "TerrainManager.h"

static std::vector<Terrain*> terrains;

TerrainManager::TerrainManager(Loader * l) {
	this->l = l;
}

TerrainManager::~TerrainManager(){
	for (Terrain * t : terrains)
		delete t;
}

void TerrainManager::addTerrain(Terrain * terrain) {
	terrains.push_back(terrain);
}

void TerrainManager::createTerrain(int gridX, int gridZ) {
	addTerrain(new Terrain(l, gridX, gridZ));
}

std::vector<Terrain*> TerrainManager::getTerrains() {
	return terrains;
}