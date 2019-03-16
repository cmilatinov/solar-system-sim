#include "HeightsGenerator.h"
#include <iostream>


HeightsGenerator::HeightsGenerator(int seed) {
	this->seed = seed;
}

HeightsGenerator::~HeightsGenerator()
{
}

float HeightsGenerator::generateHeight(int x, int z) {
	float total = 0;
	for (int i = 0; i < OCTAVES; i++) {
		total += getInterpolatedNoise((float)(x * SMOOTH_FACTOR * pow(OCTAVE_SMOOTH_FACTOR, i)),
			(float)(z * SMOOTH_FACTOR * pow(OCTAVE_SMOOTH_FACTOR, i))) * AMPLITUDE * pow(OCTAVE_AMP_FACTOR, i);
	}
	return total;
}

int HeightsGenerator::getSeed() {
	return this->seed;
}

float HeightsGenerator::getInterpolatedNoise(float x, float z) {
	int intX = (int)x;
	int intZ = (int)z;
	float fracX = x - intX;
	float fracZ = z - intZ;

	float v1 = getSmoothNoise(intX, intZ);
	float v2 = getSmoothNoise(intX + 1, intZ);
	float v3 = getSmoothNoise(intX, intZ + 1);
	float v4 = getSmoothNoise(intX + 1, intZ + 1);
	float i1 = interpolate(v1, v2, fracX);
	float i2 = interpolate(v3, v4, fracX);
	return interpolate(i1, i2, fracZ);
}

float HeightsGenerator::getNoise(int x, int z) {
	srand(x * 4345236 + z * 2133454 + this->seed);
	return (float)((rand() % 2001) - 1000.0f) / 1000.0f;
}

float HeightsGenerator::getSmoothNoise(int x, int z) {
	float corners = (getNoise(x - 1, z - 1) + getNoise(x - 1, z + 1) +
		getNoise(x + 1, z - 1) + getNoise(x + 1, z + 1)) * (3.0f/40.0f);
	float sides = (getNoise(x + 1, z) + getNoise(x - 1, z) +
		getNoise(x, z + 1) + getNoise(x, z - 1)) * (3.0f/40.0f);
	float center = getNoise(x, z) * (2.0f/5.0f);
	return corners + sides + center;
}

float HeightsGenerator::interpolate(float a, float b, float blend) {
	double theta = blend * PI;
	float f = (float)(1.0f - cos(theta)) * 0.5f;
	return a *(1.0f - f) + b * f;
}