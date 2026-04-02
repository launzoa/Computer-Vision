#include "limiars.h"

using namespace std;
using namespace cv;

Mat bimiarization(Mat &img, int limiar) {
  Mat img_processed(img.rows, img.cols, CV_8UC3);

  for (int y = 0; y < img.rows; y++) {
    for (int x = 0; x < img.cols; x++) {
      Vec3b pixel = img.at<Vec3b>(y, x);

      int color = (pixel[0] + pixel[1] + pixel[2]) / 3.0;
      int intensity;
      if (color > limiar) {
        intensity = 0;
      } else {
        intensity = 255;
      }

      img_processed.at<Vec3b>(y, x) = Vec3b(intensity, intensity, intensity);
    }
  }

  return img_processed;
}

Mat limiarization(Mat &img, int limiar) {
  Mat img_processed = Mat::zeros(img.rows, img.cols, CV_8UC3);

  for (int y = 0; y < img.rows; y++) {
    for (int x = 0; x < img.cols; x++) {
      Vec3b pixel = img.at<Vec3b>(y, x);

      int r = (pixel[0] + pixel[1] + pixel[2]) / 3.0;

      if (r > limiar) {
        r = 0;
      }
      img_processed.at<Vec3b>(y, x) = Vec3b(r, r, r);
    }
  }
  return img_processed;
}
