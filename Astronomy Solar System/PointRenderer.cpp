#include "PointRenderer.h"

PointRenderer::PointRenderer(Matrix4f * projection){
	this->shader = new PointShader("pointVertexShader.glsl", "pointFragmentShader.glsl");
	shader->use();
	shader->loadMatrix(shader->location_projectionMatrix, projection);
	shader->stop();

	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);
	
	glGenBuffers(1, &positionVboID);
	glBindBuffer(GL_ARRAY_BUFFER, positionVboID);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glGenBuffers(1, &colorVboID);
	glBindBuffer(GL_ARRAY_BUFFER, colorVboID);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

PointRenderer::~PointRenderer(){
	delete shader;
}

void PointRenderer::StartRender(Camera * cam) {
	shader->use();
	Matrix4f * viewMatrix = cam->createViewMatrix();
	shader->loadMatrix(shader->location_viewMatrix, viewMatrix);
	delete viewMatrix;
	glDisable(GL_DEPTH_TEST);
}

void PointRenderer::RenderPoints(Camera * cam, std::vector<Point*> points){
	StartRender(cam);

	BufferData(points);

	glBindVertexArray(vaoID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glDrawArrays(GL_POINTS, 0, points.size());
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindVertexArray(0);

	StopRender();
}

void PointRenderer::StopRender(){
	shader->stop();
	glEnable(GL_DEPTH_TEST);
}

void PointRenderer::BufferData(std::vector<Point*> points){
	float * vertexData = new float[points.size() * 4];
	float * colorData = new float[points.size() * 3];
	for (int c = 0; c < points.size(); c++) {
		Point * point = points.at(c);
		vertexData[c * 4] = point->position->x;
		vertexData[(c * 4) + 1] = point->position->y;
		vertexData[(c * 4) + 2] = point->position->z;
		vertexData[(c * 4) + 3] = point->opaqueDistance;

		colorData[c * 3] = point->color.x;
		colorData[(c * 3) + 1] = point->color.y;
		colorData[(c * 3) + 2] = point->color.z;
	}

	glBindBuffer(GL_ARRAY_BUFFER, positionVboID);
	glBufferData(GL_ARRAY_BUFFER, points.size() * 4 * sizeof(float), vertexData, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, colorVboID);
	glBufferData(GL_ARRAY_BUFFER, points.size() * 3 * sizeof(float), colorData, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	delete[] vertexData;
	delete[] colorData;
}