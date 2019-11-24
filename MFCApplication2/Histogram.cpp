#include "pch.h"
#include "Histogram.h"


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
