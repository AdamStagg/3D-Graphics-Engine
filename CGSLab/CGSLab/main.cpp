#include "Rasterization.h"
#include "4_Human.h"

int main() {

	//Create the camera
	camera = MatrixMULTMatrix(
		Matrix4x4({ 1, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 0, 1, 0 }, { 0, 0, -1, 1 }),
		BuildXRotationMatrix(DegToRad(-18))
	);

	bool isBilinear = true;
	bool backFaceCull = false;


	//Clear buffers
	ClearColor(0);
	ClearDepth();

	//Initialization
	RS_Initialize(RasterWidth, RasterHeight);

	while (RS_Update(Raster, RasterPixelCount))
	{
		//Initialization
		timer.Signal();
		ClearColor(0xFF000000);
		ClearDepth();


		for (size_t i = 0; i < RasterPixelCount; i++)
		{
			unsigned int pixel = _4_Human_pixels[i];

			Vector3 normal = {
				static_cast<float>(((pixel & 0x0000ff00) >> 8)),
				static_cast<float>(((pixel & 0x00ff0000) >> 16)),
				static_cast<float>(((pixel & 0xff000000) >> 24))
			};

			ScaleVector(normal, 1 / 255.0f);

			NormalizeVector(normal);

			Vector3 lightDir = { -0.577, -0.577, 0.577 };
			Vector4 lightColor = { 1, 216 / 255.0f,0,121 / 255.0f };

			Vector4 surfaceColor = { 1, 1, 1, 1 };


			NegateVector(lightDir);

			float lightRatio = VectorDOTVector(lightDir, normal);

			if (lightRatio < 0) lightRatio = 0;
			if (lightRatio > 1) lightRatio = 1;
			
			Vector4 result =
			{
				1,
				lightRatio * lightColor.y * surfaceColor.y,
				lightRatio * lightColor.z * surfaceColor.z,
				lightRatio * lightColor.w * surfaceColor.w
			};

			unsigned int finalColor =
			{
				(255u << 24) |
				static_cast<unsigned int>(255 * result.y) << 16 |
				static_cast<unsigned int>(255 * result.z) << 8 |
				static_cast<unsigned int>(255 * result.w)

			};
			Raster[i] = finalColor;
		}



	};

	//Close the program
	RS_Shutdown();
	delete[] Raster;
	delete[] DepthBuffer;

}