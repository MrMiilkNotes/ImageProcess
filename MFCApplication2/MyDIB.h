#pragma once
#include"MyDIB.h"

#include<string>
#include<functional>
#include<cstdlib>
#include<vector>

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
	int m_channels;	// ͼ��ͨ����
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

