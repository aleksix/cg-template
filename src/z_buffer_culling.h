#pragma once


#include "triangle_rasterization.h"


class ZCulling: public TriangleRasterization
{
public:
	ZCulling(unsigned short width, unsigned short height, std::string obj_file);
	virtual ~ZCulling();

	void Clear();

protected:
	virtual void DrawTriangle(float4 triangle[3], color col);
	void SetPixel(unsigned short x, unsigned short y, color color, float depth);
	std::vector<float> depth_buffer;
};
