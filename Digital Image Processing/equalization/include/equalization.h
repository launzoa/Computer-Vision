#pragma once
#ifndef EQUALIZATION_H
#define EQUALIZATION_H

#include <opencv4/opencv2/opencv.hpp>
#include <vector>

std::vector<int> cumulative_frequency(cv::Mat &img);
cv::Mat equalization(cv::Mat &img);

#endif // !EQUALIZATION_H
