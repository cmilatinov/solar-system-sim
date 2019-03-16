#include "Color.h"
#include "time.h"


Color::Color() {
	color = Vector3f(0, 0, 0);
}

Color::Color(int r, int g, int b) {
	color = Vector3f(std::min((float)r / 255.0f, 1.0f), std::min((float)g / 255.0f, 1.0f), std::min((float)b / 255.0f, 1.0f));
}

Color::Color(float r, float g, float b) {
	color = Vector3f(std::min(r, 1.0f), std::min(g, 1.0f), std::min(b, 1.0f));
}

Color::~Color()
{
}

Vector3f Color::get() {
	return color;
}

float * Color::toArray() {
	return new float[3] {color.x, color.y, color.z};
}

float Color::getR() {
	return color.x;
}

float Color::getG() {
	return color.y;
}

float Color::getB() {
	return color.z;
}

Color Color::mix(Color a1, Color b2, float factor) {
	factor = std::min(factor, 1.0f);
	float r = (a1.getR() * factor) + (b2.getR() * (1.0f - factor));
	float g = (a1.getG() * factor) + (b2.getG() * (1.0f - factor));
	float b = (a1.getB() * factor) + (b2.getB() * (1.0f - factor));
	return Color(r, g, b);
}

Color operator*(const float & left, Color& right)
{
	return Color(left * right.getR(), left * right.getG(), left * right.getB());
}
