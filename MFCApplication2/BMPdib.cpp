#include "pch.h"
#include "BMPdib.h"
#include "Histogram.h"


void BMPdib::read_bmp_(CFile& file, int lineBytes)
{
	auto sz = lineBytes * height();
	img_sptr = std::shared_ptr<unsigned char>(
		new unsigned char[sz], 
		[](unsigned char* p) {delete[] p; });
	auto tmp_sptr = std::shared_ptr<unsigned char>(
		new unsigned char[sz], 
		[](unsigned char* p) {delete[] p; });
	auto img_ptr = img_sptr.get();
	auto tmp_ptr = tmp_sptr.get();
	file.Read(tmp_ptr, sz);
	for (int idx = 0; idx < height(); ++idx) {
		memcpy(img_ptr + idx * lineBytes, tmp_ptr + (height() - 1 - idx) * lineBytes, lineBytes);
	}
}
