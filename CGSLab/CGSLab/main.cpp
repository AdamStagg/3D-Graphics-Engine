#include "Rasterization.h"

int main() {

	//Create the camera
	camera = MatrixMULTMatrix(
		Matrix4x4({ 1, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 0, 1, 0 }, { 0, 0, -1, 1 }),
		BuildXRotationMatrix(DegToRad(-18))
	);

	bool temp = true;

	//Clear buffers
	ClearColor(0);
	ClearDepth();

	//Initialization
	RS_Initialize(RasterWidth, RasterHeight);

	while(RS_Update(Raster, RasterPixelCount))
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
		if (temp) DrawGrid();


		//APPLY CUBE SHADER
		PixelShader = temp? PS_Bilinear : PS_Nearest;

		//APPLY SHADER VARIABLES
		SV_WorldMatrix = MatrixMULTMatrix(BuildYRotationMatrix(static_cast<float>(timer.TotalTime())), Matrix4x4({ 1, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 0, 1, 0 }, { 0, .25, 0, 1 }));
		SV_TextureArray =		celestial_pixels;
		SV_TextureArrayHeight = celestial_height;
		SV_TextureArrayWidth =	celestial_width;

		//DRAW CUBE
		DrawCube();

		//Create cube view matrix for the orbiting cube
		Matrix4x4 cubeViewMatrix = OrthogonalAffineInverse(SV_WorldMatrix);

		//ORBIT CUBE SHADER VARIABLES
		SV_TextureArray =		celestial_pixels;
		SV_TextureArrayHeight = celestial_height;
		SV_TextureArrayWidth =	celestial_width;

		SV_WorldMatrix = Matrix4x4({ .2f, 0, 0, 0 }, { 0, .2f, 0, 0 }, { 0, 0, .2f, 0 }, { .5f, -.8f, 0, 1 });
		/*SV_WorldMatrix = MatrixMULTMatrix(BuildYRotationMatrix(-static_cast<float>(-timer.TotalTime())), SV_WorldMatrix);
		SV_WorldMatrix = MatrixMULTMatrix(BuildXRotationMatrix(static_cast<float>(-timer.TotalTime()) * 2), SV_WorldMatrix);
		SV_WorldMatrix = MatrixMULTMatrix(SV_WorldMatrix, cubeViewMatrix);*/

		//Draw orbiting cube
		//DrawCube();

		//FOV checks
		if (GetAsyncKeyState(VK_SPACE) & 0x1) {
			VerticalFOV += 1;
		}
		if (GetAsyncKeyState(VK_RETURN) & 0x1) {
			VerticalFOV -= 1;
		}
		
		//CAMERA MOVEMENT
		if (GetAsyncKeyState('W') & 0x1) {
			camera = MatrixMULTMatrix(camera, BuildTranslationMatrix(0, -0.02, 0));
		}
		if (GetAsyncKeyState('S') & 0x1) {
			camera = MatrixMULTMatrix(camera, BuildTranslationMatrix(0, 0.02, 0));
		}
		if (GetAsyncKeyState('A') & 0x1) {
			//camera = MatrixMULTMatrix(camera, BuildTranslationMatrix(0.02, 0, 0));
			temp = !temp;
		}
		if (GetAsyncKeyState('D') & 0x1) {
			camera = MatrixMULTMatrix(camera, BuildTranslationMatrix(-0.02, 0, 0));
		}
		if (GetAsyncKeyState('Q') & 0x1) {
			camera = MatrixMULTMatrix(camera, BuildTranslationMatrix(0, 0, -.02));
		}
		if (GetAsyncKeyState('E') & 0x1) {
			camera = MatrixMULTMatrix(camera, BuildTranslationMatrix(0, 0, .02));
		}
	};

	//Close the program
	RS_Shutdown();
	delete[] Raster;
	delete[] DepthBuffer;

}