#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>

#include "Time.h"
#include "Mesh.h"
#include "Terrain.h"
#include "StaticShader.h"
#include "EntityManager.h"
#include "LightManager.h"
#include "ShapeManager.h"
#include "PointManager.h"
#include "TextManager.h"
#include "EntityRenderer.h"
#include "ShapeRenderer.h"
#include "PointRenderer.h"
#include "GuiRenderer.h"
#include "SkyboxRenderer.h"
#include "TextRenderer.h"
#include "FBO.h"
#include "TerrainShader.h"
#include "WaterShader.h"
#include "SunShader.h"
#include "Entity.h"
#include "CameraFirstPersonNoclip.h"
#include "CameraThirdPerson.h"
#include "GuiManager.h"
#include "FBO.h"
#include "PostProcessing.h"
#include "FontLoader.h"
#include "Planet.h"
#include "DynamicTextMesh.h"
#include "Planet.h"
#include "Color.h"

#pragma once
class Engine
{
public:
	Engine(GLFWwindow* window, Loader * l, float width, float height, float FOV, int samples);
	~Engine();

public:
	int textSize = 34;
	int activePlanetInfo = -1;
	float timeElapsed = 0.0f;
	bool bTickEntities = false;
	bool showOrbits = false;

	Vector3f * origin = new Vector3f();

	Matrix4f * projectionMatrix;
	
	Mesh *mercury, *venus, *earth, *mars, *jupiter, *saturn, *uranus, *neptune, *sun, *asteroid;
	unsigned int mercuryTexID, venusTexID, earthTexID, marsTexID, jupiterTexID, saturnTexID, uranusTexID, neptuneTexID, sunTexID, asteroidTexID;
	Entity * earthEntity, *mercuryEntity, *venusEntity, *marsEntity, *jupiterEntity, *saturnEntity, *uranusEntity, *neptuneEntity, *plutoEntity, *sunEntity;
	std::vector<Entity*> planets;
	Orbit * mercuryOrbit, *venusOrbit, *earthOrbit, *marsOrbit, *jupiterOrbit, *saturnOrbit, *uranusOrbit, *neptuneOrbit, *plutoOrbit;
	Font * f;

	DynamicTextMesh * mercuryText, *venusText, *earthText, *marsText, *jupiterText, *saturnText, *uranusText, *neptuneText, *generalInfo;
	std::vector<DynamicTextMesh*> planetInformation;

	EntityRenderer * eRenderer;
	SkyboxRenderer * sRenderer;
	GuiRenderer * gRenderer;
	ShapeRenderer * shRenderer;
	PointRenderer * pRenderer;
	TextRenderer * textRenderer;

	Skybox * skybox;

	EntityManager * eManager;
	LightManager * lManager;
	GuiManager * gManager;
	ShapeManager * sManager;
	PointManager * pManager;
	TextManager * textManager;

	SunShader * sunShader;

	FBO * postProcessFBO;
	FBO * msaa;
	PostProcessing * postProcessing;

	Loader * loader;
	FontLoader * fLoader;

	GLFWwindow * win;
	
	float time;

	float FOV;

	void createProjectionMatrix(float width, float height, float FOV);

public:
	void BeginRender(Color clearColor);

	void UpdateDisplay();

	void RenderScene(Camera * cam, float deltaSec);

	void RenderSun(Camera * cam, Entity * sun);

	void RenderGui();

	void AddPoint(Point * point);

	Orbit * CreateOrbit(Vector3f color, Vector3f * pos, float semiMajorAxis, float eccentricity, float inclination, float perihelionArg, float ascendingLongitude, float orbitalPeriodInDays, Time * timeOfPeriapsis);

	Entity * CreateEntity(Mesh * mesh, unsigned int texID, Vector3f pos, Vector3f rot, float scale);

	Entity * CreatePlanet(Mesh * mesh, unsigned int texID, Vector3f pos, Vector3f rot, float scale, float alpha, float delta, float epsilon, float rotationalPeriodInYears, Time * referenceTime, float rotationAtTime, Orbit * orbit);
	
	void RegisterEntity(Entity * ent);

	void CreateGUI(unsigned int texID, int x, int y, int width, int height);

	Engine * SetContrast(float contrast);
	
	void TickEntities(Camera * cam, float deltaSec);
	void UpdateText();

	void Reset(Time * time);
	void SetInitialPlanetConditions(Orbit * orbit, Entity * planet, Time * startTime, Vector3f color, float opaquePointDist);

	static float getGlobalScale();
	void setGlobalScale(float scale);

	void setYearsPerSecond(float value);
	static float getYearsPerSecond();

	void ToggleShowPlanetOrbits();

	void ToggleRenderText();
	void ToggleRenderPlanetNames();
	static bool RenderText();
};
