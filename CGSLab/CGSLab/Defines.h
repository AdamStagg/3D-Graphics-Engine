#pragma once

#define RasterWidth 500
#define RasterHeight 500
#define RasterPixelCount (RasterWidth * RasterHeight)

#define Identity2x2 Matrix2x2({1, 0}, {0, 1})
#define Identity3x3 Matrix3x3({1, 0, 0}, {0, 1, 0}, {0, 0, 1})
#define Identity4x4 Matrix4x4({1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1})

typedef unsigned int A_PIXEL;




unsigned int* Raster = nullptr;
float totalTime = 0;

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
	unsigned int color;
	Vertex() {
		values = Vector4();
		color = 0xFF000000;
	}
	Vertex(float _x, float _y, float _z, float _w, A_PIXEL _color) {
		values = { _x, _y, _z, _w };
		this->color = _color;
	}
	//operator Vector3() { return Vector3(x, y, z); }
	//operator Vector2() { return Vector2(x, y); }
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
	/*operator Matrix3x3() { return Matrix3x3((Vector3)matrix[0], (Vector3)matrix[1], Vector3()); }
	operator Matrix4x4() { return Matrix4x4((Vector4)matrix[0], (Vector4)matrix[1], Vector4(), Vector4()); }*/
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