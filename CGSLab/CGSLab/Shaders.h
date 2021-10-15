#pragma once
#include "CGSMath.h"


void (*VertexShader)(Vertex&) = 0;
void (*PixelShader)(A_PIXEL&, float, float) = 0;

Matrix4x4 SV_WorldMatrix;
unsigned int *SV_TextureArray;
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

void PS_Texture(A_PIXEL& color, float u, float v) {
	color = BGRAtoARGB(SV_TextureArray[
			(static_cast<int>(u * (SV_TextureArrayWidth))) +
			(static_cast<int>(v * (SV_TextureArrayHeight)) * SV_TextureArrayWidth)
	]);
}

void PS_Bilinear(A_PIXEL& color, float _u, float _v) {

	float u = _u * SV_TextureArrayWidth;
	float v = _v * SV_TextureArrayHeight;

	int uIndex = static_cast<int>(u);
	int vIndex = static_cast<int>(v);

	float uRatio = u - uIndex;
	float vRatio = v - vIndex;

	unsigned int topColor =		colorLerp(		BGRAtoARGB(SV_TextureArray[uIndex + vIndex * SV_TextureArrayWidth]),		BGRAtoARGB(SV_TextureArray[uIndex + 1 + vIndex * SV_TextureArrayWidth]),		uRatio);
	unsigned int bottomColor =	colorLerp(		BGRAtoARGB(SV_TextureArray[uIndex + (vIndex + 1) * SV_TextureArrayWidth]),	BGRAtoARGB(SV_TextureArray[uIndex + 1 + (vIndex + 1) * SV_TextureArrayWidth]),	uRatio);

	color = colorLerp(topColor, bottomColor, vRatio);
}
