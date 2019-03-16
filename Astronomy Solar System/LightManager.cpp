#include "LightManager.h"

static std::vector<Light*> lights;

LightManager::LightManager(Light * sun) {
	lights.push_back(sun);
}

LightManager::~LightManager(){
	for (Light *l : lights)
		delete l;
}

void LightManager::addLight(Light * l) {
	lights.push_back(l);
}

std::vector<Light*> LightManager::getAllLights() {
	return lights;
}