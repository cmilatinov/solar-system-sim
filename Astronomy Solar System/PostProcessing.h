#include "Mesh.h"
#include "ContrastShader.h"
#include "HorizontalBlurShader.h"
#include "VerticalBlurShader.h"
#include "BrightnessGateShader.h"
#include "BloomShader.h"
#include "FBO.h"
#pragma once
class PostProcessing
{
private:
	Mesh * quad;

	FBO * scene;
	FBO * blurTemp;

	FBO * temp;
	FBO * temp2;
	FBO * temp3;
	FBO * temp4;
	FBO * temp5;

	BrightnessGateShader * brightnessShader;
	BloomShader * bloomShader;

	ContrastShader * contrastShader;

	VerticalBlurShader * vBlur;
	HorizontalBlurShader * hBlur;

	void ChangeContrast(FBO * fbo);
	void Blur(FBO * fbo, FBO * out, float scale);
	void BrightnessGate(FBO * fbo);

	void Bloom(FBO * scene, FBO * bloom);

public:

	PostProcessing(Mesh * quad, FBO * fbo);
	~PostProcessing();

	void SetContrast(float contrast);

	void DoPostProcessing();
};

