#pragma once
#include "CGSMath.h"


void (*VertexShader)(Vertex&) = 0;
void (*PixelShader)(A_PIXEL&) = 0;

Matrix3x3 SV_WorldMatrix;

void VS_World(Vertex& vert) {
	vert = VectorMULTMatrix(vert, SV_WorldMatrix);
}

void PS_SetColor(A_PIXEL& color) {
	color = color;
}
