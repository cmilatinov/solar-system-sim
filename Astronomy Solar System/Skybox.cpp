#include "Skybox.h"
#include <iostream>

static const std::string suffixes[] = { std::string("_right.png"), std::string("_left.png"),
										std::string("_bottom.png"), std::string("_top.png"),
										std::string("_front.png"), std::string("_back.png")};

Skybox::Skybox(Loader * l, std::string skyboxName, float size){
	std::vector<std::string> textures;
	for(int i = 0; i < 6; i++)
		textures.push_back(skyboxName + suffixes[i]);
	texID = l->loadCubeMap(textures);
	this->size = size;
}
 
Skybox::~Skybox(){
	glDeleteTextures(1, &texID);
}
 
int Skybox::getCubeMapTextureID(){
	return this->texID;
}

float Skybox::getSize() {
	return this->size;
}

Mesh * Skybox::generateCubeMesh(Loader * loader) {
	float vertices[] = {
		//front
		-1.0, -1.0,  1.0,
		1.0, -1.0,  1.0,
		1.0,  1.0,  1.0,
		-1.0,  1.0,  1.0,
		// back
		-1.0, -1.0, -1.0,
		1.0, -1.0, -1.0,
		1.0,  1.0, -1.0,
		-1.0,  1.0, -1.0,
	};

	int indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// top
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// bottom
		4, 0, 3,
		3, 7, 4,
		// left
		4, 5, 1,
		1, 0, 4,
		// right
		3, 2, 6,
		6, 7, 3,
	};
	return loader->loadToVAO(indices, sizeof(indices), vertices, sizeof(vertices));
}