#pragma once
#include<memory>
#include"BMP1.h"
#include"BMP4.h"
#include"BMP8.h"
#include"BMP24.h"
#include"Histogram.h"

std::shared_ptr<BMPdib> create(const CString&);
Histogram histogram(std::shared_ptr<BMPdib> bmp_sptr);
void binarization(std::shared_ptr<BMPdib> bmp_sptr, int c);
void equalization(std::shared_ptr<BMPdib> bmp_sptr, const Histogram& hist);
std::vector<float> accu_hist(const Histogram& hist);
void normalized(std::shared_ptr<BMPdib> bmp_sptr, const std::vector<float>& hist_or, const std::vector<float>& hist_ta);
