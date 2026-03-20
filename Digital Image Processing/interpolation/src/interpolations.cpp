#include "interpolations.h"

using namespace cv;

Mat nearestNeighbor(Mat &img, double scale) {
  int new_rows = img.rows * scale;
  int new_cols = img.cols * scale;

  Mat img_nN(new_rows, new_cols, CV_8UC3);

  for (int y = 0; y < new_rows; y++) {
    for (int x = 0; x < new_cols; x++) {
      int x_nN = round((double)x / scale);
      int y_nN = round((double)y / scale);

      x_nN = min(x_nN, img.cols - 1);
      y_nN = min(y_nN, img.rows - 1);

      img_nN.at<Vec3b>(y, x) = img.at<Vec3b>(y_nN, x_nN);
    }
  }
  return img_nN;
}

Mat bilinear(Mat &img, double scale) {
  int new_rows = img.rows * scale;
  int new_cols = img.cols * scale;

  Mat img_bilinear(new_rows, new_cols, CV_8UC3);

  for (int y = 0; y < new_rows; y++) {
    for (int x = 0; x < new_cols; x++) {
      int x1 = floor((double)x / scale);
      int y1 = floor((double)y / scale);
      int x2 = min(x1 + 1, img.cols - 1);
      int y2 = min(y1 + 1, img.rows - 1);

      double dx = (x / scale) - x1;
      double dy = (y / scale) - y1;

      Vec3b p1 = img.at<Vec3b>(y1, x1);
      Vec3b p2 = img.at<Vec3b>(y1, x2);
      Vec3b p3 = img.at<Vec3b>(y2, x1);
      Vec3b p4 = img.at<Vec3b>(y2, x2);

      Vec3b pixel;
      for (int c = 0; c < 3; c++) {
        double val_top = (1.0 - dx) * p1[c] + dx * p2[c];
        double val_bottom = (1.0 - dx) * p3[c] + dx * p4[c];
        double val = (1.0 - dy) * val_top + dy * val_bottom;

        pixel[c] = round(val);
      }
      img_bilinear.at<Vec3b>(y, x) = pixel;
    }
  }

  return img_bilinear;
}

double cubicWeight(double x) {
  double a = -0.5;
  x = abs(x);

  double x2 = x * x;
  double x3 = x2 * x;

  if (x <= 1.0) {
    return (a + 2.0) * x3 - (a + 3.0) * x2 + 1.0;
  } else if (x < 2.0) {
    return a * x3 - 5.0 * a * x2 + 8.0 * a * x - 4.0 * a;
  }
  return 0.0;
}

Mat bicubic(Mat &img, double scale) {
  int new_rows = img.rows * scale;
  int new_cols = img.cols * scale;

  Mat img_bicubic(new_rows, new_cols, CV_8UC3);

  for (int y = 0; y < new_rows; y++) {
    for (int x = 0; x < new_cols; x++) {
      int x1 = floor((double)x / scale);
      int y1 = floor((double)y / scale);

      double dx = x / scale - x1;
      double dy = y / scale - y1;

      double sum_b = 0.0, sum_g = 0.0, sum_r = 0.0;
      for (int j = -1; j <= 2; j++) {
        for (int i = -1; i <= 2; i++) {
          int x_i = x1 + i;
          int y_j = y1 + j;

          x_i = max(0, min(x_i, img.cols - 1));
          y_j = max(0, min(y_j, img.rows - 1));

          Vec3b pixel = img.at<Vec3b>(y_j, x_i);

          double x_weight = cubicWeight(i - dx);
          double y_weight = cubicWeight(j - dy);

          sum_b += pixel[0] * (x_weight * y_weight);
          sum_g += pixel[1] * (x_weight * y_weight);
          sum_r += pixel[2] * (x_weight * y_weight);
        }
      }
      Vec3b pixel_bicub;
      pixel_bicub[0] = min(max((int)round(sum_b), 0), 255);
      pixel_bicub[1] = min(max((int)round(sum_g), 0), 255);
      pixel_bicub[2] = min(max((int)round(sum_r), 0), 255);

      img_bicubic.at<Vec3b>(y, x) = pixel_bicub;
    }
  }

  return img_bicubic;
}
