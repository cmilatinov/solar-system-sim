#include "Mesh.h"
#include "Loader.h"
#include "VectorMatrix.h"
#pragma once
class Gui
{
private:
	unsigned int texID;
	int x, y;
	int width, height;

	Matrix4f * mat;

public:
	Gui(unsigned int texID, int x, int y, int width, int height);
	~Gui();

	void setPosition(Vector2f pos);
	void setBounds(Vector2f bounds);

	Matrix4f * getModelMatrix();

	unsigned int getTextureID();
};

