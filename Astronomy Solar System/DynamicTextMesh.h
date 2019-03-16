#include "TextMesh.h"
#include "FontLoader.h"

#include <glad/glad.h>

#pragma once
class DynamicTextMesh :
	public TextMesh
{
private:

public:
	DynamicTextMesh(std::string textString, Mesh * mesh, Font * font, Vector2f pos, Vector2f bounds, Gui * background, int size);
	DynamicTextMesh(TextMesh * textMesh);
	~DynamicTextMesh(); 

	void setText(std::string text, FontLoader * loader);

	void setLowerLeftCornerPos(Vector2f pos);

	void setLowerRightCornerPos(Vector2f pos);

};

