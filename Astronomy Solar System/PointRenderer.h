#include "PointShader.h"
#include "Camera.h"
#include "glad/glad.h"
#include "Point.h"

#include <vector>

#pragma once
class PointRenderer
{
private:
	unsigned int vaoID;
	unsigned int positionVboID;
	unsigned int colorVboID;

	PointShader * shader;

	void StartRender(Camera * cam);

	void StopRender();

	void BufferData(std::vector<Point*> points);

public:
	PointRenderer(Matrix4f * projection);
	~PointRenderer();

	void RenderPoints(Camera * cam, std::vector<Point*> points);
};

