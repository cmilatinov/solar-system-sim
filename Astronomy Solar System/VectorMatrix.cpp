#include "VectorMatrix.h"

#include <algorithm>

Matrix4f::Matrix4f(float data[4][4]) {
	std::copy(&data[0][0], &data[0][0] + 16, &matrix[0][0]);
}

Matrix4f::Matrix4f() {

}

Matrix4f::~Matrix4f()
{
	//delete[] matrix;
}

Matrix4f Matrix4f::Identity() {
	Matrix4f i = Matrix4f();
	i.SetIdentity();
	return i;
}

void Matrix4f::SetIdentity() {
	float data[4][4] = {
		{ 1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 }
	};
	Set(Matrix4f(data));
}

void Matrix4f::SetZero() {
	float data[4][4] = {
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 }
	};
	Set(Matrix4f(data));
}

Matrix4f Matrix4f::Multiply(Matrix4f left, Matrix4f right) {
	float result[4][4];
	for (int i = 0; i < 4; i++) {
		float row[4] = { left.matrix[i][0], left.matrix[i][1], left.matrix[i][2], left.matrix[i][3] };
		for (int j = 0; j < 4; j++) {
			float column[4] = { right.matrix[0][j], right.matrix[1][j], right.matrix[2][j], right.matrix[3][j] };
			result[i][j] =
				(row[0] * column[0]) +
				(row[1] * column[1]) +
				(row[2] * column[2]) +
				(row[3] * column[3]);
		}
	}
	return Matrix4f(result);
}

bool Matrix4f::Equals(Matrix4f mat) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (matrix[i][j] != mat.matrix[i][j]) 
				return false;
		}
	}
	return true;
}

float Matrix4f::Determinant() {
	float f = matrix[0][0]
		* ((matrix[1][1] * matrix[2][2] * matrix[3][3] + matrix[1][2] * matrix[2][3] * matrix[3][1] + matrix[1][3] * matrix[2][1] * matrix[3][2])
			- matrix[1][3] * matrix[2][2] * matrix[3][1]
			- matrix[1][1] * matrix[2][3] * matrix[3][2]
			- matrix[1][2] * matrix[2][1] * matrix[3][3]);
	f -= matrix[0][1]
		* ((matrix[1][0] * matrix[2][2] * matrix[3][3] + matrix[1][2] * matrix[2][3] * matrix[3][0] + matrix[1][3] * matrix[2][0] * matrix[3][2])
			- matrix[1][3] * matrix[2][2] * matrix[3][0]
			- matrix[1][0] * matrix[2][3] * matrix[3][2]
			- matrix[1][2] * matrix[2][0] * matrix[3][3]);
	f += matrix[0][2]
		* ((matrix[1][0] * matrix[2][1] * matrix[3][3] + matrix[1][1] * matrix[2][3] * matrix[3][0] + matrix[1][3] * matrix[2][0] * matrix[3][1])
			- matrix[1][3] * matrix[2][1] * matrix[3][0]
			- matrix[1][0] * matrix[2][3] * matrix[3][1]
			- matrix[1][1] * matrix[2][0] * matrix[3][3]);
	f -= matrix[0][3]
		* ((matrix[1][0] * matrix[2][1] * matrix[3][2] + matrix[1][1] * matrix[2][2] * matrix[3][0] + matrix[1][2] * matrix[2][0] * matrix[3][1])
			- matrix[1][2] * matrix[2][1] * matrix[3][0]
			- matrix[1][0] * matrix[2][2] * matrix[3][1]
			- matrix[1][1] * matrix[2][0] * matrix[3][2]);
	return f;
}

