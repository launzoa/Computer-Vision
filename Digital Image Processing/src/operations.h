#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <opencv2/opencv.hpp>
#include <vector>

namespace DIP {

namespace utils {
// Calculates the median value of an array by partially sorting it.
int median(std::vector<int> arr);
// Returns the cumulative frequency distribution of pixel intensities.
std::vector<int> cum_frequency(const cv::Mat &img);
} // namespace utils

namespace noise {
// Randomly sets percentage pixels to white or black.
cv::Mat salt_and_pepper(const cv::Mat &img, double scale);
} // namespace noise

namespace color {
// Converts to grayscale using luminance weights R·0.299 + G·0.587 + B·0.114.
cv::Mat gray_scale(const cv::Mat &img);
// Applies Power-Law (Gamma) correction for non-linear brightness adjustment.
cv::Mat power_gamma(const cv::Mat &img, const double c, const double gamma);
// Binarizes the image (Black/White) based on a custom threshold value.
cv::Mat limiarization(const cv::Mat &img, const int threshold);
// Binarizes the image (Black/White) using a fixed threshold of 127.
cv::Mat binarization(const cv::Mat &img);
// Converts a standard BGR image into the HSV color space.
cv::Mat rgb_to_hsv(const cv::Mat &img);
// Converts an HSV image back into the standard BGR color space.
cv::Mat hsv_to_rgb(const cv::Mat &img);
} // namespace color

namespace pixelwise {
// Averages 'n' salt-and-pepper noised copies pixel-by-pixel to reduce noise.
cv::Mat mean_pixelwise(const cv::Mat &img, const int n);
// Calculates the median of 'n' salt-and-pepper noised copies pixel-by-pixel.
cv::Mat median_pixelwise(const cv::Mat &img, const int n);
} // namespace pixelwise

namespace spatial {
// Detects horizontal and vertical edges using the Sobel operator.
cv::Mat sobel(const cv::Mat &img);
// Detects areas of rapid intensity change using a second-derivative kernel.
cv::Mat laplacian(const cv::Mat &img);
// Replaces each pixel with the mean of its kernel (N×N) neighbourhood.
cv::Mat mean_convolution(const cv::Mat &img, int kernel_size);
} // namespace spatial

namespace histogram {
// Redistributes pixel intensities to maximise contrast.
cv::Mat histogram_equalization(const cv::Mat &img);
} // namespace histogram

namespace interpolation {
// Rescales the image by the given factor using nearest-neighbour interpolation.
cv::Mat nearest_neighbor(const cv::Mat &img, double scale);
// Rescales the image by the given factor using bilinear interpolation.
cv::Mat bilinear(const cv::Mat &img, double scale);
} // namespace interpolation

} // namespace DIP

#endif // OPERATIONS_H
