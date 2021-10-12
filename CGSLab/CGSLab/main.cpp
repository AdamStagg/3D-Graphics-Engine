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
		Vertex(-.5, 0, -.5, 1, 0xFFFFFFFF),
		Vertex(-.4, 0, -.5, 1, 0xFFFFFFFF),
		Vertex(-.3, 0, -.5, 1, 0xFFFFFFFF),
		Vertex(-.2, 0, -.5, 1, 0xFFFFFFFF),
		Vertex(-.1, 0, -.5, 1, 0xFFFFFFFF),
		Vertex(  0, 0, -.5, 1, 0xFFFFFFFF),
		Vertex( .1, 0, -.5, 1, 0xFFFFFFFF),
		Vertex( .2, 0, -.5, 1, 0xFFFFFFFF),
		Vertex( .3, 0, -.5, 1, 0xFFFFFFFF),
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




		for (size_t i = 0; i < 360 * 20; i++)
		{
			float x = sin(i / 180.0f * 3.14159);
			float y = cos(i / 180.0f * 3.14159);

			float z = lerp(1, 10, i / (360.0f * 20));

			x /= z;
			y /= z;
			Vector2 pos = NDCtoScreen(Vertex(x, y, 0, 0, 0));


			unsigned int alpha = 0x000000FF;
			unsigned int red = 0x00000011;
			unsigned int green = 0x000000F7;
			unsigned int blue = 0x000000C1;

			unsigned int finalA = ((static_cast<int>(0) - static_cast<int>(alpha)) * lerp(0, 1, i / (360.0f * 20)) + alpha);
			unsigned int finalR = ((static_cast<int>(0) - static_cast<int>(red)) * lerp(0, 1, i / (360.0f * 20)) + red);
			unsigned int finalG = ((static_cast<int>(0) - static_cast<int>(green)) * lerp(0, 1, i / (360.0f * 20)) + green);
			unsigned int finalB = ((static_cast<int>(0) - static_cast<int>(blue)) * lerp(0, 1, i / (360.0f * 20)) + blue);


			unsigned int finalColor = finalA << 24 | finalR << 16 | finalG << 8 | finalB;

			PlotPixel(Raster, RasterWidth, pos, finalColor);
		}
	}

	RS_Shutdown();
	delete[] Raster;

}