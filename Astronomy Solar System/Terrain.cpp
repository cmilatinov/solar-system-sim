#include "Terrain.h"+

#include <algorithm>

static HeightsGenerator * hGen = new HeightsGenerator(time(NULL));

Terrain::~Terrain(){
}

Terrain::Terrain(Loader *l, int x, int z) {
	this->x = x;
	this->z = z;
	this->l = l;
	this->m = generateTerrain();
	this->transform = Matrix4f::Identity();
	transform.Scale(Vector3f(Terrain::getScale(), Terrain::getScale(), Terrain::getScale()));
	transform.Translate(Vector3f(x * SIZE, 0, z * SIZE));
	if (bNeedsWater)
		this->water = new Water(WATER_HEIGHT, l);
}

Mesh * Terrain::getTerrainModel() {
	return m;
}

Matrix4f Terrain::getTransform() {
	return this->transform;
}

int Terrain::getGridX() {
	return this->x;
}

int Terrain::getGridZ() {
	return this->z;
}

Mesh * Terrain::generateTerrain() {
	std::cout << "Beginning the generation of terrain [" << this->x << "," << this->z << "]" << std::endl;
	this->heights = new float*[VERTEX_COUNT + 1];
	for (int i = 0; i < VERTEX_COUNT + 1; i++)
		heights[i] = new float[VERTEX_COUNT + 1];

	int size = 3 * (int)pow(VERTEX_COUNT + 1, 2) + 3 * (int)pow(VERTEX_COUNT - 1, 2);

	float * vertices = new float[size];
	float * normals = new float[size];
	float * colors = new float[size];
	float * reflectivity = new float[size / 3];

	for (int i = 0; i < VERTEX_COUNT + 1; i++) {
		float x = i * (SIZE / VERTEX_COUNT);
		for (int j = 0; j < VERTEX_COUNT + 1; j++) {
			float z = j * (SIZE / VERTEX_COUNT);
			float y = hGen->generateHeight((this->x * VERTEX_COUNT) + i, (this->z * VERTEX_COUNT) + j);
			this->heights[i][j] = y;
			if (y <= WATER_HEIGHT && !bNeedsWater)
				bNeedsWater = true;
			vertices[3 * ((i * (VERTEX_COUNT + 1)) + j)] = x;
			vertices[3 * ((i * (VERTEX_COUNT + 1)) + j) + 1] = y;
			vertices[3 * ((i * (VERTEX_COUNT + 1)) + j) + 2] = z;

			//VERTEX COLOR & REFLECTIVITY
			float reflectivity1 = 0.2f;
			Color c1 = Color(0, 0, 0);
			float average1 = vertices[3 * ((i * (VERTEX_COUNT + 1)) + j) + 1];
			if (average1 <= -AMPLITUDE * (1.0f / 8.0f)) {
				float factor = clamp((-(AMPLITUDE * (1.0f / 8.0f)) - average1) / 10.0f, 0, 1.0f);
				c1 = Color::mix(Color::saddleBrown(), Color::sand(), factor);
			}
			else if (average1 <= AMPLITUDE * (1.0f / 50.0f)) {
				float factor = clamp((AMPLITUDE * (1.0f / 50.0f) - average1) / 10.0f, 0, 1.0f);
				c1 = Color::mix(Color::sand(), Color::forestGreen(), factor);
			}
			else if (average1 <= AMPLITUDE * (1.0f / 8.0f)) {
				float factor = clamp((AMPLITUDE * (1.0f / 8.0f) - average1) / 10.0f, 0, 1.0f);
				c1 = Color::mix(Color::forestGreen(), Color::darkGreen(), factor);
			}
			else if (average1 <= AMPLITUDE * (1.0f / 4.0f)) {
				float factor = clamp((AMPLITUDE * (1.0f / 4.0f) - average1) / 10.0f, 0, 1.0f);
				c1 = Color::mix(Color::darkGreen(), Color::brown(), factor);
				reflectivity1 = 0.2f * factor;
			}
			else if (average1 <= AMPLITUDE * (1.0f / 2.5f)) {
				float factor = clamp((AMPLITUDE * (1.0f / 2.5f) - average1) / 10.0f, 0, 1.0f);
				c1 = Color::mix(Color::brown(), Color::lightGrey2(), factor);
				reflectivity1 = 0.2f * (1.0f - factor);
			}
			else if (average1 <= AMPLITUDE)
				c1 = Color::lightGrey2();

			colors[3 * ((i * (VERTEX_COUNT + 1)) + j)] = c1.getR();
			colors[3 * ((i * (VERTEX_COUNT + 1)) + j) + 1] = c1.getG();
			colors[3 * ((i * (VERTEX_COUNT + 1)) + j) + 2] = c1.getB();

			reflectivity[(i * (VERTEX_COUNT + 1)) + j] = reflectivity1;
		}
	}
	/*
	size = 18 * pow(VERTEX_COUNT, 2);
	float * finalVertices = new float[size];
	float * colors = new float[size];
	float * normals = new float[size];
	float * reflectivity = new float[size / 3];

	for (int i = 0; i < size / 18; i++) {
		int row = i % VERTEX_COUNT;
		int column = floor((float)i / (float)(VERTEX_COUNT));
		int bottomLeft = column * (VERTEX_COUNT + 1) + row;
		int topLeft = bottomLeft + 1;
		int bottomRight = bottomLeft + VERTEX_COUNT + 1;
		int topRight = bottomRight + 1;

		Vector3f normal1 = Vector3f(0, 0, 0);
		Vector3f normal2 = Vector3f(0, 0, 0);
		if (row % 2 == column % 2) {
			normal1 = Vector3f::Cross(
				Vector3f(vertices[topLeft * 3], vertices[(topLeft * 3) + 1], vertices[(topLeft * 3) + 2]) -
				Vector3f(vertices[bottomLeft * 3], vertices[(bottomLeft * 3) + 1], vertices[(bottomLeft * 3) + 2]),
				Vector3f(vertices[topRight * 3], vertices[(topRight * 3) + 1], vertices[(topRight * 3) + 2]) -
				Vector3f(vertices[bottomLeft * 3], vertices[(bottomLeft * 3) + 1], vertices[(bottomLeft * 3) + 2]));

			normal2 = Vector3f::Cross(
				Vector3f(vertices[topRight * 3], vertices[(topRight * 3) + 1], vertices[(topRight * 3) + 2]) -
				Vector3f(vertices[bottomLeft * 3], vertices[(bottomLeft * 3) + 1], vertices[(bottomLeft * 3) + 2]),
				Vector3f(vertices[bottomRight * 3], vertices[(bottomRight * 3) + 1], vertices[(bottomRight * 3) + 2]) -
				Vector3f(vertices[bottomLeft * 3], vertices[(bottomLeft * 3) + 1], vertices[(bottomLeft * 3) + 2]));

			normal1 = normal1.Normalize();
			normal2 = normal2.Normalize();

			finalVertices[(i * 18)] = vertices[bottomLeft * 3];
			finalVertices[(i * 18) + 1] = vertices[(bottomLeft * 3) + 1];
			finalVertices[(i * 18) + 2] = vertices[(bottomLeft * 3) + 2];
			normals[(i * 18)] = normal1.x;
			normals[(i * 18) + 1] = normal1.y;
			normals[(i * 18) + 2] = normal1.z;

			finalVertices[(i * 18) + 3] = vertices[topLeft * 3];
			finalVertices[(i * 18) + 4] = vertices[(topLeft * 3) + 1];
			finalVertices[(i * 18) + 5] = vertices[(topLeft * 3) + 2];
			normals[(i * 18) + 3] = normal1.x;
			normals[(i * 18) + 4] = normal1.y;
			normals[(i * 18) + 5] = normal1.z;

			finalVertices[(i * 18) + 6] = vertices[topRight * 3];
			finalVertices[(i * 18) + 7] = vertices[(topRight * 3) + 1];
			finalVertices[(i * 18) + 8] = vertices[(topRight * 3) + 2];
			normals[(i * 18) + 6] = normal1.x;
			normals[(i * 18) + 7] = normal1.y;
			normals[(i * 18) + 8] = normal1.z;

			finalVertices[(i * 18) + 9] = vertices[bottomLeft * 3];
			finalVertices[(i * 18) + 10] = vertices[(bottomLeft * 3) + 1];
			finalVertices[(i * 18) + 11] = vertices[(bottomLeft * 3) + 2];
			normals[(i * 18) + 9] = normal2.x;
			normals[(i * 18) + 10] = normal2.y;
			normals[(i * 18) + 11] = normal2.z;

			finalVertices[(i * 18) + 12] = vertices[topRight * 3];
			finalVertices[(i * 18) + 13] = vertices[(topRight * 3) + 1];
			finalVertices[(i * 18) + 14] = vertices[(topRight * 3) + 2];
			normals[(i * 18) + 12] = normal2.x;
			normals[(i * 18) + 13] = normal2.y;
			normals[(i * 18) + 14] = normal2.z;

			finalVertices[(i * 18) + 15] = vertices[bottomRight * 3];
			finalVertices[(i * 18) + 16] = vertices[(bottomRight * 3) + 1];
			finalVertices[(i * 18) + 17] = vertices[(bottomRight * 3) + 2];
			normals[(i * 18) + 15] = normal2.x;
			normals[(i * 18) + 16] = normal2.y;
			normals[(i * 18) + 17] = normal2.z;

			float reflectivity1 = 0.2f;

			for (int j = 0; j < 6; j++) {
				Color c1 = Color(0, 0, 0);
				float average1 = finalVertices[(i * 18) + (j * 3) + 1];
				if (average1 <= -AMPLITUDE * (1.0f / 8.0f)) {
					float factor = clamp((-(AMPLITUDE * (1.0f / 8.0f)) - average1) / 10.0f, 0, 1.0f);
					c1 = Color::mix(Color::saddleBrown(), Color::sand(), factor);
				}
				else if (average1 <= AMPLITUDE * (1.0f / 50.0f)) {
					float factor = clamp((AMPLITUDE * (1.0f / 50.0f) - average1) / 10.0f, 0, 1.0f);
					c1 = Color::mix(Color::sand(), Color::forestGreen(), factor);
				}
				else if (average1 <= AMPLITUDE * (1.0f / 8.0f)) {
					float factor = clamp((AMPLITUDE * (1.0f / 8.0f) - average1) / 10.0f, 0, 1.0f);
					c1 = Color::mix(Color::forestGreen(), Color::darkGreen(), factor);
				}
				else if (average1 <= AMPLITUDE * (1.0f / 4.0f)) {
					float factor = clamp((AMPLITUDE * (1.0f / 4.0f) - average1) / 10.0f, 0, 1.0f);
					c1 = Color::mix(Color::darkGreen(), Color::brown(), factor);
					reflectivity1 = 0.1f * factor;
				}
				else if (average1 <= AMPLITUDE * (1.0f / 2.5f)) {
					float factor = clamp((AMPLITUDE * (1.0f / 2.5f) - average1) / 10.0f, 0, 1.0f);
					c1 = Color::mix(Color::brown(), Color::lightGrey2(), factor);
					reflectivity1 = 0.1f * (1.0f - factor);
				}
				else if (average1 <= AMPLITUDE)
					c1 = Color::lightGrey2();

				colors[(i * 18) + (j * 3)] = c1.getR();
				colors[(i * 18) + (j * 3) + 1] = c1.getG();
				colors[(i * 18) + (j * 3) + 2] = c1.getB();
				reflectivity[(i * 6) + j] = reflectivity1;
			}
		}else {
			normal1 = Vector3f::Cross(
				Vector3f(vertices[bottomLeft * 3], vertices[(bottomLeft * 3) + 1], vertices[(bottomLeft * 3) + 2]) -
				Vector3f(vertices[bottomRight * 3], vertices[(bottomRight * 3) + 1], vertices[(bottomRight * 3) + 2]),
				Vector3f(vertices[topLeft * 3], vertices[(topLeft * 3) + 1], vertices[(topLeft * 3) + 2]) -
				Vector3f(vertices[bottomRight * 3], vertices[(bottomRight * 3) + 1], vertices[(bottomRight * 3) + 2]));

			normal2 = Vector3f::Cross(
				Vector3f(vertices[topLeft * 3], vertices[(topLeft * 3) + 1], vertices[(topLeft * 3) + 2]) -
				Vector3f(vertices[bottomRight * 3], vertices[(bottomRight * 3) + 1], vertices[(bottomRight * 3) + 2]),
				Vector3f(vertices[topRight * 3], vertices[(topRight * 3) + 1], vertices[(topRight * 3) + 2]) -
				Vector3f(vertices[bottomRight * 3], vertices[(bottomRight * 3) + 1], vertices[(bottomRight * 3) + 2]));

			normal1 = normal1.Normalize();
			normal2 = normal2.Normalize();

			finalVertices[(i * 18)] = vertices[bottomLeft * 3];
			finalVertices[(i * 18) + 1] = vertices[(bottomLeft * 3) + 1];
			finalVertices[(i * 18) + 2] = vertices[(bottomLeft * 3) + 2];
			normals[(i * 18)] = normal1.x;
			normals[(i * 18) + 1] = normal1.y;
			normals[(i * 18) + 2] = normal1.z;

			finalVertices[(i * 18) + 3] = vertices[topLeft * 3];
			finalVertices[(i * 18) + 4] = vertices[(topLeft * 3) + 1];
			finalVertices[(i * 18) + 5] = vertices[(topLeft * 3) + 2];
			normals[(i * 18) + 3] = normal1.x;
			normals[(i * 18) + 4] = normal1.y;
			normals[(i * 18) + 5] = normal1.z;

			finalVertices[(i * 18) + 6] = vertices[bottomRight * 3];
			finalVertices[(i * 18) + 7] = vertices[(bottomRight * 3) + 1];
			finalVertices[(i * 18) + 8] = vertices[(bottomRight * 3) + 2];
			normals[(i * 18) + 6] = normal1.x;
			normals[(i * 18) + 7] = normal1.y;
			normals[(i * 18) + 8] = normal1.z;

			finalVertices[(i * 18) + 9] = vertices[topLeft * 3];
			finalVertices[(i * 18) + 10] = vertices[(topLeft * 3) + 1];
			finalVertices[(i * 18) + 11] = vertices[(topLeft * 3) + 2];
			normals[(i * 18) + 9] = normal2.x;
			normals[(i * 18) + 10] = normal2.y;
			normals[(i * 18) + 11] = normal2.z;

			finalVertices[(i * 18) + 12] = vertices[topRight * 3];
			finalVertices[(i * 18) + 13] = vertices[(topRight * 3) + 1];
			finalVertices[(i * 18) + 14] = vertices[(topRight * 3) + 2];
			normals[(i * 18) + 12] = normal2.x;
			normals[(i * 18) + 13] = normal2.y;
			normals[(i * 18) + 14] = normal2.z;

			finalVertices[(i * 18) + 15] = vertices[bottomRight * 3];
			finalVertices[(i * 18) + 16] = vertices[(bottomRight * 3) + 1];
			finalVertices[(i * 18) + 17] = vertices[(bottomRight * 3) + 2];
			normals[(i * 18) + 15] = normal2.x;
			normals[(i * 18) + 16] = normal2.y;
			normals[(i * 18) + 17] = normal2.z;

			float reflectivity1 = 0.2f;

			for (int j = 0; j < 6; j++) {
				Color c1 = Color(0, 0, 0);
				float average1 = finalVertices[(i * 18) + (j * 3) + 1];
				if (average1 <= -AMPLITUDE * (1.0f / 8.0f)) {
					float factor = clamp((-(AMPLITUDE * (1.0f / 8.0f)) - average1) / 10.0f, 0, 1.0f);
					c1 = Color::mix(Color::saddleBrown(), Color::sand(), factor);
				}
				else if (average1 <= AMPLITUDE * (1.0f / 50.0f)) {
					float factor = clamp((AMPLITUDE * (1.0f / 50.0f) - average1) / 10.0f, 0, 1.0f);
					c1 = Color::mix(Color::sand(), Color::forestGreen(), factor);
				}
				else if (average1 <= AMPLITUDE * (1.0f / 8.0f)) {
					float factor = clamp((AMPLITUDE * (1.0f / 8.0f) - average1) / 10.0f, 0, 1.0f);
					c1 = Color::mix(Color::forestGreen(), Color::darkGreen(), factor);
				}
				else if (average1 <= AMPLITUDE * (1.0f / 4.0f)) {
					float factor = clamp((AMPLITUDE * (1.0f / 4.0f) - average1) / 10.0f, 0, 1.0f);
					c1 = Color::mix(Color::darkGreen(), Color::brown(), factor);
					reflectivity1 = 0.1f * factor;
				}
				else if (average1 <= AMPLITUDE * (1.0f / 2.5f)) {
					float factor = clamp((AMPLITUDE * (1.0f / 2.5f) - average1) / 10.0f, 0, 1.0f);
					c1 = Color::mix(Color::brown(), Color::lightGrey2(), factor);
					reflectivity1 = 0.1f * (1.0f - factor);
				}
				else if (average1 <= AMPLITUDE)
					c1 = Color::lightGrey2();

				colors[(i * 18) + (j * 3)] = c1.getR();
				colors[(i * 18) + (j * 3) + 1] = c1.getG();
				colors[(i * 18) + (j * 3) + 2] = c1.getB();
				reflectivity[(i * 6) + j] = reflectivity1;
			}
		}
	}

	Mesh * m = l->loadTerrainToVAO(finalVertices, size * sizeof(float), normals, size * sizeof(float), colors, size * sizeof(float), reflectivity, (size / 3) * sizeof(float));
	delete[] vertices; delete[] finalVertices; delete[] normals; delete[] colors; delete[] reflectivity;
	return m;
	*/
	
	int startingIndex = 3 * (int)pow(VERTEX_COUNT + 1, 2);
	for (int i = 0; i < VERTEX_COUNT - 1; i++) {
		for (int j = 0; j < VERTEX_COUNT - 1; j++) {
			int currentIndex = startingIndex + (3 * ((i * (VERTEX_COUNT - 1)) + j));
			int originalIndex = 3 * (((i + 1) * (VERTEX_COUNT + 1)) + j);

			vertices[currentIndex] = vertices[originalIndex];
			vertices[currentIndex + 1] = vertices[originalIndex + 1];
			vertices[currentIndex + 2] = vertices[originalIndex + 2];

			//VERTEX COLOR & REFLECTIVITY
			colors[currentIndex] = colors[originalIndex];
			colors[currentIndex + 1] = colors[originalIndex + 1];
			colors[currentIndex + 2] = colors[originalIndex + 2];

			reflectivity[currentIndex / 3] = reflectivity[originalIndex / 3];
		}
	}

	int indicesSize = pow(VERTEX_COUNT, 2) * 6;
	int * indices = new int[indicesSize];
	for (int i = 0; i < indicesSize / 6; i++) {
		int row = i % VERTEX_COUNT;
		int column = floor((float)i / (float)(VERTEX_COUNT));
		int bottomLeft = column * (VERTEX_COUNT + 1) + row;
		int topLeft = bottomLeft + 1;
		int bottomRight = bottomLeft + VERTEX_COUNT + 1;
		int topRight = bottomRight + 1;

		Vector3f normal1 = Vector3f(0, 0, 0);
		Vector3f normal2 = Vector3f(0, 0, 0);

		if (column % 2 == row % 2) {
			indices[(i * 6)] = bottomLeft;
			indices[(i * 6) + 1] = topLeft;
			indices[(i * 6) + 2] = topRight;
			indices[(i * 6) + 3] = bottomRight;
			indices[(i * 6) + 4] = bottomLeft;
			indices[(i * 6) + 5] = topRight;

			normal1 = Vector3f::Cross(
				Vector3f(vertices[topLeft * 3], vertices[(topLeft * 3) + 1], vertices[(topLeft * 3) + 2]) -
				Vector3f(vertices[bottomLeft * 3], vertices[(bottomLeft * 3) + 1], vertices[(bottomLeft * 3) + 2]),
				Vector3f(vertices[topRight * 3], vertices[(topRight * 3) + 1], vertices[(topRight * 3) + 2]) -
				Vector3f(vertices[bottomLeft * 3], vertices[(bottomLeft * 3) + 1], vertices[(bottomLeft * 3) + 2]));
			
			normal2 = Vector3f::Cross(
				Vector3f(vertices[topRight * 3], vertices[(topRight * 3) + 1], vertices[(topRight * 3) + 2]) -
				Vector3f(vertices[bottomLeft * 3], vertices[(bottomLeft * 3) + 1], vertices[(bottomLeft * 3) + 2]),
				Vector3f(vertices[bottomRight * 3], vertices[(bottomRight * 3) + 1], vertices[(bottomRight * 3) + 2]) -
				Vector3f(vertices[bottomLeft * 3], vertices[(bottomLeft * 3) + 1], vertices[(bottomLeft * 3) + 2]));
		}else {
			indices[(i * 6)] = bottomLeft;
			indices[(i * 6) + 1] = topLeft;
			indices[(i * 6) + 2] = bottomRight;
			indices[(i * 6) + 3] = bottomRight;
			indices[(i * 6) + 4] = topLeft;
			indices[(i * 6) + 5] = topRight;

			normal1 = Vector3f::Cross(
				Vector3f(vertices[bottomLeft * 3], vertices[(bottomLeft * 3) + 1], vertices[(bottomLeft * 3) + 2]) -
				Vector3f(vertices[bottomRight * 3], vertices[(bottomRight * 3) + 1], vertices[(bottomRight * 3) + 2]),
				Vector3f(vertices[topLeft * 3], vertices[(topLeft * 3) + 1], vertices[(topLeft * 3) + 2]) -
				Vector3f(vertices[bottomRight * 3], vertices[(bottomRight * 3) + 1], vertices[(bottomRight * 3) + 2]));

			normal2 = Vector3f::Cross(
				Vector3f(vertices[topLeft * 3], vertices[(topLeft * 3) + 1], vertices[(topLeft * 3) + 2]) -
				Vector3f(vertices[bottomRight * 3], vertices[(bottomRight * 3) + 1], vertices[(bottomRight * 3) + 2]),
				Vector3f(vertices[topRight * 3], vertices[(topRight * 3) + 1], vertices[(topRight * 3) + 2]) -
				Vector3f(vertices[bottomRight * 3], vertices[(bottomRight * 3) + 1], vertices[(bottomRight * 3) + 2]));
		}

		normal1 = normal1.Normalize();
		normal2 = normal2.Normalize();

		if (column == 0 && row < VERTEX_COUNT - 1) {
			//normals
			normals[(bottomLeft * 3)] = normal1.x;
			normals[(bottomLeft * 3) + 1] = normal1.y;
			normals[(bottomLeft * 3) + 2] = normal1.z;
			normals[(bottomRight * 3)] = normal2.x;
			normals[(bottomRight * 3) + 1] = normal2.y;
			normals[(bottomRight * 3) + 2] = normal2.z;
		}else if (column > 0 && row < VERTEX_COUNT - 1) {
			//normals
			normals[(bottomRight * 3)] = normal2.x;
			normals[(bottomRight * 3) + 1] = normal2.y;
			normals[(bottomRight * 3) + 2] = normal2.z;
			int newIndex = (column - 1) * (VERTEX_COUNT - 1) + row + (startingIndex / 3);
			normals[(newIndex * 3)] = normal1.x;
			normals[(newIndex * 3) + 1] = normal1.y;
			normals[(newIndex * 3) + 2] = normal1.z;

			//index change
			indices[(i * 6)] = newIndex;
		}else {
			//normals
			normals[(bottomLeft * 3)] = normal1.x;
			normals[(bottomLeft * 3) + 1] = normal1.y;
			normals[(bottomLeft * 3) + 2] = normal1.z;
			normals[(topRight * 3)] = normal2.x;
			normals[(topRight * 3) + 1] = normal2.y;
			normals[(topRight * 3) + 2] = normal2.z;

			//index change
			if (column % 2 == row % 2) {
				indices[(i * 6)] = bottomLeft;
				indices[(i * 6) + 1] = topLeft;
				indices[(i * 6) + 2] = topRight;
				indices[(i * 6) + 3] = topRight;
				indices[(i * 6) + 4] = bottomRight;
				indices[(i * 6) + 5] = bottomLeft;
			}else {
				indices[(i * 6)] = bottomLeft;
				indices[(i * 6) + 1] = topLeft;
				indices[(i * 6) + 2] = bottomRight;
				indices[(i * 6) + 3] = topRight;
				indices[(i * 6) + 4] = bottomRight;
				indices[(i * 6) + 5] = topLeft;
			}
		}
	}



	Mesh * m = l->loadTerrainToVAO(indices, indicesSize * sizeof(int), vertices, size * sizeof(float), normals, size * sizeof(float), colors, size * sizeof(float), reflectivity, (size / 3) * sizeof(float));
	delete[] indices; delete[] vertices; delete[] normals; delete[] colors; delete[] reflectivity;
	return m;
	
}
/*
bool checkArrayVectorIndexIsEmpty(int index, float data[]) {
	for (int i = 0; i < 3; i++) {
		if (data[index] >= 0)
			return false;
	}
	return true;
}
*/
void Terrain::applyColors(float colors[], int i, Color c1, Color c2) {
	colors[i * 18] = c1.getR();
	colors[(i * 18) + 1] = c1.getG();
	colors[(i * 18) + 2] = c1.getB();
	colors[(i * 18) + 3] = c1.getR();
	colors[(i * 18) + 4] = c1.getG();
	colors[(i * 18) + 5] = c1.getB();
	colors[(i * 18) + 6] = c1.getR();
	colors[(i * 18) + 7] = c1.getG();
	colors[(i * 18) + 8] = c1.getB();
	colors[(i * 18) + 9] = c2.getR();
	colors[(i * 18) + 10] = c2.getG();
	colors[(i * 18) + 11] = c2.getB();
	colors[(i * 18) + 12] = c2.getR();
	colors[(i * 18) + 13] = c2.getG();
	colors[(i * 18) + 14] = c2.getB();
	colors[(i * 18) + 15] = c2.getR();
	colors[(i * 18) + 16] = c2.getG();
	colors[(i * 18) + 17] = c2.getB();
}

float Terrain::average(float values[], int size) {
	float result = 0;
	for (int i = 0; i < size; i++) {
		result += values[i];
	}
	result /= size;
	return result;
}

float Terrain::max(float values[], int size) {
	float result = 0;
	for (int i = 0; i < size; i++) {
		if (i == 0)
			result = values[i];
		else if (result < values[i])
			result = values[i];
	}
	return result;
}

float Terrain::min(float values[], int size) {
	float result = 0;
	for (int i = 0; i < size; i++) {
		if (i == 0)
			result = values[i];
		else if (result > values[i])
			result = values[i];
	}
	return result;
}


bool Terrain::needsWater() {
	return bNeedsWater;
}

Water * Terrain::getWater() {
	return water;
}

float Terrain::getTerrainHeight(float x, float z) {
	return heights[(int)floor(x / 5.0f)][(int)floor(z / 5.0f)];
}

HeightsGenerator * Terrain::getHeightsGenerator(){
	return hGen;
}

float Terrain::clamp(float n, float lower, float upper) {
	return std::max(lower, std::min(n, upper));
}