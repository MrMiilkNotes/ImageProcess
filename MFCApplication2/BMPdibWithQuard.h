#pragma once
#include "BMPdib.h"
class BMPdibWithQuard :
	public BMPdib
{
public:
	virtual unsigned char quard_idx(int, int) const = 0;
	void write(const CString& filename) const;
protected:
	std::shared_ptr<RGBQuad> quard;
	void read(const CString&);
};

