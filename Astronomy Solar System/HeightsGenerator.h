#include <math.h>
#include <cstdlib>


#define AMPLITUDE 130.0f
#define SMOOTH_FACTOR  1.0f / 20.0f
#define OCTAVES 3
#define OCTAVE_AMP_FACTOR 1.0f / 3.0f
#define OCTAVE_SMOOTH_FACTOR 2.5f
#define PI 3.14159265358979323846

#pragma once
class HeightsGenerator
{
public:
	HeightsGenerator(int seed);
	~HeightsGenerator();

private: 
	int seed;

public:
	float generateHeight(int x, int z);
	int getSeed();

private:
	float getInterpolatedNoise(float x, float z);
	float getNoise(int x, int z);
	float getSmoothNoise(int x, int z);
	float interpolate(float a, float b, float blend);
};

