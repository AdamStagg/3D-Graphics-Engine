#include "Rasterization.h"

int main() {

	//Create the camera
	camera = MatrixMULTMatrix(
		Matrix4x4({ 1, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 0, 1, 0 }, { 0, 0, -1, 1 }),
		BuildXRotationMatrix(DegToRad(-18))
	);

	bool isBilinear = true;
	bool backFaceCull = false;

	unsigned int temp = colorLerp(0x5b7fadff, 0xff4e839c);

	Vertex* stoneHengeVerteces = GenerateStonehengeVertexes();
	//Clear buffers
	ClearColor(0);
	ClearDepth();

	//Initialization
	RS_Initialize(RasterWidth, RasterHeight);

	Vertex* starPos = new Vertex[3000];
	Vertex starPosOrigin = { 1, 1, 1, 1, 0, 0, 0xFFFFFFFF, {} };

	for (size_t i = 0; i < 3000; i++)
	{
		starPos[i] = { 
			50 * (((float)(rand() % 201) / 100.0f) - 1), 
			50 * (((float)(rand() % 201) / 100.0f) - 1), 
			50 * (((float)(rand() % 201) / 100.0f) - 1),
			0, 
			0, 
			0, 
			0xFFFFFFFF, 
			{} };
	}

	while (RS_Update(Raster, RasterPixelCount))
	{
		//Initialization
		timer.Signal();
		ClearColor(0xFF1010FF);
		ClearDepth();
		VertexShader = VS_PerspectiveCamera;
		SV_ViewMatrix = OrthogonalAffineInverse(camera);
		SV_ProjectionMatrix = BuildProjectionMatrix(VerticalFOV, NearPlane, FarPlane, AspectRatio);

		SV_WorldMatrix = Identity4x4;

		//Draw star field
		for (size_t i = 0; i < 3000; i++)
		{
			//Draw point
			DrawPoint(starPos[i], starPos[i].color);
		}

		PixelShader = PS_Bilinear;
		SV_TextureArray = StoneHenge_pixels;
		SV_TextureArrayWidth = StoneHenge_width;
		SV_TextureArrayHeight = StoneHenge_height;
		SV_WorldMatrix = BuildScaleMatrix(0.1f, 0.1f, 0.1f);

		//Draw the stonehenge model
		for (size_t i = 0; i < StoneHengeVertexCount; i+=3)
		{
			//DrawTriangle(stoneHengeVerteces[i], stoneHengeVerteces[i + 1], stoneHengeVerteces[i + 2]);
		}



		if (GetAsyncKeyState(VK_UP)) {
			camera = MatrixMULTMatrix(BuildXRotationMatrix(1 * static_cast<float>(timer.Delta())), camera);
		}
		if (GetAsyncKeyState(VK_DOWN)) {
			camera = MatrixMULTMatrix(BuildXRotationMatrix(-1 * static_cast<float>(timer.Delta())), camera);
		}
		if (GetAsyncKeyState(VK_RIGHT)) {
			camera = MatrixMULTMatrix(BuildYRotationMatrix(-1 * static_cast<float>(timer.Delta())), camera);
		}
		if (GetAsyncKeyState(VK_LEFT)) {
			camera = MatrixMULTMatrix(BuildYRotationMatrix(1 * static_cast<float>(timer.Delta())), camera);
		}


	};

	//Close the program
	RS_Shutdown();
	delete[] Raster;
	delete[] DepthBuffer;
	delete[] starPos;

}