#include "VectorMatrix.h"
#include <algorithm>
#pragma once
class Color
{
public:
	Vector3f color;

	friend Color operator*(const float& left, Color& right);

	Color();
	Color(int r, int g, int b);
	Color(float r, float g, float b);
	~Color();

	Vector3f get();

	float * toArray();

	float getR();

	float getG();

	float getB();

	static Color mix(Color a1, Color b2, float factor);

	static Color red(){return Color(255, 0, 0);}
	static Color lime(){return Color(0, 255, 0);}
	static Color blue(){return Color(0, 0, 255);}
	static Color cyan(){return Color(0, 255, 255);}
	static Color yellow(){return Color(255, 255, 0);}
	static Color orange(){return Color(255, 165, 0);}
	static Color purple(){return Color(128, 0, 128);}
	static Color pink(){return Color(255, 20, 147);}
	static Color white(){return Color(255, 255, 255);}
	static Color black(){return Color(0, 0, 0);}

	static Color sand(){return Color(173, 155, 55);}

	static Color grey(){return Color(128, 128, 128);}
	static Color lightGrey(){return Color(169, 169, 169);}
	static Color lightGrey2(){return Color(175, 175, 175);}
	static Color lightGrey3(){return Color(162, 162, 162);}

	static Color forestGreen(){return Color(34, 139, 34);}
	static Color darkOliveGreen(){return Color(85, 107, 47);}
	static Color darkOliveGreen2(){return Color(92, 115, 54);}
	static Color darkOliveGreen3(){return Color(100, 123, 62);}
	static Color oliveGreen(){return Color(107, 142, 35);}
	static Color green(){return Color(0, 128, 0);}
	static Color limeGreen(){return Color(50, 205, 50);}
	static Color darkGreen(){return Color(0, 100, 0);}

	static Color brown(){return Color(139, 69, 19);}
	static Color sienna(){return Color(146, 76, 30);}
	static Color saddleBrown(){return Color(154, 82, 35);}
	
	static Color oceanBlue(){return 0.7f * Color(0.2823f, 0.7882f, 0.7333f);}
	static Color skyBlue(){return Color(36, 100, 164);}
};