Matrix4f * Matrix4f::CreateProjectionMatrix(float width, float height, float FOV, float FAR_PLANE, float NEAR_PLANE) {
	float aspectRatio = width / height;
	float y_scale = (float)(1.0f / tan((FOV / 2.0f) * PI / 180) * aspectRatio);
	float x_scale = y_scale / aspectRatio;
	float frustum_length = FAR_PLANE - NEAR_PLANE;

	Matrix4f *projectionMatrix = new Matrix4f();
	projectionMatrix->matrix[0][0] = x_scale;
	projectionMatrix->matrix[1][1] = y_scale;
	projectionMatrix->matrix[2][2] = -((FAR_PLANE + NEAR_PLANE) / frustum_length);
	projectionMatrix->matrix[2][3] = -1;
	projectionMatrix->matrix[3][2] = -((2 * NEAR_PLANE * FAR_PLANE) / frustum_length);
	projectionMatrix->matrix[3][3] = 0;
	return projectionMatrix;
}

Matrix4f Matrix4f::Copy() {
	return Matrix4f(matrix);
}

void Matrix4f::Translate(Vector3f vec) {
	Translate(vec, this, this);
}

Matrix4f Matrix4f::Translate(Vector3f vec, Matrix4f *src, Matrix4f *dest) {
	if (!dest)
		dest = new Matrix4f();

	dest->matrix[3][0] += src->matrix[0][0] * vec.x + src->matrix[1][0] * vec.y + src->matrix[2][0] * vec.z;
	dest->matrix[3][1] += src->matrix[0][1] * vec.x + src->matrix[1][1] * vec.y + src->matrix[2][1] * vec.z;
	dest->matrix[3][2] += src->matrix[0][2] * vec.x + src->matrix[1][2] * vec.y + src->matrix[2][2] * vec.z;
	dest->matrix[3][3] += src->matrix[0][3] * vec.x + src->matrix[1][3] * vec.y + src->matrix[2][3] * vec.z;

	return *dest;
}

void Matrix4f::Rotate(float angle, Vector3f axis) {
	Rotate(angle, axis, this);
}

void Matrix4f::Rotate(float angle, Vector3f axis, Matrix4f *dest) {
	Rotate(angle, axis, this, dest);
}

Matrix4f Matrix4f::Rotate(float angle, Vector3f axis, Matrix4f *src, Matrix4f *dest) {
	if(!dest)
		dest = new Matrix4f();
	float c = cos(angle * PI / 180);
	float s = sin(angle * PI / 180);
	float oneminusc = 1.0f - c;
	float xy = axis.x*axis.y;
	float yz = axis.y*axis.z;
	float xz = axis.x*axis.z;
	float xs = axis.x*s;
	float ys = axis.y*s;
	float zs = axis.z*s;

	float f00 = axis.x*axis.x*oneminusc + c;
	float f01 = xy*oneminusc + zs;
	float f02 = xz*oneminusc - ys;
	// n[3] not used
	float f10 = xy*oneminusc - zs;
	float f11 = axis.y*axis.y*oneminusc + c;
	float f12 = yz*oneminusc + xs;
	// n[7] not used
	float f20 = xz*oneminusc + ys;
	float f21 = yz*oneminusc - xs;
	float f22 = axis.z*axis.z*oneminusc + c;

	float t00 = src->matrix[0][0] * f00 + src->matrix[1][0] * f01 + src->matrix[2][0] * f02;
	float t01 = src->matrix[0][1] * f00 + src->matrix[1][1] * f01 + src->matrix[2][1] * f02;
	float t02 = src->matrix[0][2] * f00 + src->matrix[1][2] * f01 + src->matrix[2][2] * f02;
	float t03 = src->matrix[0][3] * f00 + src->matrix[1][3] * f01 + src->matrix[2][3] * f02;
	float t10 = src->matrix[0][0] * f10 + src->matrix[1][0] * f11 + src->matrix[2][0] * f12;
	float t11 = src->matrix[0][1] * f10 + src->matrix[1][1] * f11 + src->matrix[2][1] * f12;
	float t12 = src->matrix[0][2] * f10 + src->matrix[1][2] * f11 + src->matrix[2][2] * f12;
	float t13 = src->matrix[0][3] * f10 + src->matrix[1][3] * f11 + src->matrix[2][3] * f12;
	dest->matrix[2][0] = src->matrix[0][0] * f20 + src->matrix[1][0] * f21 + src->matrix[2][0] * f22;
	dest->matrix[2][1] = src->matrix[0][1] * f20 + src->matrix[1][1] * f21 + src->matrix[2][1] * f22;
	dest->matrix[2][2] = src->matrix[0][2] * f20 + src->matrix[1][2] * f21 + src->matrix[2][2] * f22;
	dest->matrix[2][3] = src->matrix[0][3] * f20 + src->matrix[1][3] * f21 + src->matrix[2][3] * f22;
	dest->matrix[0][0] = t00;
	dest->matrix[0][1] = t01;
	dest->matrix[0][2] = t02;
	dest->matrix[0][3] = t03;
	dest->matrix[1][0] = t10;
	dest->matrix[1][1] = t11;
	dest->matrix[1][2] = t12;
	dest->matrix[1][3] = t13;
	return *dest;
}

