#pragma once
#include "BMPdib.h"
class BMP24 :
	public BMPdib
{
public:
	void draw(int, int, unsigned char);
	COLORREF color_at(int, int) const;
	unsigned char quard_idx(int, int) const;
	void write(const CString& filename) const {}
private:
	void read(const CString&);
};

