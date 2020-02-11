#include "projection.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <iostream>


Projection::Projection(unsigned short width, unsigned short height, std::string obj_file) : ReadObj(width, height, obj_file)
{
}

Projection::~Projection()
{
}

void Projection::DrawScene()
{
	parser->Parse();

	float3 eye{ 0, 0, 1 };
	float3 look_at{ 0, 0, 2 };
	float3 up{ 0, 1, 0 };

	float rot_angle = 0.0f * M_PI / 180.0;

	float3 z_axis = normalize(eye - look_at);
	float3 x_axis = normalize(cross(up, z_axis));
	float3 y_axis = cross(x_axis, z_axis);

	float aspect = static_cast<float>(width) / static_cast<float>(height);
	float fov_y = 100.0 * M_PI / 180.0;
	float y_scale = 1.0 / tan(fov_y / 2.0);
	float x_scale = y_scale / aspect;
	float z_far = 10;
	float z_near = 1;

	float4x4 view
	{
		{x_axis.x, x_axis.y, x_axis.z, -dot(x_axis, eye)},
		{y_axis.x, y_axis.y, y_axis.z, -dot(y_axis, eye)},
		{z_axis.x, z_axis.y, z_axis.z, -dot(z_axis, eye)},
		{0, 0, 0, 1},
	};

	// Rows here are actually columns in a real matrix
	float4x4 translate
	{
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, 0},
		{0, 0, 2, 1},
	};

	float4x4 projection
	{
		{x_scale, 0, 0, 0},
		{0, y_scale, 0, 0},
		{0, 0, z_far / (z_far - z_near), -z_near * z_far / (z_far - z_near)},
		{0, 0, 1, 0},
	};

	float4x4 rotate
	{
		{1, 0, 0, 0},
		{0, cos(rot_angle), sin(rot_angle), 0},
		{0, -sin(rot_angle), cos(rot_angle), 0},
		{0, 0, 0, 1},
	};

	float4x4 world = mul(translate, rotate);

	float4x4 translate_matrix = mul(projection, view, world);

	for (auto face : parser->GetFaces()) 
	{
		float4 translated[3];
		for (int i = 0; i < 3; i++) 
		{
			translated[i] = mul(translate_matrix, face.vertexes[i]);
			translated[i] /= translated[i].w;
			translated[i].x = x_center + x_center * translated[i].x;
			translated[i].y = y_center - y_center * translated[i].y;
		}
		DrawTriangle(translated, color(255, 255, 255));
	}
}

void Projection::DrawTriangle(float4 triangle[3], color color)
{
	DrawLine(static_cast<unsigned short>(triangle[0].x),
		static_cast<unsigned short>(triangle[0].y),
		static_cast<unsigned short>(triangle[1].x),
		static_cast<unsigned short>(triangle[1].y), color);

	DrawLine(static_cast<unsigned short>(triangle[1].x),
		static_cast<unsigned short>(triangle[1].y),
		static_cast<unsigned short>(triangle[2].x),
		static_cast<unsigned short>(triangle[2].y), color);

	DrawLine(static_cast<unsigned short>(triangle[2].x),
		static_cast<unsigned short>(triangle[2].y),
		static_cast<unsigned short>(triangle[0].x),
		static_cast<unsigned short>(triangle[0].y), color);
}
