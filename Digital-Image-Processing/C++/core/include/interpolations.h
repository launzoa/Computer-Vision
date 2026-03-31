#pragma once
#ifndef INTERPOLATIONS_H
#define INTERPOLATIONS_H

#include <opencv4/opencv2/opencv.hpp>

cv::Mat nearestNeighbor(cv::Mat &img, double scale);
cv::Mat bilinear(cv::Mat &img, double scale);
cv::Mat bicubic(cv::Mat &img, double scale);
#endif // !INTERPOLATIONS_H