void Matrix4f::Rotate(Vector3f rot) {
	Rotate(rot.x, Vector3f(1, 0, 0));
	Rotate(rot.y, Vector3f(0, 1, 0));
	Rotate(rot.z, Vector3f(0, 0, 1));
}

void Matrix4f::Scale(float x, float y, float z) {
	Matrix4f *scale = new Matrix4f();
	scale->SetIdentity();
	scale->matrix[0][0] = x;
	scale->matrix[1][1] = y;
	scale->matrix[2][2] = z;
	Set(Matrix4f::Multiply(*scale, *this));
	delete scale;
}

void Matrix4f::Scale(Vector3f scale) {
	Scale(scale, this, this);
}

Matrix4f Matrix4f::Scale(Vector3f vec, Matrix4f *src, Matrix4f *dest) {
	if (!dest)
		dest = new Matrix4f();
	dest->matrix[0][0] = src->matrix[0][0] * vec.x;
	dest->matrix[0][1] = src->matrix[0][1] * vec.x;
	dest->matrix[0][2] = src->matrix[0][2] * vec.x;
	dest->matrix[0][3] = src->matrix[0][3] * vec.x;
	dest->matrix[1][0] = src->matrix[1][0] * vec.y;
	dest->matrix[1][1] = src->matrix[1][1] * vec.y;
	dest->matrix[1][2] = src->matrix[1][2] * vec.y;
	dest->matrix[1][3] = src->matrix[1][3] * vec.y;
	dest->matrix[2][0] = src->matrix[2][0] * vec.z;
	dest->matrix[2][1] = src->matrix[2][1] * vec.z;
	dest->matrix[2][2] = src->matrix[2][2] * vec.z;
	dest->matrix[2][3] = src->matrix[2][3] * vec.z;
	return *dest;
}

void Matrix4f::Transpose(){
	Matrix4f copy = Matrix4f(*this);
	this->matrix[0][1] = copy.matrix[1][0];
	this->matrix[0][2] = copy.matrix[2][0];
	this->matrix[0][3] = copy.matrix[3][0];
	this->matrix[1][2] = copy.matrix[2][1];
	this->matrix[1][3] = copy.matrix[3][1];
	this->matrix[2][3] = copy.matrix[3][2];

	this->matrix[1][0] = copy.matrix[0][1];
	this->matrix[2][0] = copy.matrix[0][2];
	this->matrix[2][1] = copy.matrix[1][2];
	this->matrix[3][0] = copy.matrix[0][3];
	this->matrix[3][1] = copy.matrix[1][3];
	this->matrix[3][2] = copy.matrix[2][3];
}

Matrix4f Matrix4f::operator*(const Matrix4f& right) {
	return Matrix4f::Multiply(*this, right);
}

