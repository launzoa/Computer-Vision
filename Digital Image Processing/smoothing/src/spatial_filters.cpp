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
