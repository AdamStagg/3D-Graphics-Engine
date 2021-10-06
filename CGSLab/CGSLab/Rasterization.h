#pragma once
#include "Shaders.h"


void PlotPixel(unsigned int* _raster, const int _rasterIndex, const unsigned int _color) {
	_raster[_rasterIndex] = _color;
}

void PlotPixel(unsigned int* _raster, const int _width, const Vector2 _pos, const unsigned int _color) {
	_raster[ConvertDimension(_pos, _width)] = _color;
}

void ClearColor(unsigned int* _raster, const int _numPixels, unsigned int _color) {

	for (int i = 0; i < _numPixels; i++)
	{
		_raster[i] = _color;
	}
}

void Bresenham(unsigned int* _raster, const unsigned int _rasterWidth, const Vector2 _startPos, const Vector2 _endPos, const unsigned int _color) {

	int currX = _startPos.x;
	int currY = _startPos.y;
	int curr, start, end;
	float slope;

	int startX = static_cast<int>(_startPos.x), endX = static_cast<int>(_endPos.x), startY = static_cast<int>(_startPos.y), endY = static_cast<int>(_endPos.y);
	int inc = 1;

	float slopeX = abs(static_cast<int>(_endPos.y - _startPos.y) / static_cast<float>(static_cast<int>(_endPos.x - _startPos.x)));
	float slopeY = abs(static_cast<int>(_endPos.x - _startPos.x) / static_cast<float>(static_cast<int>(_endPos.y - _startPos.y)));
	float error = 0;

	bool isXDominant = abs(static_cast<int>(_endPos.y - _startPos.y) / static_cast<float>(static_cast<int>(_endPos.x - _startPos.x))) < 1;

	if (static_cast<int>(_endPos.x - _startPos.x) < 0) {
		Swap(&startX, &endX);
		currY = _endPos.y;
		inc = -inc;
	}
	if (static_cast<int>(_endPos.y - _startPos.y) < 0) {
		Swap(&startY, &endY);
		currX = _endPos.x;
		inc = -inc;
	}

	if (isXDominant) {
		start = startX;
		end = endX;
		slope = slopeX;
	}
	else {
		start = startY;
		end = endY;
		slope = slopeY;
	}

	for (curr = start; curr <= end; curr++)
	{
		PlotPixel(_raster, _rasterWidth, isXDominant ? Vector2(curr, currY) : Vector2(currX, curr), _color);
		error += slope;
		if (error > 0.5f) {
			isXDominant ? currY += inc : currX += inc;
			error--;
		}
	}
}

