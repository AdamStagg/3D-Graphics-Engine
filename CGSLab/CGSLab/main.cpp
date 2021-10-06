#include <iostream>
#include "Rasterization.h"
#include "RasterSurface.h"


int main() {

	Vertex trianglePoints[3]
	{
		Vertex(182, 214, 0, 0, 0xFFFF0000),
		Vertex(201, 170, 0, 0, 0xFF00FF00),
		Vertex(82 , 162, 0, 0, 0xFF0000FF)
	};


	Matrix3x3 m({ 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 });
	Matrix3x3 o = MatrixMULTMatrix(m, m);


	Raster = new unsigned int[RasterPixelCount];

	RS_Initialize(RasterWidth, RasterHeight);


	while (RS_Update(Raster, RasterPixelCount)) 
	{
		ClearColor(Raster, RasterPixelCount, 0xFF000000);
		
 		FillTriangle(trianglePoints[0].values, trianglePoints[1].values, trianglePoints[2].values, 0xFFE77BC9);
		


		Bresenham(Raster, RasterWidth, Vector2(trianglePoints[0].values.x, trianglePoints[0].values.y), Vector2(trianglePoints[1].values.x, trianglePoints[1].values.y), 0xFFFFFFFF);
		Bresenham(Raster, RasterWidth, Vector2(trianglePoints[1].values.x, trianglePoints[1].values.y), Vector2(trianglePoints[2].values.x, trianglePoints[2].values.y), 0xFFFFFFFF);
		Bresenham(Raster, RasterWidth, Vector2(trianglePoints[2].values.x, trianglePoints[2].values.y), Vector2(trianglePoints[0].values.x, trianglePoints[0].values.y), 0xFFFFFFFF);


	}

	RS_Shutdown();
	delete[] Raster;

}