#include "ShapeRenderer.h"
#include "Orbit.h"
#include "FadedShape.h"
#include "utils.h"

ShapeRenderer::ShapeRenderer(Matrix4f * projection) {
	this->shader = new ShapeShader("shapeVertexShader.glsl", "shapeFragmentShader.glsl");
	this->shader->use();
	this->shader->loadMatrix(shader->location_projectionMatrix, projection);
	this->shader->loadVector(shader->location_color, &Vector3f(1.0f, 1.0f, 1.0f));
	this->shader->stop();
}

ShapeRenderer::~ShapeRenderer() {
	delete shader;
}

void ShapeRenderer::StartRender(Camera * cam) {
	shader->use();
	Matrix4f * m = cam->createViewMatrix();
	shader->loadMatrix(shader->location_viewMatrix, m);
	delete m;
}

void ShapeRenderer::StopRender() {
	shader->stop();
}

void ShapeRenderer::RenderShapes(Camera * cam, std::vector<Shape*> shapes) {
	StartRender(cam);
	
	for (Shape * shape : shapes)
		RenderShape(shape);

	StopRender();
}

void ShapeRenderer::RenderShape(Shape * shape) {
	if (!shape->render) 
		return;

	Matrix4f * matrix = shape->getModelMatrix();
	shader->loadMatrix(shader->location_modelMatrix, matrix);
	delete matrix;
	shader->loadVector(shader->location_color, &shape->getColor());
	shader->loadFloat(shader->location_opacity, shape->getOpacity());
	glBindVertexArray(shape->getModel()->GetVaoId());
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	if (ClassUtils::instanceof<FadedShape>(shape)) {
		glDisable(GL_DEPTH_TEST);
		shader->loadBoolean(shader->location_trailValue, true);
		glDrawArrays(GL_LINE_STRIP, 0, shape->getModel()->GetVertexCount());
		glEnable(GL_DEPTH_TEST);
	} else {
		shader->loadBoolean(shader->location_trailValue, false);
		glDrawArrays(shape->isDashed() ? GL_LINES : GL_LINE_LOOP, 0, shape->getModel()->GetVertexCount());
	}
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindVertexArray(0);
}