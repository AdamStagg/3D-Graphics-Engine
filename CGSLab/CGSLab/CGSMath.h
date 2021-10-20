#pragma once
#include "Defines.h"

//Swap two integers without using any new memory
void Swap(int* a, int* b) {
	*a = *a + *b;
	*b = *a - *b;
	*a = *a - *b;
}
//Swap two floats without using any new memory
void Swap(float* a, float* b) {
	*a = *a + *b;
	*b = *a - *b;
	*a = *a - *b;
}
//Convert dimensions by using an X and Y position
int ConvertDimension(int _x, int _y, int _width) {
	return _y * _width + _x;
}

//Converts dimensions by using Vector2
int ConvertDimension(Vector2 _pos, int _width) {
	return static_cast<int>(floor(_pos.y + 0.5f)) * _width + static_cast<int>(floor(_pos.x + 0.5f));
}
//Linear interpolates two values
float lerp(int start, int end, float ratio) {
	return (end - start) * ratio + start;
}

float lerpf(float start, float end, float ratio) {
	return (end - start) * ratio + start;
}
//Interpolates two colors based on a given ratio
unsigned int colorLerp(unsigned int _color1, unsigned int _color2, float ratio) {

	unsigned int r1 = (_color1 & 0x00FF0000) >> 16;
	unsigned int g1 = (_color1 & 0x0000FF00) >> 8;
	unsigned int b1 = (_color1 & 0x000000FF);

	unsigned int r2 = (_color2 & 0x00FF0000) >> 16;
	unsigned int g2 = (_color2 & 0x0000FF00) >> 8;
	unsigned int b2 = (_color2 & 0x000000FF);

	unsigned int blendedRed = static_cast<unsigned int>((static_cast<int>(r2) - static_cast<int>(r1)) * ratio + r1);
	unsigned int blendedGreen = static_cast<unsigned int>((static_cast<int>(g2) - static_cast<int>(g1)) * ratio + g1);
	unsigned int blendedBlue = static_cast<unsigned int>((static_cast<int>(b2) - static_cast<int>(b1)) * ratio + b1);

	return 0xFF000000 | blendedRed << 16 | blendedGreen << 8 | blendedBlue;

}

unsigned int BGRAtoARGB(unsigned int _color) {
	return ((_color & 0x000000FF) << 24) | ((_color & 0x0000ff00) << 8) | ((_color & 0x00ff0000) >> 8) | ((_color & 0xff000000) >> 24);
}
//Interpolates two colors based on alpha
unsigned int colorLerp(unsigned int _color, unsigned int _ogcolor) {

	unsigned int alpha = _color & 0xFF000000;
	alpha = alpha >> 24;

	if (alpha == 0x000000FF) {
		return _color;
	}
	else if (alpha == 0) {
		return _ogcolor;
	}


	//old color values
	unsigned int ogalpha = (_ogcolor & 0xFF000000) >> 24;
	unsigned int ogred = (_ogcolor & 0x00FF0000) >> 16;
	unsigned int oggreen = (_ogcolor & 0x0000FF00) >> 8;
	unsigned int ogblue = (_ogcolor & 0x000000FF);

	int deltaR = ((_color & 0x00FF0000) >> 16) - static_cast<int>(ogred);
	int deltaG = ((_color & 0x0000FF00) >> 8) - static_cast<int>(oggreen);
	int deltaB = static_cast<int>(_color & 0x000000FF) - static_cast<int>(ogblue);
	int deltaA = static_cast<int>(alpha) - static_cast<int>(ogalpha);

	//lerp blend
	unsigned int blendedred = static_cast<unsigned int>(((deltaR * alpha) >> 8) + ogred);
	unsigned int blendedgreen = static_cast<unsigned int>(((deltaG * alpha) >> 8) + oggreen);
	unsigned int blendedblue = static_cast<unsigned int>(((deltaB * alpha) >> 8) + ogblue);
	unsigned int blendedalpha = static_cast<unsigned int>(((deltaA * alpha) >> 8) + ogalpha);

	//return the result
	return blendedalpha << 24 | blendedred << 16 | blendedgreen << 8 | blendedblue;
}

