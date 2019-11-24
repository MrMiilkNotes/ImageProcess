#pragma once

#include<stdexcept>
#include<memory>
#include "pch.h"

#pragma pack(1)
struct BMPFileHeader
{
	unsigned short bfType;		//标识该文件为bmp文件,判断文件是否为bmp文件，即用该值与"0x4d42"比较是否相等即可，0x4d42 = 19778
	unsigned long  bfSize;		//文件大小
	unsigned short bfReserved1;	//预保留位
	unsigned short bfReserved2;	//预保留位
	unsigned long  bfOffBits;	//图像数据区的起始位置
};
#pragma pack()

struct BMPInfoHeader
{
	unsigned long  biSize;	//图像数据大小
	long     biWidth;	//宽度
	long     biHeight;	//高度
	unsigned short biPlanes;//为1
	unsigned short biBitCount; //像素位数，8-灰度图；24-真彩色
	unsigned longbiCompression;//压缩方式
	unsigned longbiSizeImage;  //图像区数据大小
	long     biXPelsPerMeter;  //水平分辨率，像素每米
	long     biYPelsPerMeter;
	unsigned long biClrUsed;   //位图实际用到的颜色数
	unsigned short biClrImportant;//位图显示过程，重要的颜色数；0--所有都重要
};

struct RGBQuad
{
	BYTE rgbBlue;
	BYTE rgbGreen;
	BYTE rgbRed;
	BYTE rgbReserved;
};


class BMPdib
{
public:
	using size_type = std::size_t;

	virtual ~BMPdib(){}

	virtual void read(const CString&) = 0;
	virtual void draw(int, int, unsigned char) = 0;
	virtual COLORREF color_at(int, int) const = 0;
	virtual unsigned char quard_idx(int, int) const = 0;
	virtual void write(const CString& filename) const = 0;
	
	size_type width() const { return m_BMPInfoHeader.biWidth; }
	size_type height() const { return m_BMPInfoHeader.biHeight; }
protected:
	void read_bmp_(CFile& file, int lineBytes);
protected:
	std::shared_ptr<unsigned char> img_sptr;
	BMPFileHeader m_BMPFileHeader;
	BMPInfoHeader m_BMPInfoHeader;
	unsigned short biBitCount;	// m_BMPInfoHeader.biBitCount
	unsigned short m_channels;
};

