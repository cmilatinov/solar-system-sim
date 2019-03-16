#include "FadedShape.h"



FadedShape::FadedShape(Shape * initial, unsigned int vbo, std::vector<Vector3f> points, Entity * target, float minDist) : Shape(*initial){
	this->points = points;
	this->lastPoint = points.at(points.size() - 1);
	this->ent = target;
	this->minDist = minDist;
	this->vbo = vbo;

	loadDataToBuffer();
}

FadedShape::~FadedShape(){

}

void FadedShape::tick() {
	if ((ent->getPosition() - lastPoint).length() > minDist) {
		lastPoint = ent->getPosition();
		points.push_back(lastPoint);
		points.erase(points.begin());
	}

	loadDataToBuffer();
}

void FadedShape::loadDataToBuffer(){
	float * vertices = new float[points.size() * 3];
	for (int i = 0; i < points.size(); i++) {
		vertices[(3 * i)] = points.at(i).x;
		vertices[(3 * i) + 1] = points.at(i).y;
		vertices[(3 * i) + 2] = points.at(i).z;
	}

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, points.size() * 3 * sizeof(float), vertices, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	delete[] vertices;
}
