#pragma once

#include<vector>
#include"MyDIB.h"

class Histogram
{
public:
	using size_type = std::vector<float>::size_type;

	explicit Histogram(int n)
		: numColor(n), hist(n) {}

	void add(int i) { ++hist[i]; }
	float operator[](size_type idx) const { return hist[idx]; }
	float& operator[](size_type idx) { return hist[idx]; }
	void normalize(int sz);
	int max_h() const;
	int min_h() const;
private:
	int numColor;
	std::vector<float> hist;
};

Histogram histogram(const DIB1& dib);
void binarization(DIB1& dib, int c);
Histogram equalization(const Histogram& hist);
void equalization(DIB1& dib, const Histogram& hist);
std::vector<float> accu_hist(const Histogram& hist);
void normalized(DIB1& dib, const std::vector<float>& hist_or, const std::vector<float>& hist_ta);