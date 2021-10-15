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

void Parametric(const Vector2 _start, const Vector2 _end, const unsigned int _color1, const unsigned int _color2) {

	float currX = _start.x;
	float currY = _start.y;
	int curr, start, end;

	float deltaX = _end.x - _start.x;
	float deltaY = _end.y - _start.y;

	int toggle = 1;

	int startX = static_cast<int>(_start.x), endX = static_cast<int>(_end.x), startY = static_cast<int>(_start.y), endY = static_cast<int>(_end.y);
	float start2, end2;
	int inc = 1;

	float slopeX = deltaY / static_cast<float>(deltaX);
	float slopeY = deltaX / static_cast<float>(deltaY);

	bool isXDominant = abs(slopeX) < 1; //octant 1, 3, 4, 8 if true
	bool isDown = deltaY > 0;

	if (deltaX < 0 && isXDominant) {
		toggle = -1;
	}
	if (deltaY < 0 && !isXDominant) {
		toggle = -1;
	}

	if (isXDominant) {
		start = startX;
		end = endX;
		start2 = static_cast<float>(startY);
		end2 = static_cast<float>(endY);
	}
	else {
		start = startY;
		end = endY;
		start2 = static_cast<float>(startX);
		end2 = static_cast<float>(endX);
	}

	for (curr = start; toggle * curr <= toggle * end; curr += toggle)
	{
		float ratio = (curr - start) / static_cast<float>(end - start);
		isXDominant ? currY = lerp(static_cast<int>(start2), static_cast<int>(end2), ratio) : currX = lerp(static_cast<int>(start2), static_cast<int>(end2), ratio);
		PlotPixel(Vertex(static_cast<float>(isXDominant ? curr : floor(currX + 0.5)), static_cast<float>(isXDominant ? floor(currY + 0.5f) : curr), 0, 0, 0, 0, 0), colorLerp(_color1, _color2, ratio));
	}
}

void FillTriangle(const Vertex& p1, const Vertex& p2, const Vertex& p3) {

	Vertex copy_p1 = p1;
	Vertex copy_p2 = p2;
	Vertex copy_p3 = p3;

	if (VertexShader) {
		VertexShader(copy_p1);
		VertexShader(copy_p2);
		VertexShader(copy_p3);
	}

	Vertex screen_p1 = NDCtoScreen(copy_p1);
	Vertex screen_p2 = NDCtoScreen(copy_p2);
	Vertex screen_p3 = NDCtoScreen(copy_p3);


	int startX = min(min(screen_p1.x, screen_p2.x), screen_p3.x);
	int startY = min(min(screen_p1.y, screen_p2.y), screen_p3.y);
	int endX = max(max(screen_p1.x, screen_p2.x), screen_p3.x);
	int endY = max(max(screen_p1.y, screen_p2.y), screen_p3.y);

	for (float x = startX; x <= endX; x++)
	{
		for (float y = startY; y <= endY; y++)
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
				float w = berp(bya, 1 / screen_p1.w, 1 / screen_p2.w, 1 / screen_p3.w);
				//float w = berpf(bya, p1.w, p2.w, p3.w);
				//unsigned int berpColor = colorBerp(bya, screen_p1.color, screen_p2.color, screen_p3.color);
				if (PixelShader) {
					PixelShader(barycentric.color, u/w, v/w, barycentric.w);
				}

				PlotPixel(Vertex(x, y, barycentric.z, 0, u / w, v/w, 0), barycentric.color);
			}
		}
	}

}

void DrawGrid() {
	for (size_t i = 0; i < 11; i++)
	{
		Bresenham(gridPoints[i], gridPoints[i + 11], gridPoints[0].color);
		Bresenham(gridPoints[i + 22], gridPoints[i + 33], gridPoints[0].color);
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
		//Draw Triangle
		FillTriangle(cubePoints[triangles[i]], cubePoints[triangles[i + 1]], cubePoints[triangles[i + 2]]);
	}
}