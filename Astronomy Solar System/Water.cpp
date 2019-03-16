#include "Water.h"

Water::Water(int height, Loader * loader){
	this->height = height;
	this->l = loader;
	this->m = generateWaterMesh();
}

Water::~Water(){

}

Mesh * Water::generateWaterMesh() {
	int vSize = 3 * (int)pow(VERTEX_COUNT + 1, 2);
	float * vertices = new float[vSize];
	for (int i = 0; i < VERTEX_COUNT + 1; i++) {
		float x = i * (SIZE / VERTEX_COUNT);
		for (int j = 0; j < VERTEX_COUNT + 1; j++) {
			float z = j * (SIZE / VERTEX_COUNT);
			float y = this->height;
			vertices[3 * ((i * (VERTEX_COUNT + 1)) + j)] = x;
			vertices[3 * ((i * (VERTEX_COUNT + 1)) + j) + 1] = y;
			vertices[3 * ((i * (VERTEX_COUNT + 1)) + j) + 2] = z;
		}
	}

	int iSize = (int)(6 * pow(VERTEX_COUNT, 2));
	int * indices = new int[iSize];
	for (int i = 0; i < iSize / 6; i++) {
		int column = (int)floor(floor(i) / VERTEX_COUNT);
		int row = (int)((int)floor(i) % VERTEX_COUNT);
		int bottomLeft = ((column * (VERTEX_COUNT + 1)) + row);
		int topLeft = bottomLeft + 1;
		int bottomRight = bottomLeft + VERTEX_COUNT + 1;
		int topRight = bottomRight + 1;

		indices[i * 6] = bottomLeft;
		indices[(i * 6) + 1] = topLeft;
		indices[(i * 6) + 2] = topRight;
		indices[(i * 6) + 3] = bottomLeft;
		indices[(i * 6) + 4] = topRight;
		indices[(i * 6) + 5] = bottomRight;
	}

	int vBytes = vSize * sizeof(float);
	int iBytes = iSize * sizeof(int);
	Mesh * m = l->loadToVAO(indices, iBytes, vertices, vBytes);
	delete[] vertices, indices;
	return m;
}

Mesh * Water::getWaterMesh() {
	return this->m;
}