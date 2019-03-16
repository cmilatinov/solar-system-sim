#include "Engine.h"
#include "Display.h"
#include "Skybox.h"

#include <cmath>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <map>
#include <vector>

static float yearsPerSecond = 0.5;

static const float NEAR_PLANE = 0.0007f;
static const float FAR_PLANE = 1000.0f;

static int msaaSamples;


static bool renderText = true;

static float globalScale = 1.0f / 4.25875e-4f;

Engine::Engine(GLFWwindow* window, Loader * l, float width, float height, float FOV, int samples) {
	this->FOV = FOV;
	msaaSamples = samples;
	this->win = window;
	createProjectionMatrix(width, height, FOV);
	this->loader = l;
	this->fLoader = new FontLoader(loader);


	//Entities
	this->eManager = new EntityManager(loader, projectionMatrix);
	this->eRenderer = new EntityRenderer(projectionMatrix);

	//Light Manager
	this->lManager = new LightManager(new Light(Vector3f(0,0,0), Vector3f(1, 1, 1)));//-3000 * Vector3f(0.4, -1.0, 0.8)

	//GUIs
	this->gManager = new GuiManager(loader);
	Mesh * gMesh = gManager->getGuiMesh();
	this->gRenderer = new GuiRenderer(gMesh);

	//Text
	this->textManager = new TextManager();
	this->textRenderer = new TextRenderer(gMesh);
	f = fLoader->loadFont("georgia25");
	
	mercuryText = new DynamicTextMesh(fLoader->loadString("V", f, textSize, Vector2f()));
	textManager->addText(mercuryText);
	planetInformation.push_back(mercuryText);

	venusText = new DynamicTextMesh(fLoader->loadString("V", f, textSize, Vector2f()));
	textManager->addText(venusText);
	planetInformation.push_back(venusText);

	earthText = new DynamicTextMesh(fLoader->loadString("V", f, textSize, Vector2f()));
	textManager->addText(earthText);
	planetInformation.push_back(earthText);

	marsText = new DynamicTextMesh(fLoader->loadString("V", f, textSize, Vector2f()));
	textManager->addText(marsText);
	planetInformation.push_back(marsText);

	jupiterText = new DynamicTextMesh(fLoader->loadString("V", f, textSize, Vector2f()));
	textManager->addText(jupiterText);
	planetInformation.push_back(jupiterText);

	saturnText = new DynamicTextMesh(fLoader->loadString("V", f, textSize, Vector2f()));
	textManager->addText(saturnText);
	planetInformation.push_back(saturnText);

	uranusText = new DynamicTextMesh(fLoader->loadString("V", f, textSize, Vector2f()));
	textManager->addText(uranusText);
	planetInformation.push_back(uranusText);

	neptuneText = new DynamicTextMesh(fLoader->loadString("V", f, textSize, Vector2f()));
	textManager->addText(neptuneText);
	planetInformation.push_back(neptuneText);

	generalInfo = new DynamicTextMesh(fLoader->loadString("V", f, textSize, Vector2f()));
	textManager->addText(generalInfo);

	//Skybox Renderer
	this->sRenderer = new SkyboxRenderer(projectionMatrix, Skybox::generateCubeMesh(loader));
	this->skybox = new Skybox(l, "bkg3", 500.0f);

	//Shapes
	this->sManager = new ShapeManager(loader);
	this->shRenderer = new ShapeRenderer(projectionMatrix);

	//Points
	this->pManager = new PointManager();
	this->pRenderer = new PointRenderer(projectionMatrix);

	//Sun Shader
	this->sunShader = new SunShader("sunVertexShader.glsl", "sunFragmentShader.glsl");
	this->sunShader->use();
	this->sunShader->loadMatrix(this->sunShader->location_projectionMatrix, projectionMatrix);
	this->sunShader->stop();

	//Time
	setYearsPerSecond(0.33f);

	//Orbits
	mercuryOrbit = CreateOrbit(Vector3f(1, 1, 1), origin, 3.871f, 0.20564f, 7.006f, 29.12f, 48.34f, 87.969257f, new Time(5, 22, 2006, 10, 33));
	venusOrbit = CreateOrbit(Vector3f(1, 1, 1), origin, 7.233f, 0.00676f, 3.398f, 55.10f, 76.67f, 224.700799f, new Time(7, 12, 2000, 12, 0));
	earthOrbit = CreateOrbit(Vector3f(1, 1, 1), origin, 10.0f, 0.01673f, 0.0f, 102.93f, 0.0f, 365.25636f, new Time(1, 4, 2000, 0, 0));
	marsOrbit = CreateOrbit(Vector3f(1, 1, 1), origin, 15.237f, 0.09337f, 1.852f, 286.37f, 49.71f, 686.92971f, new Time(10, 12, 2001));

	jupiterOrbit = CreateOrbit(Vector3f(1, 1, 1), origin, 52.025f, 0.04854f, 1.299f, -86.02f, 100.29f, 4330.59502575f, new Time(4, 4, 2011));
	saturnOrbit = CreateOrbit(Vector3f(1, 1, 1), origin, 95.415f, 0.05551f, 2.494f, -20.78f, 113.64f, 10746.94031475f, new Time(8, 15, 2003));
	uranusOrbit = CreateOrbit(Vector3f(1, 1, 1), origin, 191.88f, 0.04686f, 0.773f, 98.47f, 73.96f, 30588.74040675f, new Time(1, 8, 2050));
	neptuneOrbit = CreateOrbit(Vector3f(1, 1, 1), origin, 300.70f, 0.00895f, 1.770f, -85.11f, 131.79f, 59799.9024525f, new Time(11, 13, 2046));

	//Post Processing
	this->postProcessFBO = new FBO(Display::getWidth(), Display::getHeight(), false);
	this->msaa = new FBO(Display::getWidth(), Display::getHeight(), msaaSamples);
	this->postProcessing = new PostProcessing(gManager->getGuiMesh(), postProcessFBO);

	//Load entity meshes
	mercury = loader->loadFromOBJ("res\\mesh\\planet\\mercury\\mercury.obj");
	venus = loader->loadFromOBJ("res\\mesh\\planet\\venus\\venus.obj");
	earth = loader->loadFromOBJ("res\\mesh\\planet\\earth\\earth.obj");
	mars = loader->loadFromOBJ("res\\mesh\\planet\\mars\\mars.obj");
	jupiter = loader->loadFromOBJ("res\\mesh\\planet\\jupiter\\jupiter.obj");
	saturn = loader->loadFromOBJ("res\\mesh\\planet\\saturn\\saturn.obj");
	uranus = loader->loadFromOBJ("res\\mesh\\planet\\uranus\\uranus.obj");
	neptune = loader->loadFromOBJ("res\\mesh\\planet\\neptune\\neptune.obj");
	
	sun = loader->loadFromOBJ("res\\mesh\\planet\\sun\\sun.obj");

	asteroid = loader->loadFromOBJ("res\\mesh\\planet\\moon\\moon.obj");
	asteroidTexID = loader->loadTexture("Moon_2k.png");
	asteroid->setCenter(Vector3f());
	asteroid->setRadius(8.70f);
	
	Time * J2000 = Time::J2000();
	
	//Setup Entities
	//Mercury
	mercury->setCenter(Vector3f());
	mercury->setRadius(8.70f);
	mercuryTexID = loader->loadTexture("Mercury_2k.png");
	mercuryEntity = CreatePlanet(mercury, mercuryTexID, Vector3f(0, 0, 0), Vector3f(0, 0, 0), 0.383f * globalScale * 4.25875e-4f / 8.70f, 281.0f, 61.4143f, 0.1f, 1407.5f / (24.0f * 365.25f), J2000, 0.0f, mercuryOrbit);
	mercuryEntity->mass = 3.3e+23;
	((Planet*)mercuryEntity)->text = new DynamicTextMesh(fLoader->loadString(" Mercury ", f, textSize, Vector2f()));
	textManager->addText(((Planet*)mercuryEntity)->text);
	planets.push_back(mercuryEntity);
	
	//Venus
	venus->setCenter(Vector3f());
	venus->setRadius(8.70f);
	venusTexID = loader->loadTexture("Venus_2k.png");
	venusEntity = CreatePlanet(venus, venusTexID, Vector3f(0, 0, 0), Vector3f(0, 0, 0), 0.984f * globalScale * 4.25875e-4f / 8.70f, 272.76f, 67.16f, 177.4f, 5832.5f / (24.0f * 365.25f), J2000, 0.0f, venusOrbit);
	venusEntity->mass = 4.87e+24;
	((Planet*)venusEntity)->text = new DynamicTextMesh(fLoader->loadString(" Venus ", f, textSize, Vector2f()));
	textManager->addText(((Planet*)venusEntity)->text);
	planets.push_back(venusEntity);

	//Earth
	earth->setCenter(Vector3f());
	earth->setRadius(8.70f);
	earthTexID = loader->loadTexture("Earth_2k.png");
	earthEntity = CreatePlanet(earth, earthTexID, Vector3f(0, 0, 0), Vector3f(0, 0, 0), globalScale * 4.25875e-4f / 8.70f, 0.0f, 90.0f, 23.44f, 1.0f / 365.25f, J2000, 0.0f, earthOrbit);
	earthEntity->mass = 5.97e+24;
	((Planet*)earthEntity)->text = new DynamicTextMesh(fLoader->loadString(" Earth ", f, textSize, Vector2f()));
	textManager->addText(((Planet*)earthEntity)->text);
	planets.push_back(earthEntity);

	//Mars
	mars->setCenter(Vector3f());
	mars->setRadius(8.70f);
	marsTexID = loader->loadTexture("Mars_2k.png");
	marsEntity = CreatePlanet(mars, marsTexID, Vector3f(0, 0, 0), Vector3f(0, 0, 0), 0.532f * globalScale * 4.25875e-4f / 8.70f, 317.68143f, 52.88650f, 25.19f, 24.623 / (24.0f * 365.25f), J2000, 0.0f, marsOrbit);
	marsEntity->mass = 6.42e+23;
	((Planet*)marsEntity)->text = new DynamicTextMesh(fLoader->loadString(" Mars ", f, textSize, Vector2f()));
	textManager->addText(((Planet*)marsEntity)->text);
	planets.push_back(marsEntity);
	
	//Jupiter
	jupiter->setCenter(Vector3f());
	jupiter->setRadius(8.70f);
	jupiterTexID = loader->loadTexture("Jupiter_2k.png");
	jupiterEntity = CreatePlanet(jupiter, jupiterTexID, Vector3f(0, 0, 0), Vector3f(0, 0, 0), 11.21f * globalScale * 4.25875e-4f / 8.70f, 268.056595f, 64.495303f, 3.12f, 9.925 / (24.0f * 365.25f), J2000, 0.0f, jupiterOrbit);
	jupiterEntity->mass = 1.898e+27;
	((Planet*)jupiterEntity)->text = new DynamicTextMesh(fLoader->loadString(" Jupiter ", f, textSize, Vector2f()));
	textManager->addText(((Planet*)jupiterEntity)->text);
	planets.push_back(jupiterEntity);

	//Saturn
	saturn->setCenter(Vector3f());
	saturn->setRadius(16.823f);
	saturnTexID = loader->loadTexture("Saturn_2k.png");
	saturnEntity = CreatePlanet(saturn, saturnTexID, Vector3f(0, 0, 0), Vector3f(0, 0, 0), 9.45f * globalScale * 4.25875e-4f / 8.70f, 40.589f, 83.537f, 26.73f, 10.656 / (24.0f * 365.25f), J2000, 0.0f, saturnOrbit);
	saturnEntity->mass = 5.68e+26;
 	((Planet*)saturnEntity)->text = new DynamicTextMesh(fLoader->loadString(" Saturn ", f, textSize, Vector2f()));
	textManager->addText(((Planet*)saturnEntity)->text);
	planets.push_back(saturnEntity);

	//Uranus
	uranus->setCenter(Vector3f());
	uranus->setRadius(16.823f);
	uranusTexID = loader->loadTexture("Uranus_2k.png");
	uranusEntity = CreatePlanet(uranus, uranusTexID, Vector3f(0, 0, 0), Vector3f(0, 0, 0), 4.01f * globalScale * 4.25875e-4f / 8.70f, 257.311f, -15.175f, 97.86f, 17.24 / (24.0f * 365.25f), J2000, 0.0f, uranusOrbit);
	uranusEntity->mass = 8.68e+25;
	((Planet*)uranusEntity)->text = new DynamicTextMesh(fLoader->loadString(" Uranus ", f, textSize, Vector2f()));
	textManager->addText(((Planet*)uranusEntity)->text);
	planets.push_back(uranusEntity);

	//Neptune
	neptune->setCenter(Vector3f());
	neptune->setRadius(8.70f);
	neptuneTexID = loader->loadTexture("Neptune_2k.png");
	neptuneEntity = CreatePlanet(neptune, neptuneTexID, Vector3f(0, 0, 0), Vector3f(0, 0, 0), 3.88f * globalScale * 4.25875e-4f / 8.70f, 299.36f, 43.46f, 29.56f, 16.11 / (24.0f * 365.25f), J2000, 0.0f, neptuneOrbit);
	neptuneEntity->mass = 1.02e+26;
	((Planet*)neptuneEntity)->text = new DynamicTextMesh(fLoader->loadString(" Neptune ", f, textSize, Vector2f()));
	textManager->addText(((Planet*)neptuneEntity)->text);
	planets.push_back(neptuneEntity);

	//Sun
	sun->setCenter(Vector3f());
	sun->setRadius(8.70f);
	sunTexID = loader->loadTexture("Sun_2k.png");
	sunEntity = new Entity(sun, Vector3f(), Vector3f(), 0.4f);
	sunEntity->setTextureID(sunTexID);
	sunEntity->setAffectedByGravity(false);
	
	Reset(J2000);

	//Enable depth testing
	glEnable(GL_DEPTH_TEST);

	//Enable back face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Provoking Vertices
	glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);

	//Round Points
	glEnable(GL_POINT_SMOOTH);
	glPointSize(10);

	//Anti-aliased lines
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(2.0f);
}