unsigned int colorBerp(Vector3 bya, unsigned int color1, unsigned int color2, unsigned int color3) {

	unsigned int color1A = (color1 & 0xFF000000) >> 24;
	unsigned int color2A = (color2 & 0xFF000000) >> 24;
	unsigned int color3A = (color3 & 0xFF000000) >> 24;

	unsigned int color1R = (color1 & 0x00FF0000) >> 16;
	unsigned int color2R = (color2 & 0x00FF0000) >> 16;
	unsigned int color3R = (color3 & 0x00FF0000) >> 16;

	unsigned int color1G = (color1 & 0x0000FF00) >> 8;
	unsigned int color2G = (color2 & 0x0000FF00) >> 8;
	unsigned int color3G = (color3 & 0x0000FF00) >> 8;

	unsigned int color1B = (color1 & 0x000000FF);
	unsigned int color2B = (color2 & 0x000000FF);
	unsigned int color3B = (color3 & 0x000000FF);

	unsigned int finalA = (static_cast<unsigned int>(bya.z * color1A + bya.x * color2A + bya.y * color3A)) << 24;
	unsigned int finalR = (static_cast<unsigned int>(bya.z * color1R + bya.x * color2R + bya.y * color3R)) << 16;
	unsigned int finalG = (static_cast<unsigned int>(bya.z * color1G + bya.x * color2G + bya.y * color3G)) << 8;
	unsigned int finalB = (static_cast<unsigned int>(bya.z * color1B + bya.x * color2B + bya.y * color3B));

	return finalA | finalR | finalG | finalB;
}

Vertex berp(Vertex ratios, Vertex p1, Vertex p2, Vertex p3) {
	return Vertex
	(
		ratios.x * p1.x + ratios.y * p2.x + ratios.z * p3.x,
		ratios.x * p1.y + ratios.y * p2.y + ratios.z * p3.y,
		ratios.x * p1.z + ratios.y * p2.z + ratios.z * p3.z,
		ratios.x * p1.w + ratios.y * p2.w + ratios.z * p3.w,
		0,
		0,
		colorBerp(ratios, p1.color, p2.color, p3.color),
		{}
	);
}

float berp(Vertex ratios, float p1, float p2, float p3) {
	return ratios.x * p1 + ratios.y * p2 + ratios.z * p3;
}

float berpf(Vertex ratios, float p1, float p2, float p3) {
	return (ratios.z * p1 + ratios.x * p2 + ratios.y * p3);
}

//Implicit Line Equation
float ImplicitLineEquation(Vector2 _test, Vector2 _start, Vector2 _end) {
	return (_start.y - _end.y) * _test.x + (_end.x - _start.x) * _test.y + (_start.x * _end.y - _start.y * _end.x);
}
//Finds the Barycentric coordinates from 3 given points
Vertex FindBarycentric(Vertex pointA, Vertex pointB, Vertex pointC, Vector2 curr) {
	float beta = ImplicitLineEquation(pointB, pointA, pointC);
	float gamma = ImplicitLineEquation(pointC, pointB, pointA);
	float alpha = ImplicitLineEquation(pointA, pointC, pointB);


	float b = ImplicitLineEquation(curr, pointA, pointC);
	float y = ImplicitLineEquation(curr, pointB, pointA);
	float a = ImplicitLineEquation(curr, pointC, pointB);
	return Vertex(a / alpha, b / beta, y / gamma, 0, 0, 0, 0, {});
}

