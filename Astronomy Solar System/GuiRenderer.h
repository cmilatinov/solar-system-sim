#include <GLAD/glad.h>
#include "Gui.h"
#include "GuiShader.h"
#pragma once
class GuiRenderer
{
private:
	

	Mesh * guiMesh;

	void StartRender();

	void StopRender();

	void RenderGui(Gui * gui);
public:
	GuiShader * shader;
	GuiRenderer(Mesh * guiMesh);
	~GuiRenderer();

	void RenderGUI(std::vector<Gui*> guis);
};

