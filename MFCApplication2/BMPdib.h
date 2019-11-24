#pragma once

#include<stdexcept>
#include<memory>
#include "pch.h"

#pragma pack(1)
struct BMPFileHeader
{
	unsigned short bfType;		//��ʶ���ļ�Ϊbmp�ļ�,�ж��ļ��Ƿ�Ϊbmp�ļ������ø�ֵ��"0x4d42"�Ƚ��Ƿ���ȼ��ɣ�0x4d42 = 19778
	unsigned long  bfSize;		//�ļ���С
	unsigned short bfReserved1;	//Ԥ����λ
	unsigned short bfReserved2;	//Ԥ����λ
	unsigned long  bfOffBits;	//ͼ������������ʼλ��
};
#pragma pack()

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

