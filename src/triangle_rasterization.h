#pragma once


#include "projection.h"


class TriangleRasterization: public Projection
{
public:
	TriangleRasterization(unsigned short width, unsigned short height, std::string obj_file);
	virtual ~TriangleRasterization();

protected:
	virtual void DrawTriangle(float4 traingle[3], color col);

	float EdgeFunction(float2 a, float2 b, float2 c);
};
