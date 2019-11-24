#pragma once
#include "BMPdibWithQuard.h"
class BMP4 :
	public BMPdibWithQuard
{
public:
	void draw(int, int, unsigned char);
	COLORREF color_at(int, int) const;
	unsigned char quard_idx(int, int) const;

};

