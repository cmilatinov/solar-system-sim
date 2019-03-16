#include <vector>

#include "PointManager.h"

static std::vector<Point*> points;

PointManager::PointManager(){
}


PointManager::~PointManager(){
	for (Point * p : points)
		delete p;
}

std::vector<Point*> PointManager::getAllPoints(){
	return points;
}

void PointManager::addPoint(Point * point){
	points.push_back(point);
}

void PointManager::removePoint(Point * point) {
	for (std::vector<Point*>::iterator it = points.begin(); it != points.end(); ++it) {
		if (*it == point) {
			points.erase(it);
			return;
		}
	}
}
