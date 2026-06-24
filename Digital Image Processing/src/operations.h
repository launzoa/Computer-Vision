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
// Binarizes the image using Otsu's thresholding method.
cv::Mat otsu_thresholding(const cv::Mat &img);
// Applies a pseudo-color map to a grayscale image.
cv::Mat pseudo_color(const cv::Mat &img);
// Converts a standard BGR image into the HSV color space.
cv::Mat rgb_to_hsv(const cv::Mat &img);
// Converts an HSV image back into the standard BGR color space.
cv::Mat hsv_to_rgb(const cv::Mat &img);
// Converts a standard BGR image into the HSL color space.
cv::Mat rgb_to_hsl(const cv::Mat &img);
// Converts an HSL image back into the standard BGR color space.
cv::Mat hsl_to_rgb(const cv::Mat &img);
// Equalizes a color image using only the L information from the HSL model.
cv::Mat equalize_hsl(const cv::Mat &img);
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
// Replaces each pixel with the minimum of its kernel neighbourhood.
cv::Mat min_filter(const cv::Mat &img, int kernel_size);
// Replaces each pixel with the maximum of its kernel neighbourhood.
cv::Mat max_filter(const cv::Mat &img, int kernel_size);
// Replaces each pixel with the midpoint (min+max)/2 of its kernel neighbourhood.
cv::Mat midpoint_filter(const cv::Mat &img, int kernel_size);
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

namespace morphology {
// Dilates a binary image using a square kernel.
cv::Mat dilation(const cv::Mat &img, int kernel_size);
// Erodes a binary image using a square kernel.
cv::Mat erosion(const cv::Mat &img, int kernel_size);
} // namespace morphology

namespace frequency {
// Computes the 2D Discrete Cosine Transform (DCT-II) for a 128x128 image.
cv::Mat dct_2d(const cv::Mat &img);
// Computes the Inverse 2D Discrete Cosine Transform (IDCT-II).
cv::Mat idct_2d(const cv::Mat &dct_img);
// Applies an ideal low-pass filter in the frequency domain.
cv::Mat low_pass_filter(const cv::Mat &dct_img, int cutoff);
// Applies an ideal high-pass filter in the frequency domain.
cv::Mat high_pass_filter(const cv::Mat &dct_img, int cutoff);
// Adds noise at a specific frequency coordinate (u, v) with a given magnitude.
cv::Mat add_frequency_noise(const cv::Mat &dct_img, int u, int v, double magnitude);
} // namespace frequency

} // namespace DIP

#endif // OPERATIONS_H
