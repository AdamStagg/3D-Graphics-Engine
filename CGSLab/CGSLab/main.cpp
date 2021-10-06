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
		timer.Signal();
		totalTime += timer.Delta();

		VertexShader = VS_World;
		SV_WorldMatrix = BuildZRotationMatrix(totalTime);

		ClearColor(Raster, RasterPixelCount, 0xFF000000);
		
 		FillTriangle(trianglePoints[0], trianglePoints[1], trianglePoints[2], 0xFF808080);

		Bresenham(Raster, RasterWidth, trianglePoints[0], trianglePoints[1], 0xFFFFFFFF);
		Bresenham(Raster, RasterWidth, trianglePoints[1], trianglePoints[2], 0xFFFFFFFF);
		Bresenham(Raster, RasterWidth, trianglePoints[2], trianglePoints[0], 0xFFFFFFFF);


	}

	RS_Shutdown();
	delete[] Raster;

}