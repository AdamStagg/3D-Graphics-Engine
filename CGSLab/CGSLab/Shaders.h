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
	vert.x = vert.x / vert.w;
	vert.y = vert.y / vert.w;
	vert.z = vert.z / vert.w;
	vert.w = vert.w;
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

void PS_Bilinear(A_PIXEL& color, float _u, float _v, float _z) {

	float u = _u * SV_TextureArrayWidth;
	float v = _v * SV_TextureArrayHeight;

	int uIndex = static_cast<int>(u);
	int vIndex = static_cast<int>(v);

	float uRatio = u - uIndex;
	float vRatio = v - vIndex;

	int mipLevel = ((_z - NearPlane) / (FarPlane - NearPlane)) * celestial_numlevels;

	mipLevel = 4;
	int offset = celestial_leveloffsets[mipLevel];
	int modifiedWidth = SV_TextureArrayWidth >> mipLevel;
	//uIndex = uIndex >> mipLevel;
	//vIndex = vIndex >> mipLevel;

	unsigned int topColor = colorLerp(
		BGRAtoARGB(SV_TextureArray[offset + ConvertDimension(uIndex, vIndex, modifiedWidth)]),
		BGRAtoARGB(SV_TextureArray[offset + ConvertDimension(uIndex + 1, vIndex, modifiedWidth)]),
		uRatio);
	unsigned int bottomColor = colorLerp(
		BGRAtoARGB(SV_TextureArray[offset + ConvertDimension(uIndex, (vIndex + 1), modifiedWidth)]),
		BGRAtoARGB(SV_TextureArray[offset + ConvertDimension(uIndex + 1, (vIndex + 1), modifiedWidth)]),
		uRatio);


	color = colorLerp(topColor, bottomColor, vRatio);
}