Matrix4f Matrix4f::operator-(const Matrix4f& right) {
	Matrix4f* result = new Matrix4f();
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result->matrix[i][j] = matrix[i][j] - right.matrix[i][j];
		}
	}
	return *result;
}

Matrix4f Matrix4f::operator+(const Matrix4f& right) {
	Matrix4f* result = new Matrix4f();
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result->matrix[i][j] = matrix[i][j] + right.matrix[i][j];
		}
	}
	return *result;
}

Vector3f operator*(const Matrix4f &left, const Vector3f &right) {
	float vec[4] = { right.x, right.y, right.z, 1.0f };
	float tempVec[4] = { right.x, right.y, right.z, 1.0f };
	for (int i = 0; i < 4; i++) {
		float row[4] = { left.matrix[0][i], left.matrix[1][i], left.matrix[2][i], left.matrix[3][i] };
		tempVec[i] = (vec[0] * row[0]) + (vec[1] * row[1]) + (vec[2] * row[2]) + (vec[3] * row[3]);
	}
	float x = tempVec[0] / tempVec[3];
	float y = tempVec[1] / tempVec[3];
	float z = tempVec[2] / tempVec[3];
	return Vector3f(x, y, z);
}

void Matrix4f::Print() {
	std::cout << char(201) << "\t" << matrix[0][0] << "\t" << matrix[0][1] << "\t" << matrix[0][2] << "\t" << matrix[0][3] << "\t" << char(187) << std::endl;
	std::cout << char(186) << "\t" << matrix[1][0] << "\t" << matrix[1][1] << "\t" << matrix[1][2] << "\t" << matrix[1][3] << "\t" << char(186) << std::endl;
	std::cout << char(186) << "\t" << matrix[2][0] << "\t" << matrix[2][1] << "\t" << matrix[2][2] << "\t" << matrix[2][3] << "\t" << char(186) << std::endl;
	std::cout << char(200) << "\t" << matrix[3][0] << "\t" << matrix[3][1] << "\t" << matrix[3][2] << "\t" << matrix[3][3] << "\t" << char(188) << std::endl;
}

void Matrix4f::Set(Matrix4f m) {
	std::copy(&m.matrix[0][0], &m.matrix[0][0] + 16, &matrix[0][0]);
}

float Matrix4f::Cos(float angle) {
	if (cos(angle) < 0.000001f && cos(angle) > -0.000001f)
		return 0;
	else return cos(angle);
}

float Matrix4f::Sin(float angle) {
	if (sin(angle) < 0.000001f && sin(angle) > -0.000001f)
		return 0;
	else return sin(angle);
}

float Matrix4f::ToRadians(float angle) {
	return angle * PI / 180;
}



float Matrix4f:: Determinant3x3(float t00, float t01, float t02,
					float t10, float t11, float t12, 
					float t20, float t21, float t22)
{
	return   t00 * (t11 * t22 - t12 * t21)
		+ t01 * (t12 * t20 - t10 * t22)
		+ t02 * (t10 * t21 - t11 * t20);
}

