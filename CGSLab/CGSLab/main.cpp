#include "Rasterization.h"
#include "main.h"

int main() {
	//Initialization
	RS_Initialize(RasterWidth, RasterHeight);

	//Create the camera
	camera = MatrixMULTMatrix(
		Matrix4x4({ 1, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 0, 1, 0 }, { 0, 0, -4, 1 }),
		BuildXRotationMatrix(DegToRad(-18))
	);

	//VERTEX DATA
	Vertex* stoneHengeVerteces = GenerateStonehengeVertexes();
	Matrix4x4 stoneHengeMatrix = Identity4x4; //used for world matrix for rotation


	//STAR DATA
	Vertex* starPos = new Vertex[3000];
	Vertex starPosOrigin = { 1, 1, 1, 1, 0, 0, 0xFFFFFFFF, {} };
	GenerateStars(starPos);


	//LIGHT DATA
	Vector3 directionalLightDir = { -0.577f, -0.577f, 0.577f };
	unsigned int directionalLightColor = 0xFFC0C0F0;

	Vector3 pointLightPos = { -1, 0.5, 1 };
	unsigned int pointLightColor = 0xFFFFFF00;


	//SET SHADER LIGHT VARIABLES
	SV_DirectionalLightPos = directionalLightDir;
	SV_AmbientLightPercent = 0.3f;
	SV_DirectionalLightColor = directionalLightColor;
	SV_PointLightPos = pointLightPos;
	SV_PointLightColor = pointLightColor;



	do
	{
		//Initialization
		timer.Signal();
		ClearColor(0xFF000060);
		ClearDepth();
		VertexShader = VS_PerspectiveVertexLighting;
		SV_ViewMatrix = OrthogonalAffineInverse(camera);
		SV_ProjectionMatrix = BuildProjectionMatrix(VerticalFOV, NearPlane, FarPlane, AspectRatio);

		//SV_WorldMatrix = Identity4x4;

		//Draw star field
		for (size_t i = 0; i < 3000; i++)
		{
			//SV_WorldMatrix = BuildScaleMatrix(0, 0, 0);
			//Draw point
			DrawPoint(starPos[i], starPos[i].color);
		}
		
		SV_WorldMatrix = stoneHengeMatrix;
		PixelShader = PS_NearestLight;
		SV_TextureArray = StoneHenge_pixels;
		SV_TextureArrayWidth = StoneHenge_width;
		SV_TextureArrayHeight = StoneHenge_height;
		//SV_WorldMatrix = BuildScaleMatrix(0.1f, 0.1f, 0.1f);

		//Draw the stonehenge model
		for (size_t i = 0; i < StoneHengeIndexCount; i += 3)
		{
			//Parametric(stoneHengeVerteces[StoneHenge_indicies[i]],		stoneHengeVerteces[StoneHenge_indicies[i + 1]], 0);
			//Parametric(stoneHengeVerteces[StoneHenge_indicies[i+1]],	stoneHengeVerteces[StoneHenge_indicies[i + 2]], 0);
			//Parametric(stoneHengeVerteces[StoneHenge_indicies[i]],		stoneHengeVerteces[StoneHenge_indicies[i + 2]], 0);
			DrawTriangle(stoneHengeVerteces[StoneHenge_indicies[i]], stoneHengeVerteces[StoneHenge_indicies[i + 1]], stoneHengeVerteces[StoneHenge_indicies[i + 2]]);
		}



		if (GetAsyncKeyState(VK_UP)) {
			for (size_t i = 0; i < StoneHengeVertexCount; i++)
			{
				stoneHengeMatrix = MatrixMULTMatrix(stoneHengeMatrix, BuildXRotationMatrix(DegToRad(1 * static_cast<float>(0.02f * timer.Delta()))));

			}
		}
		if (GetAsyncKeyState(VK_DOWN)) {
			for (size_t i = 0; i < StoneHengeVertexCount; i++)
			{
				stoneHengeMatrix = MatrixMULTMatrix(stoneHengeMatrix, BuildXRotationMatrix(DegToRad(-1 * static_cast<float>(0.02f * timer.Delta()))));
			}
		}
		if (GetAsyncKeyState(VK_RIGHT)) {
			for (size_t i = 0; i < StoneHengeVertexCount; i++)
			{
				stoneHengeMatrix = MatrixMULTMatrix(stoneHengeMatrix, BuildYRotationMatrix(DegToRad(1 * static_cast<float>(0.02f * timer.Delta()))));
			}
		}
		if (GetAsyncKeyState(VK_LEFT)) {
			for (size_t i = 0; i < StoneHengeVertexCount; i++)
			{
				stoneHengeMatrix = MatrixMULTMatrix(stoneHengeMatrix, BuildYRotationMatrix(DegToRad(-1 * static_cast<float>(0.02f * timer.Delta()))));
			}
		}



	} while (RS_Update(Raster, RasterPixelCount));

	//Close the program
	RS_Shutdown();
	delete[] Raster;
	delete[] DepthBuffer;
	delete[] starPos;

}