Vector3 VectorMULTMatrix(Vector3 vect, Matrix3x3 matrix) {
	return Vector3
	(
		/*x*/vect.x * matrix.matrix[0].x + vect.y * matrix.matrix[1].x + vect.z * matrix.matrix[2].x,
		/*y*/vect.x * matrix.matrix[0].y + vect.y * matrix.matrix[1].y + vect.z * matrix.matrix[2].y,
		/*z*/vect.x * matrix.matrix[0].z + vect.y * matrix.matrix[1].z + vect.z * matrix.matrix[2].z
	);
}
Vector4 VectorMULTMatrix(Vector4 vect, Matrix4x4 matrix) {
	return Vector4
	(
		/*x*/vect.x * matrix.matrix[0].x + vect.y * matrix.matrix[1].x + vect.z * matrix.matrix[2].x + vect.w * matrix.matrix[3].x,
		/*y*/vect.x * matrix.matrix[0].y + vect.y * matrix.matrix[1].y + vect.z * matrix.matrix[2].y + vect.w * matrix.matrix[3].y,
		/*z*/vect.x * matrix.matrix[0].z + vect.y * matrix.matrix[1].z + vect.z * matrix.matrix[2].z + vect.w * matrix.matrix[3].z,
		/*w*/vect.x * matrix.matrix[0].w + vect.y * matrix.matrix[1].w + vect.z * matrix.matrix[2].w + vect.w * matrix.matrix[3].w
	);
}
Vertex VectorMULTMatrix(Vertex vect, Matrix4x4 matrix) {
	return Vertex
	{
		/*x*/vect.x * matrix.matrix[0].x + vect.y * matrix.matrix[1].x + vect.z * matrix.matrix[2].x + vect.w * matrix.matrix[3].x,
		/*y*/vect.x * matrix.matrix[0].y + vect.y * matrix.matrix[1].y + vect.z * matrix.matrix[2].y + vect.w * matrix.matrix[3].y,
		/*z*/vect.x * matrix.matrix[0].z + vect.y * matrix.matrix[1].z + vect.z * matrix.matrix[2].z + vect.w * matrix.matrix[3].z,
		/*w*/vect.x * matrix.matrix[0].w + vect.y * matrix.matrix[1].w + vect.z * matrix.matrix[2].w + vect.w * matrix.matrix[3].w,
		vect.u,
		vect.v,
		vect.color,
		vect.normal
	};
}

Vertex VectorMULTMatrix(Vertex vect, Matrix3x3 matrix) {
	return Vertex
	(
		/*x*/vect.x * matrix.matrix[0].x + vect.y * matrix.matrix[1].x + vect.z * matrix.matrix[2].x,
		/*y*/vect.x * matrix.matrix[0].y + vect.y * matrix.matrix[1].y + vect.z * matrix.matrix[2].y,
		/*z*/vect.x * matrix.matrix[0].z + vect.y * matrix.matrix[1].z + vect.z * matrix.matrix[2].z,
		/*w*/0,
		vect.u,
		vect.v,
		vect.color,
		vect.normal
	);
}
Matrix3x3 MatrixMULTMatrix(Matrix3x3 m1, Matrix3x3 m2) {
	return Matrix3x3
	(
		/*row1*/VectorMULTMatrix(m1.matrix[0], m2),
		/*row2*/VectorMULTMatrix(m1.matrix[1], m2),
		/*row3*/VectorMULTMatrix(m1.matrix[2], m2)
	);
}

Matrix4x4 MatrixMULTMatrix(Matrix4x4 m1, Matrix4x4 m2) {
	return Matrix4x4(
		/*row1*/VectorMULTMatrix(m1.matrix[0], m2),
		/*row2*/VectorMULTMatrix(m1.matrix[1], m2),
		/*row3*/VectorMULTMatrix(m1.matrix[2], m2),
		/*row4*/VectorMULTMatrix(m1.matrix[3], m2)
	);
}

Matrix2x2 MatrixInverse(Matrix2x2 m) {
	return Matrix2x2(
		Vector2(m.matrix[1].y, -m.matrix[0].y),
		Vector2(-m.matrix[1].x, m.matrix[0].x)
	);
}

Matrix4x4 MatrixInverse(Matrix4x4 m) {
	return Matrix4x4();
}

Vertex NDCtoScreen(const Vertex& v) {
	return Vertex(
		(v.x + 1) * (RasterWidth >> 1),
		(1 - v.y) * (RasterHeight >> 1),
		v.z,
		v.w,
		v.u,
		v.v,
		v.color,
		v.normal
	);
}