Matrix4f Matrix4f::Inverse() {
	float determinant = Determinant();

	if (determinant != 0) {
		//if (dest == NULL)
		Matrix4f dest = Matrix4f();
		float determinant_inv = 1.0f / determinant;

		// first row
		float t00 = Determinant3x3(matrix[1][1], matrix[1][2], matrix[1][3], matrix[2][1], matrix[2][2], matrix[2][3], matrix[3][1], matrix[3][2], matrix[3][3]);
		float t01 = -Determinant3x3(matrix[1][0], matrix[1][2], matrix[1][3], matrix[2][0], matrix[2][2], matrix[2][3], matrix[3][0], matrix[3][2], matrix[3][3]);
		float t02 = Determinant3x3(matrix[1][0], matrix[1][1], matrix[1][3], matrix[2][0], matrix[2][1], matrix[2][3], matrix[3][0], matrix[3][1], matrix[3][3]);
		float t03 = -Determinant3x3(matrix[1][0], matrix[1][1], matrix[1][2], matrix[2][0], matrix[2][1], matrix[2][2], matrix[3][0], matrix[3][1], matrix[3][2]);
		// second row
		float t10 = -Determinant3x3(matrix[0][1], matrix[0][2], matrix[0][3], matrix[2][1], matrix[2][2], matrix[2][3], matrix[3][1], matrix[3][2], matrix[3][3]);
		float t11 = Determinant3x3(matrix[0][0], matrix[0][2], matrix[0][3], matrix[2][0], matrix[2][2], matrix[2][3], matrix[3][0], matrix[3][2], matrix[3][3]);
		float t12 = -Determinant3x3(matrix[0][0], matrix[0][1], matrix[0][3], matrix[2][0], matrix[2][1], matrix[2][3], matrix[3][0], matrix[3][1], matrix[3][3]);
		float t13 = Determinant3x3(matrix[0][0], matrix[0][1], matrix[0][2], matrix[2][0], matrix[2][1], matrix[2][2], matrix[3][0], matrix[3][1], matrix[3][2]);
		// third row
		float t20 = Determinant3x3(matrix[0][1], matrix[0][2], matrix[0][3], matrix[1][1], matrix[1][2], matrix[1][3], matrix[3][1], matrix[3][2], matrix[3][3]);
		float t21 = -Determinant3x3(matrix[0][0], matrix[0][2], matrix[0][3], matrix[1][0], matrix[1][2], matrix[1][3], matrix[3][0], matrix[3][2], matrix[3][3]);
		float t22 = Determinant3x3(matrix[0][0], matrix[0][1], matrix[0][3], matrix[1][0], matrix[1][1], matrix[1][3], matrix[3][0], matrix[3][1], matrix[3][3]);
		float t23 = -Determinant3x3(matrix[0][0], matrix[0][1], matrix[0][2], matrix[1][0], matrix[1][1], matrix[1][2], matrix[3][0], matrix[3][1], matrix[3][2]);
		// fourth row
		float t30 = -Determinant3x3(matrix[0][1], matrix[0][2], matrix[0][3], matrix[1][1], matrix[1][2], matrix[1][3], matrix[2][1], matrix[2][2], matrix[2][3]);
		float t31 = Determinant3x3(matrix[0][0], matrix[0][2], matrix[0][3], matrix[1][0], matrix[1][2], matrix[1][3], matrix[2][0], matrix[2][2], matrix[2][3]);
		float t32 = -Determinant3x3(matrix[0][0], matrix[0][1], matrix[0][3], matrix[1][0], matrix[1][1], matrix[1][3], matrix[2][0], matrix[2][1], matrix[2][3]);
		float t33 = Determinant3x3(matrix[0][0], matrix[0][1], matrix[0][2], matrix[1][0], matrix[1][1], matrix[1][2], matrix[2][0], matrix[2][1], matrix[2][2]);

		// transpose and divide by the determinant
		dest.matrix[0][0] = t00*determinant_inv;
		dest.matrix[1][1] = t11*determinant_inv;
		dest.matrix[2][2] = t22*determinant_inv;
		dest.matrix[3][3] = t33*determinant_inv;
		dest.matrix[0][1] = t10*determinant_inv;
		dest.matrix[1][0] = t01*determinant_inv;
		dest.matrix[2][0] = t02*determinant_inv;
		dest.matrix[0][2] = t20*determinant_inv;
		dest.matrix[1][2] = t21*determinant_inv;
		dest.matrix[2][1] = t12*determinant_inv;
		dest.matrix[0][3] = t30*determinant_inv;
		dest.matrix[3][0] = t03*determinant_inv;
		dest.matrix[1][3] = t31*determinant_inv;
		dest.matrix[3][1] = t13*determinant_inv;
		dest.matrix[3][2] = t23*determinant_inv;
		dest.matrix[2][3] = t32*determinant_inv;
		return dest;
	}else 
		return Matrix4f();
}

