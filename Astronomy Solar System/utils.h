#include "VectorMatrix.h"
#include "Camera.h" 
#include "Display.h"

#include <string>
#include <vector>

#define PI 3.14159265358979323846

#pragma once
namespace StringUtils {

	static std::vector<std::string> split(std::string string, std::string regex) {
		std::string str = string;
		std::vector<std::string> result;

		size_t pos = 0;
		std::string token;
		while ((pos = str.find(regex)) != std::string::npos) {
			token = str.substr(0, pos);
			result.push_back(token);
			str.erase(0, pos + regex.length());
		}
		result.push_back(str);

		return result;
	}

	static bool startsWith(std::string string, std::string token) {
		return string.find(token) == 0;
	}

	static unsigned char getCharacterAt(std::string string, int index) {
		return unsigned char(string.substr(index, 1).c_str()[0]);
	}

	static int numSpaces(std::string string) {
		std::string strIterate = string;
		int count = 0;
		int token;
		while ((token = strIterate.find(" ")) != std::string::npos) {
			strIterate.erase(0, token + 1);
			count++;
		}
		return count;
	}

	template<typename T>
	std::string to_string_with_precision(const T a_value, const int n = 6)
	{
		std::ostringstream out;
		out << std::setprecision(n) << a_value;
		return out.str();
	}
};

namespace ClassUtils {
	
	template<typename Base, typename T>
	inline bool instanceof(const T *ptr) {
		return dynamic_cast<const Base*>(ptr) != nullptr;
	}

};

namespace MathUtils {
	
	static Vector2f Calculate2DScreenPosition(Matrix4f projectionMatrix, Vector3f center, Camera * cam) {
		Matrix4f * proj = new Matrix4f(projectionMatrix);
		Matrix4f * viewMatrix = cam->createViewMatrix();
		viewMatrix->Transpose();
		proj->Transpose();
		center.Transform(viewMatrix);
		center.Transform(proj);

		Vector2f screenSpace = (1.0f / 2.0f) * Vector2f(center.x, center.y) + Vector2f(0.5f, 0.5f);
		screenSpace = Vector2f(screenSpace.x, (1.0f - screenSpace.y));

		delete viewMatrix;
		delete proj;

		return screenSpace;
	}

	static float SemiMajorAxis(Vector3f pos, Vector3f vel, float mass) {
		float dist = pos.length();
		float velocity = vel.length();
		float mu = 6.67e-11 * (mass + 2e+30);

		//Vector3f hVec = Vector3f::Cross(pos, vel);
		//Vector3f node = Vector3f::Cross(Vector3f(0, 1, 0), hVec);

		//Vector3f eVec = (1.0f / mu) * ((pow(velocity, 2) - (mu / dist)) * pos - (Vector3f::Dot(pos, vel) * vel));
		//float e = eVec.length();

		float mechanicalEnergy = (pow(velocity, 2) / 2.0f) - (mu / dist);
		float a = -mu / (2.0f * mechanicalEnergy);

		return a / 1.5e+11;
	}

	static float PeriodInYears(Vector3f pos, Vector3f vel, float mass) {
		float a = SemiMajorAxis(pos, vel, mass);

		float period = sqrt(pow(a, 3));

		return period;
	}

};