Matrix4x4 BuildXRotationMatrix(float angle) {
	return Matrix4x4(
		Vector4(1, 0, 0, 0),
		Vector4(0, cos(angle), -sin(angle), 0),
		Vector4(0, sin(angle), cos(angle), 0),
		Vector4(0, 0, 0, 1)
	);
}

Matrix4x4 BuildYRotationMatrix(float angle) {
	return Matrix4x4(
		Vector4(cos(angle), 0, sin(angle), 0),
		Vector4(0, 1, 0, 0),
		Vector4(-sin(angle), 0, cos(angle), 0),
		Vector4(0, 0, 0, 1)
	);
}

Matrix4x4 BuildZRotationMatrix(float angle) {
	return Matrix4x4(
		Vector4(cos(angle), -sin(angle), 0, 0),
		Vector4(sin(angle), cos(angle), 0, 0),
		Vector4(0, 0, 1, 0),
		Vector4(0, 0, 0, 1)
	);
}

Matrix2x2 MatrixTranspose(Matrix2x2 matrix) {
	return Matrix2x2(
		Vector2(matrix.matrix[0].x, matrix.matrix[1].x),
		Vector2(matrix.matrix[0].y, matrix.matrix[1].y)
	);
}

Matrix3x3 MatrixTranspose(Matrix3x3 matrix) {
	return Matrix3x3(
		Vector3(matrix.matrix[0].x, matrix.matrix[1].x, matrix.matrix[2].x),
		Vector3(matrix.matrix[0].y, matrix.matrix[1].y, matrix.matrix[2].y),
		Vector3(matrix.matrix[0].z, matrix.matrix[1].z, matrix.matrix[2].z)
	);
}

Matrix4x4 MatrixTranspose(Matrix4x4 matrix) {
	return Matrix4x4(
		Vector4(matrix.matrix[0].x, matrix.matrix[1].x, matrix.matrix[2].x, matrix.matrix[3].x),
		Vector4(matrix.matrix[0].y, matrix.matrix[1].y, matrix.matrix[2].y, matrix.matrix[3].y),
		Vector4(matrix.matrix[0].z, matrix.matrix[1].z, matrix.matrix[2].z, matrix.matrix[3].z),
		Vector4(matrix.matrix[0].w, matrix.matrix[1].w, matrix.matrix[2].w, matrix.matrix[3].w)
	);
}

float MatrixDeterminant(const Matrix2x2 matrix) {
	return matrix.matrix[0].x * matrix.matrix[1].y - matrix.matrix[0].y * matrix.matrix[1].x;
}

float MatrixDeterminant(const Matrix3x3 matrix) {
	return
		(
			matrix.matrix[0].x * (matrix.matrix[1].y * matrix.matrix[2].z - matrix.matrix[2].y * matrix.matrix[1].z) -
			matrix.matrix[0].y * (matrix.matrix[1].x * matrix.matrix[2].z - matrix.matrix[2].x * matrix.matrix[1].z) +
			matrix.matrix[0].z * (matrix.matrix[1].x * matrix.matrix[2].y - matrix.matrix[2].x * matrix.matrix[1].y)
			);
}

float MatrixDeterminant(const Matrix4x4 m) {

	float d1 = MatrixDeterminant(Matrix3x3({ m.matrix[1].y, m.matrix[1].z, m.matrix[1].w }, { m.matrix[2].y, m.matrix[2].z, m.matrix[2].w }, { m.matrix[3].y, m.matrix[3].z, m.matrix[3].w }));
	float d2 = MatrixDeterminant(Matrix3x3({ m.matrix[1].x, m.matrix[1].z, m.matrix[1].w }, { m.matrix[2].x, m.matrix[2].z, m.matrix[2].w }, { m.matrix[3].x, m.matrix[3].z, m.matrix[3].w }));
	float d3 = MatrixDeterminant(Matrix3x3({ m.matrix[1].x, m.matrix[1].y, m.matrix[1].w }, { m.matrix[2].x, m.matrix[2].y, m.matrix[2].w }, { m.matrix[3].x, m.matrix[3].y, m.matrix[3].w }));
	float d4 = MatrixDeterminant(Matrix3x3({ m.matrix[1].x, m.matrix[1].y, m.matrix[1].z }, { m.matrix[2].x, m.matrix[2].y, m.matrix[2].z }, { m.matrix[3].x, m.matrix[3].y, m.matrix[3].z }));

	return(m.matrix[0].x * d1 - m.matrix[0].y * d2 + m.matrix[0].z * d3 - m.matrix[0].w * d4);
}