bool Matrix4f::IsNonZero() {
	return Equals(*(new Matrix4f()));
}

Vector3f::Vector3f() {
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

Vector3f::Vector3f(float posX, float posY, float posZ) {
	x = posX;
	y = posY;
	z = posZ;
}

Vector3f::~Vector3f() {
}

void Vector3f::Print() {
	std::cout << char(201) << "\t" << x << "\t" << char(187) << std::endl;
	std::cout << char(186) << "\t" << y << "\t" << char(186) << std::endl;
	std::cout << char(200) << "\t" << z << "\t" << char(188) << std::endl;
}

Vector3f Vector3f::Reverse() {
	Vector3f vec = Vector3f(-x, -y, -z);
	return vec;
}

void Vector3f::Transform(Matrix4f *transform) {
	float vec[4] = { x, y, z, 1.0f };
	float tempVec[4] = { x, y, z, 1.0f };
	for (int i = 0; i < 4; i++) {
		float row[4] = { transform->matrix[i][0], transform->matrix[i][1], transform->matrix[i][2], transform->matrix[i][3] };
		tempVec[i] = (vec[0] * row[0]) + (vec[1] * row[1]) + (vec[2] * row[2]) + (vec[3] * row[3]);
	}
	this->x = tempVec[0] / tempVec[3];
	this->y = tempVec[1] / tempVec[3];
	this->z = tempVec[2] / tempVec[3];
}

void Vector3f::Substract(Vector3f vec) {
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
}

Vector3f Vector3f::Normalize(){
	float length = this->length();
	this->x /= length;
	this->y /= length;
	this->z /= length;
	return *this;
}


Vector3f Vector3f::Sub(Vector3f a, Vector3f b) {
	return Vector3f(a.x - b.x, a.y - b.y, a.z - b.z);
}

void Vector3f::SetZero() {
	x = 0;
	y = 0;
	z = 0;
}

float Vector3f::length() {
	return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

Vector3f Vector3f::Cross(Vector3f a, Vector3f b) {
	Vector3f i = Vector3f(1, 0, 0);
	Vector3f j = Vector3f(0, 1, 0);
	Vector3f k = Vector3f(0, 0, 1);
	Vector3f result1 = ((a.y * b.z) - (b.y * a.z)) * i;
	Vector3f result2 = ((a.x * b.z) - (b.x * a.z)) * j;
	Vector3f result3 = ((a.x * b.y) - (b.x * a.y)) * k;
	return result1 - result2 + result3;
}

float Vector3f::Dot(Vector3f a, Vector3f b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

Vector2f operator+(const Vector2f & left, const Vector2f & right){
	return Vector2f(left.x + right.x, left.y + right.y);
}

Vector2f operator-(const Vector2f & left, const Vector2f & right){
	return Vector2f(left.x - right.x, left.y - right.y);
}

Vector2f operator*(const float & left, const Vector2f & right){
	return Vector2f(left * right.x, left * right.y);
}

Vector3f operator*(const float& left, const Vector3f& right) {
	return Vector3f(left * right.x, left * right.y, left * right.z);
}

Vector3f operator+(const Vector3f& left, const Vector3f& right) {
	return Vector3f(left.x + right.x, left.y + right.y, left.z + right.z);
}

Vector3f operator-(const Vector3f& left, const Vector3f& right) {
	return Vector3f(left.x - right.x, left.y - right.y, left.z - right.z);
}

Vector4f operator*(const float & left, const Vector4f & right)
{
	return Vector4f(left * right.x, left * right.y, left * right.z, left * right.w);
}

Vector4f operator*(const Matrix4f & left, const Vector4f & right)
{
	return Vector4f(left.matrix[0][0] * right.x + left.matrix[0][1] * right.x + left.matrix[0][2] * right.x + left.matrix[0][3] * right.x,
					left.matrix[1][0] * right.y + left.matrix[1][1] * right.y + left.matrix[1][2] * right.y + left.matrix[1][3] * right.y, 
					left.matrix[2][0] * right.z + left.matrix[2][1] * right.z + left.matrix[2][2] * right.z + left.matrix[2][3] * right.z, 
					left.matrix[3][0] * right.w + left.matrix[3][1] * right.w + left.matrix[3][2] * right.w + left.matrix[3][3] * right.w);
}

Vector4f operator+(const Vector4f & left, const Vector4f & right)
{
	return Vector4f(left.x + right.x, left.y + right.y, left.z + right.z, left.w + right.w);
}

Vector4f operator-(const Vector4f & left, const Vector4f & right)
{
	return Vector4f(left.x - right.x, left.y - right.y, left.z - right.z, left.w - right.w);
}

Vector4f::Vector4f()
{
}

Vector4f::Vector4f(float x, float y, float z, float w){
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

Vector4f::~Vector4f()
{
}

Vector4f Vector4f::Sub(Vector4f a, Vector4f b)
{
	return Vector4f(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

float Vector4f::Dot(Vector4f a, Vector4f b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
}

Vector4f Vector4f::Reverse()
{
	return -1.0f * (*this);
}

float Vector4f::length()
{
	return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2) + pow(w, 2));
}

void Vector4f::SetZero(){
	x = 0;
	y = 0;
	z = 0;
	w = 0;
}

void Vector4f::Print(){
	std::cout << char(201) << "\t" << x << "\t" << char(187) << std::endl;
	std::cout << char(186) << "\t" << y << "\t" << char(186) << std::endl;
	std::cout << char(186) << "\t" << z << "\t" << char(186) << std::endl;
	std::cout << char(200) << "\t" << w << "\t" << char(188) << std::endl;
}

void Vector4f::Transform(Matrix4f * transform){
	Vector4f result = (*transform) * (*this);
	x = result.x;
	y = result.y;
	z = result.z;
	w = result.w;
}

void Vector4f::Substract(Vector4f vec){
	Vector4f result = (*this) - vec;
	x = result.x;
	y = result.y;
	z = result.z;
	w = result.w;
}

Vector4f Vector4f::Normalize()
{
	float length = this->length();
	this->x /= length;
	this->y /= length;
	this->z /= length;
	this->w /= length;
	return *this;
}

Vector4f Vector4f::NormalizePlane(){
	float length = Vector3f(this->x, this->y, this->z).length();
	return Vector4f(this->x / length, this->y / length, this->z / length, this->w / length);
}

Vector2f::Vector2f(){
	this->x = 0.0f;
	this->y = 0.0f;
}

Vector2f::Vector2f(float x, float y){
	this->x = x;
	this->y = y;
}

Vector2f::~Vector2f(){
}

Vector2f Vector2f::Sub(Vector2f a, Vector2f b)
{
	return Vector2f(a.x - b.x, a.y - b.y);
}

float Vector2f::Dot(Vector2f a, Vector2f b)
{
	return (a.x * b.x) + (a.y * b.y);
}

Vector2f Vector2f::Reverse(){
	return -1.0f * Vector2f();
}

float Vector2f::length(){
	return sqrt(pow(x, 2) + pow(y, 2));
}

void Vector2f::SetZero(){
	this->x = 0.0f;
	this->y = 0.0f;
}

void Vector2f::Print(){
	std::cout << char(201) << "\t" << x << "\t" << char(187) << std::endl;
	std::cout << char(200) << "\t" << y << "\t" << char(188) << std::endl;
}

void Vector2f::Substract(Vector2f vec){
	this->x -= vec.x;
	this->y -= vec.y;
}

Vector2f Vector2f::Normalize(){
	float length = this->length();
	this->x /= length;
	this->y /= length;
	return *this;
}
