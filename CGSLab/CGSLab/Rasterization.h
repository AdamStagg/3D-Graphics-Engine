#pragma once
#include "Shaders.h"
#include <math.h>


void PlotPixel(const int _rasterIndex, const unsigned int _color) {
	Raster[_rasterIndex] = _color;
}

void PlotPixel(const Vertex _pos, const unsigned int _color) {
	if (_pos.x >= 0 && _pos.x < RasterWidth && _pos.y >= 0 && _pos.y < RasterHeight)
	{
		int index = ConvertDimension(_pos.x, _pos.y, RasterWidth);
		if (_pos.z < DepthBuffer[index]) {
			DepthBuffer[index] = _pos.z;
			Raster[index] = _color;
		}
	}
}

void ClearColor(unsigned int _color) {

	for (int i = 0; i < RasterPixelCount; i++)
	{
		Raster[i] = _color;
	}
}

void ClearDepth(float depth = 1.0f) {
	for (size_t i = 0; i < RasterPixelCount; i++)
	{
		DepthBuffer[i] = depth;
	}
}

//0 for don't draw, 1 for draw normal, 2 for special
int ClipLine(Vertex& v1, Vertex& v2) {

	if (v1.z < NearPlane && v2.z < NearPlane) {
		//don't draw
		return -1;
	}
	else if (v1.z > NearPlane && v2.z > NearPlane) {
		//draw normal
		return 1;
	}
	if (v1.z < NearPlane) {
		float ratio = (NearPlane - v1.z) / ((v2.z - v1.z));
		v1.x = lerpf(v1.x, v2.x, ratio);
		v1.y = lerpf(v1.y, v2.y, ratio);
		v1.z = lerpf(v1.z, v2.z, ratio);
		v1.w = lerpf(v1.w, v2.w, ratio);
		v1.u = lerpf(v1.u, v2.u, ratio);
		v1.v = lerpf(v1.v, v2.v, ratio);
	}
	if (v2.z < NearPlane) {
		float ratio = (NearPlane - v2.z) / ((v1.z - v2.z));
		v2.x = lerpf(v2.x, v1.x, ratio);
		v2.y = lerpf(v2.y, v1.y, ratio);
		v2.z = lerpf(v2.z, v1.z, ratio);
		v2.w = lerpf(v2.w, v1.w, ratio);
		v2.u = lerpf(v2.u, v1.u, ratio);
		v2.v = lerpf(v2.v, v1.v, ratio);
	}

	return 0;
}

int ClipTriangle(Vertex& v1, Vertex& v2, Vertex& v3, Vertex& v4) {

	//fully behind the nearplane
	if (v1.z < NearPlane && v2.z < NearPlane && v3.z < NearPlane) return -1;

	//fully infront of nearplane
	if (v1.z > NearPlane && v2.z > NearPlane && v3.z > NearPlane) return 0;
	
	Vertex copy_v1 = v1;
	Vertex copy_v2 = v2;
	Vertex copy_v3 = v3;
	
	Vertex ABA = copy_v1;
	Vertex CAA = copy_v1;
	Vertex ABB = copy_v2;
	Vertex BCB = copy_v2;
	Vertex BCC = copy_v3;
	Vertex CAC = copy_v3;

	int ABClip = ClipLine(ABA, ABB);
	int BCClip = ClipLine(BCB, BCC);
	int CAClip = ClipLine(CAC, CAA);
	
	if (ABClip == -1) {
		v1 = CAA;
		v2 = BCB;
		v3 = copy_v3;
		return 1;
	}
	else if (ABClip == 1) {
		v1 = copy_v1;
		v2 = copy_v2;
		v3 = BCC;
		v4 = CAC;
		return 2;
	}
	else if (BCClip == -1) {
		v1 = copy_v1;
		v2 = ABB;
		v3 = CAC;
		return 1;
	}
	else if (BCClip == 1) {
		v2 = copy_v2;
		v3 = copy_v3;
		v1 = ABA;
		v4 = CAA;
		return 2;
	}
	else if (CAClip == -1) {
		v2 = copy_v2;
		v1 = ABA;
		v3 = BCC;
		return 1;
	}
	else if (CAClip == 1) {
		v4 = copy_v3;
		v1 = copy_v1;
		v3 = BCB;
		v2 = ABB;

		return 2;
	}
	//return 1 is 1 triangle, return 2 is 2 triangles
	

}

