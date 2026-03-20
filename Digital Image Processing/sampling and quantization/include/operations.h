#pragma once
#ifndef OPERATIONS_H
#define OPEARTIONS_H

#include <opencv4/opencv2/opencv.hpp>

cv::Mat downSampling(cv::Mat &img, int factor);
cv::Mat quantization(cv::Mat &img, int factor);
#endif // OPERATIONS_H
