#pragma once

#include <iostream>
#include "XTime.h"
#include "RasterSurface.h"
#include <Windows.h>
#include "celestial.h"

#define PI 3.14159265358979323846

#define RasterWidth 1024
#define RasterHeight 720
#define RasterPixelCount (RasterWidth * RasterHeight)

#define Identity2x2 Matrix2x2({1, 0}, {0, 1})
#define Identity3x3 Matrix3x3({1, 0, 0}, {0, 1, 0}, {0, 0, 1})
#define Identity4x4 Matrix4x4({1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1})

#define cot(x) (1/tan(x))
#define csc(x) (1/sin(x))
#define sec(x) (1/cos(x))

typedef unsigned int A_PIXEL;


struct Vector2 {
	union {
		struct {
			float x, y;
		};
		struct
		{
			float a[2];
		};
	};
	Vector2()
	{
		x = 0; y = 0;
	}
	Vector2(float _x, float _y) {
		x = _x;		y = _y;
	}
	//operator Vector3() { return Vector3(x, y, 0); }
	//operator Vector4() { return Vector4(x, y, 0, 0); }
};


struct Vector3 {
	union {
		struct {
			float x, y, z;
		};
		struct {
			float a[3];
		};
	};
	Vector3() {
		x = 0; y = 0; z = 0;
	}
	Vector3(float _x, float _y, float _z) {
		x = _x;
		y = _y;
		z = _z;
	}
	operator Vector2() { return Vector2(x, y); }
	//operator Vector4() { return Vector4(x, y, z, 0); }
};

struct Vector4 {
	union {
		struct {
			float x, y, z, w;
		};
		struct {
			float a[4];
		};
	};
	Vector4() {
		x = 0; y = 0; z = 0; w = 0;
	}
	Vector4(float _x, float _y, float _z, float _w) {
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}
	operator Vector3() { return Vector3(x, y, z); }
	operator Vector2() { return Vector2(x, y); }
};

struct Vertex {
	union {
		struct {
			float x, y, z, w;
		};
		struct {
			Vector4 values;
		};
	};
	float u, v;
	unsigned int color;
	Vertex() {
		values = Vector4();
		u = 0;
		v = 0;
		color = 0xFF000000;
	}
	Vertex(float _x, float _y, float _z, float _w, float _u, float _v, A_PIXEL _color) {
		values = { _x, _y, _z, _w };
		u = _u;
		v = _v;
		this->color = _color;
	}
	operator Vector4() { return Vector4(x, y, z, w); }
	operator Vector3() { return Vector3(x, y, z); }
	operator Vector2() { return Vector2(x, y); }
};

struct Matrix2x2 {
	Vector2 matrix[2];

	Matrix2x2() {
		matrix[0] = { 0, 0 };
		matrix[1] = { 0, 0 };
	}
	Matrix2x2(Vector2 v1, Vector2 v2) {
		matrix[0] = v1;
		matrix[1] = v2;
	}
	//operator Matrix3x3() { return Matrix3x3((Vector3)matrix[0], (Vector3)matrix[1], Vector3()); }
	//operator Matrix4x4() { return Matrix4x4((Vector4)matrix[0], (Vector4)matrix[1], Vector4(), Vector4()); }
};

struct Matrix3x3 {
	Vector3 matrix[3];

	Matrix3x3() {
		matrix[0] = { 0, 0, 0 };
		matrix[1] = { 0, 0, 0 };
		matrix[2] = { 0, 0, 0 };
	}

	Matrix3x3(Vector3 v1, Vector3 v2, Vector3 v3) {
		matrix[0] = v1;
		matrix[1] = v2;
		matrix[2] = v3;
	}
	operator Matrix2x2() { return Matrix2x2((Vector2)matrix[0], (Vector2)matrix[1]); }
	//operator Matrix4x4() { return Matrix4x4((Vector4)matrix[0], (Vector4)matrix[1], (Vector4)matrix[2], Vector4()); }
};

struct Matrix4x4 {
	Vector4 matrix[4];

	Matrix4x4() {
		matrix[0] = { 0, 0, 0, 0 };
		matrix[1] = { 0, 0, 0, 0 };
		matrix[2] = { 0, 0, 0, 0 };
		matrix[3] = { 0, 0, 0, 0 };
	}
	Matrix4x4(Vector4 v1, Vector4 v2, Vector4 v3, Vector4 v4) {
		matrix[0] = v1;
		matrix[1] = v2;
		matrix[2] = v3;
		matrix[3] = v4;
	}
	operator Matrix3x3() { return Matrix3x3((Vector3)matrix[0], (Vector3)matrix[1], (Vector3)matrix[2]); }
	operator Matrix2x2() { return Matrix2x2((Vector2)matrix[0], (Vector2)matrix[1]); }
};


float NearPlane = .1f;
float FarPlane = 10.0f;
float VerticalFOV = 90.0f;
float AspectRatio = static_cast<float>(RasterHeight) / static_cast<float>(RasterWidth);

unsigned int* Raster = new unsigned int[RasterPixelCount];
float* DepthBuffer = new float[RasterPixelCount];
Matrix4x4 camera;
XTime timer;

int triangles[]{
	0, 4, 1, 5, 1, 4, // front face
	1, 5, 2, 6, 2, 5, // right face
	2, 6, 3, 7, 3, 6,// back face
	3, 7, 0, 4, 0, 7, // left face
	0, 3, 1, 2, 1, 3, // top face
	4, 7, 5, 6, 5, 7 // bottom face
};