void Bresenham(const Vertex& _startPos, const Vertex& _endPos, const unsigned int _color) {

	Vertex copy_start = _startPos;
	Vertex copy_end = _endPos;

	if (VertexShader) {
		VertexShader(copy_start);
		VertexShader(copy_end);
	}

	Vertex screen_start = NDCtoScreen(copy_start);
	Vertex screen_end = NDCtoScreen(copy_end);

	if (true)
	{
		int dx = (screen_end.x - screen_start.x);
		int dy = (screen_end.y - screen_start.y);
		unsigned int total = (std::abs(dx) > std::abs(dy)) ? std::abs(dx) : std::abs(dy);
		for (size_t i = 0; i < total; i++)
		{
			float ratio = static_cast<float>(i) / total;
			unsigned int curr_x = lerp(screen_start.x, screen_end.x, ratio);
			unsigned int curr_y = lerp(screen_start.y, screen_end.y, ratio);
			float curr_z = lerpf(screen_start.z, screen_end.z, ratio);

			A_PIXEL copyColor = _color;
			if (PixelShader) {
				PixelShader(copyColor, 0, 0, 0);
			}
			PlotPixel(Vertex(static_cast<float>(curr_x), static_cast<float>(curr_y), curr_z, 1, 0, 0, 0), copyColor);
		}
	}

	//int currX = static_cast<int>(screen_start.x);
	//int currY = static_cast<int>(screen_start.y);
	//int curr, start, end;
	//float slope;

	//int startX = static_cast<int>(screen_start.x), endX = static_cast<int>(screen_end.x), startY = static_cast<int>(screen_start.y), endY = static_cast<int>(screen_end.y);
	//int inc = 1;

	//float slopeX = abs(static_cast<int>(screen_end.y - screen_start.y) / static_cast<float>(static_cast<int>(screen_end.x - screen_start.x)));
	//float slopeY = abs(static_cast<int>(screen_end.x - screen_start.x) / static_cast<float>(static_cast<int>(screen_end.y - screen_start.y)));
	//float error = 0;

	//bool isXDominant = abs(static_cast<int>(screen_end.y - screen_start.y) / static_cast<float>(static_cast<int>(screen_end.x - screen_start.x))) < 1;

	//if (static_cast<int>(screen_end.x - screen_start.x) < 0) {
	//	Swap(&startX, &endX);
	//	currY = static_cast<int>(screen_end.y);
	//	inc = -inc;
	//}
	//if (static_cast<int>(screen_end.y - screen_start.y) < 0) {
	//	Swap(&startY, &endY);
	//	currX = static_cast<int>(screen_end.x);
	//	inc = -inc;
	//}

	//if (isXDominant) {
	//	start = startX;
	//	end = endX;
	//	slope = slopeX;
	//}
	//else {
	//	start = startY;
	//	end = endY;
	//	slope = slopeY;
	//}

	//for (curr = start; curr <= end; curr++)
	//{
	//	A_PIXEL copyColor = _color;

	//	if (PixelShader) {
	//		PixelShader(copyColor, 0, 0);
	//	}
	//	if (end != 0) {
	//		float z = lerpf(screen_start.z, screen_end.z, (curr - start) / static_cast<float>((end - start)));
	//		PlotPixel(isXDominant ? Vertex(static_cast<float>(curr), static_cast<float>(currY), z, 1, 0, 0, 0) : Vertex(static_cast<float>(currX), static_cast<float>(curr), z, 1, 0, 0, 0), copyColor);
	//	}
	//	error += slope;
	//	if (error > 0.5f) {
	//		isXDominant ? currY += inc : currX += inc;
	//		error--;
	//	}
	//}
}