Matrix4x4 OrthogonalAffineInverse(const Matrix4x4& matrix) {
	Matrix3x3 pos = MatrixTranspose(matrix);
	Vector3 translation(matrix.matrix[3].x, matrix.matrix[3].y, matrix.matrix[3].z);
	translation = VectorMULTMatrix(translation, pos);
	translation.x = -translation.x;
	translation.y = -translation.y;
	translation.z = -translation.z;

	return Matrix4x4(
		Vector4(pos.matrix[0].x, pos.matrix[0].y, pos.matrix[0].z, 0),
		Vector4(pos.matrix[1].x, pos.matrix[1].y, pos.matrix[1].z, 0),
		Vector4(pos.matrix[2].x, pos.matrix[2].y, pos.matrix[2].z, 0),
		Vector4(translation.x, translation.y, translation.z, 1)
	);
}

float DegToRad(float degrees) {
	return (degrees * (static_cast<float>(PI) / 180.0f));
}

float RadToDeg(float radians) {
	return (radians * (180.0f / static_cast<float>(PI)));
}

Matrix4x4 BuildProjectionMatrix(float FOV, float nearPlane, float farPlane, float aspectRatio) {

	float yscale = cot(DegToRad(FOV / 2));
	return Matrix4x4
	(
		Vector4(yscale * aspectRatio, 0, 0, 0),
		Vector4(0, yscale, 0, 0),
		Vector4(0, 0, (farPlane) / (farPlane - nearPlane), 1),
		Vector4(0, 0, (-(farPlane * nearPlane)) / (farPlane - nearPlane), 0)
	);
}

Matrix4x4 BuildTranslationMatrix(float x, float y, float z) {
	return Matrix4x4
	(
		{ 1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, 0 },
		{ x, y, z, 1 }
	);
}

Matrix4x4 BuildScaleMatrix(float xScale, float yScale, float zScale) {
	return Matrix4x4
	(
		{ xScale, 0, 0, 0 },
		{ 0, yScale, 0, 0 },
		{ 0, 0, zScale, 0 },
		{ 0, 0, 0, 1 }
	);
}

void PerspectiveDivide(Vertex& v) {
	v.x /= v.w;
	v.y /= v.w;
	v.z /= v.w;
}

float VectorDOTVector(const Vector2 v1, const Vector2 v2) {
	return v1.x * v2.x + v1.y * v2.y;
}

