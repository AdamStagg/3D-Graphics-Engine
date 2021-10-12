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

//Interpolates two colors based on a given ratio
unsigned int colorLerp(unsigned int _color1, unsigned int _color2, float ratio) {

	unsigned int r1 = (_color1 & 0x00FF0000) >> 16;
	unsigned int g1 = (_color1 & 0x0000FF00) >> 8;
	unsigned int b1 = (_color1 & 0x000000FF);

	unsigned int r2 = (_color2 & 0x00FF0000) >> 16;
	unsigned int g2 = (_color2 & 0x0000FF00) >> 8;
	unsigned int b2 = (_color2 & 0x000000FF);

	unsigned int blendedRed = (static_cast<int>(r2) - static_cast<int>(r1)) * ratio + r1;
	unsigned int blendedGreen = (static_cast<int>(g2) - static_cast<int>(g1)) * ratio + g1;
	unsigned int blendedBlue = (static_cast<int>(b2) - static_cast<int>(b1)) * ratio + b1;

	return 0xFF000000 | blendedRed << 16 | blendedGreen << 8 | blendedBlue;

}
//Interpolates two colors based on alpha
unsigned int colorLerp(unsigned int _color, unsigned int _ogcolor) {

	//alpha channel first
	unsigned int alpha = _color & 0xFF000000;
	alpha = alpha >> 24;

	//code to optimize because a lot of sprites dont need to blend, if it is fully opaque or fully transparent
	//if the ratio is 1, just return the new color
	if (alpha / 255.0f == 1) {
		return _color;
	}
	//if the ratio is 0 return the old color
	else if (alpha / 255.0f == 0) {
		return _ogcolor;
	}

	//new color rgb
	unsigned int red   = (_color & 0x00FF0000) >> 16;
	unsigned int green = (_color & 0x0000FF00) >> 8;
	unsigned int blue  = (_color & 0x000000FF);

	//old color values
	unsigned int ogalpha = (_ogcolor & 0xFF000000) >> 24;
	unsigned int ogred   = (_ogcolor & 0x00FF0000) >> 16;
	unsigned int oggreen = (_ogcolor & 0x0000FF00) >> 8;
	unsigned int ogblue  = (_ogcolor & 0x000000FF);

	//lerp blend
	unsigned int blendedred = static_cast<unsigned int>(((static_cast<int>(red) - static_cast<int>(ogred)) * (alpha / 255.0f) + ogred));
	unsigned int blendedgreen = static_cast<unsigned int>(((static_cast<int>(green) - static_cast<int>(oggreen)) * (alpha / 255.0f) + oggreen));
	unsigned int blendedblue = static_cast<unsigned int>(((static_cast<int>(blue) - static_cast<int>(ogblue)) * (alpha / 255.0f) + ogblue));
	unsigned int blendedalpha = static_cast<unsigned int>(((static_cast<int>(alpha) - static_cast<int>(ogalpha)) * (alpha / 255.0f) + ogalpha));



	//return the result
	return blendedalpha << 24 | blendedred << 16 | blendedgreen << 8 | blendedblue;
}
//Implicit Line Equation
float ImplicitLineEquation(Vector2 _test, Vector2 _start, Vector2 _end) {
	return (_start.y - _end.y) * _test.x + (_end.x - _start.x) * _test.y + (_start.x * _end.y - _start.y * _end.x);
}
//Finds the Barycentric coordinates from 3 given points
Vector4 FindBarycentric(Vertex pointA, Vertex pointB, Vertex pointC, Vector2 curr) {
	float beta	= ImplicitLineEquation(pointB, pointA, pointC);
	float gamma = ImplicitLineEquation(pointC, pointB, pointA);
	float alpha = ImplicitLineEquation(pointA, pointC, pointB);


	float b		= ImplicitLineEquation(curr, pointA, pointC);
	float y		= ImplicitLineEquation(curr, pointB, pointA);
	float a		= ImplicitLineEquation(curr, pointC, pointB);
	return Vector4(b/beta, y/gamma, a/alpha, 0);
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
		vect.color
	};
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

Vertex NDCtoScreen(Vertex v) {
	return Vertex(
		(v.x + 1) * (RasterWidth >> 1),
		(1- v.y) * (RasterWidth >> 1),
		(v.z + 1) * (RasterWidth >> 1),
		(v.w + 1) * (RasterWidth >> 1),
		v.color
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
		Vector4(cos(angle),	0,	sin(angle),0),
		Vector4(0,			1,	0,0),
		Vector4(-sin(angle),0,	cos(angle),0),
		Vector4(0, 0, 0, 1)
	);
}

Matrix4x4 BuildZRotationMatrix(float angle) {
	return Matrix4x4(
		Vector4(cos(angle), -sin(angle),	0,0),
		Vector4(sin(angle), cos(angle),		0,0),
		Vector4(0,			0,				1,0),
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
		Vector4(matrix.matrix[0].x, matrix.matrix[1].x, matrix.matrix[2].x, matrix.matrix[2].x),
		Vector4(matrix.matrix[0].y, matrix.matrix[1].y, matrix.matrix[2].y, matrix.matrix[2].y),
		Vector4(matrix.matrix[0].z, matrix.matrix[1].z, matrix.matrix[2].z, matrix.matrix[2].z),
		Vector4(matrix.matrix[0].w, matrix.matrix[1].w, matrix.matrix[2].w, matrix.matrix[2].w)
	);
}

float MatrixDeterminant(Matrix2x2 matrix) {
	return matrix.matrix[0].x * matrix.matrix[1].y - matrix.matrix[0].y * matrix.matrix[1].x;
}

float MatrixDeterminant(Matrix3x3 matrix) {
	return
	(
		matrix.matrix[0].x * (matrix.matrix[1].y * matrix.matrix[2].z - matrix.matrix[2].y * matrix.matrix[1].z) -
		matrix.matrix[0].y * (matrix.matrix[1].x * matrix.matrix[2].z - matrix.matrix[2].x * matrix.matrix[1].z) +
		matrix.matrix[0].z * (matrix.matrix[1].x * matrix.matrix[2].y - matrix.matrix[2].x * matrix.matrix[1].y)
	);
}

float MatrixDeterminant(Matrix4x4 matrix) {
	return 0;
}