void MidPoint(const Vector2 _startPos, const Vector2 _endPos, const unsigned int _color) {

	float currX = _startPos.x;
	float currY = _startPos.y;
	int curr, start, end;

	float deltaX = _endPos.x - _startPos.x;
	float deltaY = _endPos.y - _startPos.y;

	int startX = static_cast<int>(_startPos.x), endX = static_cast<int>(_endPos.x), startY = static_cast<int>(_startPos.y), endY = static_cast<int>(_endPos.y);
	int inc = 1;
	int toggle = 1;
	float yInc = -1;
	float xInc = 1;
	int yTogg = 1;

	float slopeX = deltaY / static_cast<float>(deltaX);
	float slopeY = deltaX / static_cast<float>(deltaY);
	float error = 0;

	bool isXDominant = abs(slopeX) < 1; //octant 1, 3, 4, 8 if true
	bool isRight = deltaX > 0;
	bool isDown = deltaY > 0;

	if (static_cast<int>(_endPos.x - _startPos.x) < 0) {
		Swap(&startX, &endX);
		currY = _endPos.y;
		inc = -inc;
		toggle = -toggle;
		xInc = -xInc;
	}
	if (static_cast<int>(_endPos.y - _startPos.y) < 0) {
		Swap(&startY, &endY);
		currX = _endPos.x;
		inc = -inc;
		toggle = -toggle;
		yInc = -yInc;
	}

	if (isXDominant) {
		start = startX;
		end = endX;
		yInc = 0.5f;
		xInc = static_cast<float>(inc);
		yTogg = 1;
	}
	else {
		start = startY;
		end = endY;
		toggle = 1;
		xInc *= -0.5f;
		yTogg = -1;
	}
	Vector4 _end{ _endPos.x, _endPos.y, 0, 0 };
	Vector4 _start{ _startPos.x, _startPos.y, 0, 0 };

	for (curr = start; curr <= end; curr++)
	{
		PlotPixel(isXDominant ? Vertex(static_cast<float>(curr), static_cast<float>(currY), 0, 0, 0, 0, 0) : Vertex(static_cast<float>(currX), static_cast<float>(curr), 0, 0, 0, 0, 0), _color);
		Vector4 mid(isXDominant ? curr + xInc : currX + xInc, isXDominant ? currY + yInc : curr + yInc, 0, 0);
		if (toggle * ((yTogg * static_cast<int>(_endPos.x - _startPos.x)) < 0 ? ImplicitLineEquation(mid, _end, _start) : ImplicitLineEquation(mid, _start, _end)) < 0) {
			isXDominant ? currY += inc : currX += inc;
		}
	}
}

