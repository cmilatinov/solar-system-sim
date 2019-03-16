#pragma once
#include <string>
#include <vector>
#include "Loader.h"
class Skybox{
	
private:
	unsigned int texID;

	float size;

public:
	Skybox(Loader * l, std::string skyboxName, float size);
	~Skybox();
	
	int getCubeMapTextureID();
	
	float getSize();

	static Mesh * generateCubeMesh(Loader * l);
};