#include "pch.h"
#include "MyDIB.h"
#include "pch.h"


DIB1::~DIB1() {

}

void DIB1::Read(const CString& fileName) {
	CFile file;
	VERIFY(file.Open(fileName, CFile::modeRead));
	// 读取 BMPFileHeader
	file.Read(&m_BMPFileHeader, sizeof(BMPFileHeader));
	// 读取 BMPInfoHeader
	ULONGLONG posi = file.Seek(sizeof(BMPFileHeader), CFile::begin);
	file.Read(&m_BMPInfoHeader, sizeof(BMPInfoHeader));
	posi = file.Seek(sizeof(BMPInfoHeader) + posi, CFile::begin);
	// read information
	m_width = m_BMPInfoHeader.biWidth;
	m_height = m_BMPInfoHeader.biHeight;
	real_size = m_BMPInfoHeader.biSize;
	//计算图像每行像素所占的字节数（必须是4的倍数）
	int biBitCount = m_BMPInfoHeader.biBitCount;
	int lineByte = (m_width + 3) / 4 * 4;

	if (biBitCount == 8) {
		m_channels = 1;
		//		读取调色板
		Quard = (RGBQuad*)malloc(sizeof(RGBQuad) * 256);
		file.Read(Quard, 256 * sizeof(RGBQuad));
		posi = file.Seek(posi + 256 * sizeof(RGBQuad), CFile::begin);

		//_colorAt = _color1_;
		_colorAt = std::bind(&DIB1::_color1_, this, std::placeholders::_1, std::placeholders::_2);
	}
	else if (biBitCount == 24) {
		m_channels = 3;
		Quard = nullptr;
		lineByte *= 3;

		//_colorAt = _color3_;
		_colorAt = std::bind(&DIB1::_color3_, this, std::placeholders::_1, std::placeholders::_2);
	}
	//读取图片内容
	_read_bmp_(file, lineByte);
	file.Close();
}

bool DIB1::Write(const CString& fileName) {
	CFile file;
	
	VERIFY(file.Open(fileName, CFile::modeCreate | CFile::modeWrite));

	try {
		UINT lineByte = (m_width + 3) / 4 * 4 * m_channels;
		unsigned char* dataBuff = (unsigned char*)malloc(m_height * lineByte);
		for (int index = 0; index < m_height; index++)
			memcpy(dataBuff + (m_height - 1 - index) * lineByte, m_BMPdata + index * lineByte, lineByte);
		file.Write(&m_BMPFileHeader, sizeof(m_BMPFileHeader));
		file.Write(&m_BMPInfoHeader, sizeof(m_BMPInfoHeader));
		if(Quard != nullptr) file.Write(Quard, sizeof(RGBQuad) * 256);
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
	file.Read(dataBuff, lineByte * m_height);
	for (int index = 0; index < m_height; index++)
		memcpy(m_BMPdata + index * lineByte, dataBuff + (m_height - 1 - index) * lineByte, lineByte);
	free(dataBuff);
	dataBuff = nullptr;
}

inline COLORREF DIB1::_color1_(int x, int y) const {
	unsigned char* tmp = m_BMPdata + m_width * x + y;
	return RGB(Quard[*tmp].rgbRed, Quard[*tmp].rgbRed, Quard[*tmp].rgbRed);// Quard[*tmp].rgbGreen, Quard[*tmp].rgbBlue
}

inline COLORREF DIB1::_color3_(int x, int y) const {
	unsigned char* tmp = m_BMPdata + m_width * x * 3 + y * 3;
	return RGB(*(tmp + 1), *(tmp), *(tmp + 2));
}