void MidPoint(unsigned int* _raster, const unsigned int _rasterWidth, const Vector2 _startPos, const Vector2 _endPos, const unsigned int _color) {

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
		//isXDominant ? curr2 = static_cast<int>(_endPos.y) : curr = static_cast<int>(_endPos.y); //currY = _endPos.y;
		inc = -inc;
		toggle = -toggle;
		xInc = -xInc;
	}
	if (static_cast<int>(_endPos.y - _startPos.y) < 0) {
		Swap(&startY, &endY);
		currX = _endPos.x;
		//isXDominant ? static_cast<int>(_endPos.x) : static_cast<int>(_endPos.x); //currX = _endPos.x;
		inc = -inc;
		toggle = -toggle;
		yInc = -yInc;
	}

	if (isXDominant) {
		start = startX;
		end = endX;
		yInc = 0.5f;
		xInc = inc;
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

#if 1 //this code works for all lines in one for loop, unsure if the gradient is correct though
	for (curr = start; curr <= end; curr++)
	{
		PlotPixel(_raster, _rasterWidth, isXDominant ? Vector2(curr, currY) : Vector2(currX, curr), _color);
		Vector4 mid(isXDominant ? curr + xInc : currX + xInc, isXDominant ? currY + yInc : curr + yInc, 0, 0);
		if (toggle * ((yTogg * static_cast<int>(_endPos.x - _startPos.x)) < 0 ? ImplicitLineEquation(mid, _end, _start) : ImplicitLineEquation(mid, _start, _end)) < 0) {
			isXDominant ? currY += inc : currX += inc;
		}
	}
#endif

#if 0 //all lines working and gradient is correct, will simplify later
	if (isRight && isDown && isXDominant) { //o1

		currY = startY;
		for (curr = start; curr <= end; curr++)
		{
			PlotPixel(_raster, _rasterWidth, Vector2(curr, currY), _color);
			Vector2 mid(curr + 1, currY + 0.5f);
			if (ImplicitLineEquation(mid, _startPos, _endPos) < 0) {
				currY++;
			}
		}
	}
	else if (isRight && isDown && !isXDominant) { //o2

		currX = startX;
		for (curr = start; curr <= end; curr++) {
			PlotPixel(_raster, _rasterWidth, Vector2(currX, curr), _color);
			Vector2 mid(currX + 0.5f, curr + 1);
			if (ImplicitLineEquation(mid, _startPos, _endPos) > 0) {
				currX++;
			}
		}

	}
	else if (!isRight && isDown && !isXDominant) { //o3
		currX = startX;
		for (curr = start; curr <= end; curr++) {
			PlotPixel(_raster, _rasterWidth, Vector2(currX, curr), _color);
			Vector2 mid(currX - 0.5f, curr + 1);
			if (ImplicitLineEquation(mid, _startPos, _endPos) < 0) {
				currX--;
			}
		}
	}
	else if (!isRight && isDown && isXDominant) { //o4
		currY = startY;
		for (curr = start; curr >= end; curr--)
		{
			PlotPixel(_raster, _rasterWidth, Vector2(curr, currY), _color);
			Vector2 mid(curr + 1, currY - 0.5f);
			if (ImplicitLineEquation(mid, _startPos, _endPos) > 0) {
				currY++;
			}
		}
	}
	else if (!isRight && !isDown && isXDominant) { //o5
		currY = startY;
		for (curr = start; curr >= end; curr--)
		{
			PlotPixel(_raster, _rasterWidth, Vector2(curr, currY), _color);
			Vector2 mid(curr + 1, currY + 0.5f);
			if (ImplicitLineEquation(mid, _startPos, _endPos) < 0) {
				currY--;
			}
		}
	}
	else if (!isRight && !isDown && !isXDominant) { //o6
		currX = startX;
		for (curr = start; curr >= end; curr--) {
			PlotPixel(_raster, _rasterWidth, Vector2(currX, curr), _color);
			Vector2 mid(currX + 0.5f, curr + 1);
			if (ImplicitLineEquation(mid, _startPos, _endPos) > 0) {
				currX--;
			}
		}
	}
	else if (isRight && !isDown && !isXDominant) { //o7
		currX = startX;
		for (curr = start; curr >= end; curr--) {
			PlotPixel(_raster, _rasterWidth, Vector2(currX, curr), _color);
			Vector2 mid(currX + 0.5f, curr - 1);
			if (ImplicitLineEquation(mid, _startPos, _endPos) < 0) {
				currX++;
			}
		}
	}
	else { //o8
		currY = startY;
		for (curr = start; curr <= end; curr++)
		{
			PlotPixel(_raster, _rasterWidth, Vector2(curr, currY), _color);
			Vector2 mid(curr - 1, currY + 0.5f);
			if (ImplicitLineEquation(mid, _startPos, _endPos) > 0) {
				currY--;
			}
		}
	}
#endif
}

void Parametric(unsigned int* _raster, const int _rasterWidth, const Vector2 _start, const Vector2 _end, const unsigned int _color1, const unsigned int _color2) {

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
	bool isRight = deltaX > 0;
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
		start2 = startY;
		end2 = endY;
	}
	else {
		start = startY;
		end = endY;
		start2 = startX;
		end2 = endX;
	}

	for (curr = start; toggle * curr <= toggle * end; curr += toggle)
	{
		float ratio = (curr - start) / static_cast<float>(end - start);
		isXDominant ? currY = lerp(start2, end2, ratio) : currX = lerp(start2, end2, ratio);
		PlotPixel(_raster, _rasterWidth, Vector2(static_cast<float>(isXDominant ? curr : floor(currX + 0.5)), static_cast<float>(isXDominant ? floor(currY + 0.5f) : curr)), colorLerp(_color1, _color2, ratio));
	}
}

void FillTriangle(Vector4 p1, Vector4 p2, Vector4 p3, A_PIXEL color) {


	float startX = std::min(std::min(p1.x, p2.x), p3.x);
	float startY = std::min(std::min(p1.y, p2.y), p3.y);
	float endX = std::max(std::max(p1.x, p2.x), p3.x);
	float endY = std::max(std::max(p1.y, p2.y), p3.y);

	for (float x = startX; x < endX; x++)
	{
		for (float y = startY; y < endY; y++)
		{
			Vector4 bya = FindBarycentric(p1, p2, p3, Vector4(x, y, 0, 0));
			if (bya.x >= 0 && bya.x <= 1 &&
				bya.y >= 0 && bya.y <= 1 &&
				bya.z >= 0 && bya.z <= 1)
			{
				PlotPixel(Raster, RasterWidth, Vector2(x, y), color);
			}
		}
	}

}