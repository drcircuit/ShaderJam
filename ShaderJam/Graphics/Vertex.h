#pragma once
#include <DirectXMath.h>

struct Vertex {
	Vertex() {}
	Vertex(float x, float y, float tx, float ty, float w, float h) :pos(x, y), tex(tx, ty), iResolution(w,h) {}
	DirectX::XMFLOAT2 pos;
	DirectX::XMFLOAT2 tex;
	DirectX::XMFLOAT2 iResolution;	
};