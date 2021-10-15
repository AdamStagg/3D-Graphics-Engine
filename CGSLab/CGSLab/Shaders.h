#pragma once
#include "CGSMath.h"


void (*VertexShader)(Vertex&) = 0;
void (*PixelShader)(A_PIXEL&, float, float, float) = 0;

Matrix4x4 SV_WorldMatrix;
unsigned int* SV_TextureArray;
unsigned int SV_TextureArrayWidth = 0;
unsigned int SV_TextureArrayHeight = 0;

void VS_World(Vertex& vert) {
	vert.values = VectorMULTMatrix(vert.values, SV_WorldMatrix);
}

void VS_PerspectiveCamera(Vertex& vert) {
	Matrix4x4 viewMatrix = OrthogonalAffineInverse(camera);
	Matrix4x4 projectionMatrix = BuildProjectionMatrix(VerticalFOV, NearPlane, FarPlane, AspectRatio);

	Matrix4x4 finalMatrix = MatrixMULTMatrix(SV_WorldMatrix, viewMatrix);
	finalMatrix = MatrixMULTMatrix(finalMatrix, projectionMatrix);

	vert.values = VectorMULTMatrix(vert.values, finalMatrix);
	//vert.x = vert.x / vert.w;
	//vert.y = vert.y / vert.w;
	//vert.z = vert.z / vert.w;
	//vert.w = vert.w;
}

void PS_SetColor(A_PIXEL& color) {
	color = color;
}

void PS_Nearest(A_PIXEL& color, float u, float v, float z = 0) {
	color = BGRAtoARGB(SV_TextureArray[
		(static_cast<int>(u * (SV_TextureArrayWidth))) +
			(static_cast<int>(v * (SV_TextureArrayHeight)) * SV_TextureArrayWidth)
	]);
}

void PS_Bilinear(A_PIXEL& color, float uRatio, float vRatio, float _z) {

	int mipLevel = ((_z - NearPlane) / (FarPlane - NearPlane)) * celestial_numlevels;
	if (mipLevel >= 10) return;
	int offset = celestial_leveloffsets[mipLevel];
	int modifiedWidth = SV_TextureArrayWidth >> mipLevel;
	int modifiedHeight = SV_TextureArrayHeight >> mipLevel;

	float modifiedU = uRatio * modifiedWidth;
	float modifiedV = vRatio * modifiedHeight;

	int uIndex = static_cast<int>(modifiedU);
	int vIndex = static_cast<int>(modifiedV);

	unsigned int topColor = colorLerp(
		BGRAtoARGB(SV_TextureArray[offset + ConvertDimension(modifiedU, modifiedV, modifiedWidth)]),
		BGRAtoARGB(SV_TextureArray[offset + ConvertDimension(modifiedU + 1, modifiedV, modifiedWidth)]),
		uRatio);
	unsigned int bottomColor = colorLerp(
		BGRAtoARGB(SV_TextureArray[offset + ConvertDimension(modifiedU, modifiedV + 1, modifiedWidth)]),
		BGRAtoARGB(SV_TextureArray[offset + ConvertDimension(modifiedU + 1, modifiedV + 1, modifiedWidth)]),
		vRatio);


	color = colorLerp(topColor, bottomColor, vRatio);
}

void PS_Trilinear(A_PIXEL& color, float uRatio, float vRatio, float _z) {
	float mipLevel = ((_z - NearPlane) / (FarPlane - NearPlane)) * celestial_numlevels;
	if (mipLevel >= 10) return;
	int offset = celestial_leveloffsets[static_cast<int>(mipLevel)];
	int modifiedWidth = SV_TextureArrayWidth >> static_cast<int>(mipLevel);
	int modifiedHeight = SV_TextureArrayHeight >> static_cast<int>(mipLevel);

	float modifiedU = uRatio * modifiedWidth;
	float modifiedV = vRatio * modifiedHeight;

	int uIndex = static_cast<int>(modifiedU);
	int vIndex = static_cast<int>(modifiedV);

	unsigned int topColor1 = colorLerp(
		BGRAtoARGB(SV_TextureArray[offset + ConvertDimension(modifiedU, modifiedV, modifiedWidth)]),
		BGRAtoARGB(SV_TextureArray[offset + ConvertDimension(modifiedU + 1, modifiedV, modifiedWidth)]),
		uRatio);
	unsigned int bottomColor2 = colorLerp(
		BGRAtoARGB(SV_TextureArray[offset + ConvertDimension(modifiedU, modifiedV + 1, modifiedWidth)]),
		BGRAtoARGB(SV_TextureArray[offset + ConvertDimension(modifiedU + 1, modifiedV + 1, modifiedWidth)]),
		vRatio);


	unsigned int firstMipColor = colorLerp(topColor1, bottomColor2, vRatio);

	if (mipLevel >= 9) {
		color = firstMipColor;
	}
	else {
		mipLevel++;
		offset = celestial_leveloffsets[static_cast<int>(mipLevel)];
		modifiedWidth = SV_TextureArrayWidth >> static_cast<int>(mipLevel);
		modifiedHeight = SV_TextureArrayHeight >> static_cast<int>(mipLevel);

		modifiedU = uRatio * modifiedWidth;
		modifiedV = vRatio * modifiedHeight;

		uIndex = static_cast<int>(modifiedU);
		vIndex = static_cast<int>(modifiedV);

		unsigned int topColor2 = colorLerp(
			BGRAtoARGB(SV_TextureArray[offset + ConvertDimension(modifiedU, modifiedV, modifiedWidth)]),
			BGRAtoARGB(SV_TextureArray[offset + ConvertDimension(modifiedU + 1, modifiedV, modifiedWidth)]),
			uRatio);
		unsigned int bottomColor2 = colorLerp(
			BGRAtoARGB(SV_TextureArray[offset + ConvertDimension(modifiedU, modifiedV + 1, modifiedWidth)]),
			BGRAtoARGB(SV_TextureArray[offset + ConvertDimension(modifiedU + 1, modifiedV + 1, modifiedWidth)]),
			vRatio);

		unsigned int secondMipColor = colorLerp(topColor2, bottomColor2, vRatio);

		color = colorLerp(firstMipColor, secondMipColor, mipLevel - floor(mipLevel));
	}
}