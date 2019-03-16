#include <glad/glad.h>
#include <vector>
#include "Mesh.h"
#include "Shape.h"
#include "FadedShape.h"
#include "Orbit.h"

#pragma once
class Loader
{
public:
	std::vector<unsigned int> vbos;

public:
	Loader();
	~Loader();

	Mesh * loadToVAO(int indices[], int iBytes, float vertices[], int vBytes, float normals[], int nBytes, float uvs[], int uBytes);
	Mesh * loadToVAO(int indices[], int iBytes, float vertices[], int vBytes, float normals[], int nBytes, float uvs[], int uBytes, float colors[], int cBytes);
	Mesh * loadToVAO(float vertices[], int bytes);
	Mesh * loadToVAO(int indices[], int iBytes, float vertices[], int vBytes);
	Mesh * loadToVAO(int indices[], int iBytes, float vertices[], int vBytes, float uvs[], int uBytes);
	Mesh * loadTerrainToVAO(float vertices[], int vBytes, float normals[], int nBytes, float colors[], int cBytes, float reflectivity[], int rBytes);
	Mesh * loadTerrainToVAO(int indices[], int iBytes, float vertices[], int vBytes, float normals[], int nBytes, float colors[], int cBytes, float reflectivity[], int rBytes);

	Mesh * loadFromOBJ(char * fileName);

	Mesh * createCircle(float radius, int vertexCount);

	Shape * createOrbitShape(Orbit * orbit, Vector3f color, Vector3f * pos, int vertexCount);

	Shape * createDynamicLine(float * vertices, int vBytes, bool dashed, Vector3f color, Vector3f * pos);

	FadedShape * createTrail(Orbit * orbit, Entity * target, float meanAnomalyStart, Vector3f color, int vertexCount, float minDist);
	
	unsigned int bindIndicesBuffer(int indices[], int bytes);
	unsigned int storeDataInAttributeList(unsigned int attributeList, float data[], int bytes, int stride);
	unsigned int storeDynamicDataInAttributeList(unsigned int attributeList, float data[], int bytes, int stride);

	unsigned int createEmptyVBO(int size);
	void addInstancedAttribute(unsigned int vao, unsigned int vbo, int attribute, int dataSize, int stride, int offset);
	unsigned int createVAO();

	unsigned int loadTexture(const char * name);
	unsigned int loadCubeMap(std::vector<std::string> textures);
	
};

