#include "spatial_filters.h"
#include <vector>

using namespace cv;
using namespace std;

Mat mean_filter(Mat &img, int n) {
  Mat img_filtered = Mat::zeros(img.rows, img.cols, CV_8UC3);
  int pad = n / 2;

  for (int y = pad; y < img.rows - pad; y++) {
    for (int x = pad; x < img.cols - pad; x++) {
      int sum_b = 0;
      int sum_g = 0;
      int sum_r = 0;

      for (int ky = -pad; ky <= pad; ky++) {
        for (int kx = -pad; kx <= pad; kx++) {
          Vec3b pixel = img.at<Vec3b>(y + ky, x + kx);

          sum_b += pixel[0];
          sum_g += pixel[1];
          sum_r += pixel[2];
        }
      }
      int b = min(max(sum_b / (n * n), 0), 255);
      int g = min(max(sum_g / (n * n), 0), 255);
      int r = min(max(sum_r / (n * n), 0), 255);

      img_filtered.at<Vec3b>(y, x) = Vec3b(b, g, r);
    }
  }
  return img_filtered;
}

void sort_pixels(vector<int> &arr, int size) {
  for (int i = 0; i < size - 1; i++) {
    int l = arr[i];
    int idx = i;
    for (int j = i + 1; j < size; j++) {
      if (arr[j] < l) {
        l = arr[j];
        idx = j;
      }
    }
    arr[idx] = arr[i];
    arr[i] = l;
  }
}

Mat median_filter(cv::Mat &img, int n) {
  Mat img_filtered = Mat::zeros(img.rows, img.cols, CV_8UC3);
  int pad = n / 2;
  int kernel_area = n * n;

  for (int y = pad; y < img.rows - pad; y++) {
    for (int x = pad; x < img.cols - pad; x++) {
      vector<int> median_b;
      vector<int> median_g;
      vector<int> median_r;

      for (int ky = -pad; ky <= pad; ky++) {
        for (int kx = -pad; kx <= pad; kx++) {
          Vec3b pixel = img.at<Vec3b>(y + ky, x + kx);
          median_b.push_back(pixel[0]);
          median_g.push_back(pixel[1]);
          median_r.push_back(pixel[2]);
        }
      }

      sort_pixels(median_b, kernel_area);
      sort_pixels(median_g, kernel_area);
      sort_pixels(median_r, kernel_area);

      int median_idx = kernel_area / 2;
      int b = median_b[median_idx];
      int g = median_g[median_idx];
      int r = median_r[median_idx];

      img_filtered.at<Vec3b>(y, x) = Vec3b(b, g, r);
    }
  }
  return img_filtered;
}
