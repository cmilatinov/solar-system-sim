#include <GLAD/glad.h>
#include <vector>
#include "Shape.h"
#include "ShapeShader.h"
#include "Camera.h"
#include "Orbit.h"
#pragma once
class ShapeRenderer
{
private:
	ShapeShader * shader;

	Mesh * circle;
	Orbit * mercury;

	void StartRender(Camera * cam);

	void StopRender();

	void RenderShape(Shape * shape);

public:
	ShapeRenderer(Matrix4f * projection);
	~ShapeRenderer();

	void RenderShapes(Camera * cam, std::vector<Shape*> shapes);
};

