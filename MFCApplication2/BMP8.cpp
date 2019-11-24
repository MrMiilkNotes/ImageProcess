#include "pch.h"
#include "BMP8.h"

void BMP8::draw(int x, int y, unsigned char c)
{
	*(img_sptr.get() + width() * x + y) = c;
}

COLORREF BMP8::color_at(int x, int y) const
{
	RGBQuad tmp = *(quard.get() + *(img_sptr.get() + width() * x + y));
	return RGB(tmp.rgbRed, tmp.rgbGreen, tmp.rgbBlue);
}

unsigned char BMP8::quard_idx(int x, int y) const
{
	return *(img_sptr.get() + width() * x + y);
}
