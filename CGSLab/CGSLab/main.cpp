#include <iostream>
#include "Rasterization.h"
#include "RasterSurface.h"
#include "XTime.h"
#include <Windows.h>
#include "4_Castle.h"
#include "0_Helicopter.h"

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


	while (RS_Update(Raster, RasterPixelCount))
	{
		//Initialization
		timer.Signal();
		ClearColor(0xFF000000);

		unsigned int heliColor = 0xFFd141ad;
		unsigned int cstlColor = 0xFF4e79d6;

		for (size_t i = 0; i < RasterPixelCount; i++)
		{
			float heliR = (((_0_Helicopter_pixels[i] & 0xFF000000) >> 24));
			float heliG = (((_0_Helicopter_pixels[i] & 0x00ff0000) >> 16));
			float heliB = (((_0_Helicopter_pixels[i] & 0x0000ff00) >> 8));
			float cstlR = (((_4_Castle_pixels[i] & 0xFF000000) >> 24));
			float cstlG = (((_4_Castle_pixels[i] & 0x00ff0000) >> 16));
			float cstlB = (((_4_Castle_pixels[i] & 0x0000ff00) >> 8));

			unsigned int heliDepth = heliR + heliG + heliB;
			unsigned int cstlDepth = cstlR + cstlB + cstlG;

			float heliZ = static_cast<float>(heliDepth) / (3 << 8);
			float cstlZ = static_cast<float>(cstlDepth) / (3 << 8);

			if (heliZ > cstlZ) {
				Raster[i] = colorLerp(0x00000000, cstlColor, cstlZ);
			}
			else {
				Raster[i] = colorLerp(0x00000000, heliColor, heliZ);
			}
		}



	};

	RS_Shutdown();
	delete[] Raster;

}