#include "VectorMatrix.h"
#pragma once
class Mesh
{
public:
	Mesh();
	Mesh(unsigned int vao, int vertexCount);
	~Mesh();

	unsigned int GetVaoId();

	int GetVertexCount();
	void SetVertexCount(int count);

	void setCenter(Vector3f center);
	void setRadius(float radius);

	Vector3f getCenter();
	float getRadius();

	unsigned int * vbos;

private: 
	unsigned int vaoID;
	int vertexCount;

	Vector3f center;
	float radius;
};

