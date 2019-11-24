#include "pch.h"
#include "Histogram.h"

inline float distence(float lhs, float rhs) {
	return lhs > rhs ? lhs - rhs : rhs - lhs;
}

Histogram histogram(const DIB1& dib) {

	Histogram hist(256);

	for (int i = 0; i < dib.height(); ++i) {
		for (int j = 0; j < dib.width(); ++j) {
			hist.add(dib.idxAt(i, j));
		}
	}
	hist.normalize(dib.height() * dib.width());

	return hist;
}

void binarization(DIB1& dib, int c) {
	for (int i = 0; i < dib.height(); ++i) {
		for (int j = 0; j < dib.width(); ++j) {
			if (dib.idxAt(i, j) > c) dib.drawAt(i, j, 255);
			else dib.drawAt(i, j, 0);
		}
	}
}

Histogram equalization(const Histogram& hist) {
	std::vector<float> tmp_t(256);
	std::vector<int> tmp_t_1(256);

	tmp_t[0] = hist[0];
	for (int i = 1; i < 256; ++i) {
		tmp_t[i] = tmp_t[i - 1] + hist[i];
	}

	for (int i = 0; i < 256; ++i) {
		tmp_t_1[i] = 255 * (tmp_t[i]) + 0.5;
	}
	Histogram hist_n(256);
	for (int i = 0; i < 256; ++i) {
		hist_n[tmp_t_1[i]] += hist[i];
	}

	return hist_n;
}

void equalization(DIB1& dib, const Histogram& hist) {
	std::vector<float> tmp_t(256);
	std::vector<int> tmp_t_1(256);

	tmp_t[0] = hist[0];
	for (int i = 1; i < 256; ++i) {
		tmp_t[i] = tmp_t[i - 1] + hist[i];
	}

	for (int i = 0; i < 256; ++i) {
		tmp_t_1[i] = 255 * (tmp_t[i]) + 0.5;
	}
	/*Histogram hist_n(256);
	for (int i = 0; i < 256; ++i) {
		hist_n[tmp_t_1[i]] += hist[i];
	}*/

	for (int i = 0; i < dib.height(); ++i) {
		for (int j = 0; j < dib.width(); ++j) {
			dib.drawAt(i, j, tmp_t_1[dib.idxAt(i, j)]);
		}
	}
}

std::vector<float> accu_hist(const Histogram& hist) {
	std::vector<float> tmp_t(256);
	// std::vector<int> tmp_t_1(256);

	tmp_t[0] = hist[0];
	for (int i = 1; i < 256; ++i) {
		tmp_t[i] = tmp_t[i - 1] + hist[i];
	}
	return tmp_t;
}

void normalized(DIB1& dib, const std::vector<float>& hist_or, const std::vector<float>& hist_ta) {
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

	for (int i = 0; i < dib.height(); ++i) {
		for (int j = 0; j < dib.width(); ++j) {
			dib.drawAt(i, j, o2t[dib.idxAt(i, j)]);
		}
	}
}

void Histogram::normalize(int sz)
{
	for (int i = 0; i < hist.size(); ++i) {
		hist[i] /= sz;
	}
}

int Histogram::max_h() const
{
	for (auto i = hist.size() - 1; i != 0; --i) {
		if (hist[i] != 0) return i;
	}
	return 0;
}

int Histogram::min_h() const
{
	for (decltype(hist.size()) i = 0; i < hist.size(); ++i) {
		if (hist[i] != 0) return i;
	}
	return 255;
}
