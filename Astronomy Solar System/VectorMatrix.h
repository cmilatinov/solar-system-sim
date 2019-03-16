#pragma once
#include <math.h>
#include <iostream>

# define PI 3.14159265358979323846

class Matrix4f;

class Vector2f
{
public:
	float x, y;

	Vector2f();
	Vector2f(float x, float y);
	~Vector2f();

	static Vector2f Sub(Vector2f a, Vector2f b);
	static float Dot(Vector2f a, Vector2f b);

	friend Vector2f operator+(const Vector2f& left, const Vector2f& right);
	friend Vector2f operator-(const Vector2f& left, const Vector2f& right);
	friend Vector2f operator*(const float& left, const Vector2f& right);

	Vector2f Reverse();

	float length();

	void SetZero();
	void Print();
	void Substract(Vector2f vec);
	Vector2f Normalize();

};

class Vector3f
{
public:
	float x, y, z;

	Vector3f();
	Vector3f(float x, float y, float z);
	~Vector3f();

	static Vector3f Sub(Vector3f a, Vector3f b);
	static Vector3f Cross(Vector3f a, Vector3f b);
	static float Dot(Vector3f a, Vector3f b);

	friend Vector3f operator*(const float& left, const Vector3f& right);
	friend Vector3f operator*(const Matrix4f& left, const Vector3f& right);
	friend Vector3f operator+(const Vector3f& left, const Vector3f& right);
	//friend Vector3f operator+=(const Vector3f& left, const Vector3f& right);
	friend Vector3f operator-(const Vector3f& left, const Vector3f& right);
	
	Vector3f Reverse();
	
	float length();

	void SetZero();
	void Print();
	void Transform(Matrix4f *transform);
	void Substract(Vector3f vec);
	Vector3f Normalize();

};

class Vector4f
{
public:
	float x, y, z, w;

	Vector4f();
	Vector4f(float x, float y, float z, float w);
	~Vector4f();

	static Vector4f Sub(Vector4f a, Vector4f b);
	static float Dot(Vector4f a, Vector4f b);

	friend Vector4f operator*(const float& left, const Vector4f& right);
	friend Vector4f operator*(const Matrix4f& left, const Vector4f& right);
	friend Vector4f operator+(const Vector4f& left, const Vector4f& right);
	friend Vector4f operator-(const Vector4f& left, const Vector4f& right);

	Vector4f Reverse();

	float length();

	void SetZero();
	void Print();
	void Transform(Matrix4f *transform);
	void Substract(Vector4f vec);
	Vector4f Normalize();
	Vector4f NormalizePlane();

};


class Matrix4f
{
public:
	float matrix[4][4];

	Matrix4f(float data[4][4]);
	Matrix4f();	
	~Matrix4f();

	static Matrix4f Identity();
	static Matrix4f Multiply(Matrix4f left, Matrix4f right);
	static Matrix4f * CreateProjectionMatrix(float width, float height, float FOV, float FAR_PLANE, float NEAR_PLANE);

	Matrix4f operator *(const Matrix4f& right);
	Matrix4f operator -(const Matrix4f& right);
	Matrix4f operator +(const Matrix4f& right);

	Matrix4f Inverse();
	Matrix4f Copy();

	float Determinant();

	bool Equals(Matrix4f mat);
	bool IsNonZero();
	
	void SetIdentity();
	void SetZero();

	void Translate(Vector3f vec);
	void Rotate(Vector3f rot);
	void Rotate(float angle, Vector3f axis);
	void Rotate(float angle, Vector3f axis, Matrix4f *dest);
	void Scale(float x, float y, float z);
	void Scale(Vector3f scale);
	static Matrix4f Translate(Vector3f vec, Matrix4f *src, Matrix4f *dest);
	static Matrix4f Rotate(float angle, Vector3f axis, Matrix4f *src, Matrix4f *dest);
	static Matrix4f Scale(Vector3f scale, Matrix4f *src, Matrix4f *dest);
	
	void Transpose();

	void Print();	

private: 
	void Set(Matrix4f m);
	float Cos(float angle);
	float Sin(float angle);
	float ToRadians(float angle);
	float Determinant3x3(float t00, float t01, float t02,
		float t10, float t11, float t12,
		float t20, float t21, float t22);
};




