#include "operations.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <opencv2/core/hal/interface.h>
#include <vector>

using namespace std;
using namespace cv;

namespace DIP {

namespace utils {
int median(vector<int> arr) {
  int n = arr.size();
  int med = n / 2;

  for (int i = 0; i <= med; ++i) {
    int lower = arr[i];
    int idx = i;

    for (int j = i + 1; j < n; ++j) {
      if (lower > arr[j]) {
        lower = arr[j];
        idx = j;
      }
    }
    if (i != idx) {
      arr[idx] = arr[i];
      arr[i] = lower;
    }
  }

  if (n % 2 == 0) {
    return (arr[med] + arr[med - 1]) / 2;
  } else {
    return arr[med];
  }
}
} // namespace utils

namespace noise {
Mat salt_and_pepper(const Mat &img, double scale) {
  Mat out = img.clone();
  const int total = img.rows * img.cols;
  const int n_dots = static_cast<int>(total * scale);

  for (int i = 0; i < n_dots; ++i) {
    const int y = rand() % img.rows;
    const int x = rand() % img.cols;

    const double c = static_cast<double>(rand()) / RAND_MAX;
    const uchar v = (c >= 0.5) ? 255 : 0;

    out.at<Vec3b>(y, x) = Vec3b(v, v, v);
  }
  return out;
}
} // namespace noise

namespace color {
Mat gray_scale(const Mat &img) {

  Mat out = Mat::zeros(img.rows, img.cols, CV_8UC3);

  for (int y = 0; y < img.rows; ++y) {
    for (int x = 0; x < img.cols; ++x) {
      const Vec3b p = img.at<Vec3b>(y, x);
      const uchar gray =
          static_cast<uchar>(p[0] * 0.114 + p[1] * 0.587 + p[2] * 0.299);
      out.at<Vec3b>(y, x) = Vec3b(gray, gray, gray);
    }
  }
  return out;
}

Mat power_gamma(const Mat &img, const double c, const double gamma) {
  Mat out = Mat::zeros(img.rows, img.cols, CV_8UC3);

  for (int y = 0; y < img.rows; ++y) {
    for (int x = 0; x < img.cols; ++x) {
      Vec3b p = img.at<Vec3b>(y, x);
      Vec3b result;

      for (int ch = 0; ch < 3; ++ch) {
        double r = (double)p[ch] / 255.0;
        double s = c * pow(r, gamma);

        result[ch] = min(255, max(0, (int)round(s * 255)));
      }
      out.at<Vec3b>(y, x) = result;
    }
  }
  return out;
}

Mat limiarization(const Mat &img, const int threshold) {
  Mat out = Mat::zeros(img.rows, img.cols, CV_8UC3);

  for (int y = 0; y < img.rows; ++y) {
    for (int x = 0; x < img.cols; ++x) {
      Vec3b p = img.at<Vec3b>(y, x);

      int gray = p[0] * 0.114 + p[1] * 0.587 +
                 p[2] * 0.299; // convert to grayscale if the pixel is not

      if (gray > threshold) {
        out.at<Vec3b>(y, x) = Vec3b(255, 255, 255);
      }
      // else isn't necessary because the 'out' image is initially filled with
      // zeros
    }
  }
  return out;
}

Mat binarization(const Mat &img) {
  Mat out = Mat::zeros(img.rows, img.cols, CV_8UC3);
  int threshold = 255 / 2;

  for (int y = 0; y < img.rows; ++y) {
    for (int x = 0; x < img.cols; ++x) {
      Vec3b p = img.at<Vec3b>(y, x);

      uchar gray = static_cast<uchar>(
          p[0] * 0.114 + p[1] * 0.587 +
          p[2] * 0.299); // convert to grayscale if the pixel is not

      if (gray > threshold) {
        out.at<Vec3b>(y, x) = Vec3b(255, 255, 255);
      }
    }
  }

  return out;
}

Mat rgb_to_hsv(const Mat &img) {
  Mat out = Mat::zeros(img.rows, img.cols, CV_8UC3);
  for (int y = 0; y < img.rows; ++y) {
    for (int x = 0; x < img.cols; ++x) {
      Vec3b bgr = img.at<Vec3b>(y, x);
      double b = bgr[0] / 255.0;
      double g = bgr[1] / 255.0;
      double r = bgr[2] / 255.0;

      double cmax = std::max({r, g, b});
      double cmin = std::min({r, g, b});
      double delta = cmax - cmin;

      double h = 0.0;
      double s = 0.0;
      double v = cmax;

      if (delta > 0.0) {
        if (cmax == r) {
          h = 60.0 * fmod(((g - b) / delta), 6.0);
        } else if (cmax == g) {
          h = 60.0 * (((b - r) / delta) + 2.0);
        } else if (cmax == b) {
          h = 60.0 * (((r - g) / delta) + 4.0);
        }

        if (h < 0.0)
          h += 360.0;

        s = delta / cmax;
      }

      h = round(h / 2.0);
      s = round(s * 255.0);
      v = round(v * 255.0);

      out.at<Vec3b>(y, x) = Vec3b(h, s, v);
    }
  }
  return out;
}

Mat hsv_to_rgb(const Mat &img) {
  Mat out = Mat::zeros(img.rows, img.cols, CV_8UC3);

  for (int y = 0; y < img.rows; ++y) {
    for (int x = 0; x < img.cols; ++x) {
      Vec3b hsv = img.at<Vec3b>(y, x);

      double h = hsv[0] * 2.0;
      double s = hsv[1] / 255.0;
      double v = hsv[2] / 255.0;

      double c = v * s;
      double xx = c * (1.0 - std::abs(fmod(h / 60.0, 2.0) - 1.0));
      double m = v - c;

      double r = 0.0, g = 0.0, b = 0.0;

      if (h >= 0 && h < 60) {
        r = c;
        g = xx;
        b = 0;
      } else if (h >= 60 && h < 120) {
        r = xx;
        g = c;
        b = 0;
      } else if (h >= 120 && h < 180) {
        r = 0;
        g = c;
        b = xx;
      } else if (h >= 180 && h < 240) {
        r = 0;
        g = xx;
        b = c;
      } else if (h >= 240 && h < 300) {
        r = xx;
        g = 0;
        b = c;
      } else if (h >= 300 && h < 360) {
        r = c;
        g = 0;
        b = xx;
      }

      b = min(255, max(0, (int)round((b + m) * 255.0)));
      g = min(255, max(0, (int)round((g + m) * 255.0)));
      r = min(255, max(0, (int)round((r + m) * 255.0)));

      out.at<Vec3b>(y, x) = Vec3b(b, g, r);
    }
  }

  return out;
}
} // namespace color

namespace pixelwise {
Mat mean_pixelwise(const Mat &img, const int n) {
  vector<Mat> noisy(n); // Build n independently noised copies
  for (int k = 0; k < n; ++k)
    noisy[k] = noise::salt_and_pepper(img, 0.23);

  Mat out = Mat::zeros(img.rows, img.cols, CV_8UC3);

  for (int y = 0; y < img.rows; ++y) {
    for (int x = 0; x < img.cols; ++x) {
      int sum[3] = {0, 0, 0};

      for (int k = 0; k < n; ++k) {
        const Vec3b p = noisy[k].at<Vec3b>(y, x);
        for (int c = 0; c < 3; ++c)
          sum[c] += p[c];
      }

      out.at<Vec3b>(y, x) = Vec3b(sum[0] / n, sum[1] / n, sum[2] / n);
    }
  }
  return out;
}

Mat median_pixelwise(const Mat &img, const int n) {
  vector<Mat> noisy(n); // Build n independently noised copies
  for (int k = 0; k < n; ++k)
    noisy[k] = noise::salt_and_pepper(img, 0.23);

  Mat out = Mat::zeros(img.rows, img.cols, CV_8UC3);

  for (int y = 0; y < img.rows; ++y) {
    for (int x = 0; x < img.cols; ++x) {
      vector<vector<int>> median_arr(3, vector<int>(n));

      for (int k = 0; k < n; ++k) {
        Vec3b p = noisy[k].at<Vec3b>(y, x);

        for (int c = 0; c < 3; ++c) {
          median_arr[c][k] = p[c];
        }
      }
      int b = utils::median(median_arr[0]);
      int g = utils::median(median_arr[1]);
      int r = utils::median(median_arr[2]);

      out.at<Vec3b>(y, x) = Vec3b(b, g, r);
    }
  }
  return out;
}
} // namespace pixelwise

namespace spatial {
Mat sobel(const Mat &img) {
  Mat out = Mat::zeros(img.rows, img.cols, CV_8UC3);

  int y_kernel[9] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
  int x_kernel[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
  int offset = 3 / 2;

  for (int y = offset; y < img.rows - offset; ++y) {
    for (int x = offset; x < img.cols - offset; ++x) {
      int y_sum[3] = {0, 0, 0};
      int x_sum[3] = {0, 0, 0};
      int k = 0;

      for (int ky = -offset; ky <= offset; ++ky) {
        for (int kx = -offset; kx <= offset; ++kx) {

          Vec3b p = img.at<Vec3b>(y + ky, x + kx);

          for (int c = 0; c < 3; c++) {
            y_sum[c] += p[c] * y_kernel[k];
            x_sum[c] += p[c] * x_kernel[k];
          }
          k++;
        }
      }

      Vec3b p;
      for (int c = 0; c < 3; c++) {
        int magnitude = abs(y_sum[c]) + abs(x_sum[c]);
        p[c] = min(255, max(0, magnitude / 4));
      }

      out.at<Vec3b>(y, x) = p;
    }
  }

  return out;
}

Mat laplacian(const Mat &img) {

  Mat out = Mat::zeros(img.rows, img.cols, CV_8UC3);
  int kernel[9] = {-1, -1, -1, -1, 8, -1, -1, -1, -1};
  const int offset = 3 / 2;

  for (int y = offset; y < img.rows - offset; ++y) {
    for (int x = offset; x < img.cols - offset; ++x) {
      int sum[3] = {0, 0, 0};
      int k = 0;

      for (int ky = -offset; ky <= offset; ++ky) {
        for (int kx = -offset; kx <= offset; ++kx) {
          Vec3b p = img.at<Vec3b>(y + ky, x + kx);
          for (int c = 0; c < 3; ++c) {
            sum[c] += p[c] * kernel[k];
          }
          ++k;
        }
      }
      int b = min(255, abs(sum[0] / 8));
      int g = min(255, abs(sum[1] / 8));
      int r = min(255, abs(sum[2] / 8));

      out.at<Vec3b>(y, x) = Vec3b(b, g, r);
    }
  }
  return out;
}

Mat mean_convolution(const Mat &img, int kernel_size) {
  Mat out = Mat::zeros(img.rows, img.cols, CV_8UC3);

  const int offset = kernel_size / 2;
  const int kernel_area = kernel_size * kernel_size;

  for (int y = offset; y < img.rows - offset; ++y) {
    for (int x = offset; x < img.cols - offset; ++x) {
      int sum[3] = {0, 0, 0};

      for (int ky = -offset; ky <= offset; ++ky) {
        for (int kx = -offset; kx <= offset; ++kx) {
          const Vec3b p = img.at<Vec3b>(y + ky, x + kx);
          for (int c = 0; c < 3; ++c)
            sum[c] += p[c];
        }
      }

      out.at<Vec3b>(y, x) = Vec3b(sum[0] / kernel_area, sum[1] / kernel_area,
                                  sum[2] / kernel_area);
    }
  }
  return out;
}
} // namespace spatial

namespace histogram {

vector<int> cum_frequency(const Mat &img) {
  vector<int> hist(256, 0);

  for (int y = 0; y < img.rows; ++y)
    for (int x = 0; x < img.cols; ++x)
      hist[img.at<Vec3b>(y, x)[0]]++;

  vector<int> cdf(256, 0);
  cdf[0] = hist[0];
  for (int i = 1; i < 256; ++i)
    cdf[i] = cdf[i - 1] + hist[i];

  return cdf;
}

Mat histogram_equalization(const Mat &img) {
  Mat out = Mat::zeros(img.rows, img.cols, CV_8UC3);

  const vector<int> cdf = cum_frequency(img);

  // Find the first non-zero CDF value
  int cdf_min = 0;
  for (int i = 0; i < 256; ++i) {
    if (cdf[i] > 0) {
      cdf_min = cdf[i];
      break;
    }
  }

  const double denom = static_cast<double>(img.rows * img.cols) - cdf_min;

  for (int y = 0; y < img.rows; ++y) {
    for (int x = 0; x < img.cols; ++x) {
      const int p = img.at<Vec3b>(y, x)[0];

      const uchar mapped =
          (denom > 0)
              ? static_cast<uchar>(clamp(
                    static_cast<int>(round((cdf[p] - cdf_min) / denom * 255.0)),
                    0, 255))
              : 0;

      out.at<Vec3b>(y, x) = Vec3b(mapped, mapped, mapped);
    }
  }
  return out;
}
} // namespace histogram

namespace interpolation {
Mat nearest_neighbor(const Mat &img, double scale) {
  const int rows_out = static_cast<int>(img.rows * scale);
  const int cols_out = static_cast<int>(img.cols * scale);

  Mat out = Mat::zeros(rows_out, cols_out, CV_8UC3);

  for (int y = 0; y < rows_out; ++y) {
    for (int x = 0; x < cols_out; ++x) {
      const int y_src = min(static_cast<int>(round(y / scale)), img.rows - 1);
      const int x_src = min(static_cast<int>(round(x / scale)), img.cols - 1);
      out.at<Vec3b>(y, x) = img.at<Vec3b>(y_src, x_src);
    }
  }
  return out;
}

Mat bilinear(const Mat &img, double scale) {
  const int rows_out = static_cast<int>(img.rows * scale);
  const int cols_out = static_cast<int>(img.cols * scale);

  Mat out = Mat::zeros(rows_out, cols_out, CV_8UC3);

  for (int y = 0; y < rows_out; ++y) {
    for (int x = 0; x < cols_out; ++x) {
      const double x_src = x / scale;
      const double y_src = y / scale;

      const int x1 = static_cast<int>(floor(x_src));
      const int y1 = static_cast<int>(floor(y_src));
      const int x2 = min(x1 + 1, img.cols - 1);
      const int y2 = min(y1 + 1, img.rows - 1);

      const double dx = x_src - x1;
      const double dy = y_src - y1;

      const Vec3b p00 = img.at<Vec3b>(y1, x1);
      const Vec3b p10 = img.at<Vec3b>(y1, x2);
      const Vec3b p01 = img.at<Vec3b>(y2, x1);
      const Vec3b p11 = img.at<Vec3b>(y2, x2);

      Vec3b result;
      for (int c = 0; c < 3; ++c) {
        const double val = p00[c] + (p10[c] - p00[c]) * dx +
                           (p01[c] - p00[c]) * dy +
                           (p11[c] + p00[c] - p10[c] - p01[c]) * dx * dy;
        result[c] = static_cast<uchar>(clamp(static_cast<int>(val), 0, 255));
      }
      out.at<Vec3b>(y, x) = result;
    }
  }
  return out;
}
} // namespace interpolation
} // namespace DIP
