#include "Entity.h"
#include "Orbit.h"
#include "Point.h"
#include "TextMesh.h"
#include "FadedShape.h"

#pragma once
class Asteroid
{
public:
	Asteroid(Entity * ent, Orbit * orbit, Point * point, FadedShape * trail, TextMesh * text);
	~Asteroid();

	Entity * ent;
	Point * point;
	TextMesh * text;
	FadedShape * trail;
	Orbit * orbit;
};

