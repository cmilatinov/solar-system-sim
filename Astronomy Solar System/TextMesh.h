#include "Mesh.h"
#include "Font.h"
#include "Gui.h"

#include <string>

#pragma once
class TextMesh
{
private:
	Mesh * mesh;
	Font * font;

	Vector2f pos, bounds;
	Gui * background;

	Matrix4f * modelMatrix;

	

public:
	TextMesh(std::string textString, Mesh * mesh, Font * font, Vector2f pos, Vector2f bounds, Gui * background, int size);
	~TextMesh();

	bool render = true;
	bool isOnScreen = true;

	Mesh * getMesh();
	Font * getFont();

	Vector2f getScreenPosition();
	Vector2f getTextBounds();

	virtual void setScreenPosition(Vector2f pos);
	virtual void setBounds(Vector2f bounds);

	Gui * getBackground();

	Matrix4f * getModelMatrix();

	int size;
	std::string textString;
};

