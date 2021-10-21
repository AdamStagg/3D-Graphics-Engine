#include "Rasterization.h"

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


	//SET SHADER LIGHT VARIABLES
	SV_DirectionalLightPos = { -0.577f, -0.577f, 0.577f };
	SV_DirectionalLightColor = 0xFFC0C0F0;

	SV_AmbientLightPercent = 0.1f;
	
	SV_PointLightPos = { -1, 0.5, 1 };
	SV_PointLightColor = 0xFFFFFF00;

	SV_SpotLightPos = { 2, 1, -2 };
	SV_SpotLightColor = 0xFFFF0000;
	SV_SpotLightDir = Normalize({ -1, -1, 1 });
	SV_ConeRatio = cos(DegToRad(30.0f));
	SV_InnerConeRatio = .5f;
	SV_OuterConeRatio = 3;
	

	do
	{
		//Initialization
		timer.Signal();
		ClearColor(0xFF000060);
		ClearDepth();
		VertexShader = VS_PerspectiveVertexLighting;
		SV_ViewMatrix = OrthogonalAffineInverse(camera);
		SV_ProjectionMatrix = BuildProjectionMatrix(VerticalFOV, NearPlane, FarPlane, AspectRatio);

		SV_PointLightRadius = 2.5f + (2.5f * sin(timer.TotalTime()));
		SV_WorldMatrix = stoneHengeMatrix;
		//SV_WorldMatrix = Identity4x4;


		//Draw star field
		for (size_t i = 0; i < 3000; i++)
		{
			//SV_WorldMatrix = BuildScaleMatrix(0, 0, 0);
			//Draw point
			//DrawPoint(starPos[i], starPos[i].color);
		}
		
		VertexShader = VS_PerspectiveVertexLighting;
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
	

		if (GetAsyncKeyState(VK_RIGHT)) {
			for (size_t i = 0; i < StoneHengeVertexCount; i++)
			{
				camera = MatrixMULTMatrix(camera,BuildYRotationMatrix(DegToRad(1 * static_cast<float>(0.02f * timer.Delta()))));
			}
		}
		if (GetAsyncKeyState(VK_LEFT)) {
			for (size_t i = 0; i < StoneHengeVertexCount; i++)
			{
				camera = MatrixMULTMatrix(camera,BuildYRotationMatrix(DegToRad(-1 * static_cast<float>(0.02f * timer.Delta()))));
			}
		}



	} while (RS_Update(Raster, RasterPixelCount));

	//Close the program
	RS_Shutdown();
	delete[] Raster;
	delete[] DepthBuffer;
	delete[] starPos;

}


