#include "limiars.h"
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace std;
using namespace cv;

Mat gray_scale(Mat &img) {
  Mat img_gray = Mat::zeros(img.rows, img.cols, CV_8UC3);

  for (int y = 0; y < img.rows; y++) {
    for (int x = 0; x < img.cols; x++) {
      Vec3b pixel = img.at<Vec3b>(y, x);
      int gray = 0;

      for (int k = 0; k < 3; k++) {
        gray += pixel[k];
      }
      gray = round(gray / 3.0);
      img_gray.at<Vec3b>(y, x) = Vec3b(gray, gray, gray);
    }
  }

  return img_gray;
}

int main() {
  Mat img = imread("../assets/dark_magician.webp", IMREAD_COLOR);
  Mat img_gray = gray_scale(img);

  namedWindow("DIP", WINDOW_AUTOSIZE);
  Mat screen;

  vector<Mat> arr_imgs = {img, img_gray};
  hconcat(arr_imgs, screen);

  imshow("DIP", screen);
  waitKey();

  return 0;
}
