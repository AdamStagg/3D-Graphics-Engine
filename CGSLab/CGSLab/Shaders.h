#pragma once
#include "CGSMath.h"


void (*VertexShader)(Vertex&) = 0;
void (*PixelShader)(A_PIXEL&) = 0;

Matrix4x4 SV_WorldMatrix;

void VS_World(Vertex& vert) {
	vert.values = VectorMULTMatrix(vert.values, SV_WorldMatrix);
}

void VS_PerspectiveCamera(Vertex& vert) {
	Matrix4x4 viewMatrix = OrthogonalAffineInverse(*camera);
	Matrix4x4 projectionMatrix = BuildProjectionMatrix(VerticalFOV, NearPlane, FarPlane, AspectRatio);

	Matrix4x4 finalMatrix = MatrixMULTMatrix(SV_WorldMatrix, viewMatrix);
	finalMatrix = MatrixMULTMatrix(finalMatrix, projectionMatrix);

	vert.values = VectorMULTMatrix(vert.values, finalMatrix);
	vert.x = vert.x / vert.w;
	vert.y = vert.y / vert.w;
	vert.z = vert.z / vert.w;
	vert.w = 1;
}

