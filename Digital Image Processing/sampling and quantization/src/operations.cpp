#include "operations.h"

using namespace cv;

Mat downSampling(Mat &img, int factor) {
  int new_rows = img.rows / factor;
  int new_cols = img.cols / factor;

  Mat sampled_img(new_rows, new_cols, CV_8UC3);

  for (int y = 0; y < new_rows; y++) {
    for (int x = 0; x < new_cols; x++) {
      sampled_img.at<Vec3b>(y, x) = img.at<Vec3b>(y * factor, x * factor);
    }
  }

  return sampled_img;
}

Mat quantization(Mat &img, int factor) {
  Mat quantization_img = img.clone();
  int length = 256 / factor;

  for (int y = 0; y < img.rows; y++) {
    for (int x = 0; x < img.cols; x++) {
      Vec3b &pixel = quantization_img.at<Vec3b>(y, x);

      for (int c = 0; c < 3; c++) {
        int value = (pixel[c] / length) * length + (length / 2);
        pixel[c] = min(255, value);
      }
    }
  }

  return quantization_img;
}