void Parametric(const Vertex& _start, const Vertex& _end, const unsigned int _color1, const unsigned int _color2 = 0) {


	Vertex copy_start = _start;
	Vertex copy_end = _end;

	if (VertexShader) {
		VertexShader(copy_start);
		VertexShader(copy_end);
	}

	int clipResult = ClipLine(copy_start, copy_end);

	if (clipResult == -1) {
		return;
	}
	PerspectiveDivide(copy_start);
	PerspectiveDivide(copy_end);

	Vertex screen_start = NDCtoScreen(copy_start);
	Vertex screen_end = NDCtoScreen(copy_end);


	int dx = std::abs(screen_end.x - screen_start.x);
	int dy = std::abs(screen_end.y - screen_start.y);
	unsigned int total = (dx > dy) ? dx : dy;
	for (size_t i = 0; i < total; i++)
	{
		float ratio = static_cast<float>(i) / total;
		unsigned int curr_x = lerp(screen_start.x, screen_end.x, ratio);
		unsigned int curr_y = lerp(screen_start.y, screen_end.y, ratio);
		float curr_z = lerpf(screen_start.z, screen_end.z, ratio);

		A_PIXEL copyColor = _color1;
		if (PixelShader) {
			PixelShader(copyColor, 0, 0, 0);
		}
		PlotPixel(Vertex(static_cast<float>(curr_x), static_cast<float>(curr_y), curr_z, 1, 0, 0, 0), copyColor);
	}

	//float currX = screen_start.x;
	//float currY = screen_start.y;
	//int curr, start, end;

	//float deltaX = screen_end.x - screen_start.x;
	//float deltaY = screen_end.y - screen_start.y;

	//int toggle = 1;

	//int startX = static_cast<int>(screen_start.x), endX = static_cast<int>(screen_end.x), startY = static_cast<int>(screen_start.y), endY = static_cast<int>(screen_end.y);
	//float start2, end2;
	//int inc = 1;

	//float slopeX = deltaY / static_cast<float>(deltaX);
	//float slopeY = deltaX / static_cast<float>(deltaY);

	//bool isXDominant = abs(slopeX) < 1; //octant 1, 3, 4, 8 if true
	//bool isDown = deltaY > 0;

	//if (deltaX < 0 && isXDominant) {
	//	toggle = -1;
	//}
	//if (deltaY < 0 && !isXDominant) {
	//	toggle = -1;
	//}

	//if (isXDominant) {
	//	start = startX;
	//	end = endX;
	//	start2 = static_cast<float>(startY);
	//	end2 = static_cast<float>(endY);
	//}
	//else {
	//	start = startY;
	//	end = endY;
	//	start2 = static_cast<float>(startX);
	//	end2 = static_cast<float>(endX);
	//}

	//for (curr = start; toggle * curr <= toggle * end; curr += toggle)
	//{
	//	float ratio = (curr - start) / static_cast<float>(end - start);
	//	//float z = lerpf()
	//	float currZ = lerpf(screen_start.z, screen_end.z, ratio);
	//	isXDominant ? currY = lerp(static_cast<int>(start2), static_cast<int>(end2), ratio) : currX = lerp(static_cast<int>(start2), static_cast<int>(end2), ratio);
	//	PlotPixel(Vertex(static_cast<float>(isXDominant ? curr : floor(currX + 0.5)), static_cast<float>(isXDominant ? floor(currY + 0.5f) : curr), currZ, 0, 0, 0, 0), _color2 != 0 ? colorLerp(_color1, _color2, ratio) : _color1);
	//}
}

void tempFillTriangle(const Vertex& p1, const Vertex& p2, const Vertex& p3) {

	Vertex copy_p1 = p1;
	Vertex copy_p2 = p2;
	Vertex copy_p3 = p3;

	Vertex d;

	if (VertexShader) {
		VertexShader(copy_p1);
		VertexShader(copy_p2);
		VertexShader(copy_p3);
	}

	int numTriangles = ClipTriangle(copy_p1, copy_p2, copy_p3, d);

	if (numTriangles == -1) { //don't draw
		return;
	}

	PerspectiveDivide(copy_p1);
	PerspectiveDivide(copy_p2);
	PerspectiveDivide(copy_p3);

	Vertex screen_p1 = NDCtoScreen(copy_p1);
	Vertex screen_p2 = NDCtoScreen(copy_p2);
	Vertex screen_p3 = NDCtoScreen(copy_p3);


	int startX = min(min(screen_p1.x, screen_p2.x), screen_p3.x);
	int startY = min(min(screen_p1.y, screen_p2.y), screen_p3.y);
	int endX = max(max(screen_p1.x, screen_p2.x), screen_p3.x);
	int endY = max(max(screen_p1.y, screen_p2.y), screen_p3.y);

	for (int x = startX; x <= endX; x++)
	{
		for (int y = startY; y <= endY; y++)
		{
			Vertex bya = FindBarycentric(screen_p1, screen_p2, screen_p3, Vector2(x, y));
			if (bya.x >= 0 && bya.x <= 1 &&
				bya.y >= 0 && bya.y <= 1 &&
				bya.z >= 0 && bya.z <= 1)
			{
				//float z = berpf(bya, screen_p1.z, screen_p2.z, screen_p3.z);

				Vertex barycentric = berp(bya, screen_p1, screen_p2, screen_p3);
				float z = berp(bya, screen_p1.z, screen_p2.z, screen_p3.z);
				float u = berp(bya, screen_p1.u / screen_p1.w, screen_p2.u / screen_p2.w, screen_p3.u / screen_p3.w);
				float v = berp(bya, screen_p1.v / screen_p1.w, screen_p2.v / screen_p2.w, screen_p3.v / screen_p3.w);
				//float u = berp(bya, screen_p1.u, screen_p2.u, screen_p3.u);
				//float v = berp(bya, screen_p1.v, screen_p2.v, screen_p3.v);
				float RecipW = berp(bya, 1 / screen_p1.w, 1 / screen_p2.w, 1 / screen_p3.w);
				//float w = berpf(bya, p1.w, p2.w, p3.w);
				//unsigned int berpColor = colorBerp(bya, screen_p1.color, screen_p2.color, screen_p3.color);
				if (PixelShader) {
					PixelShader(barycentric.color, u / RecipW, v / RecipW, barycentric.w);
				}

				PlotPixel(Vertex(x, y, barycentric.z, 0, 0, 0, 0), barycentric.color);
			}
		}
	}

}

