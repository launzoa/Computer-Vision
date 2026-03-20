#include "colors.h"
#include <iostream>

using namespace std;
using namespace cv;

void grayColor(Mat &img) {
  for (int y = 0; y < img.rows; y++) {
    for (int x = 0; x < img.cols; x++) {
      Vec3b &pixel = img.at<Vec3b>(y, x);

      unsigned int b = pixel[0];
      unsigned int g = pixel[1];
      unsigned int r = pixel[2];

      unsigned int gray = (r + g + b) / 3;
      pixel[0] = gray;
      pixel[1] = gray;
      pixel[2] = gray;
    }
  }
}

void invertGrayColor(Mat &img) {
  for (int y = 0; y < img.rows; y++) {
    for (int x = 0; x < img.cols; x++) {
      Vec3b &pixel = img.at<Vec3b>(y, x);

      unsigned int b = pixel[0];
      unsigned int g = pixel[1];
      unsigned int r = pixel[2];

      unsigned int gray = (r + g + b) / 3;
      unsigned int invGray = 255 - gray;

      pixel[0] = invGray;
      pixel[1] = invGray;
      pixel[2] = invGray;
    }
  }
}

void invertColor(Mat &img) {
  for (int y = 0; y < img.rows; y++) {
    for (int x = 0; x < img.cols; x++) {
      Vec3b &pixel = img.at<Vec3b>(y, x);

      unsigned int invB = 255 - pixel[0];
      unsigned int invG = 255 - pixel[1];
      unsigned int invR = 255 - pixel[2];

      pixel[0] = invB;
      pixel[1] = invG;
      pixel[2] = invR;
    }
  }
}
