#include "spatial_filters.h"
#include <vector>

using namespace cv;
using namespace std;

Mat mean_filter(Mat &img, int m, int n) {
  Mat img_filtered = Mat::zeros(img.rows, img.cols, CV_8UC3);

  int y_pad = m / 2;
  int x_pad = n / 2;

  for (int y = y_pad; y < img.rows - y_pad; y++) {
    for (int x = x_pad; x < img.cols - x_pad; x++) {
      int sum = 0;

      for (int ky = -y_pad; ky < y_pad; ky++) {
        for (int kx = -x_pad; kx < x_pad; kx++) {
          Vec3b pixel = img.at<Vec3b>(y - ky, x - kx);

          int gray = 0;
          for (int k = 0; k < 3; k++) {
            gray += pixel[k];
          }
          gray = round(gray / 3.0);
          sum += gray;
        }
      }

      int val = round((double)sum / (m * n));
      val = min(max(val, 0), 255);
      img_filtered.at<Vec3b>(y, x) = Vec3b(val, val, val);
    }
  }
  return img_filtered;
}
