#include "Point.h"



Point::Point(Vector3f * pos, Vector3f color){
	this->position = pos;
	this->color = color;
}


Point::~Point()
{
}
