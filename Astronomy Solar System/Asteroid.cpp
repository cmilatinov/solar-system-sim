#include "Asteroid.h"



Asteroid::Asteroid(Entity * ent, Orbit * orbit, Point * point, FadedShape * trail, TextMesh * text) {
	this->ent = ent;
	this->orbit = orbit;
	this->point = point;
	this->trail = trail;
	this->text = text;
}


Asteroid::~Asteroid(){
	delete orbit;
	delete point;
	delete trail;
	delete text;
	delete ent;
}
