#include "spatial_filters.h"
#include <cstdlib>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
using namespace std;
using namespace cv;

void salt_and_pepper(Mat &img, int percent) {
  int n = (img.rows * img.cols) / percent;

  for (int k = 0; k < n; k++) {
    int i = rand() % img.rows;
    int j = rand() % img.cols;

    int noise = (rand() % 2) * 255;
    img.at<Vec3b>(i, j) = Vec3b(noise, noise, noise);
  }
}

int main() {
  Mat img = imread("../assets/dark_magician.webp", IMREAD_COLOR);
  salt_and_pepper(img, 10);

  Mat img_mean = mean_filter(img, 3);

  namedWindow("DIP", WINDOW_AUTOSIZE);

  Mat screen;
  vector<Mat> arr_imgs = {img, img_mean};
  hconcat(arr_imgs, screen);

  imshow("DIP", screen);
  waitKey();

  return 0;
}
