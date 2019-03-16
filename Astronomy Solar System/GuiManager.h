#include <vector>
#include "Mesh.h"
#include "Loader.h"
#include "Gui.h"
#pragma once
class GuiManager
{
private:
	Mesh * guiMesh;

	void addGui(Gui * gui);

public:
	GuiManager(Loader * loader);
	~GuiManager();

	std::vector<Gui*> getAllGuis();

	void createGui(unsigned int texID, int x, int y, int width, int height);

	static Mesh * generateGuiMesh(Loader * loader);
		
	Mesh * getGuiMesh();
};
