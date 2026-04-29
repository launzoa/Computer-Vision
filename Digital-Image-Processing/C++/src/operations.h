#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <opencv2/opencv.hpp>
#include <vector>

namespace ImageFiltering {

// —— Operations ───────────────────────────────────────────────────────────────

int median(const std::vector<int> arr);
//// Returns the cumulative frequency distribution of pixel intensities.
std::vector<int> cum_frequency(const cv::Mat &img);

// ── Noise ────────────────────────────────────────────────────────────────────

/// Randomly sets percentage pixels to white or black.
cv::Mat salt_and_pepper(const cv::Mat &img, double scale);

// ── Colour transforms ────────────────────────────────────────────────────────

/// Converts to grayscale using luminance weights R·0.299 + G·0.587 + B·0.114.
cv::Mat gray_scale(const cv::Mat &img);
cv::Mat power_gamma(const cv::Mat &img, const int c, const int gamma);
cv::Mat binarization(const cv::Mat &img, const int threshold);

// —— Pixelwise Filtering ——————————————————————————————————————————————————————

/// Averages 10 salt-and-pepper noised copies pixel-by-pixel to reduce noise.
cv::Mat mean_pixelwise(const cv::Mat &img, const int n);
cv::Mat median_pixelwise(const cv::Mat &img, const int n);

// ── Spatial filtering ────────────────────────────────────────────────────────
cv::Mat sobel(const cv::Mat &img);
cv::Mat laplacian(const cv::Mat &img);
/// Replaces each pixel with the mean of its kernel (N×N) neighbourhood.
cv::Mat mean_convolution(const cv::Mat &img, int kernel_size);

// ── Histogram ────────────────────────────────────────────────────────────────

/// Redistributes pixel intensities to maximise contrast.
cv::Mat histogram_equalization(const cv::Mat &img);

// —— Interpolation ────────────────────────────────────────────────────────────

/// Rescales the image by the given factor using nearest-neighbour
/// interpolation.
cv::Mat nearest_neighbor(const cv::Mat &img, double scale);

/// Rescales the image by the given factor using bilinear interpolation.
cv::Mat bilinear(const cv::Mat &img, double scale);

} // namespace ImageFiltering

#endif // OPERATIONS_H
