#include <iostream>
#include "Rasterization.h"
#include "RasterSurface.h"
#include "XTime.h"
#include <Windows.h>

int main() {

	const Vertex gridPoints[] = {
		//X change, close Z
		Vertex(-.5f, 0, -.5f, 1, 0xFFFFFFFF),
		Vertex(-.4f, 0, -.5f, 1, 0xFFFFFFFF),
		Vertex(-.3f, 0, -.5f, 1, 0xFFFFFFFF),
		Vertex(-.2f, 0, -.5f, 1, 0xFFFFFFFF),
		Vertex(-.1f, 0, -.5f, 1, 0xFFFFFFFF),
		Vertex(0.0f, 0, -.5f, 1, 0xFFFFFFFF),
		Vertex(.1f, 0, -.5f, 1, 0xFFFFFFFF),
		Vertex(.2f, 0, -.5f, 1, 0xFFFFFFFF),
		Vertex(.3f, 0, -.5f, 1, 0xFFFFFFFF),
		Vertex(.4f, 0, -.5f, 1, 0xFFFFFFFF),
		Vertex(.5f, 0, -.5f, 1, 0xFFFFFFFF),
		//X change, far Z
		Vertex(-.5f, 0, .5f, 1, 0xFFFFFFFF),
		Vertex(-.4f, 0, .5f, 1, 0xFFFFFFFF),
		Vertex(-.3f, 0, .5f, 1, 0xFFFFFFFF),
		Vertex(-.2f, 0, .5f, 1, 0xFFFFFFFF),
		Vertex(-.1f, 0, .5f, 1, 0xFFFFFFFF),
		Vertex(0, 0, .5f, 1, 0xFFFFFFFF),
		Vertex(.1f, 0, .5f, 1, 0xFFFFFFFF),
		Vertex(.2f, 0, .5f, 1, 0xFFFFFFFF),
		Vertex(.3f, 0, .5f, 1, 0xFFFFFFFF),
		Vertex(.4f, 0, .5f, 1, 0xFFFFFFFF),
		Vertex(.5f, 0, .5f, 1, 0xFFFFFFFF),
		//Left X changing Z
		Vertex(-.5f, 0,-.5f, 1, 0xFFFFFFFF),
		Vertex(-.5f, 0,-.4f, 1, 0xFFFFFFFF),
		Vertex(-.5f, 0,-.3f, 1, 0xFFFFFFFF),
		Vertex(-.5f, 0,-.2f, 1, 0xFFFFFFFF),
		Vertex(-.5f, 0,-.1f, 1, 0xFFFFFFFF),
		Vertex(-.5f, 0,  0, 1, 0xFFFFFFFF),
		Vertex(-.5f, 0, .1f, 1, 0xFFFFFFFF),
		Vertex(-.5f, 0, .2f, 1, 0xFFFFFFFF),
		Vertex(-.5f, 0, .3f, 1, 0xFFFFFFFF),
		Vertex(-.5f, 0, .4f, 1, 0xFFFFFFFF),
		Vertex(-.5f, 0, .5f, 1, 0xFFFFFFFF),
		//Right X changing Z
		Vertex(.5f, 0, -.5f, 1, 0xFFFFFFFF),
		Vertex(.5f, 0, -.4f, 1, 0xFFFFFFFF),
		Vertex(.5f, 0, -.3f, 1, 0xFFFFFFFF),
		Vertex(.5f, 0, -.2f, 1, 0xFFFFFFFF),
		Vertex(.5f, 0, -.1f, 1, 0xFFFFFFFF),
		Vertex(.5f, 0, 0, 1, 0xFFFFFFFF),
		Vertex(.5f, 0, .1f, 1, 0xFFFFFFFF),
		Vertex(.5f, 0, .2f, 1, 0xFFFFFFFF),
		Vertex(.5f, 0, .3f, 1, 0xFFFFFFFF),
		Vertex(.5f, 0, .4f, 1, 0xFFFFFFFF),
		Vertex(.5f, 0, .5f, 1, 0xFFFFFFFF)
	};

	const Vertex cubePoints[] = {
		Vertex(-.25f,	-.25f, -.25f,	1, 0xFF00FF00),
		Vertex(.25f,	-.25f, -.25f,	1, 0xFF00FF00),
		Vertex(.25f,	-.25f,  .25f,	1, 0xFF00FF00),
		Vertex(-.25f,	-.25f,  .25f,	1, 0xFF00FF00),
		Vertex(-.25f,	 .25f, -.25f,	1, 0xFF00FF00),
		Vertex(.25f,	 .25f, -.25f,	1, 0xFF00FF00),
		Vertex(.25f,	 .25f,  .25f,	1, 0xFF00FF00),
		Vertex(-.25f,	 .25f,  .25f,	1, 0xFF00FF00)
	};

	//const Vertex cameraPoint(0, 0, 0, 0, 0);

	Matrix4x4 cameraMatrix = MatrixMULTMatrix(
		Matrix4x4({ 1, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 0, 1, 0 }, { 0, 0, -1, 1 }),
		BuildXRotationMatrix(DegToRad(18))
	);

	camera = &cameraMatrix;



	XTime timer;
	timer.Restart();

	Raster = new unsigned int[RasterPixelCount];
	ClearColor(0);

	RS_Initialize(RasterWidth, RasterHeight);


	while(RS_Update(Raster, RasterPixelCount))
	{
		//Initialization
		timer.Signal();
		ClearColor(0xFF000000);


		//APPLY GRID SHADER
		VertexShader = VS_PerspectiveCamera;
		SV_WorldMatrix = Identity4x4;
		//DRAW GRID

		for (size_t i = 0; i < 11; i++)
		{
			Bresenham(gridPoints[i], gridPoints[i + 11], gridPoints[0].color);
			Bresenham(gridPoints[i + 22], gridPoints[i + 33], gridPoints[0].color);
		}


		//APPLY CUBE SHADER
		VertexShader = VS_PerspectiveCamera;
		Matrix4x4 rotMat = BuildYRotationMatrix(static_cast<float>(timer.TotalTime()));
		Matrix4x4 tranMat = Matrix4x4({ 1, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 0, 1, 0 }, { 0, -.25, 0, 1 });
		Matrix4x4 cubeMatrix = MatrixMULTMatrix(rotMat, tranMat);

		SV_WorldMatrix = cubeMatrix;

		//SV_WorldMatrix = MatrixMULTMatrix(rotMat, viewMatrix);

		Matrix4x4 cubeViewMatrix = OrthogonalAffineInverse(SV_WorldMatrix);
		//DRAW CUBE
		for (size_t i = 0; i < 4; i++)
		{
			Bresenham(cubePoints[i], cubePoints[(i + 4)], cubePoints[0].color);
			Bresenham(cubePoints[i], cubePoints[(i + 1) % 4], cubePoints[0].color);
			Bresenham(cubePoints[i + 4], cubePoints[4 + (i + 1) % 4], cubePoints[0].color);
		}
		Matrix4x4 secondCube = Matrix4x4({ .2f, 0, 0, 0 }, { 0, .2f, 0, 0 }, { 0, 0, .2f, 0 }, { .5f, -.8f, 0, 1 });
		secondCube = MatrixMULTMatrix(BuildYRotationMatrix(-static_cast<float>(timer.TotalTime())), secondCube);
		secondCube = MatrixMULTMatrix(BuildXRotationMatrix(static_cast<float>(timer.TotalTime()) * 2), secondCube);
		secondCube = MatrixMULTMatrix(secondCube, cubeViewMatrix);
		SV_WorldMatrix = secondCube;

		for (size_t i = 0; i < 4; i++)
		{
			Bresenham(cubePoints[i], cubePoints[(i + 4)], 0xFF00FFFF);
			Bresenham(cubePoints[i], cubePoints[(i + 1) % 4], 0xFF00FFFF);
			Bresenham(cubePoints[i + 4], cubePoints[4 + (i + 1) % 4], 0xFF00FFFF);
		}
		if (GetAsyncKeyState(VK_SPACE) & 0x1) {
			VerticalFOV += 0.01f;
		}
		if (GetAsyncKeyState(VK_RETURN) & 0x1) {
			VerticalFOV -= 0.01f;
		}
	};

	RS_Shutdown();
	delete[] Raster;

}