void Engine::Reset(Time * t) {
	SetInitialPlanetConditions(mercuryOrbit, mercuryEntity, t, Color(84, 117, 108).get(), 37.0f);
	SetInitialPlanetConditions(venusOrbit, venusEntity, t, Color(234, 143, 34).get(), 74.0f);
	SetInitialPlanetConditions(earthOrbit, earthEntity, t, Color(34, 129, 204).get(), 80.0f);
	SetInitialPlanetConditions(marsOrbit, marsEntity, t, Color(255, 50, 15).get(), 35.0f);

	SetInitialPlanetConditions(jupiterOrbit, jupiterEntity, t, Color(255, 143, 56).get(), 795.0f);
	SetInitialPlanetConditions(saturnOrbit, saturnEntity, t, Color(255, 195, 145).get(), 730.0f);
	SetInitialPlanetConditions(uranusOrbit, uranusEntity, t, Color(136, 197, 250).get(), 330.0f);
	SetInitialPlanetConditions(neptuneOrbit, neptuneEntity, t, Color(11, 144, 245).get(), 290.0f);
}

void Engine::SetInitialPlanetConditions(Orbit * orbit, Entity * planet, Time * startTime, Vector3f color, float opaquePointDist) {
	float days = orbit->timeOfPeriapsis->difference(*startTime) / (3600 * 24);
	float orbitalPeriod = orbit->orbitalPeriodInDays;
	float meanAnomaly = 0.0f;

	meanAnomaly += fmod(360 * days / orbitalPeriod, 360);

	if (((Planet*)planet)->trail != nullptr) {
		sManager->removeShape(((Planet*)planet)->trail);
		delete ((Planet*)planet)->trail;
	}
	
	if (((Planet*)planet)->point != nullptr) {
		pManager->removePoint(((Planet*)planet)->point);
		delete ((Planet*)planet)->point;
	}

	FadedShape * fShape = loader->createTrail(orbit, planet, meanAnomaly, color, 200, (PI * orbit->semiMajorAxis) / 200.0f);
	sManager->addShape(fShape);
	((Planet*)planet)->trail = fShape;

	Point * point = new Point(planet->getPositionPointer(), color);
	point->opaqueDistance = opaquePointDist;
	planet->maxDist = opaquePointDist;
	pManager->addPoint(point);
	((Planet*)planet)->point = point;

	Matrix4f * transform = orbit->getConversionMatrix();
	Vector3f * vel = orbit->getOrbitalVelocity(meanAnomaly);
	Vector3f * pos = orbit->getOrbitalPosition(meanAnomaly);
	
	vel->Transform(transform);
	pos->Transform(transform);

	planet->setPosition(*pos);
	planet->velocity = *vel;
	
	delete transform;
	delete pos;
	delete vel;
}

