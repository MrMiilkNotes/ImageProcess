#include "pch.h"
#include "BMP1.h"

void BMP1::draw(int x, int y, unsigned char c)
{
	throw std::runtime_error("no implement");
}

COLORREF BMP1::color_at(int x, int y) const
{
	auto tmp_ = img_sptr.get() + (width() + 31) / 32 * 4 * x + y / 8;
	auto tmp = *(quard.get() + ((*tmp_ & (0x01 << (7 - y % 8))) >> (7 - y % 8)));
	return RGB(tmp.rgbRed, tmp.rgbGreen, tmp.rgbBlue);
}

unsigned char BMP1::quard_idx(int x, int y) const
{
	auto tmp_ = img_sptr.get() + (width() + 31) / 32 * 4 * x + y / 8;
	return (*tmp_ & ((0x01 << (7 - y % 8))) >> (7 - y % 8)) & 0x01;
}