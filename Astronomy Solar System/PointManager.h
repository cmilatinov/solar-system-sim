#include "VectorMatrix.h"
#include "Point.h"
#pragma once
class PointManager
{
public:
	PointManager();
	~PointManager();

	std::vector<Point*> getAllPoints();

	void addPoint(Point* point);
	void removePoint(Point * point);
};

