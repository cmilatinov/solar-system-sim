#include <vector>
#include <map>
#include <utility>
#include <limits>

#include "Entity.h"
#include "Loader.h"
#include "FontLoader.h"
#include "Camera.h"
#include "Display.h"
#include "Planet.h"

#pragma once
class EntityManager
{
private:
	Loader * loader;
	
	Matrix4f * getModelMatrix(Entity * ent);

	bool checkEntityIsInFrustum(Camera * cam, Entity * e);

	bool checkPointIsInFrustum(Camera * cam, Vector3f point);

	static const int MAX_INSTANCES = 10000;
	static const int DATA_LENGTH = 16;

	Matrix4f * projectionMatrix;

public:
	EntityManager(Loader *loader, Matrix4f * projection);
	~EntityManager();

	std::vector<Entity*> getAllEntities();

	std::map<Mesh*, std::vector<Entity*>> getEntityHashmap();

	Entity * createEntity(Mesh * mesh, unsigned int texID, Vector3f pos, Vector3f rot, float scale);

	void tickEntities(Matrix4f projection, Camera * cam, float deltaSec);

	Entity * getClosestEntity(Entity * ent);

	void addEntity(Entity * ent);

	void removeEntity(Entity * ent);

	int getNumEntitiesToRender(Mesh * model);

	static void setTickEntities(bool value);
	static bool getTickEntities();
	static void resizeTexts(FontLoader * loader);
};

