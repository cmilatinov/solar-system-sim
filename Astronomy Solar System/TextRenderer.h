#include "TextShader.h"
#include "TextMesh.h"

#include <vector>

#pragma once
class TextRenderer
{
private:
	
	
	Mesh * guiMesh;

	void StartRender();
	void StopRender();

	void RenderText(TextMesh * text);
	void RenderBackground(TextMesh * gui);

public:
	TextShader * shader;
	
	TextRenderer(Mesh * backgroundMesh);
	~TextRenderer();

	void RenderTexts(std::vector<TextMesh*> texts);
};

