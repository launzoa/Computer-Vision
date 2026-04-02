#pragma once
#ifndef LIMIARS_H
#define LIMIARS_H

#include <opencv4/opencv2/opencv.hpp>

cv::Mat bimiarization(cv::Mat &img, int limiar);
cv::Mat limiarization(cv::Mat &img, int limiar);

#endif // LIMIARS_H
