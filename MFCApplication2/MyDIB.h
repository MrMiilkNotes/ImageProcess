#pragma once
#include"MyDIB.h"

#include<string>
#include<functional>
#include<cstdlib>
#include<vector>

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

class DIB1
{
	typedef std::function<COLORREF (int, int)> color_func;
	typedef std::function<void (int, int, unsigned char)> draw_func;
	typedef std::function<int (int, int)> color_idx_func;
public:
	~DIB1();

	void Read(const CString& filename);
	bool Write(const CString& fileName);
	int width() const { return m_width; }
	int height() const { return m_height; }
	unsigned char* data() { return m_BMPdata; }
	color_func _colorAt;
	draw_func drawAt;
	color_idx_func idxAt;
private:
	BMPFileHeader m_BMPFileHeader;	
	BMPInfoHeader m_BMPInfoHeader;
	RGBQuad* Quard;
	int m_width, m_height, real_size;
	int m_channels;	// 图像通道数
	int biBitCount;
	unsigned char* m_BMPdata;

	void _read_bmp_(CFile& file, int lineByte);
	COLORREF _color1_(int x, int y) const;
	int _idx1_(int x, int y) const;
	void _draw1(int x, int y, unsigned char c = 0);
	COLORREF _color3_(int x, int y) const;
	COLORREF _color4_(int x, int y) const;
	COLORREF _color5_(int x, int y) const;
};