void FillTriangle(const Vertex& p1, const Vertex& p2, const Vertex& p3) {
	Vertex screen_p1 = NDCtoScreen(p1);
	Vertex screen_p2 = NDCtoScreen(p2);
	Vertex screen_p3 = NDCtoScreen(p3);


	int startX = min(min(screen_p1.x, screen_p2.x), screen_p3.x);
	int startY = min(min(screen_p1.y, screen_p2.y), screen_p3.y);
	int endX = max(max(screen_p1.x, screen_p2.x), screen_p3.x);
	int endY = max(max(screen_p1.y, screen_p2.y), screen_p3.y);

	for (int x = startX; x <= endX; x++)
	{
		for (int y = startY; y <= endY; y++)
		{
			Vertex bya = FindBarycentric(screen_p1, screen_p2, screen_p3, Vector2(x, y));
			if (bya.x >= 0 && bya.x <= 1 &&
				bya.y >= 0 && bya.y <= 1 &&
				bya.z >= 0 && bya.z <= 1)
			{
				//float z = berpf(bya, screen_p1.z, screen_p2.z, screen_p3.z);

				Vertex barycentric = berp(bya, screen_p1, screen_p2, screen_p3);
				float u = berp(bya, screen_p1.u / screen_p1.w, screen_p2.u / screen_p2.w, screen_p3.u / screen_p3.w);
				float v = berp(bya, screen_p1.v / screen_p1.w, screen_p2.v / screen_p2.w, screen_p3.v / screen_p3.w);
				//float u = berp(bya, screen_p1.u, screen_p2.u, screen_p3.u);
				//float v = berp(bya, screen_p1.v, screen_p2.v, screen_p3.v);
				float RecipW = berp(bya, 1 / screen_p1.w, 1 / screen_p2.w, 1 / screen_p3.w);
				//float w = berpf(bya, p1.w, p2.w, p3.w);
				//unsigned int berpColor = colorBerp(bya, screen_p1.color, screen_p2.color, screen_p3.color);
				if (PixelShader) {
					PixelShader(barycentric.color, u / RecipW, v / RecipW, barycentric.w);
				}

				PlotPixel(Vertex(x, y, barycentric.z, 0, 0, 0, 0), barycentric.color);
			}
		}
	}
}

bool BackFaceCull(Vertex v1, Vertex v2, Vertex v3) {
	Vector3 vector1 = Subtract(v2, v1);
	Vector3 Vector2 = Subtract(v3, v1);


	Vector3 crossProduct = Cross(vector1,Vector2);
	Vector3 cameraPos = camera.matrix[3];

	float result = DOT(cameraPos, crossProduct);

	if (v1.u == uvs[0] ? result < 0 : result > 0) {
		return false;
	}
	return true;
}


