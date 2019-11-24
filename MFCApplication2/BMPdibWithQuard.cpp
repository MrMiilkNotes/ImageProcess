#include "pch.h"
#include "BMPdibWithQuard.h"

void BMPdibWithQuard::write(const CString& filename) const
{
	CFile file;

	file.Open(filename, CFile::modeCreate | CFile::modeWrite);
	unsigned char* dataBuff = nullptr;
	try {
		UINT lineByte = (m_BMPInfoHeader.biBitCount * width() + 31) / 32 * 4;
		dataBuff = new unsigned char [height() * lineByte];
		auto img_ptr = img_sptr.get();
		for (int index = 0; index < height(); index++)
			memcpy(dataBuff + (height() - 1 - index) * lineByte, img_ptr + index * lineByte, lineByte);
		file.Write(&m_BMPFileHeader, sizeof(m_BMPFileHeader));
		file.Write(&m_BMPInfoHeader, sizeof(m_BMPInfoHeader));
		file.Write(quard.get(), sizeof(RGBQuad) * (1 << biBitCount));
		file.Write(dataBuff, height() * lineByte);
	}
	catch (CException * pe) {
		pe->Delete();
		AfxMessageBox(_T("write error"));
	}
	if (dataBuff) delete[] dataBuff;
	file.Close();
}

void BMPdibWithQuard::read(const CString& filename)
{
	CFile file(filename, CFile::modeRead);
	file.Read(&m_BMPFileHeader, sizeof(BMPFileHeader));
	file.Read(&m_BMPInfoHeader, sizeof(BMPInfoHeader));
	// m_width = m_BMPInfoHeader.biWidth;
	// m_height = m_BMPInfoHeader.biHeight;
	biBitCount = m_BMPInfoHeader.biBitCount;
	m_channels = 1;
	int sz = 1 << biBitCount;
	quard = std::shared_ptr<RGBQuad>(new RGBQuad[sz], [](RGBQuad* p) {delete[] p; });
	file.Read(quard.get(), sz * sizeof(RGBQuad));
	int lineBytes = (biBitCount * m_BMPInfoHeader.biWidth + 31) / 32 * 4;
	read_bmp_(file, lineBytes);
}
