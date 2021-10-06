#pragma once

#define RasterWidth 500
#define RasterHeight 500
#define RasterPixelCount (RasterWidth * RasterHeight)


typedef unsigned int A_PIXEL;

unsigned int* Raster = nullptr;

struct Vector2 {
	float x, y;
	Vector2() 
	{
		x = 0; y = 0;
	}
	Vector2(float _x, float _y) {
		x = _x;		y = _y;
	}
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
};

struct Vector4 {
	float x, y, z, w;
	Vector4() {
		x = 0; y = 0; z = 0; w = 0;
	}
	Vector4(float _x, float _y, float _z, float _w) {
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}
};
struct Vertex {
	Vector4 values;
	unsigned int color;
	Vertex() {
		values = Vector4();
		color = 0xFF000000;
	}
	Vertex(float x, float y, float z, float w, unsigned int color) {
		values = { x, y, z, w };
		this->color = color;
	}

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
};