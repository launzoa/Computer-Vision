#include "equalizations.h"
#include <opencv2/highgui.hpp>
#include <opencv4/opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main() {

  Mat img = imread("../assets/Hawkes_Bay_NZ.jpg", IMREAD_COLOR);
  Mat img_equalized = equalization(img);

  namedWindow("DIP", WINDOW_AUTOSIZE);

  Mat screen;
  vector<Mat> arr_imgs = {img, img_equalized};
  hconcat(arr_imgs, screen);

  imshow("DIP", screen);
  waitKey(0);

  return 0;
}
