#include "Mesh.h"
#include "VectorMatrix.h"
#pragma once
class Shape{

private:
	Vector3f color;
	Mesh * mesh;
	Vector3f * pos;
	bool dashed;
	float opacity = 1.0f;

	

public:
	Shape(Mesh *  mesh, Vector3f color, Vector3f * pos);
	~Shape();

	unsigned int shapeVBO;
	bool render = true;
	
	Mesh * getModel();

	Vector3f getColor();

	virtual Matrix4f * getModelMatrix();

	void setDashed(bool value);
	void setOpacity(float value);

	bool isDashed();
	float getOpacity();
};

