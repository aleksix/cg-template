#include "color_space.h"
#include <iostream>


ColorSpace::ColorSpace(unsigned short width, unsigned short height): BlackImage(width, height)
{

}

ColorSpace::~ColorSpace()
{
}

// Color_space.png test is a bit too precise? Starting from x=143, y=0, every 15 pixels the G element of the image is off by 1 on the result
void ColorSpace::DrawScene()
{
	for (unsigned short x = 0; x < width; ++x)
	{
		for (unsigned short y = 0; y < height; ++y)
		{
			SetPixel(x, y, color(
				static_cast<unsigned char>((y * 255) / height),
				static_cast<unsigned char>((x * 255) / width),
				0));
		}
	}
}

void ColorSpace::SetPixel(unsigned short x, unsigned short y, color color)
{
	if(y < height && y >= 0 && x < width && x >= 0)
		frame_buffer[y * width + x] = color;
}

