#include "equalizations.h"
#include <vector>
using namespace cv;
using namespace std;

vector<int> cumulative_frequency(Mat &img) {
  vector<int> hist(256, 0);

  for (int y = 0; y < img.rows; y++) {
    for (int x = 0; x < img.cols; x++) {
      Vec3b pixel = img.at<Vec3b>(y, x);

      int gray = round((pixel[0] + pixel[1] + pixel[2]) / 3.0);
      hist[gray] += 1;
    }
  }

  vector<int> cdf(256, 0);
  cdf[0] = hist[0];

  for (int i = 1; i < 256; i++) {
    cdf[i] = cdf[i - 1] + hist[i];
  }
  return cdf;
}

Mat equalization(Mat &img) {
  Mat img_equalizated = Mat::zeros(img.rows, img.cols, CV_8UC3);
  vector<int> cdf = cumulative_frequency(img);

  for (int y = 0; y < img.rows; y++) {
    for (int x = 0; x < img.cols; x++) {
      Vec3b pixel = img.at<Vec3b>(y, x);

      int gray = round((pixel[0] + pixel[1] + pixel[2]) / 3.0);

      int val = round(((double)255 / (img.rows * img.cols)) * cdf[gray]);
      val = min(max(val, 0), 255);

      img_equalizated.at<Vec3b>(y, x) = Vec3b(val, val, val);
    }
  }

  return img_equalizated;
}
