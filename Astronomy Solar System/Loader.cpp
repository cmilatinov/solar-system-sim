#include "Loader.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <fstream>
#include <sstream>
#include <string>

#include "OBJ_Loader.h"

Loader::Loader(){

}


Loader::~Loader(){
	for (unsigned int i = 0; i < vbos.size(); i++) {
		glDeleteBuffers(1, &vbos.at(i));
	}
}

Mesh * Loader::loadToVAO(int indices[], int iBytes, float vertices[], int vBytes) {
	unsigned int vao = createVAO();
	bindIndicesBuffer(indices, iBytes);
	storeDataInAttributeList(0, vertices, vBytes, 3);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return new Mesh(vao, iBytes / sizeof(int));
}

Mesh * Loader::loadToVAO(int indices[], int iBytes, float vertices[], int vBytes, float uvs[], int uBytes)
{
	unsigned int vao = createVAO();
	unsigned int indicesVBO = bindIndicesBuffer(indices, iBytes);
	unsigned int verticesVBO = storeDataInAttributeList(0, vertices, vBytes, 3);
	unsigned int uvsVBO = storeDataInAttributeList(1, uvs, uBytes, 2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	Mesh * mesh = new Mesh(vao, iBytes / sizeof(int));
	mesh->vbos = new unsigned int[3]{ indicesVBO, verticesVBO, uvsVBO };
	return mesh;
}

Mesh * Loader::loadToVAO(float vertices[], int bytes) {
	unsigned int vao = createVAO();
	unsigned int verticesVBO = storeDataInAttributeList(0, vertices, bytes,  3);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	Mesh * mesh = new Mesh(vao, (bytes / sizeof(float)) / 3);
	mesh->vbos = new unsigned int[1]{ verticesVBO };
	return mesh;
}

Mesh * Loader::loadToVAO(int indices[], int iBytes, float vertices[], int vBytes, float normals[], int nBytes, float uvs[], int uBytes) {
	unsigned int vao = createVAO();
	bindIndicesBuffer(indices, iBytes);
	storeDataInAttributeList(0, vertices, vBytes, 3);
	storeDataInAttributeList(1, normals, nBytes, 3);
	storeDataInAttributeList(2, uvs, uBytes, 2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return new Mesh(vao, iBytes / sizeof(int));
}

Mesh * Loader::loadToVAO(int indices[], int iBytes, float vertices[], int vBytes, float normals[], int nBytes, float uvs[], int uBytes, float colors[], int cBytes) {
	unsigned int vao = createVAO();
	bindIndicesBuffer(indices, iBytes);
	storeDataInAttributeList(0, vertices, vBytes, 3);
	storeDataInAttributeList(1, normals, nBytes, 3);
	storeDataInAttributeList(2, uvs, uBytes, 2);
	storeDataInAttributeList(3, colors, cBytes, 3);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return new Mesh(vao, iBytes / sizeof(int));
}


Mesh * Loader::loadTerrainToVAO(float vertices[], int vBytes, float normals[], int nBytes, float colors[], int cBytes, float reflectivity[], int rBytes) {
	unsigned int vao = createVAO();
	storeDataInAttributeList(0, vertices, vBytes, 3);
	storeDataInAttributeList(1, normals, nBytes, 3);
	storeDataInAttributeList(2, colors, cBytes, 3);
	storeDataInAttributeList(3, reflectivity, rBytes, 1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return new Mesh(vao, vBytes / (sizeof(float) * 3));
}

Mesh * Loader::loadTerrainToVAO(int indices[], int iBytes, float vertices[], int vBytes, float normals[], int nBytes, float colors[], int cBytes, float reflectivity[], int rBytes) {
	unsigned int vao = createVAO();
	bindIndicesBuffer(indices, iBytes);
	storeDataInAttributeList(0, vertices, vBytes, 3);
	storeDataInAttributeList(1, normals, nBytes, 3);
	storeDataInAttributeList(2, colors, cBytes, 3);
	storeDataInAttributeList(3, reflectivity, rBytes, 1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return new Mesh(vao, iBytes / (sizeof(int)));
}

unsigned int Loader::storeDynamicDataInAttributeList(unsigned int attributeList, float data[], int bytes, int size) {
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	vbos.push_back(vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, bytes, data, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(attributeList, size, GL_FLOAT, GL_FALSE, 0, (void*)0);
	return vbo;
}

unsigned int Loader::storeDataInAttributeList(unsigned int attributeList, float data[], int bytes, int size) {
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	vbos.push_back(vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, bytes, data, GL_STATIC_DRAW);
	glVertexAttribPointer(attributeList, size, GL_FLOAT, GL_FALSE, 0, (void*)0);
	return vbo;
}

unsigned int Loader::createEmptyVBO(int size){
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	vbos.push_back(vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return vbo;
}

void Loader::addInstancedAttribute(unsigned int vao, unsigned int vbo, int attribute, int dataSize, int stride, int offset){
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(attribute);
	glVertexAttribPointer(attribute, dataSize, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
	glVertexAttribDivisor(attribute, 1);
	glDisableVertexAttribArray(attribute);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

unsigned int Loader::createVAO() {
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	return vao;
}

unsigned int Loader::bindIndicesBuffer(int indices[], int bytes) {
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, bytes, indices, GL_STATIC_DRAW);
	return vbo;
}

unsigned int Loader::loadTexture(const char * name) {
	unsigned int texture;

	std::string fullName = std::string("res\\texture\\") + std::string(name);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(fullName.c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);
	if (data)
	{	
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture : \"" << name << "\"" << std::endl;

	stbi_image_free(data);
	return texture;
}

unsigned int Loader::loadCubeMap(std::vector<std::string> textures) {
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	std::string prefix = "res\\texture\\";

	int width, height, nrChannels;
	for (int i = 0; i < textures.size(); i++)
	{
		unsigned char *data = stbi_load((prefix + textures[i]).c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << (prefix + textures[i]) << std::endl;
			stbi_image_free(data);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

Mesh * Loader::loadFromOBJ(char * name) {
	objl::OBJLoader loader;
	bool loaded = loader.LoadFile(name);
	if (loaded) {
		int iSize = 0;
		int vSize = 0;
		for (objl::Mesh currentMesh : loader.LoadedMeshes) {
			iSize += currentMesh.Indices.size();
			vSize += currentMesh.Vertices.size();
		}
		int uSize = vSize * 2;
		vSize *= 3;

		int * indices = new int[iSize];
		float * colors = new float[vSize];
		float * vertices = new float[vSize];
		float * normals = new float[vSize];
		float * uvs = new float[uSize];
		int currentVertex = 0;
		int indexOffset = 0;
		for (objl::Mesh currentMesh : loader.LoadedMeshes) {
			for (objl::Vertex vertex : currentMesh.Vertices) {
				
				vertices[currentVertex * 3] = vertex.Position.X;
				vertices[(currentVertex * 3) + 1] = vertex.Position.Y;
				vertices[(currentVertex * 3) + 2] = vertex.Position.Z;

				normals[currentVertex * 3] = vertex.Normal.X;
				normals[(currentVertex * 3) + 1] = vertex.Normal.Y;
				normals[(currentVertex * 3) + 2] = vertex.Normal.Z;

				colors[currentVertex * 3] = currentMesh.MeshMaterial.Kd.X;
				colors[(currentVertex * 3) + 1] = currentMesh.MeshMaterial.Kd.Y;
				colors[(currentVertex * 3) + 2] = currentMesh.MeshMaterial.Kd.Z;

				uvs[currentVertex * 2] = vertex.TextureCoordinate.X;
				uvs[(currentVertex * 2) + 1] = vertex.TextureCoordinate.Y;

				currentVertex++;
			}
			int currentIndex = 0;
			for (unsigned int index : currentMesh.Indices) {
				indices[indexOffset + currentIndex] = index + indexOffset;
				currentIndex++;
			}
			indexOffset += currentMesh.Indices.size();
		}
		Mesh * mesh = loadToVAO(indices, iSize * sizeof(int), vertices, vSize * sizeof(float), normals, vSize * sizeof(float), uvs, uSize * sizeof(float), colors, vSize * sizeof(float));
		delete[] indices;
		delete[] vertices;
		delete[] normals;
		delete[] colors;
		delete[] uvs;
		return mesh;
	}
	return NULL;
}

Mesh * Loader::createCircle(float radius, int vertexCount){
	int vSize = (vertexCount) * 3;
	float * vertices = new float[vSize];

	float increment = (2.0f * PI) / (float) vertexCount;

	for (int i = 0; i < vertexCount; i++) {
		vertices[(i * 3)] = radius * cos(increment * i);
		vertices[(i * 3) + 1] = 0.0f;
		vertices[(i * 3) + 2] = radius * sin(increment * i);
	}

	return loadToVAO(vertices, vSize * sizeof(float));
}

Shape * Loader::createOrbitShape(Orbit * orbit, Vector3f color, Vector3f * pos, int vertexCount){
	int vSize = (vertexCount) * 3;
	float * vertices = new float[vSize];

	Matrix4f * transform = orbit->getConversionMatrix();

	float increment = 360.0f / (float)vertexCount;

	for (int i = 0; i < vertexCount; i++) {
		Vector3f * point = orbit->getOrbitalPosition(i * increment);
		point->Transform(transform);
		vertices[(i * 3)] = point->x;
		vertices[(i * 3) + 1] = point->y;
		vertices[(i * 3) + 2] = point->z;
	}

	Mesh * mesh = loadToVAO(vertices, vSize * sizeof(float));

	Shape * shape = new Shape(mesh, color, pos);

	delete[] vertices;

	return shape;
}

Shape * Loader::createDynamicLine(float * vertices, int vBytes, bool dashed, Vector3f color, Vector3f * pos) {
	unsigned int vao = createVAO();
	unsigned int vbo = storeDynamicDataInAttributeList(0, vertices, vBytes, 3);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	Mesh * mesh = new Mesh(vao, (vBytes / sizeof(float)) / 3);
	Shape * shape = new Shape(mesh, color, pos);
	shape->shapeVBO = vbo;
	shape->setDashed(dashed);
	return shape;
}

FadedShape * Loader::createTrail(Orbit * orbit, Entity * target, float meanAnomalyStart, Vector3f color, int vertexCount, float minDist){
	int vSize = (vertexCount + 1) * 3;
	float * vertices = new float[vSize];
	float * opacityValues = new float[vertexCount + 1];

	std::vector<Vector3f> points;

	Matrix4f * transform = orbit->getConversionMatrix();

	float increment = 360.0f / (float)vertexCount;

	for (int i = 0; i < vertexCount + 1; i++) {
		Vector3f * point = orbit->getOrbitalPosition(-(i * increment) + meanAnomalyStart);
		point->Transform(transform);
		points.push_back(*point);
		vertices[(i * 3)] = point->x;
		vertices[(i * 3) + 1] = point->y;
		vertices[(i * 3) + 2] = point->z;
		opacityValues[i] = ((float)i / (float)vertexCount);
	}

	unsigned int vao = createVAO();
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	vbos.push_back(vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vSize * sizeof(float), vertices, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	storeDataInAttributeList(1, opacityValues, (vertexCount + 1) * sizeof(float), 1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	Mesh * mesh = new Mesh(vao, vertexCount + 1);
	Shape * shape = new Shape(mesh, color, new Vector3f());
	FadedShape * fShape = new FadedShape(shape, vbo, points, target, minDist);

	delete[] vertices;
	delete[] opacityValues;

	return fShape;
}
