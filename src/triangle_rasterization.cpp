#include "triangle_rasterization.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <iostream>


TriangleRasterization::TriangleRasterization(unsigned short width, unsigned short height, std::string obj_file) : Projection(width, height, obj_file)
{
}

TriangleRasterization::~TriangleRasterization()
{
}

void TriangleRasterization::DrawTriangle(float4 triangle[3], color col)
{
	float2 bb_begin = max(float2{0, 0}, min(min(triangle[0].xy(), triangle[1].xy()), triangle[2].xy()));
	float2 bb_end = min(float2{static_cast<float>(width - 1), static_cast<float>(height - 1)}, 
		max(max(triangle[0].xy(), triangle[1].xy()), triangle[2].xy()));

	float area = EdgeFunction(triangle[0].xy(), triangle[1].xy(), triangle[2].xy());
	bool drawn = false;

	for (float x = bb_begin.x; x <= bb_end.x; x++) 
	{
		for (float y = bb_begin.y; y <= bb_end.y; y++) 
		{
			float area0 = EdgeFunction(triangle[0].xy(), triangle[1].xy(), float2{x, y});
			float area1 = EdgeFunction(triangle[1].xy(), triangle[2].xy(), float2{x, y});
			float area2 = EdgeFunction(triangle[2].xy(), triangle[0].xy(), float2{x, y});
			if (area0 >= 0.0 && area1 >= 0.0 && area2 >= 0.0) 
			{
				SetPixel(x, y, color(255, 0, 0));
				drawn = true;
			}
		}
	}

	if (drawn)
	{
		Projection::DrawTriangle(triangle, col);
	}
}

float TriangleRasterization::EdgeFunction(float2 a, float2 b, float2 c)
{
	return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
}



// Barycentric:
/*
S(parallelogram) = Edge(A, B, C)
S(triangle) = S(parallelogram) / 2
S(triangle) = Edge(A, B, P) / 2
S(ABP) / S(ABC) = Edge(A, B, P) / Edge (A, B, C)
*/