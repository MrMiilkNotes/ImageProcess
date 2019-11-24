#include "pch.h"
#include "BMP24.h"

void BMP24::draw(int x, int y, unsigned char c)
{
	// *(img_sptr.get() + width() * x + y) = c;
	throw std::runtime_error("no implement");
}

COLORREF BMP24::color_at(int x, int y) const
{
	auto tmp_ = img_sptr.get() + width() * x * 3 + y * 3;
	return RGB(*(tmp_+2), *(tmp_ + 1), *tmp_);
}

unsigned char BMP24::quard_idx(int x, int y) const
{
	throw std::runtime_error("bmp24 has no Quard");
}

void BMP24::read(const CString& filename)
{
	CFile file(filename, CFile::modeRead);
	file.Read(&m_BMPFileHeader, sizeof(BMPFileHeader));
	file.Read(&m_BMPInfoHeader, sizeof(BMPInfoHeader));
	// m_width = m_BMPInfoHeader.biWidth;
	// m_height = m_BMPInfoHeader.biHeight;
	biBitCount = m_BMPInfoHeader.biBitCount;
	m_channels = 1;
	int lineBytes = (biBitCount * m_BMPInfoHeader.biWidth + 31) / 32 * 4;
	read_bmp_(file, lineBytes);
}
