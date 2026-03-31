#include "colors.h"
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

int main() {

  Mat img1 = imread("../assets/dark_magician.webp");
  Mat img2 = img1.clone();

  namedWindow("DIP", WINDOW_AUTOSIZE);
  while (true) {
    Mat screen;
    vector<Mat> arr_imgs = {img1, img2};
    hconcat(arr_imgs, screen);
    imshow("DIP", screen);

    char key = (char)waitKey(30);

    if (key == 27)
      break;

    if (key == '1') {
      img2 = img1.clone();
      grayColor(img2);
    }

    if (key == '2') {
      img2 = img1.clone();
      invertGrayColor(img2);
    }

    if (key == '3') {
      img2 = img1.clone();
      invertColor(img2);
    }

    if (key == ' ')
      img2 = img1.clone();
  }

  destroyAllWindows();

  return 0;
}
