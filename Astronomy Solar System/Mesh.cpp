#include "Mesh.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

Mesh::Mesh(){

}

Mesh::Mesh(unsigned int vao, int vertices){
	vaoID = vao;
	vertexCount = vertices;
}

Mesh::~Mesh(){
	glDeleteVertexArrays(1, &vaoID);
}


unsigned int Mesh::GetVaoId() {
	return vaoID;
}

int Mesh::GetVertexCount() {
	return vertexCount;
}

void Mesh::SetVertexCount(int count){
	this->vertexCount = count;
}

void Mesh::setCenter(Vector3f center){
	this->center = center;
}

void Mesh::setRadius(float radius){
	this->radius = radius;
}

Vector3f Mesh::getCenter(){
	return center;
}

float Mesh::getRadius(){
	return radius;
}