Engine::~Engine(){
	glfwDestroyWindow(win);
	delete projectionMatrix;

	delete eRenderer;
	delete sRenderer;
	delete gRenderer;
	delete shRenderer;
	delete pRenderer;

	delete skybox;

	delete eManager;
	delete lManager;
	delete gManager;
	delete sManager;
	delete pManager;

	delete sunShader;

	delete postProcessFBO;
	delete msaa;
	delete postProcessing;

	delete loader;
	delete fLoader;
}

void Engine::BeginRender(Color clearColor) {
	glClearColor(clearColor.getR(), clearColor.getG(), clearColor.getB(), 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Engine::RenderScene(Camera * cam, float deltaSec) {
	
	if (msaaSamples > 1)
		msaa->bind();
	else
		postProcessFBO->bind();
	
	BeginRender(Color::black());
	
	glDisable(GL_CULL_FACE);
	sRenderer->RenderSkyBox(cam, skybox);
	glEnable(GL_CULL_FACE);

	RenderSun(cam, sunEntity);
	shRenderer->RenderShapes(cam, sManager->getAllShapes());
	
	TickEntities(cam, deltaSec);
	eRenderer->RenderEntities(cam, lManager->getAllLights(), eManager);	

	if (msaaSamples > 1) {
		msaa->resolve(postProcessFBO);
		msaa->unbind();
	}else
		postProcessFBO->unbind();

	postProcessing->DoPostProcessing();
	pRenderer->RenderPoints(cam, pManager->getAllPoints());
	
	if (renderText)
		textRenderer->RenderTexts(textManager->getAllTexts());

	if (bTickEntities)
		timeElapsed += getYearsPerSecond() * deltaSec;

}

void Engine::RenderSun(Camera * cam, Entity * sun){
	sunShader->use();
	
	//Cam Dir
	Vector3f camDir = cam->getPosition() - sun->getPosition();
	camDir.Normalize();
	sunShader->loadVector(sunShader->location_cameraDirection, &camDir);
	
	//View Matrix
	Matrix4f * viewMatrix = cam->createViewMatrix();
	sunShader->loadMatrix(sunShader->location_viewMatrix, viewMatrix);
	delete viewMatrix;
	
	//Model Matrix
	Matrix4f * modelMatrix = sun->getModelMatrix();
	sunShader->loadMatrix(sunShader->location_modelMatrix, modelMatrix);
	delete modelMatrix;

	//Render
	glBindVertexArray(sun->getModel()->GetVaoId());
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sun->getTextureID());
	glDrawElements(GL_TRIANGLES, sun->getModel()->GetVertexCount(), GL_UNSIGNED_INT, (void*)0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	sunShader->stop();
}

void Engine::RenderGui() {
	gRenderer->RenderGUI(gManager->getAllGuis());
}

void Engine::AddPoint(Point * point){
	pManager->addPoint(point);
}

Orbit * Engine::CreateOrbit(Vector3f color, Vector3f * pos, float semiMajorAxis, float eccentricity, float inclination, float perihelionArg, float ascendingLongitude, float orbitalPeriodInDays, Time * timeOfPeriapsis){
	Orbit * orbit = new Orbit(semiMajorAxis, eccentricity, inclination, perihelionArg, ascendingLongitude, orbitalPeriodInDays, timeOfPeriapsis);
	Shape * shape = loader->createOrbitShape(orbit, color, pos, 200);
	shape->setOpacity(0.5f);
	shape->setDashed(true);
	shape->render = showOrbits;
	orbit->shape = shape;
	sManager->addShape(shape);
	return orbit;
}

void Engine::UpdateDisplay() {
	glfwSwapBuffers(win);
	glfwPollEvents();
}

Engine * Engine::SetContrast(float contrast){
	this->postProcessing->SetContrast(contrast);
	return this;
}

void Engine::TickEntities(Camera * cam, float deltaSec){
	eManager->tickEntities(*projectionMatrix, cam, deltaSec);
	UpdateText();
}

void Engine::UpdateText(){
	
	std::string info = " Simulation Speed :   " + StringUtils::to_string_with_precision(getYearsPerSecond() * 365.25, 4) + " days/s \n"
					   " Current Time :   " + Time(timeElapsed).to_string();
	if (generalInfo->textString != info) {
		generalInfo->setText(info, fLoader);
		generalInfo->setLowerRightCornerPos(Vector2f(Display::getWidth(), Display::getHeight()));
	}
	
	for (int i = 0; i < planetInformation.size(); i++)
		if (i != activePlanetInfo)
			planetInformation.at(i)->isOnScreen = false;

	if (activePlanetInfo < 0) return;

	Planet * planet = (Planet*) planets.at(activePlanetInfo);
	
	std::string text = " Acceleration :   " + StringUtils::to_string_with_precision(1.50222861e-4f * planet->acceleration.length() / (pow(getYearsPerSecond(), 2) * 10.0f), 4) + " m/s² \n" +
					   " Velocity :   " + StringUtils::to_string_with_precision(4.74372f * planet->velocity.length() / (getYearsPerSecond() * 10.0f), 4) + " km/s \n" +
					   " Mass :   " + StringUtils::to_string_with_precision(planet->mass, 4) + " kg \n" +
					   " Rotational Period :   " + StringUtils::to_string_with_precision(planet->rotationalPeriodInYears * 365.25, 4) + " days \n" +
					   " Obliquity to Orbit :   " + StringUtils::to_string_with_precision(planet->epsilon, 4) + "° \n" +
					   " Semi-Major Axis :   " + StringUtils::to_string_with_precision(MathUtils::SemiMajorAxis(1.5e+10 * planet->getPosition(), (4740.57172f / (10.0f * getYearsPerSecond())) * planet->velocity, planet->mass), 4) + " AU \n"
					   " Orbital Period :   " + StringUtils::to_string_with_precision(MathUtils::PeriodInYears(1.5e+10 * planet->getPosition(), (4740.57172f / (10.0f * getYearsPerSecond())) * planet->velocity, planet->mass), 4) + " years ";
	
	planetInformation.at(activePlanetInfo)->isOnScreen = true;
	
	if (planetInformation.at(activePlanetInfo)->textString != text) {
		planetInformation.at(activePlanetInfo)->setText(text, fLoader);
		planetInformation.at(activePlanetInfo)->setLowerLeftCornerPos(Vector2f(0, Display::getHeight()));
	}
}

void Engine::setYearsPerSecond(float value){
	float oldValue = yearsPerSecond;
	yearsPerSecond = value;
	if (mercuryOrbit != nullptr) {
		for (Entity * ent : planets) {
			Planet * planet = (Planet*)ent;
			planet->orbit->updateMeanMotion();
		}
	}
	std::vector<Entity*> ents = eManager->getAllEntities();
	for (Entity * ent : ents) {
		ent->velocity = (yearsPerSecond / oldValue) * ent->velocity;
	}
	
}

float Engine::getYearsPerSecond(){	
	return yearsPerSecond;
}

void Engine::ToggleShowPlanetOrbits(){
	showOrbits = !showOrbits;
	for (Entity * ent : planets) {
		Planet * planet = (Planet *)ent;
		planet->orbit->shape->render = showOrbits;
	}
}

void Engine::ToggleRenderText(){
	renderText = !renderText;
}

void Engine::ToggleRenderPlanetNames(){
	for (Entity * ent : planets) {
		Planet * planet = (Planet *)ent;
		planet->text->render = !planet->text->render;
	}
}

bool Engine::RenderText(){
	return renderText;
}

void Engine::RegisterEntity(Entity * ent) {
	eManager->addEntity(ent);
}

Entity * Engine::CreateEntity(Mesh * mesh, unsigned int texID, Vector3f pos, Vector3f rot, float scale) {
	return eManager->createEntity(mesh, texID, pos, rot, scale);
}

Entity * Engine::CreatePlanet(Mesh * mesh, unsigned int texID, Vector3f pos, Vector3f rot, float scale, float alpha, float delta, float epsilon, float rotationalPeriodInYears, Time * referenceTime, float rotationAtTime, Orbit * orbit){
	Entity * ent = new Planet(mesh, alpha, delta, epsilon, rotationalPeriodInYears, referenceTime, rotationAtTime);
	ent->setPosition(pos);
	ent->setRotation(rot);
	ent->setScale(scale);
	ent->setTextureID(texID);
	eManager->addEntity(ent);

	Vector3f north = Vector3f(0, 10.0f, 0), south = Vector3f(0, -10.0f, 0);
	Matrix4f * transform = ((Planet*)ent)->getRotationMatrix();
	north.Transform(transform);
	south.Transform(transform);
	delete transform;

	Matrix4f * scaleMat = new Matrix4f();
	scaleMat->SetIdentity();
	Vector3f v = Vector3f(ent->getScale(), ent->getScale(), ent->getScale());
	scaleMat->Scale(v);
	north.Transform(scaleMat);
	south.Transform(scaleMat);
	delete scaleMat;

	float * vertices = new float[6];
	vertices[0] = south.x;
	vertices[1] = south.y;
	vertices[2] = south.z;
	vertices[3] = north.x;
	vertices[4] = north.y;
	vertices[5] = north.z;
	
	Mesh * axisMesh = loader->loadToVAO(vertices, 6 * sizeof(float));
	delete[] vertices;


	Shape * axisShape = new Shape(axisMesh, Vector3f(0, 1, 1), ent->getPositionPointer());
	axisShape->setDashed(true);
	sManager->addShape(axisShape);
	((Planet*)ent)->axis = axisShape;
	
	((Planet*)ent)->orbit = orbit;

	return ent;
}

void Engine::CreateGUI(unsigned int texID, int x, int y, int width, int height){
	gManager->createGui(texID, x, y, width, height);
}

float Engine::getGlobalScale() {
	return globalScale;
}

void Engine::setGlobalScale(float scale) {
	float oldScale = globalScale;
	globalScale = scale;
	float * scales = new float[8]{ 0.383f, 0.984f, 1.0f, 0.532f, 11.21f, 9.45f, 4.01f, 3.88f };
	for (int i = 0; i < planets.size(); i++) {
		Entity * ent = planets.at(i);
		Planet * planet = (Planet *)ent;
		planet->setScale(scales[i] * globalScale * 4.25875e-4f / 8.70f);
		planet->point->opaqueDistance *= scale / oldScale;
		planet->maxDist = planet->point->opaqueDistance;
		planet->trail->minDist *= scale / oldScale;
	}
	delete scales;
}

void Engine::createProjectionMatrix(float width, float height, float FOV) {
	float aspectRatio = width / height;
	float y_scale = (float)(1.0f / tan((FOV / 2.0f) * PI / 180) * aspectRatio);
	float x_scale = y_scale / aspectRatio;
	float frustum_length = FAR_PLANE - NEAR_PLANE;
		
	if (projectionMatrix != nullptr)
		delete projectionMatrix;

	projectionMatrix = new Matrix4f();
	projectionMatrix->SetZero();
	projectionMatrix->matrix[0][0] = x_scale;
	projectionMatrix->matrix[1][1] = y_scale;
	projectionMatrix->matrix[2][2] = -((FAR_PLANE + NEAR_PLANE) / frustum_length);
	projectionMatrix->matrix[2][3] = -1;
	projectionMatrix->matrix[3][2] = -((2 * NEAR_PLANE * FAR_PLANE) / frustum_length);
	projectionMatrix->matrix[3][3] = 0;
}

