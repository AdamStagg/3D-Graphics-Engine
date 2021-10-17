#include "Rasterization.h"

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
		VertexShader = VS_PerspectiveCamera;


		//APPLY GRID SHADER
		PixelShader = 0;

		//SETUP SHADER VARIABLES
		SV_WorldMatrix = Identity4x4;

		//DRAW GRID
		DrawGrid();

		//APPLY CUBE SHADER
		PixelShader = isBilinear? PS_Bilinear: PS_Trilinear;

		//APPLY SHADER VARIABLES
		SV_WorldMatrix = MatrixMULTMatrix(BuildYRotationMatrix(static_cast<float>(timer.TotalTime())), Matrix4x4({ 1, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 0, 1, 0 }, { 0, .25, 0, 1 }));
		SV_TextureArray = celestial_pixels;
		SV_TextureArrayHeight = celestial_height;
		SV_TextureArrayWidth = celestial_width;

		//DRAW CUBE
		DrawCube();



		//Create cube view matrix for the orbiting cube
		Matrix4x4 cubeViewMatrix = OrthogonalAffineInverse(SV_WorldMatrix);

		//ORBIT CUBE SHADER VARIABLES
		SV_TextureArray = celestial_pixels;
		SV_TextureArrayHeight = celestial_height;
		SV_TextureArrayWidth = celestial_width;

		SV_WorldMatrix = Matrix4x4({ .2f, 0, 0, 0 }, { 0, .2f, 0, 0 }, { 0, 0, .2f, 0 }, { .5f, .7f, 0, 1 });
		SV_WorldMatrix = MatrixMULTMatrix(BuildYRotationMatrix(-static_cast<float>(-timer.TotalTime())), SV_WorldMatrix);
		SV_WorldMatrix = MatrixMULTMatrix(BuildXRotationMatrix(static_cast<float>(-timer.TotalTime()) * 2), SV_WorldMatrix);
		SV_WorldMatrix = MatrixMULTMatrix(SV_WorldMatrix, cubeViewMatrix);

		//Draw orbiting cube
		DrawCube();

		//FOV checks
		if (GetAsyncKeyState(VK_SPACE)) {
			VerticalFOV += 45 * timer.Delta();
		}
		if (GetAsyncKeyState(VK_RETURN)) {
			VerticalFOV -= 45 * timer.Delta();
		}

		//CAMERA MOVEMENT
		if (GetAsyncKeyState('W')) {
			camera = MatrixMULTMatrix(camera, BuildTranslationMatrix(0, 1 * timer.Delta(), 0));
		}
		if (GetAsyncKeyState('S')) {
			camera = MatrixMULTMatrix(camera, BuildTranslationMatrix(0, -1 * timer.Delta(), 0));
		}
		if (GetAsyncKeyState('A')) {
			camera = MatrixMULTMatrix(camera, BuildTranslationMatrix(-1 * timer.Delta(), 0, 0));
		}
		if (GetAsyncKeyState('D')) {
			camera = MatrixMULTMatrix(camera, BuildTranslationMatrix(1 * timer.Delta(), 0, 0));
		}
		if (GetAsyncKeyState('Q')) {
			camera = MatrixMULTMatrix(camera, BuildTranslationMatrix(0, 0, -1 * timer.Delta()));
		}
		if (GetAsyncKeyState('E')) {
			camera = MatrixMULTMatrix(camera, BuildTranslationMatrix(0, 0, 1 * timer.Delta()));
		}
		if (GetAsyncKeyState('R')) {
			VerticalFOV = 90;
			camera = MatrixMULTMatrix(
				Matrix4x4({ 1, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 0, 1, 0 }, { 0, 0, -1, 1 }),
				BuildXRotationMatrix(DegToRad(-18))
			);
		}
		if (GetAsyncKeyState('T') & 0x1) {
			isBilinear = !isBilinear;
			std::cout << "Mode set to: " << (isBilinear ? "Bilinear" : "Trilinear") << std::endl;
		}
		if (GetAsyncKeyState('Y') & 0x1) {
			backFaceCull = !backFaceCull;
			std::cout << "Back face culling: " << (backFaceCull ? "On" : "Off") << std::endl;
		}

	};

	//Close the program
	RS_Shutdown();
	delete[] Raster;
	delete[] DepthBuffer;

}