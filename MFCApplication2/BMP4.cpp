#include "pch.h"
#include "BMP4.h"


void BMP4::draw(int x, int y, unsigned char c)
{
	// *(img_sptr.get() + width() * x + y) = c;
	throw std::runtime_error("no implement");
}

COLORREF BMP4::color_at(int x, int y) const
{
	auto tmp_ = img_sptr.get() + width() * x / 2 + y / 2;
	auto tmp = *(quard.get() + (y % 2 ? (*tmp_ & 0x0f) : (*tmp_ >> 4)));
	return RGB(tmp.rgbRed, tmp.rgbGreen, tmp.rgbBlue);
}

unsigned char BMP4::quard_idx(int x, int y) const
{
	auto tmp = img_sptr.get() + width() * x / 2 + y / 2;
	return y % 2 ? (*tmp & 0x0f) : (*tmp >> 4);
}
