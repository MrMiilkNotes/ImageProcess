#include "pch.h"
#include "MyDIB.h"


DIB1::~DIB1() {

}

void DIB1::Read(const CString& fileName) {
	CFile file;
	VERIFY(file.Open(fileName, CFile::modeRead));
	// 读取 BMPFileHeader
	file.Read(&m_BMPFileHeader, sizeof(BMPFileHeader));
	// 读取 BMPInfoHeader
	//ULONGLONG posi = file.Seek(sizeof(BMPFileHeader), CFile::begin);
	file.Read(&m_BMPInfoHeader, sizeof(BMPInfoHeader));
	//posi = file.Seek(sizeof(BMPInfoHeader) + posi, CFile::begin);
	// read information
	m_width = m_BMPInfoHeader.biWidth;
	m_height = m_BMPInfoHeader.biHeight;
	real_size = m_BMPInfoHeader.biSize;
	//计算图像每行像素所占的字节数（必须是4的倍数）
	biBitCount = m_BMPInfoHeader.biBitCount;
	//int lineByte = (m_width + 3) / 4 * 4;
	int lineByte = (biBitCount * m_width + 31) / 32 * 4;

	if (biBitCount == 8) {
		// 256色
		m_channels = 1;
		// 读取调色板
		Quard = (RGBQuad*)malloc(sizeof(RGBQuad) * 256);
		file.Read(Quard, 256 * sizeof(RGBQuad));
		//posi = file.Seek(posi + 256 * sizeof(RGBQuad), CFile::begin);

		_colorAt = std::bind(&DIB1::_color1_, this, std::placeholders::_1, std::placeholders::_2);
		idxAt = std::bind(&DIB1::_idx1_, this, std::placeholders::_1, std::placeholders::_2);
		drawAt = std::bind(&DIB1::_draw1, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		//读取图片内容
		_read_bmp_(file, lineByte);
	}
	else if (biBitCount == 4) {
		m_channels = 1;
		Quard = (RGBQuad*)malloc(sizeof(RGBQuad) * (1 << biBitCount));
		file.Read(Quard, sizeof(RGBQuad) * (1 << biBitCount));
		//posi = file.Seek(posi + sizeof(RGBQuad) * (long)(1 << biBitCount), CFile::begin);

		_colorAt = std::bind(&DIB1::_color4_, this, std::placeholders::_1, std::placeholders::_2);
		//读取图片内容
		_read_bmp_(file, lineByte);
	}
	else if (biBitCount == 1) {
		m_channels = 1;
		Quard = (RGBQuad*)malloc(sizeof(RGBQuad) * (1 << biBitCount));
		file.Read(Quard, sizeof(RGBQuad) * (1 << biBitCount));
		//posi = file.Seek(posi + sizeof(RGBQuad) * (long)(1 << biBitCount), CFile::begin);
		//file.Seek(-1, CFile::current);
		_colorAt = std::bind(&DIB1::_color5_, this, std::placeholders::_1, std::placeholders::_2);
		//读取图片内容
		_read_bmp_(file, lineByte);
	}
	else if (biBitCount == 24) {
		// 24位真彩色
		m_channels = 3;
		Quard = nullptr;

		//_colorAt = _color3_;
		_colorAt = std::bind(&DIB1::_color3_, this, std::placeholders::_1, std::placeholders::_2);
		//读取图片内容
		_read_bmp_(file, lineByte);
	}
	file.Close();
}

bool DIB1::Write(const CString& fileName) {
	CFile file;
	
	VERIFY(file.Open(fileName, CFile::modeCreate | CFile::modeWrite));

	try {
		UINT lineByte = (m_BMPInfoHeader.biBitCount * m_width + 31) / 32 * 4;
		unsigned char* dataBuff = (unsigned char*)malloc(m_height * lineByte);
		for (int index = 0; index < m_height; index++)
			memcpy(dataBuff + (m_height - 1 - index) * lineByte, m_BMPdata + index * lineByte, lineByte);
		file.Write(&m_BMPFileHeader, sizeof(m_BMPFileHeader));
		file.Write(&m_BMPInfoHeader, sizeof(m_BMPInfoHeader));
		if(Quard != nullptr) file.Write(Quard, sizeof(RGBQuad) * (1 << m_BMPInfoHeader.biBitCount));
		file.Write(dataBuff, m_height * lineByte);
		free(dataBuff);
	}
	catch (CException * pe) {
		pe->Delete();
		AfxMessageBox(_T("write error"));
		file.Close();
		return false;
	}
	file.Close();
	return true;
}

void DIB1::_read_bmp_(CFile& file, int lineByte) {
	m_BMPdata = (unsigned char*)malloc(lineByte * m_height);
	unsigned char* dataBuff = (unsigned char*)malloc(lineByte * m_height);
	//file.Read(m_BMPdata, lineByte * m_height);
	file.Read(dataBuff, lineByte * m_height);
	for (int index = 0; index < m_height; index++)
		memcpy(m_BMPdata + index * lineByte, dataBuff + (m_height - 1 - index) * lineByte, lineByte);
	free(dataBuff);
	dataBuff = nullptr;
}

inline COLORREF DIB1::_color1_(int x, int y) const {
	unsigned char* tmp = m_BMPdata + m_width * x + y;
	auto test = Quard[*tmp];
	return RGB(Quard[*tmp].rgbRed, Quard[*tmp].rgbGreen, Quard[*tmp].rgbBlue);// Quard[*tmp].rgbGreen, Quard[*tmp].rgbBlue
}

int DIB1::_idx1_(int x, int y) const
{
	unsigned char* tmp = m_BMPdata + m_width * x + y;
	return *tmp;
}

void DIB1::_draw1(int x, int y, unsigned char c)
{
	unsigned char* tmp = m_BMPdata + m_width * x + y;
	*tmp = c;
}

inline COLORREF DIB1::_color4_(int x, int y) const {
	unsigned char* tmp = m_BMPdata + m_width * x/2 + y/2;
	int test = y%2?(*tmp & 0x0f): (*tmp >> 4);
	return RGB(Quard[test].rgbRed, Quard[test].rgbGreen, Quard[test].rgbBlue);// Quard[*tmp].rgbGreen, Quard[*tmp].rgbBlue
}

inline COLORREF DIB1::_color5_(int x, int y) const {
	unsigned char* tmp = m_BMPdata + (m_width + 31) / 32 * 4 * x + y / 8;
	int test = (*tmp & (0x01 << (7 - y % 8))) >> (7 - y % 8);
	//int test = *tmp << (7 - y % 8);
	//test = test >> 7;
	test &= 0x01;
	return RGB(Quard[test].rgbRed, Quard[test].rgbGreen, Quard[test].rgbBlue);// Quard[*tmp].rgbGreen, Quard[*tmp].rgbBlue
}

inline COLORREF DIB1::_color3_(int x, int y) const {
	unsigned char* tmp = m_BMPdata + m_width * x * 3 + y * 3;
	return RGB(*(tmp + 2), *(tmp + 1), *(tmp));
}