int uvs[]{ //all the triangles are set up the same way so we can just reuse the same uvs
	0, 1, 0, 0, 1, 1, //triangle 1  //bottomL,  bottomR, topL
	1, 0, 1, 1, 0, 0//triangle 2	//topR, bottomR, topL
};

const Vertex gridPoints[] = {
	//X change, close Z
	Vertex(-.5f, 0, -.5f, 1, 0, 0, 0xFFFFFFFF),
	Vertex(-.4f, 0, -.5f, 1, 0, 0, 0xFFFFFFFF),
	Vertex(-.3f, 0, -.5f, 1, 0, 0, 0xFFFFFFFF),
	Vertex(-.2f, 0, -.5f, 1, 0, 0, 0xFFFFFFFF),
	Vertex(-.1f, 0, -.5f, 1, 0, 0, 0xFFFFFFFF),
	Vertex(0.0f, 0, -.5f, 1, 0, 0, 0xFFFFFFFF),
	Vertex(.1f, 0, -.5f,  1, 0, 0, 0xFFFFFFFF),
	Vertex(.2f, 0, -.5f,  1, 0, 0, 0xFFFFFFFF),
	Vertex(.3f, 0, -.5f,  1, 0, 0, 0xFFFFFFFF),
	Vertex(.4f, 0, -.5f,  1, 0, 0, 0xFFFFFFFF),
	Vertex(.5f, 0, -.5f,  1, 0, 0, 0xFFFFFFFF),
	//X change, far Z
	Vertex(-.5f, 0, .5f,  1, 0, 0, 0xFFFFFFFF),
	Vertex(-.4f, 0, .5f,  1, 0, 0, 0xFFFFFFFF),
	Vertex(-.3f, 0, .5f,  1, 0, 0, 0xFFFFFFFF),
	Vertex(-.2f, 0, .5f,  1, 0, 0, 0xFFFFFFFF),
	Vertex(-.1f, 0, .5f,  1, 0, 0, 0xFFFFFFFF),
	Vertex(0, 0, .5f, 1,	 0, 0, 0xFFFFFFFF),
	Vertex(.1f, 0, .5f, 1,   0, 0, 0xFFFFFFFF),
	Vertex(.2f, 0, .5f, 1,   0, 0, 0xFFFFFFFF),
	Vertex(.3f, 0, .5f, 1,   0, 0, 0xFFFFFFFF),
	Vertex(.4f, 0, .5f, 1,   0, 0, 0xFFFFFFFF),
	Vertex(.5f, 0, .5f, 1,   0, 0, 0xFFFFFFFF),
	//Left X changing Z
	Vertex(-.5f, 0,-.5f, 1, 0, 0, 0xFFFFFFFF),
	Vertex(-.5f, 0,-.4f, 1, 0, 0, 0xFFFFFFFF),
	Vertex(-.5f, 0,-.3f, 1, 0, 0, 0xFFFFFFFF),
	Vertex(-.5f, 0,-.2f, 1, 0, 0, 0xFFFFFFFF),
	Vertex(-.5f, 0,-.1f, 1, 0, 0, 0xFFFFFFFF),
	Vertex(-.5f, 0,  0,  1, 0, 0, 0xFFFFFFFF),
	Vertex(-.5f, 0, .1f, 1, 0, 0, 0xFFFFFFFF),
	Vertex(-.5f, 0, .2f, 1, 0, 0, 0xFFFFFFFF),
	Vertex(-.5f, 0, .3f, 1, 0, 0, 0xFFFFFFFF),
	Vertex(-.5f, 0, .4f, 1, 0, 0, 0xFFFFFFFF),
	Vertex(-.5f, 0, .5f, 1, 0, 0, 0xFFFFFFFF),
	//Right X changing Z
	Vertex(.5f, 0, -.5f, 1, 0, 0, 0xFFFFFFFF),
	Vertex(.5f, 0, -.4f, 1, 0, 0, 0xFFFFFFFF),
	Vertex(.5f, 0, -.3f, 1, 0, 0, 0xFFFFFFFF),
	Vertex(.5f, 0, -.2f, 1, 0, 0, 0xFFFFFFFF),
	Vertex(.5f, 0, -.1f, 1, 0, 0, 0xFFFFFFFF),
	Vertex(.5f, 0, 0,    1, 0, 0, 0xFFFFFFFF),
	Vertex(.5f, 0, .1f,  1, 0, 0, 0xFFFFFFFF),
	Vertex(.5f, 0, .2f,  1, 0, 0, 0xFFFFFFFF),
	Vertex(.5f, 0, .3f,  1, 0, 0, 0xFFFFFFFF),
	Vertex(.5f, 0, .4f,  1, 0, 0, 0xFFFFFFFF),
	Vertex(.5f, 0, .5f,  1, 0, 0, 0xFFFFFFFF)
};

Vertex cubePoints[] = {
	Vertex(-.25f,	-.25f, -.25f,	1, 0, 0, 0xFFFF0000),
	Vertex(.25f,	-.25f, -.25f,	1, 0, 0, 0xFF00FF00),
	Vertex(.25f,	-.25f,  .25f,	1, 0, 0, 0xFF0000FF),
	Vertex(-.25f,	-.25f,  .25f,	1, 0, 0, 0xFFFF00FF),
	Vertex(-.25f,	 .25f, -.25f,	1, 0, 0, 0xFFFFFF00),
	Vertex(.25f,	 .25f, -.25f,	1, 0, 0, 0xFFFFFFFF),
	Vertex(.25f,	 .25f,  .25f,	1, 0, 0, 0xFF00FFFF),
	Vertex(-.25f,	 .25f,  .25f,	1, 0, 0, 0xFF000000)
};