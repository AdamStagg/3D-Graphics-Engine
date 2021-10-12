#include <iostream>
#include "Rasterization.h"
#include "RasterSurface.h"
#include "XTime.h"
#include <Windows.h>

int main() {

	const Vertex trianglePoints[3]
	{
		Vertex(.5f, .5f, 0, 0, 0xFFFF0000),
		Vertex(-.5f, .5f, 0, 0, 0xFF00FF00),
		Vertex(0 , -.5f, 0, 0, 0xFF0000FF)
	};

	const Vertex gridPoints[] = {
		//X change, close Z
		Vertex(-.5f, 0, -.5f, 1, 0xFFFFFFFF),
		Vertex(-.4f, 0, -.5f, 1, 0xFFFFFFFF),
		Vertex(-.3f, 0, -.5f, 1, 0xFFFFFFFF),
		Vertex(-.2f, 0, -.5f, 1, 0xFFFFFFFF),
		Vertex(-.1f, 0, -.5f, 1, 0xFFFFFFFF),
		Vertex(0.0f, 0, -.5f, 1, 0xFFFFFFFF),
		Vertex( .1f, 0, -.5f, 1, 0xFFFFFFFF),
		Vertex( .2f, 0, -.5f, 1, 0xFFFFFFFF),
		Vertex( .3f, 0, -.5f, 1, 0xFFFFFFFF),
		Vertex( .4f, 0, -.5f, 1, 0xFFFFFFFF),
		Vertex( .5f, 0, -.5f, 1, 0xFFFFFFFF),
		//X change, far Z
		Vertex(-.5f, 0, .5f, 1, 0xFFFFFFFF),
		Vertex(-.4f, 0, .5f, 1, 0xFFFFFFFF),
		Vertex(-.3f, 0, .5f, 1, 0xFFFFFFFF),
		Vertex(-.2f, 0, .5f, 1, 0xFFFFFFFF),
		Vertex(-.1f, 0, .5f, 1, 0xFFFFFFFF),
		Vertex(  0, 0, .5f, 1, 0xFFFFFFFF),
		Vertex( .1f, 0, .5f, 1, 0xFFFFFFFF),
		Vertex( .2f, 0, .5f, 1, 0xFFFFFFFF),
		Vertex( .3f, 0, .5f, 1, 0xFFFFFFFF),
		Vertex( .4f, 0, .5f, 1, 0xFFFFFFFF),
		Vertex( .5f, 0, .5f, 1, 0xFFFFFFFF),
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

	RS_Initialize(RasterWidth, RasterHeight);
	ClearColor(Raster, RasterPixelCount, 0xFF000000);

	while (RS_Update(Raster, RasterPixelCount))
	{
		//Initialization
		timer.Signal();
		totalTime += static_cast<float>(timer.Delta());
		ClearColor(Raster, RasterPixelCount, 0xFF000000);




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
		Matrix4x4 rotMat = BuildYRotationMatrix(totalTime);
		Matrix4x4 tranMat = Matrix4x4({ 1, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 0, 1, 0 }, { 0, -.25, 0, 1 });
		SV_WorldMatrix = MatrixMULTMatrix(rotMat, tranMat);

		//SV_WorldMatrix = MatrixMULTMatrix(rotMat, viewMatrix);

		Matrix4x4 cubeViewMatrix = OrthogonalAffineInverse(SV_WorldMatrix);
		//DRAW CUBE
		for (size_t i = 0; i < 4; i++)
		{
			Bresenham(cubePoints[i], cubePoints[(i + 4)], cubePoints[0].color);
			Bresenham(cubePoints[i], cubePoints[(i + 1) % 4], cubePoints[0].color);
			Bresenham(cubePoints[i + 4], cubePoints[4 + (i + 1) % 4], cubePoints[0].color);
		}
		if (orbit) {
			Matrix4x4 secondCube = Matrix4x4({ .2f, 0, 0, 0 }, { 0, .2f, 0, 0 }, { 0, 0, .2f, 0 }, { .5f, -.8f, 0, 1 });
			secondCube = MatrixMULTMatrix(BuildYRotationMatrix(-totalTime), secondCube);
			secondCube = MatrixMULTMatrix(BuildXRotationMatrix(totalTime * 2), secondCube);
			secondCube = MatrixMULTMatrix(secondCube, cubeViewMatrix);
			SV_WorldMatrix = secondCube;

			for (size_t i = 0; i < 4; i++)
			{
				Bresenham(cubePoints[i], cubePoints[(i + 4)], 0xFF00FFFF);
				Bresenham(cubePoints[i], cubePoints[(i + 1) % 4], 0xFF00FFFF);
				Bresenham(cubePoints[i + 4], cubePoints[4 + (i + 1) % 4], 0xFF00FFFF);
			}
		}
		if (GetAsyncKeyState(VK_SPACE) & 0x1) {
			VerticalFOV += 0.01f;
		}
		if (GetAsyncKeyState(VK_RETURN) & 0x1) {
			VerticalFOV -= 0.01f;
		}
		if (GetAsyncKeyState(VK_BACK) & 0x1) {
			orbit = !orbit;
		}

	}

	RS_Shutdown();
	delete[] Raster;

}