#include "z_buffer_culling.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <iostream>


ZCulling::ZCulling(unsigned short width, unsigned short height, std::string obj_file) : TriangleRasterization(width, height, obj_file)
{
	depth_buffer.reserve(width * height);
}

ZCulling::~ZCulling()
{
}

void ZCulling::Clear()
{
	TriangleRasterization::Clear();
	depth_buffer.resize(width * height, std::numeric_limits<float>::lowest());
}


void ZCulling::DrawTriangle(float4 triangle[3], color col)
{
	float2 bb_begin = max(float2{ 0,0 }, min(min(triangle[0].xy(), triangle[1].xy()), triangle[2].xy()));
	float2 bb_end = min(float2{ static_cast<float>(width - 1), static_cast<float>(height - 1) }, max(max(triangle[0].xy(), triangle[1].xy()), triangle[2].xy()));

	float area = EdgeFunction(triangle[0].xy(), triangle[1].xy(), triangle[2].xy());
	bool drawn = false;

	for (float x = bb_begin.x; x <= bb_end.x; x++) 
	{
		for (float y = bb_begin.y; y <= bb_end.y; y++) 
		{
			float area0 = EdgeFunction(triangle[2].xy(), triangle[0].xy(), float2{ x, y });
			float area1 = EdgeFunction(triangle[1].xy(), triangle[2].xy(), float2{ x, y });
			float area2 = EdgeFunction(triangle[0].xy(), triangle[1].xy(), float2{ x, y });
			if (area0 >= 0 && area1 >= 0 && area2 >= 0)
			{
				drawn = true;
				float u = area0 / area;
				float v = area1 / area;
				float w = area2 / area;
				float z = u * (-triangle[0].z) + v * (-triangle[1].z) + w * (-triangle[2].z);
				SetPixel(x, y, color(255 * u, 255 * v, 255 * w), z);
			}
		}
	}

	if (drawn)
	{
		Projection::DrawTriangle(triangle, col);
	}
}	

void ZCulling::SetPixel(unsigned short x, unsigned short y, color color, float depth)
{
	if (depth_buffer[y * width + x] < depth)
	{
		depth_buffer[y * width + x] = depth;
		TriangleRasterization::SetPixel(x, y, color);
	}
}