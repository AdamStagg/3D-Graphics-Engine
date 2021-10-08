#pragma once
#include "CGSMath.h"


void (*VertexShader)(Vertex&) = 0;
void (*PixelShader)(A_PIXEL&) = 0;

Matrix4x4 SV_WorldMatrix;

void VS_World(Vertex& vert) {
	vert.values = VectorMULTMatrix(vert, SV_WorldMatrix);
}

