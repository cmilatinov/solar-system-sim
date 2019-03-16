#include <map>
#include <vector>
#include <utility>
#include "Mesh.h"
#include "Shape.h"
#include "Loader.h"
#pragma once
class ShapeManager
{
private:
	Loader * loader;

public:
	ShapeManager(Loader *loader);
	~ShapeManager();

	std::vector<Shape*> getAllShapes();

	std::map<Mesh*, std::vector<Shape*>> getShapesHashmap();

	void addShape(Shape * sh);
	void removeShape(Shape * shape);
};

