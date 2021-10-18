#include "Rasterization.h"
#include "0_cave_bluelarge.h"

int main() {

	//Create the camera
	camera = MatrixMULTMatrix(
		Matrix4x4({ 1, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 0, 1, 0 }, { 0, 0, -1, 1 }),
		BuildXRotationMatrix(DegToRad(18))
	);

	//Clear buffers
	ClearColor(0);
	ClearDepth();

	//Initialization
	RS_Initialize(RasterWidth, RasterHeight);

	bool temp = true;

	while(RS_Update(Raster, RasterPixelCount))
	{	
		//Initialization
		timer.Signal();
		ClearColor(0xFF000000);
		ClearDepth();
		
		for (size_t i = 0; i < RasterPixelCount; i++)
		{

			float uRatio = (i % RasterWidth) / static_cast<float>(RasterWidth);
			float vRatio = (i / RasterWidth) / static_cast<float>(RasterHeight);

			int pixelIndex = ConvertDimension(uRatio * _0_cave_bluelarge_width, vRatio * _0_cave_bluelarge_height, _0_cave_bluelarge_width);

			unsigned int color1 = colorLerp(BGRAtoARGB(_0_cave_bluelarge_pixels[pixelIndex]), BGRAtoARGB(_0_cave_bluelarge_pixels[pixelIndex + 1]), uRatio);
			unsigned int color2 = colorLerp(BGRAtoARGB(_0_cave_bluelarge_pixels[pixelIndex + _0_cave_bluelarge_height]), BGRAtoARGB(_0_cave_bluelarge_pixels[pixelIndex + 1 + _0_cave_bluelarge_height]), uRatio);

			unsigned finalColor = colorLerp(color1, color2, vRatio);
			Raster[i] = finalColor;
		}

		



		
	};

	//Close the program
	RS_Shutdown();
	delete[] Raster;
	delete[] DepthBuffer;

}