float VectorDOTVector(const Vector3 v1, const Vector3 v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float VectorDOTVector(const Vector4 v1, const Vector4 v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

float VectorDOTVector(const Vertex v1, const Vertex v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

Vector3 VectorCROSSVector(const Vector3 v1, const Vector3 v2) {
	return Vector3
	(
		v1.x * (v1.y * v2.z - v2.y * v1.z),
		v1.y * (v1.x * v2.z - v2.x * v1.z),
		v1.z * (v1.x * v2.y - v2.x * v1.y)
	);
}

Vector2 VectorSUBTRACTVector(const Vector2 v1, const Vector2 v2) {
	return Vector2
	(
		v1.x - v2.x,
		v1.y - v2.y
	);
}

Vector3 VectorSUBTRACTVector(const Vector3 v1, const Vector3 v2) {
	return Vector3
	(
		v1.x - v2.x,
		v1.y - v2.y,
		v1.z - v2.z
	);
}

Vector4 VectorSUBTRACTVector(const Vector4 v1, const Vector4 v2) {
	return Vector4
	(
		v1.x - v2.x,
		v1.y - v2.y,
		v1.z - v2.z,
		v1.w - v2.w
	);
}

float VectorMagnitude(const Vector2 v) {
	return sqrt(v.x * v.x + v.y * v.y);
}

float VectorMagnitude(const Vector3 v) {
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

void NormalizeVector(Vector2& v) {
	float mag = VectorMagnitude(v);
	v.x /= mag;
	v.y /= mag;
}

void NormalizeVector(Vector3& v) {
	float mag = VectorMagnitude(v);
	v.x /= mag;
	v.y /= mag;
	v.z /= mag;
}

void Saturate(float& value, float max) {
	if (value > max)
	{
		value = 1;
		return;
	}
	if (value < 0)
	{
		value = 0;
		return;
	}
}

//void Saturate(unsigned int& value) {
//	if (value > 1) {
//		value = 1;
//		return;
//	}
//	if (value < 0) {
//		value = 0;
//		return;
//	}
//}

unsigned int Saturate(unsigned int value, int max) {
	if (value > max) {
		return max;
	}
	if (value < 0) {
		return 0;
	}
	return value;
}

float Saturate(float value, float max) {
	if (value > max) {
		return max;
	}
	if (value < 0) {
		return 0;
	}
	return value;
}

float Saturate(float value) {
	if (value > 1) {
		return 1;
	}
	if (value < 0) {
		return 0;
	}
	return value;
}

unsigned int ScaleColor(unsigned int color, float ratio) {
	unsigned int A = static_cast<unsigned int>(((color & 0xff000000) >> 24) * ratio);
	unsigned int R = static_cast<unsigned int>(((color & 0x00ff0000) >> 16) * ratio);
	unsigned int G = static_cast<unsigned int>(((color & 0x0000ff00) >> 8) * ratio);
	unsigned int B = static_cast<unsigned int>((color & 0x000000ff) * ratio);

	return (A << 24) | (R << 16) | (G << 8) | B;
}

unsigned int CombineColors(unsigned int color1, unsigned int color2) {
	unsigned int color1A = (color1 & 0xff000000) >> 24;
	unsigned int color2A = (color2 & 0xff000000) >> 24;
	unsigned int color1R = (color1 & 0x00ff0000) >> 16;
	unsigned int color2R = (color2 & 0x00ff0000) >> 16;
	unsigned int color1G = (color1 & 0x0000ff00) >> 8;
	unsigned int color2G = (color2 & 0x0000ff00) >> 8;
	unsigned int color1B = (color1 & 0x000000ff);
	unsigned int color2B = (color2 & 0x000000ff);

	unsigned int finalA = Saturate(color1A + color2A, 255);
	unsigned int finalR = Saturate(color1R + color2R, 255);
	unsigned int finalG = Saturate(color1G + color2G, 255);
	unsigned int finalB = Saturate(color1B + color2B, 255);

	return (finalA << 24) | (finalR << 16) | (finalG << 8) | finalB;
}

unsigned int ModulateColors(unsigned int color1, unsigned int color2) {
	float color1A = static_cast<float>((color1 & 0xff000000) >> 24) / 255.0f;
	float color2A = static_cast<float>((color2 & 0xff000000) >> 24) / 255.0f;
	float color1R = static_cast<float>((color1 & 0x00ff0000) >> 16) / 255.0f;
	float color2R = static_cast<float>((color2 & 0x00ff0000) >> 16) / 255.0f;
	float color1G = static_cast<float>((color1 & 0x0000ff00) >> 8) / 255.0f;
	float color2G = static_cast<float>((color2 & 0x0000ff00) >> 8) / 255.0f;
	float color1B = static_cast<float>(color1 & 0x000000ff) / 255.0f;
	float color2B = static_cast<float>(color2 & 0x000000ff) / 255.0f;


	unsigned int finalA = static_cast<unsigned int>(Saturate(color1A * color2A, 1.0f) * 255.0f);
	unsigned int finalR = static_cast<unsigned int>(Saturate(color1R * color2R, 1.0f) * 255.0f);
	unsigned int finalG = static_cast<unsigned int>(Saturate(color1G * color2G, 1.0f) * 255.0f);
	unsigned int finalB = static_cast<unsigned int>(Saturate(color1B * color2B, 1.0f) * 255.0f);

	return (finalA << 24) | (finalR << 16) | (finalG << 8) | finalB;
}
