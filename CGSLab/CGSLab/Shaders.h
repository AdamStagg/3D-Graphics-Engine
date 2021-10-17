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

void PS_Bilinear(A_PIXEL& color, float u, float v, float _z) {

	int mipLevel = ((_z - NearPlane) / (FarPlane - NearPlane)) * celestial_numlevels;
	
	if (mipLevel >= 10) mipLevel = 9;
	int modifiedWidth = SV_TextureArrayWidth >> mipLevel;
	int modifiedHeight = SV_TextureArrayHeight >> mipLevel;
	int offset = celestial_leveloffsets[mipLevel] - 1 - modifiedWidth;


	float uPixel = u * modifiedWidth;
	float vPixel = v * modifiedHeight;

	int uIndex = static_cast<int>(uPixel);
	int vIndex = static_cast<int>(vPixel);

	float uRatio = (uPixel) - static_cast<float>(uIndex);
	float vRatio = (vPixel) - static_cast<float>(vIndex);

	unsigned int topColor = colorLerp(
		BGRAtoARGB(SV_TextureArray[offset + ConvertDimension(uIndex, vIndex,		modifiedWidth)]),
		BGRAtoARGB(SV_TextureArray[offset + ConvertDimension(uIndex + 1, vIndex,	modifiedWidth)]),
		uRatio);
	unsigned int bottomColor = colorLerp(
		BGRAtoARGB(SV_TextureArray[offset + ConvertDimension(uIndex,  vIndex+ 1,		modifiedWidth)]),
		BGRAtoARGB(SV_TextureArray[offset + ConvertDimension(uIndex + 1, vIndex + 1, modifiedWidth)]),
		uRatio);


	color = colorLerp(topColor, bottomColor, vRatio);
}

void PS_Trilinear(A_PIXEL& color, float u, float v, float _z) {
	float mipLevel = ((_z - NearPlane) / (FarPlane - NearPlane)) * celestial_numlevels;
	
	if (mipLevel >= 10) return;
	int modifiedWidth = SV_TextureArrayWidth >> static_cast<int>(mipLevel);
	int modifiedHeight = SV_TextureArrayHeight >> static_cast<int>(mipLevel);
	int offset = celestial_leveloffsets[static_cast<int>(mipLevel)] - 1 - modifiedWidth;


	int uIndex = static_cast<int>(u * modifiedWidth);
	int vIndex = static_cast<int>(v * modifiedHeight);

	float uRatio = (u * modifiedWidth) - static_cast<float>(uIndex);
	float vRatio = (v * modifiedHeight) - static_cast<float>(vIndex);

	unsigned int topColor1 = colorLerp(
		BGRAtoARGB(SV_TextureArray[offset + ConvertDimension(uIndex, vIndex, modifiedWidth)]),
		BGRAtoARGB(SV_TextureArray[offset + ConvertDimension(uIndex + 1, vIndex, modifiedWidth)]),
		uRatio);
	unsigned int bottomColor1 = colorLerp(
		BGRAtoARGB(SV_TextureArray[offset + ConvertDimension(uIndex, vIndex + 1, modifiedWidth)]),
		BGRAtoARGB(SV_TextureArray[offset + ConvertDimension(uIndex + 1, vIndex + 1, modifiedWidth)]),
		uRatio);



	unsigned int firstMipColor = colorLerp(topColor1, bottomColor1, vRatio);

	if (mipLevel >= 9) {
		color = firstMipColor;
	}
	else {
		mipLevel++;
		modifiedWidth = SV_TextureArrayWidth >> static_cast<int>(mipLevel);
		modifiedHeight = SV_TextureArrayHeight >> static_cast<int>(mipLevel);
		offset = celestial_leveloffsets[static_cast<int>(mipLevel)] - 1 - modifiedWidth;


		uIndex = static_cast<int>(u * modifiedWidth);
		vIndex = static_cast<int>(v * modifiedHeight);

		uRatio = (u * modifiedWidth) - static_cast<float>(uIndex);
		vRatio = (v * modifiedHeight) - static_cast<float>(vIndex);

		unsigned int topColor2 = colorLerp(
			BGRAtoARGB(SV_TextureArray[offset + ConvertDimension(uIndex, vIndex, modifiedWidth)]),
			BGRAtoARGB(SV_TextureArray[offset + ConvertDimension(uIndex + 1, vIndex, modifiedWidth)]),
			uRatio);
		unsigned int bottomColor2 = colorLerp(
			BGRAtoARGB(SV_TextureArray[offset + ConvertDimension(uIndex, vIndex + 1, modifiedWidth)]),
			BGRAtoARGB(SV_TextureArray[offset + ConvertDimension(uIndex + 1, vIndex + 1, modifiedWidth)]),
			uRatio);


		unsigned int secondMipColor = colorLerp(topColor2, bottomColor2, vRatio);

		color = colorLerp(firstMipColor, secondMipColor, mipLevel - floor(mipLevel));
	}
}