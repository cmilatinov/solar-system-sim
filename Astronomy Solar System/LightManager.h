#include <vector>
#include "Light.h"
#pragma once
class LightManager
{
public:
	LightManager(Light * sun);
	~LightManager();

	const static int maxLights = 1;

	void addLight(Light * l);

	std::vector<Light*> getAllLights();
};

