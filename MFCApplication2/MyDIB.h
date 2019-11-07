#pragma once
#include"MyDIB.h"

#include<string>
#include<functional>
#include<cstdlib>

#pragma pack(1)
struct BMPFileHeader
{
	unsigned short bfType;		//��ʶ���ļ�Ϊbmp�ļ�,�ж��ļ��Ƿ�Ϊbmp�ļ������ø�ֵ��"0x4d42"�Ƚ��Ƿ���ȼ��ɣ�0x4d42 = 19778
	unsigned long  bfSize;		//�ļ���С
	unsigned short bfReserved1;	//Ԥ����λ
	unsigned short bfReserved2;	//Ԥ����λ
	unsigned long  bfOffBits;	//ͼ������������ʼλ��
};

struct BMPInfoHeader
{
	unsigned long  biSize;	//ͼ�����ݴ�С
	long     biWidth;	//���
	long     biHeight;	//�߶�
	unsigned short biPlanes;//Ϊ1
	unsigned short biBitCount; //����λ����8-�Ҷ�ͼ��24-���ɫ
	unsigned longbiCompression;//ѹ����ʽ
	unsigned longbiSizeImage;  //ͼ�������ݴ�С
	long     biXPelsPerMeter;  //ˮƽ�ֱ��ʣ�����ÿ��
	long     biYPelsPerMeter;
	unsigned long biClrUsed;   //λͼʵ���õ�����ɫ��
	unsigned short biClrImportant;//λͼ��ʾ���̣���Ҫ����ɫ����0--���ж���Ҫ
};

struct RGBQuad
{
	unsigned char rgbBlue;
	unsigned char rgbGreen;
	unsigned char rgbRed;
	unsigned char rgbReserved;
};
#pragma pack()

class DIB1
{
private:
	BMPFileHeader m_BMPFileHeader;	
	BMPInfoHeader m_BMPInfoHeader;
	RGBQuad* Quard;
	int m_width, m_height, real_size, m_channels;
	unsigned char* m_BMPdata;

	void _read_bmp_(CFile& file, int lineByte);
	//typedef COLORREF (DIB1::*color_func)(int, int) const;
	typedef std::function<COLORREF (int, int)> color_func;
	COLORREF _color1_(int x, int y) const;
	COLORREF _color3_(int x, int y) const;
public:
	~DIB1();

	void Read(const CString& filename);
	bool Write(const CString& fileName);
	int width() const { return m_width; }
	int height() const { return m_height; }
	unsigned char* data() { return m_BMPdata; }
	color_func _colorAt;
};

