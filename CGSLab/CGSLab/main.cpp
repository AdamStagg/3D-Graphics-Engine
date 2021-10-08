#include <iostream>
#include "Rasterization.h"
#include "RasterSurface.h"
#include "XTime.h"

int main() {

	const Vertex trianglePoints[3]
	{
		Vertex(.5, .5, 0, 0, 0xFFFF0000),
		Vertex(-.5, .5, 0, 0, 0xFF00FF00),
		Vertex(0 , -.5, 0, 0, 0xFF0000FF)
	};

	const Vertex gridPoints[] = {
		//X change, close Z
		Vertex( .3, 0, -.5, 1, 0xFFFFFFFF),
		Vertex(-.5, 0, -.5, 1, 0xFFFFFFFF),
		Vertex(-.4, 0, -.5, 1, 0xFFFFFFFF),
		Vertex(-.3, 0, -.5, 1, 0xFFFFFFFF),
		Vertex(-.2, 0, -.5, 1, 0xFFFFFFFF),
		Vertex(-.1, 0, -.5, 1, 0xFFFFFFFF),
		Vertex(  0, 0, -.5, 1, 0xFFFFFFFF),
		Vertex( .1, 0, -.5, 1, 0xFFFFFFFF),
		Vertex( .2, 0, -.5, 1, 0xFFFFFFFF),
		Vertex( .4, 0, -.5, 1, 0xFFFFFFFF),
		Vertex( .5, 0, -.5, 1, 0xFFFFFFFF),
		//X change, far Z
		Vertex(-.5, 0, .5, 1, 0xFFFFFFFF),
		Vertex(-.4, 0, .5, 1, 0xFFFFFFFF),
		Vertex(-.3, 0, .5, 1, 0xFFFFFFFF),
		Vertex(-.2, 0, .5, 1, 0xFFFFFFFF),
		Vertex(-.1, 0, .5, 1, 0xFFFFFFFF),
		Vertex(  0, 0, .5, 1, 0xFFFFFFFF),
		Vertex( .1, 0, .5, 1, 0xFFFFFFFF),
		Vertex( .2, 0, .5, 1, 0xFFFFFFFF),
		Vertex( .3, 0, .5, 1, 0xFFFFFFFF),
		Vertex( .4, 0, .5, 1, 0xFFFFFFFF),
		Vertex( .5, 0, .5, 1, 0xFFFFFFFF),
		//Left X changing Z
		Vertex(-.5, 0,-.5, 1, 0xFFFFFFFF),
		Vertex(-.5, 0,-.4, 1, 0xFFFFFFFF),
		Vertex(-.5, 0,-.3, 1, 0xFFFFFFFF),
		Vertex(-.5, 0,-.2, 1, 0xFFFFFFFF),
		Vertex(-.5, 0,-.1, 1, 0xFFFFFFFF),
		Vertex(-.5, 0,  0, 1, 0xFFFFFFFF),
		Vertex(-.5, 0, .1, 1, 0xFFFFFFFF),
		Vertex(-.5, 0, .2, 1, 0xFFFFFFFF),
		Vertex(-.5, 0, .3, 1, 0xFFFFFFFF),
		Vertex(-.5, 0, .4, 1, 0xFFFFFFFF),
		Vertex(-.5, 0, .5, 1, 0xFFFFFFFF),
		//Right X changing Z
		Vertex(.5, 0,-.5, 1, 0xFFFFFFFF),
		Vertex(.5, 0,-.4, 1, 0xFFFFFFFF),
		Vertex(.5, 0,-.3, 1, 0xFFFFFFFF),
		Vertex(.5, 0,-.2, 1, 0xFFFFFFFF),
		Vertex(.5, 0,-.1, 1, 0xFFFFFFFF),
		Vertex(.5, 0,  0, 1, 0xFFFFFFFF),
		Vertex(.5, 0, .1, 1, 0xFFFFFFFF),
		Vertex(.5, 0, .2, 1, 0xFFFFFFFF),
		Vertex(.5, 0, .3, 1, 0xFFFFFFFF),
		Vertex(.5, 0, .4, 1, 0xFFFFFFFF),
		Vertex(.5, 0, .5, 1, 0xFFFFFFFF)
	};

	const Vertex cubePoints[] = {
		Vertex(-.25, -.25, -.25, 1, 0xFF00FF00),
		Vertex(.25, -.25, -.25,  1, 0xFF00FF00),
		Vertex(.25, -.25,  .25,  1, 0xFF00FF00),
		Vertex(-.25, -.25,  .25, 1, 0xFF00FF00),
		Vertex(-.25,  .25, -.25, 1, 0xFF00FF00),
		Vertex(.25,  .25, -.25,  1, 0xFF00FF00),
		Vertex(.25,  .25,  .25,  1, 0xFF00FF00),
		Vertex(-.25,  .25,  .25, 1, 0xFF00FF00)
	};

	//const Vertex trianglePoints[3]
	//{
	//	NDCtoScreen(tp[0]),
	//	NDCtoScreen(tp[1]),
	//	NDCtoScreen(tp[2])
	//};
	XTime timer;
	timer.Restart();

	Raster = new unsigned int[RasterPixelCount];

	RS_Initialize(RasterWidth, RasterHeight);
	ClearColor(Raster, RasterPixelCount, 0xFF000000);

	while (RS_Update(Raster, RasterPixelCount)) 
	{
		//Initialization
		timer.Signal();
		totalTime += timer.Delta();
		ClearColor(Raster, RasterPixelCount, 0xFF000000);
		
		//APPLY GRID SHADER
		VertexShader = VS_World;
		SV_WorldMatrix = Matrix4x4({ 1, 0, 0, 0 }, { 0, 1, 0 ,0 }, { 0, 0, 1 ,0 }, { 0, 0, 0, 1 });

		//DRAW GRID
		for (size_t i = 0; i < 11; i++)
		{
			Bresenham(gridPoints[i], gridPoints[i + 11], gridPoints[0].color);
			Bresenham(gridPoints[i + 22], gridPoints[i + 33], gridPoints[0].color);
		}


		//APPLY CUBE SHADER
		VertexShader = VS_World;
		Matrix4x4 rotMat = BuildYRotationMatrix(totalTime);
		Matrix4x4 tranMat = Matrix4x4({ 1, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 0, 1, 0 }, { 0, -.25, 0, 1 });
		rotMat = MatrixMULTMatrix(rotMat, tranMat);

		SV_WorldMatrix = rotMat;

		//DRAW CUBE
		for (size_t i = 0; i < 4; i++)
		{
			Bresenham(cubePoints[i], cubePoints[(i + 4)], cubePoints[0].color);
			Bresenham(cubePoints[i], cubePoints[(i + 1) % 4], cubePoints[0].color);
			Bresenham(cubePoints[i + 4], cubePoints[4 + (i + 1)%4], cubePoints[0].color);
		}


	}

	RS_Shutdown();
	delete[] Raster;

}