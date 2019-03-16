#include "VectorMatrix.h"
#include "StaticShader.h"
#include "Entity.h"
#include "EntityManager.h"
#include "Camera.h"
#include "Light.h"

#include <vector>
#include <map>
#include <utility>
#pragma once
class EntityRenderer
{
private:
	StaticShader * shader;

	void StartRender(Camera * cam);

	void StopRender();

	Matrix4f * getModelMatrix(Entity * e);

	void RenderEntity(Entity * e);

public:
	EntityRenderer(Matrix4f * projection);
	~EntityRenderer();

	void RenderEntities(Camera * cam, std::vector<Light*> lights, std::vector<Entity*> entities);

	void RenderEntities(Camera * cam, std::vector<Light*> lights, EntityManager * eManager);

	void ClipPlane(Vector4f * plane);
};

