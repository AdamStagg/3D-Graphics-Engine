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

	unsigned int blendedRed =	static_cast<unsigned int>((static_cast<int>(r2) - static_cast<int>(r1)) * ratio + r1);
	unsigned int blendedGreen = static_cast<unsigned int>((static_cast<int>(g2) - static_cast<int>(g1)) * ratio + g1);
	unsigned int blendedBlue =	static_cast<unsigned int>((static_cast<int>(b2) - static_cast<int>(b1)) * ratio + b1);

	return 0xFF000000 | blendedRed << 16 | blendedGreen << 8 | blendedBlue;

}

//Interpolates two colors based on a given ratio for triangles
unsigned int colorLerpTriangle(unsigned int _color1, unsigned int _color2, int ratio, int ratio2) {

	unsigned int r1 = (_color1 & 0x00FF0000) >> 16;
	unsigned int g1 = (_color1 & 0x0000FF00) >> 8;
	unsigned int b1 = (_color1 & 0x000000FF);

	unsigned int r2 = (_color2 & 0x00FF0000) >> 16;
	unsigned int g2 = (_color2 & 0x0000FF00) >> 8;
	unsigned int b2 = (_color2 & 0x000000FF);

	int dR = (static_cast<int>(r2) - static_cast<int>(r1));
	int dG = (static_cast<int>(g2) - static_cast<int>(g1));
	int dB = (static_cast<int>(b2) - static_cast<int>(b1));

	int ratioR = (static_cast<int>(dR * ratio));
	int ratioG = (static_cast<int>(dG * ratio));
	int ratioB = (static_cast<int>(dB * ratio));

	int ratioR2 = ratioR >> 16;
	int ratioG2 = ratioG >> 16;
	int ratioB2 = ratioB >> 16;

	unsigned int blendedRed = static_cast<unsigned int>(	ratioR2 + r1);
	unsigned int blendedGreen = static_cast<unsigned int>(	ratioG2 + g1);
	unsigned int blendedBlue = static_cast<unsigned int>(	ratioB2 + b1);

	return 0xFF000000 | blendedRed << 16 | blendedGreen << 8 | blendedBlue;

}
//Interpolates two colors based on alpha
unsigned int colorLerp(unsigned int _color, unsigned int _ogcolor) {

	//alpha channel first
	unsigned int alpha = _color & 0xFF000000;
	alpha = alpha >> 24;

	//code to optimize because a lot of sprites dont need to blend, if it is fully opaque or fully transparent
	//if the ratio is 1, just return the new color
	if (alpha == 0x000000FF) {
		return _color;
	}
	//if the ratio is 0 return the old color
	else if (alpha == 0) {
		return _ogcolor;
	}

	//new color rgb
	unsigned int blue  = (_color & 0x000000FF);

	//old color values
	unsigned int ogalpha = (_ogcolor & 0xFF000000) >> 24;
	unsigned int ogred   = (_ogcolor & 0x00FF0000) >> 16;
	unsigned int oggreen = (_ogcolor & 0x0000FF00) >> 8;
	unsigned int ogblue  = (_ogcolor & 0x000000FF);

	int dr = (static_cast<int>((_color & 0x00FF0000) >> 16) - static_cast<int>(ogred));
	int dg = (static_cast<int>((_color & 0x0000FF00) >> 8)	- static_cast<int>(oggreen));
	int db = (static_cast<int>((_color & 0x000000FF))		- static_cast<int>(ogblue));
	int da = (static_cast<int>(alpha)						- static_cast<int>(ogalpha));

	//lerp blend
	unsigned int blendedred =	static_cast<unsigned int>((dr * alpha) >> 8 + ogred);
	unsigned int blendedgreen = static_cast<unsigned int>((dg * alpha) >> 8 + oggreen);
	unsigned int blendedblue =	static_cast<unsigned int>((db * alpha) >> 8 + ogblue);
	unsigned int blendedalpha = static_cast<unsigned int>((da * alpha) >> 8 + ogalpha);



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
//Implicit Line Equation
float ImplicitLineEquation(Vector2 _test, Vector2 _start, Vector2 _end) {
	return (_start.y - _end.y) * _test.x + (_end.x - _start.x) * _test.y + (_start.x * _end.y - _start.y * _end.x);
}
//Finds the Barycentric coordinates from 3 given points
Vector4 FindBarycentric(Vertex pointA, Vertex pointB, Vertex pointC, Vector2 curr) {

	float b		= ImplicitLineEquation(curr, pointC, pointA);
	float y		= ImplicitLineEquation(curr, pointA, pointB);
	float a		= ImplicitLineEquation(curr, pointB, pointC);
	return Vector4(b, y, a, 0);
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
	(
		/*x*/vect.x * matrix.matrix[0].x + vect.y * matrix.matrix[1].x + vect.z * matrix.matrix[2].x + vect.w * matrix.matrix[3].x,
		/*y*/vect.x * matrix.matrix[0].y + vect.y * matrix.matrix[1].y + vect.z * matrix.matrix[2].y + vect.w * matrix.matrix[3].y,
		/*z*/vect.x * matrix.matrix[0].z + vect.y * matrix.matrix[1].z + vect.z * matrix.matrix[2].z + vect.w * matrix.matrix[3].z,
		/*w*/vect.x * matrix.matrix[0].w + vect.y * matrix.matrix[1].w + vect.z * matrix.matrix[2].w + vect.w * matrix.matrix[3].w,
		vect.color
	);
}
Vertex VectorMULTMatrix(Vertex vect, Matrix3x3 matrix) {
	return Vertex
	(
		/*x*/vect.x * matrix.matrix[0].x + vect.y * matrix.matrix[1].x + vect.z * matrix.matrix[2].x,
		/*y*/vect.x * matrix.matrix[0].y + vect.y * matrix.matrix[1].y + vect.z * matrix.matrix[2].y,
		/*z*/vect.x * matrix.matrix[0].z + vect.y * matrix.matrix[1].z + vect.z * matrix.matrix[2].z,
		/*w*/0,
		vect.color
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

Vertex NDCtoScreen(Vertex v) {
	return Vertex(
		(v.x + 1) * (RasterWidth >> 1),
		(v.y + 1) * (RasterWidth >> 1),
		(v.z + 1) * (RasterWidth >> 1),
		(v.w + 1) * (RasterWidth >> 1),
		v.color
	);
}

Matrix3x3 BuildXRotationMatrix(float angle) {
	return Matrix3x3(
		Vector3(1,	0,			0),
		Vector3(0,	cos(angle),	-sin(angle)),
		Vector3(0,	sin(angle),	cos(angle))
	);
}

Matrix3x3 BuildYRotationMatrix(float angle) {
	return Matrix3x3(
		Vector3(cos(angle),	0,	sin(angle)),
		Vector3(0,			1,	0),
		Vector3(-sin(angle),0,	cos(angle))
	);
}

Matrix3x3 BuildZRotationMatrix(float angle) {
	return Matrix3x3(
		Vector3(cos(angle), -sin(angle),	0),
		Vector3(sin(angle), cos(angle),		0),
		Vector3(0,			0,				1)
	);
}