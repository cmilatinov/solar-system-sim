#include "GuiManager.h"

static std::vector<Gui*> guis;

GuiManager::GuiManager(Loader * loader){
	guiMesh = GuiManager::generateGuiMesh(loader);
}

GuiManager::~GuiManager(){
	for (Gui * g : guis)
		delete g;
	delete guiMesh;
}

std::vector<Gui*> GuiManager::getAllGuis(){
	return guis;
}

void GuiManager::createGui(unsigned int texID, int x, int y, int width, int height){
	Gui * gui = new Gui(texID, x, y, width, height);
	addGui(gui);
}

void GuiManager::addGui(Gui * gui){
	guis.push_back(gui);
}

Mesh * GuiManager::getGuiMesh() {
	return guiMesh;
}

Mesh * GuiManager::generateGuiMesh(Loader * loader) {
	float vertices[] = {
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f
	};
	int indices[] = {
		0, 1, 3,
		3, 1, 2
	};
	float uvs[] = {
		0, 1,
		0, 0,
		1, 0,
		1, 1
	};
	Mesh * result = loader->loadToVAO(indices, sizeof(indices), vertices, sizeof(vertices), uvs, sizeof(uvs));
	return result;
}
