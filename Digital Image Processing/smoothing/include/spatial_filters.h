#pragma once
#ifndef SPATIAL_FILTERS_H
#define SPATIAL_FILTERS_H

#include <opencv4/opencv2/opencv.hpp>

cv::Mat mean_filter(cv::Mat &img, int m, int n);
cv::Mat median_filter(cv::Mat &img, int m, int n);

#endif // !SPATIAL_FILTERS_H
