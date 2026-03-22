#include "interpolations.h"
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv4/opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main() {

  Mat img = imread("assets/Lena320x240.jpg", IMREAD_COLOR);

  double scale;
  cout << "Escala para as interpolações: ";
  cin >> scale;

  Mat img_nN = nearestNeighbor(img, scale);
  Mat img_bilinear = bilinear(img, scale);
  Mat img_bicubic = bicubic(img, scale);

  Mat screen;
  vector<Mat> array_imagens = {img_nN, img_bilinear, img_bicubic};
  hconcat(array_imagens, screen);

  namedWindow("Interpolation", WINDOW_AUTOSIZE);
  imshow("Interpolation", screen);
  waitKey(0);
  return 0;
}
