#include "pch.h"
#include "tools.h"

std::shared_ptr<BMPdib> create(const CString& filename)
{
	BMPFileHeader m_BMPFileHeader;
	BMPInfoHeader m_BMPInfoHeader;
	CFile file(filename, CFile::modeRead);
	file.Read(&m_BMPFileHeader, sizeof(BMPFileHeader));
	file.Read(&m_BMPInfoHeader, sizeof(BMPInfoHeader));
	file.Close();
	auto img_sptr = std::shared_ptr<BMPdib>();
	switch (m_BMPInfoHeader.biBitCount)
	{
	case 8:
		img_sptr.reset(new BMP8());
		break;
	case 4:
		img_sptr.reset(new BMP4());
		break;
	case 1:
		img_sptr.reset(new BMP1());
		break;
	case 24:
		img_sptr.reset(new BMP24());
		break;
	default:
		break;
	}
	img_sptr->read(filename);
	return img_sptr;
}

inline float distence(float lhs, float rhs) {
	return lhs > rhs ? lhs - rhs : rhs - lhs;
}

Histogram histogram(std::shared_ptr<BMPdib> bmp_sptr) {

	Histogram hist(256);

	for (int i = 0; i < bmp_sptr->height(); ++i) {
		for (int j = 0; j < bmp_sptr->width(); ++j) {
			hist.add(bmp_sptr->quard_idx(i, j));
		}
	}
	hist.normalize(bmp_sptr->height() * bmp_sptr->width());

	return hist;
}

void binarization(std::shared_ptr<BMPdib> bmp_sptr, int c) {
	for (int i = 0; i < bmp_sptr->height(); ++i) {
		for (int j = 0; j < bmp_sptr->width(); ++j) {
			if (bmp_sptr->quard_idx(i, j) > c) bmp_sptr->draw(i, j, 255);
			else bmp_sptr->draw(i, j, 255);
		}
	}
}

void equalization(std::shared_ptr<BMPdib> bmp_sptr, const Histogram& hist) {
	std::vector<float> tmp_t(256);
	std::vector<int> tmp_t_1(256);

	tmp_t[0] = hist[0];
	for (int i = 0; i < 255; ++i) {
		tmp_t[i+1] = tmp_t[i] + hist[i];
	}

	for (int i = 0; i < 256; ++i) {
		tmp_t_1[i] = 255 * (tmp_t[i]) + 0.5;
	}

	for (int i = 0; i < bmp_sptr->height(); ++i) {
		for (int j = 0; j < bmp_sptr->width(); ++j) {
			bmp_sptr->draw(i, j, tmp_t_1[bmp_sptr->quard_idx(i, j)]);
		}
	}
}

std::vector<float> accu_hist(const Histogram& hist) {
	std::vector<float> tmp_t(256);

	tmp_t[0] = hist[0];
	for (int i = 0; i < 255; ++i) {
		tmp_t[i+1] = tmp_t[i] + hist[i];
	}
	return tmp_t;
}

void normalized(std::shared_ptr<BMPdib> bmp_sptr, const std::vector<float>& hist_or, const std::vector<float>& hist_ta) {
	std::vector<int> o2t(256);
	for (int i = 0; i < 256; ++i) {
		float dist = distence(hist_or[i], hist_ta[0]);
		int tag = 0;
		for (int j = 1; j < 256; ++j) {
			float tmp = distence(hist_or[i], hist_ta[j]);
			if (tmp > dist) break;
			if (tmp < dist) {
				tag = j;
				dist = tmp;
			}
		}
		o2t[i] = tag;
	}

	for (int i = 0; i < bmp_sptr->height(); ++i) {
		for (int j = 0; j < bmp_sptr->width(); ++j) {
			bmp_sptr->draw(i, j, o2t[bmp_sptr->quard_idx(i, j)]);
		}
	}
}