void DrawTriangle(const Vertex& p1, const Vertex& p2, const Vertex& p3) {

	Vertex copy_p1 = p1;
	Vertex copy_p2 = p2;
	Vertex copy_p3 = p3;

	Vertex FourthVertex;

	if (VertexShader) {
		VertexShader(copy_p1);
		VertexShader(copy_p2);
		VertexShader(copy_p3);
	}

	int numTriangles = ClipTriangle(copy_p1, copy_p2, copy_p3, FourthVertex);
	PerspectiveDivide(copy_p1);
	PerspectiveDivide(copy_p2);
	PerspectiveDivide(copy_p3);
	if (BackFaceCull(copy_p1, copy_p2, copy_p3)) {
		return;
	}
	PerspectiveDivide(FourthVertex);

	if (numTriangles == -1) { //don't draw
		return;
	}
	else if (numTriangles == 0 || numTriangles == 1) { //draw one triangle
		FillTriangle(copy_p1, copy_p2, copy_p3);
	}
	else { //draw two triangles
		FillTriangle(copy_p1, copy_p2, copy_p3);
		FillTriangle(copy_p1, copy_p3, FourthVertex);
	}

}

void DrawGrid() {
	for (size_t i = 0; i < 11; i++)
	{
		Parametric(gridPoints[i], gridPoints[i + 11], gridPoints[0].color);
		Parametric(gridPoints[i + 22], gridPoints[i + 33], gridPoints[0].color);
	}
}

void DrawCube() {
	for (size_t i = 0; i < sizeof(triangles) / sizeof(triangles[0]); i += 3)
	{
		if (i % 2 == 0) { //Set UVS of triangle 1
			cubePoints[triangles[i]].u = uvs[0];
			cubePoints[triangles[i]].v = uvs[1];
			cubePoints[triangles[i + 1]].u = uvs[2];
			cubePoints[triangles[i + 1]].v = uvs[3];
			cubePoints[triangles[i + 2]].u = uvs[4];
			cubePoints[triangles[i + 2]].v = uvs[5];
		}
		else { //set UVS of triangle2
			cubePoints[triangles[i]].u = uvs[6];
			cubePoints[triangles[i]].v = uvs[7];
			cubePoints[triangles[i + 1]].u = uvs[8];
			cubePoints[triangles[i + 1]].v = uvs[9];
			cubePoints[triangles[i + 2]].u = uvs[10];
			cubePoints[triangles[i + 2]].v = uvs[11];
		}
		Vector3 normal;
		if (i == 3) {
			normal = { 0, 0, -1 };
		}
		else if (i == 0) {
			normal = { 0, 0, 1 };
		}
		else if (i == 6) {
			normal = { 1, 0, 0 };
		}
		else if (i == 9) {
			normal = { 1, 0, 0 };
		}
		else if (i == 12) {
			normal = { 0, 0, 1 };
		}
		else if (i == 15) {
			normal = { 0, 0, 1 };
		}
		else if (i == 18) {
			normal = { -1, 0, 0 };
		}
		else if (i == 21) {
			normal = { -1, 0, 0 };
		}
		else if (i == 22) {
			normal = { 0, 1, 0 };
		}
		else if (i == 25) {
			normal = { 0, 1, 0 };
		}
		else if (i == 28) {
			normal = { 0, -1, 0 };
		}
		else if (i == 31) {
			normal = { 0, -1, 0 };
		}
		cubePoints[triangles[i]].normal = normal;
		cubePoints[triangles[i + 1]].normal = normal;
		cubePoints[triangles[i + 2]].normal = normal;
		//Draw Triangle
		DrawTriangle(cubePoints[triangles[i]], cubePoints[triangles[i + 1]], cubePoints[triangles[i + 2]]);
	}
}