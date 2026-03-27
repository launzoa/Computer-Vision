#include "sampling.h"
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv4/opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main() {
  Mat img = imread("assets/Lena320x240.jpg", IMREAD_COLOR);

  int downSample_factor;
  cout << "Fator de amostragem: ";
  cin >> downSample_factor;
  Mat downSample_img = downSampling(img, downSample_factor);

  int quantization_factor;
  cout << "Fator de quantização: ";
  cin >> quantization_factor;
  Mat quantization_img = quantization(img, quantization_factor);

  namedWindow("SamplingAndQuantization", WINDOW_AUTOSIZE);

  Mat screen(img.rows, img.cols + downSample_img.cols + quantization_img.cols,
             CV_8UC3);
  img.copyTo(screen(Rect(0, 0, img.cols, img.rows)));
  downSample_img.copyTo(
      screen(Rect(img.cols, 0, downSample_img.cols, downSample_img.rows)));
  quantization_img.copyTo(
      screen(Rect(img.cols + downSample_img.cols, 0, quantization_img.cols,
                  quantization_img.rows)));

  imshow("DIP", screen);

  cout << "Pressione qualquer tecla na janela da imagem para sair..." << endl;
  waitKey(0);

  return 0;
}
