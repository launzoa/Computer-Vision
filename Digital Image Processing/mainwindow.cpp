#include "mainwindow.h"
#include "src/operations.h"

#include <QApplication>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QImage>
#include <QPixmap>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QFile>

using namespace cv;

static QString operationDescription(const QString &op) {
  static const QMap<QString, QString> descriptions = {
      {"Gray Scale", "Converts the image to grayscale using luminance weights "
                     "(R·0.299 + G·0.587 + B·0.114)."},
      {"Binarization", "Converts image to absolute Black and White based on a "
                       "fixed median threshold."},
      {"Limiarization", "Converts image to absolute Black and White based on a "
                        "user-defined threshold."},
      {"Gamma Correction", "Applies Power-Law transformation to adjust "
                           "non-linear brightness (S = c * r^gamma)."},
      {"RGB to HSV",
       "Converts the image to the Hue, Saturation, and Value color space."},
      {"HSV to RGB", "Converts the image from the Hue, Saturation, and Value "
                     "color space back to RGB."},
      {"Mean (pixel-wise)", "Reduces salt-and-pepper noise by averaging 10 "
                            "independently noised copies."},
      {"Median (pixel-wise)",
       "Removes noise by taking the median of 10 independently noised copies."},
      {"Nearest Neighbor", "Rescales the image by snapping each output pixel "
                           "to its nearest source pixel."},
      {"Bilinear", "Rescales the image by interpolating between the four "
                   "nearest source pixels."},
      {"Histogram Equalization", "Enhances global contrast by redistributing "
                                 "pixel intensities uniformly."},
      {"Mean Convolution",
       "Smooths the image with a uniform averaging kernel of the chosen size."},
      {"Sobel Edge Detection",
       "Detects horizontal and vertical edges using directional gradients."},
      {"Laplacian", "Detects areas of rapid intensity change using a "
                    "second-derivative kernel."},
      {"Dilation", "Expands white regions in a binary image using max filter."},
      {"Erosion", "Shrinks white regions in a binary image using min filter."},
      {"Otsu Binarization", "Binarizes image using optimal Otsu thresholding."},
      {"Pseudo Coloring", "Applies a Jet-like colormap to a grayscale image."},
      {"Equalize HSL", "Equalizes only the L channel of the HSL color model."},
      {"Min Filter", "Replaces pixel with minimum value in the neighborhood."},
      {"Max Filter", "Replaces pixel with maximum value in the neighborhood."},
      {"Midpoint Filter", "Replaces pixel with (min+max)/2 in the neighborhood."},
      {"DCT Forward", "Computes the 2D DCT of a 128x128 representation."},
      {"DCT Inverse", "Computes the 2D IDCT from the stored DCT matrix."},
      {"DCT Low-Pass", "Applies an ideal low-pass filter in the frequency domain."},
      {"DCT High-Pass", "Applies an ideal high-pass filter in the frequency domain."},
      {"DCT Add Noise", "Adds a specific frequency noise to the DCT matrix."}};
  return descriptions.value(op, QString());
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  setupUI();
  applyStyles();
  loadDefaultImage();
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUI() {
  setWindowTitle("Image Filtering Pipeline");
  resize(1150,
         750); // Aumentei um pouco a largura para caber os novos parâmetros

  auto *central = new QWidget(this);
  setCentralWidget(central);

  // ── Top bar ───────────────────────────────────────────────────────────
  btnOpen = new QPushButton("📂  Open Image");
  btnOpen->setFixedHeight(34);
  btnOpen->setObjectName("btnOpen");

  comboOperations = new QComboBox();
  comboOperations->setFixedHeight(34);
  comboOperations->addItems(
      {"Gray Scale", "Binarization", "Limiarization", "Gamma Correction",
       "RGB to HSV", "HSV to RGB",
       "Mean (pixel-wise)", "Median (pixel-wise)", "Nearest Neighbor",
       "Bilinear", "Histogram Equalization", "Mean Convolution",
       "Sobel Edge Detection", "Laplacian", "Dilation", "Erosion",
       "Otsu Binarization", "Pseudo Coloring", "Equalize HSL",
       "Min Filter", "Max Filter", "Midpoint Filter",
       "DCT Forward", "DCT Inverse", "DCT Low-Pass", "DCT High-Pass", "DCT Add Noise"});

  btnApply = new QPushButton("▶  Apply");
  btnApply->setFixedHeight(34);
  btnApply->setObjectName("btnApply");

  btnReset = new QPushButton("↺  Reset");
  btnReset->setFixedHeight(34);
  btnReset->setObjectName("btnReset");

  auto *topBar = new QHBoxLayout();
  topBar->setSpacing(8);
  topBar->addWidget(btnOpen);
  topBar->addSpacing(8);
  topBar->addWidget(comboOperations, 1);
  topBar->addSpacing(8);
  topBar->addWidget(btnApply);
  topBar->addWidget(btnReset);

  lblDescription = new QLabel();
  lblDescription->setWordWrap(true);
  lblDescription->setObjectName("lblDescription");

  // ── Parameter panel ───────────────────────────────────────────────────
  groupParams = new QGroupBox("Parameters");
  auto *paramLayout = new QHBoxLayout(groupParams);
  paramLayout->setContentsMargins(12, 4, 12, 4);

  lblScaleHint = new QLabel("Scale factor:");
  spinScale = new QDoubleSpinBox();
  spinScale->setRange(0.10, 4.00);
  spinScale->setValue(0.50);
  spinScale->setFixedWidth(90);

  lblKernelHint = new QLabel("Kernel size:");
  spinKernel = new QSpinBox();
  spinKernel->setRange(3, 15);
  spinKernel->setSingleStep(2);
  spinKernel->setValue(3);
  spinKernel->setFixedWidth(76);
  connect(spinKernel, QOverload<int>::of(&QSpinBox::valueChanged), this,
          [this](int v) {
            if (v % 2 == 0)
              spinKernel->setValue(v + 1);
          });

  lblThresholdHint = new QLabel("Threshold:");
  spinThreshold = new QSpinBox();
  spinThreshold->setRange(0, 255);
  spinThreshold->setValue(127);
  spinThreshold->setFixedWidth(76);

  // NOVO: Parâmetro da Constante C do Gamma
  lblGammaCHint = new QLabel("Constant (C):");
  spinGammaC = new QDoubleSpinBox();
  spinGammaC->setRange(0.1, 10.0);
  spinGammaC->setSingleStep(0.1);
  spinGammaC->setValue(1.0);
  spinGammaC->setFixedWidth(76);

  lblGammaHint = new QLabel("Gamma (γ):");
  spinGamma = new QDoubleSpinBox();
  spinGamma->setRange(0.01, 10.0);
  spinGamma->setSingleStep(0.1);
  spinGamma->setValue(2.2);
  spinGamma->setFixedWidth(76);

  // --- Frequency Domain Parameters ---
  lblCutoffHint = new QLabel("Cutoff Freq:");
  spinCutoff = new QSpinBox();
  spinCutoff->setRange(1, 128);
  spinCutoff->setValue(64);
  spinCutoff->setFixedWidth(60);

  lblNoiseUHint = new QLabel("Noise U:");
  spinNoiseU = new QSpinBox();
  spinNoiseU->setRange(0, 127);
  spinNoiseU->setValue(64);
  spinNoiseU->setFixedWidth(50);

  lblNoiseVHint = new QLabel("Noise V:");
  spinNoiseV = new QSpinBox();
  spinNoiseV->setRange(0, 127);
  spinNoiseV->setValue(64);
  spinNoiseV->setFixedWidth(50);

  lblNoiseMagHint = new QLabel("Magnitude:");
  spinNoiseMag = new QDoubleSpinBox();
  spinNoiseMag->setRange(0.0, 1000000.0);
  spinNoiseMag->setValue(10000.0);
  spinNoiseMag->setFixedWidth(90);

  paramLayout->addWidget(lblScaleHint);
  paramLayout->addWidget(spinScale);
  paramLayout->addWidget(lblKernelHint);
  paramLayout->addWidget(spinKernel);
  paramLayout->addWidget(lblThresholdHint);
  paramLayout->addWidget(spinThreshold);

  // Adiciona os dois parâmetros do Gamma no layout
  paramLayout->addWidget(lblGammaCHint);
  paramLayout->addWidget(spinGammaC);
  paramLayout->addWidget(lblGammaHint);
  paramLayout->addWidget(spinGamma);

  paramLayout->addWidget(lblCutoffHint);
  paramLayout->addWidget(spinCutoff);
  paramLayout->addWidget(lblNoiseUHint);
  paramLayout->addWidget(spinNoiseU);
  paramLayout->addWidget(lblNoiseVHint);
  paramLayout->addWidget(spinNoiseV);
  paramLayout->addWidget(lblNoiseMagHint);
  paramLayout->addWidget(spinNoiseMag);

  paramLayout->addStretch();

  // ── Hover Panel ───────────────────────────────────────────────────────
  groupPixelInfo = new QGroupBox("Pixel Inspector (Hover over Result Image)");
  auto *pixelLayout = new QHBoxLayout(groupPixelInfo);
  txtPos = new QLineEdit("-");
  txtPos->setReadOnly(true);
  txtPos->setFixedWidth(100);
  txtRGB = new QLineEdit("-");
  txtRGB->setReadOnly(true);
  txtRGB->setFixedWidth(120);
  txtHSV = new QLineEdit("-");
  txtHSV->setReadOnly(true);
  txtHSV->setFixedWidth(120);
  txtGray = new QLineEdit("-");
  txtGray->setReadOnly(true);
  txtGray->setFixedWidth(120);

  pixelLayout->addWidget(new QLabel("Position:"));
  pixelLayout->addWidget(txtPos);
  pixelLayout->addWidget(new QLabel("RGB:"));
  pixelLayout->addWidget(txtRGB);
  pixelLayout->addWidget(new QLabel("HSV (OpenCV):"));
  pixelLayout->addWidget(txtHSV);
  pixelLayout->addWidget(new QLabel("Luminance:"));
  pixelLayout->addWidget(txtGray);
  pixelLayout->addStretch();

  // ── Image panes ───────────────────────────────────────────────────────
  lblOriginal = new QLabel("Input");
  lblOriginal->setAlignment(Qt::AlignCenter);
  lblOriginal->setObjectName("imagePane");
  lblOriginal->setMinimumSize(420, 400);

  auto *lblArrow = new QLabel("→");
  lblArrow->setAlignment(Qt::AlignCenter);
  lblArrow->setFont(QFont("Arial", 40, QFont::Bold));
  lblArrow->setObjectName("arrow");
  lblArrow->setFixedWidth(60);

  lblResult = new QLabel("Result will appear here.");
  lblResult->setAlignment(Qt::AlignCenter);
  lblResult->setObjectName("imagePane");
  lblResult->setMinimumSize(420, 400);

  lblResult->setMouseTracking(true);
  lblResult->installEventFilter(this);

  auto *imagesRow = new QHBoxLayout();
  imagesRow->addWidget(lblOriginal, 1);
  imagesRow->addWidget(lblArrow, 0);
  imagesRow->addWidget(lblResult, 1);

  // ── Main layout ───────────────────────────────────────────────────────
  auto *mainLayout = new QVBoxLayout(central);
  mainLayout->addLayout(topBar);
  mainLayout->addWidget(lblDescription);
  mainLayout->addWidget(groupParams);
  mainLayout->addWidget(groupPixelInfo);
  mainLayout->addLayout(imagesRow, 1);

  statusBar()->showMessage("Ready.");

  connect(btnOpen, &QPushButton::clicked, this, &MainWindow::onOpenClicked);
  connect(btnApply, &QPushButton::clicked, this, &MainWindow::onApplyClicked);
  connect(btnReset, &QPushButton::clicked, this, &MainWindow::onResetClicked);
  connect(comboOperations, &QComboBox::currentTextChanged, this,
          &MainWindow::onOperationChanged);

  onOperationChanged(comboOperations->currentText());
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
  if (watched == lblResult && event->type() == QEvent::MouseMove) {
    auto *mouseEvent = static_cast<QMouseEvent *>(event);
    QPixmap pix = lblResult->pixmap();

    if (!matProcessed.empty() && !pix.isNull()) {
      int offsetX = (lblResult->width() - pix.width()) / 2;
      int offsetY = (lblResult->height() - pix.height()) / 2;
      int mouseX = mouseEvent->pos().x() - offsetX;
      int mouseY = mouseEvent->pos().y() - offsetY;

      if (mouseX >= 0 && mouseX < pix.width() && mouseY >= 0 &&
          mouseY < pix.height()) {
        int imgX = std::clamp((mouseX * matProcessed.cols) / pix.width(), 0,
                              matProcessed.cols - 1);
        int imgY = std::clamp((mouseY * matProcessed.rows) / pix.height(), 0,
                              matProcessed.rows - 1);

        Vec3b p = matProcessed.at<Vec3b>(imgY, imgX);
        int b = p[0], g = p[1], r = p[2];
        int gray = static_cast<int>(0.299 * r + 0.587 * g + 0.114 * b);

        Mat pxMat(1, 1, CV_8UC3, Scalar(b, g, r));
        Mat hsvMat;
        cvtColor(pxMat, hsvMat, COLOR_BGR2HSV);
        Vec3b hsv = hsvMat.at<Vec3b>(0, 0);

        txtPos->setText(QString("X: %1, Y: %2").arg(imgX).arg(imgY));
        txtRGB->setText(QString("R: %1, G: %2, B: %3").arg(r).arg(g).arg(b));
        txtHSV->setText(QString("H: %1, S: %2, V: %3")
                            .arg(hsv[0] * 2)
                            .arg(hsv[1])
                            .arg(hsv[2]));
        txtGray->setText(QString::number(gray));
      } else {
        txtPos->setText("Out of bounds");
        txtRGB->setText("-");
        txtHSV->setText("-");
        txtGray->setText("-");
      }
    }
  }
  return QMainWindow::eventFilter(watched, event);
}

void MainWindow::updatePreview() {
  if (matCurrent.empty())
    return;

  QString op = comboOperations->currentText();
  matPreview = matCurrent.clone();

  if (op == "Mean (pixel-wise)" || op == "Median (pixel-wise)") {
    matPreview = DIP::noise::salt_and_pepper(matCurrent, 0.23);
  } else if (op == "Mean Convolution") {
    matPreview = DIP::noise::salt_and_pepper(matCurrent, 0.10);
  } else if (op == "Histogram Equalization") {
    matPreview = DIP::color::gray_scale(matCurrent);
  }

  showImage(lblOriginal, matPreview);
}

void MainWindow::onApplyClicked() {
  if (matCurrent.empty())
    return;

  const QString op = comboOperations->currentText();
  statusBar()->showMessage("Applying " + op + " …");

  btnApply->setEnabled(false);
  QApplication::processEvents();

  if (op == "Gray Scale")
    matProcessed = DIP::color::gray_scale(matCurrent);
  else if (op == "Binarization")
    matProcessed = DIP::color::binarization(matCurrent);
  else if (op == "Limiarization")
    matProcessed =
        DIP::color::limiarization(matCurrent, spinThreshold->value());

  else if (op == "Gamma Correction")
    matProcessed = DIP::color::power_gamma(matCurrent, spinGammaC->value(),
                                           spinGamma->value());

  else if (op == "RGB to HSV")
    matProcessed = DIP::color::rgb_to_hsv(matCurrent);

  else if (op == "HSV to RGB")
    matProcessed = DIP::color::hsv_to_rgb(matCurrent);

  else if (op == "Mean (pixel-wise)")
    matProcessed = DIP::pixelwise::mean_pixelwise(matCurrent, 10);
  else if (op == "Median (pixel-wise)")
    matProcessed = DIP::pixelwise::median_pixelwise(matCurrent, 10);
  else if (op == "Nearest Neighbor")
    matProcessed =
        DIP::interpolation::nearest_neighbor(matCurrent, spinScale->value());
  else if (op == "Bilinear")
    matProcessed = DIP::interpolation::bilinear(matCurrent, spinScale->value());
  else if (op == "Histogram Equalization")
    matProcessed = DIP::histogram::histogram_equalization(matPreview);
  else if (op == "Mean Convolution")
    matProcessed =
        DIP::spatial::mean_convolution(matPreview, spinKernel->value());
  else if (op == "Sobel Edge Detection")
    matProcessed = DIP::spatial::sobel(matCurrent);
  else if (op == "Laplacian")
    matProcessed = DIP::spatial::laplacian(matCurrent);
  else if (op == "Dilation")
    matProcessed = DIP::morphology::dilation(matCurrent, spinKernel->value());
  else if (op == "Erosion")
    matProcessed = DIP::morphology::erosion(matCurrent, spinKernel->value());
  else if (op == "Otsu Binarization")
    matProcessed = DIP::color::otsu_thresholding(matCurrent);
  else if (op == "Pseudo Coloring")
    matProcessed = DIP::color::pseudo_color(matCurrent);
  else if (op == "Equalize HSL")
    matProcessed = DIP::color::equalize_hsl(matCurrent);
  else if (op == "Min Filter")
    matProcessed = DIP::spatial::min_filter(matCurrent, spinKernel->value());
  else if (op == "Max Filter")
    matProcessed = DIP::spatial::max_filter(matCurrent, spinKernel->value());
  else if (op == "Midpoint Filter")
    matProcessed = DIP::spatial::midpoint_filter(matCurrent, spinKernel->value());
  else if (op == "DCT Forward") {
    matDCT = DIP::frequency::dct_2d(matCurrent);
    Mat visual(matDCT.rows, matDCT.cols, CV_8UC3);
    for(int y=0; y<matDCT.rows; ++y) {
      for(int x=0; x<matDCT.cols; ++x) {
        double val = matDCT.at<double>(y, x);
        int log_val = std::clamp(static_cast<int>(20.0 * log10(1.0 + abs(val))), 0, 255);
        visual.at<Vec3b>(y, x) = Vec3b(log_val, log_val, log_val);
      }
    }
    matProcessed = visual;
  }
  else if (op == "DCT Inverse") {
    if(!matDCT.empty()) matProcessed = DIP::frequency::idct_2d(matDCT);
    else matProcessed = matCurrent.clone();
  }
  else if (op == "DCT Low-Pass") {
    matDCT = DIP::frequency::dct_2d(matCurrent);
    Mat filtered = DIP::frequency::low_pass_filter(matDCT, spinCutoff->value());
    matProcessed = DIP::frequency::idct_2d(filtered);
  }
  else if (op == "DCT High-Pass") {
    matDCT = DIP::frequency::dct_2d(matCurrent);
    Mat filtered = DIP::frequency::high_pass_filter(matDCT, spinCutoff->value());
    matProcessed = DIP::frequency::idct_2d(filtered);
  }
  else if (op == "DCT Add Noise") {
    matDCT = DIP::frequency::dct_2d(matCurrent);
    Mat noisy = DIP::frequency::add_frequency_noise(matDCT, spinNoiseU->value(), spinNoiseV->value(), spinNoiseMag->value());
    matProcessed = DIP::frequency::idct_2d(noisy);
  }

  showImage(lblResult, matProcessed);
  btnApply->setEnabled(true);

  statusBar()->showMessage("Done: " + op + " applied. Change tool to commit.");
}

void MainWindow::onOperationChanged(const QString &operation) {
  if (!matProcessed.empty()) {
    matCurrent = matProcessed.clone();
    matProcessed = Mat();
    lblResult->clear();
    lblResult->setText("Result will appear here\nafter applying an operation.");
  }

  lblDescription->setText(operationDescription(operation));
  updateParamPanel(operation);
  updatePreview();
}

void MainWindow::updateParamPanel(const QString &operation) {
  const bool needsScale =
      (operation == "Nearest Neighbor" || operation == "Bilinear");
  const bool needsKernel = (operation == "Mean Convolution" || operation == "Dilation" || operation == "Erosion" || operation == "Min Filter" || operation == "Max Filter" || operation == "Midpoint Filter");
  const bool needsThreshold = (operation == "Limiarization");
  const bool needsGamma = (operation == "Gamma Correction");
  const bool needsCutoff = (operation == "DCT Low-Pass" || operation == "DCT High-Pass");
  const bool needsNoise = (operation == "DCT Add Noise");

  lblScaleHint->setVisible(needsScale);
  spinScale->setVisible(needsScale);
  lblKernelHint->setVisible(needsKernel);
  spinKernel->setVisible(needsKernel);
  lblThresholdHint->setVisible(needsThreshold);
  spinThreshold->setVisible(needsThreshold);

  lblGammaCHint->setVisible(needsGamma);
  spinGammaC->setVisible(needsGamma);
  lblGammaHint->setVisible(needsGamma);
  spinGamma->setVisible(needsGamma);

  lblCutoffHint->setVisible(needsCutoff);
  spinCutoff->setVisible(needsCutoff);
  
  lblNoiseUHint->setVisible(needsNoise);
  spinNoiseU->setVisible(needsNoise);
  lblNoiseVHint->setVisible(needsNoise);
  spinNoiseV->setVisible(needsNoise);
  lblNoiseMagHint->setVisible(needsNoise);
  spinNoiseMag->setVisible(needsNoise);

  groupParams->setVisible(needsScale || needsKernel || needsThreshold ||
                          needsGamma || needsCutoff || needsNoise);
}

void MainWindow::onResetClicked() {
  if (matOriginal.empty())
    return;
  matCurrent = matOriginal.clone();
  matProcessed = Mat();
  lblResult->clear();
  lblResult->setText("Result cleared.");
  statusBar()->showMessage("Result reset to original image.");
  updatePreview();
}

void MainWindow::onOpenClicked() {
  auto fileDialogCallback = [this](const QString &fileName,
                                   const QByteArray &fileContent) {
    if (fileName.isEmpty() || fileContent.isEmpty())
      return;

    std::vector<uchar> buf(fileContent.begin(), fileContent.end());
    Mat img = cv::imdecode(buf, cv::IMREAD_COLOR);
    if (img.empty()) {
      statusBar()->showMessage("Error: could not decode " + fileName);
      return;
    }

    matOriginal = img;
    matCurrent = img.clone();
    matProcessed = Mat();

    lblResult->clear();
    statusBar()->showMessage("Loaded: " + fileName);
    updatePreview();
  };

  QFileDialog::getOpenFileContent(
      "Images (*.png *.jpg *.jpeg *.bmp *.tiff);;All Files (*)",
      fileDialogCallback);
}

void MainWindow::applyStyles() {
  setStyleSheet(R"(
        QMainWindow, QWidget { background-color: #F4F6F8; font-family: 'Segoe UI', Arial, sans-serif; font-size: 13px; color: #2C3E50; }
        QPushButton { border: 1px solid #BDC3C7; border-radius: 6px; padding: 0 14px; background: #FFFFFF; color: #2C3E50; }
        QPushButton:hover { background: #EAF0FB; border-color: #3498DB; }
        QPushButton#btnApply { background: #2980B9; color: #FFFFFF; border: none; font-weight: bold; }
        QPushButton#btnApply:hover { background: #3498DB; }
        QComboBox, QSpinBox, QDoubleSpinBox, QLineEdit { border: 1px solid #BDC3C7; border-radius: 4px; padding: 2px 6px; background: #FFFFFF; }
        QComboBox:hover, QLineEdit:focus { border-color: #3498DB; }
        QLabel#lblDescription { color: #7F8C8D; font-style: italic; }
        QLabel#imagePane { background-color: #FFFFFF; border: 2px solid #D5DBE1; border-radius: 12px; }
        QGroupBox { border: 1px solid #D5DBE1; border-radius: 6px; margin-top: 6px; background: #FFFFFF; }
        QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 6px; color: #5D6D7E; font-weight: bold; }
    )");
}

void MainWindow::loadDefaultImage() {
  QFile file(":/assets/Lena320x240.jpg");
  if (file.open(QIODevice::ReadOnly)) {
    QByteArray fileContent = file.readAll();
    std::vector<uchar> buf(fileContent.begin(), fileContent.end());
    matOriginal = cv::imdecode(buf, cv::IMREAD_COLOR);
  }

  if (matOriginal.empty()) {
    matOriginal = Mat(400, 400, CV_8UC3);
    for (int y = 0; y < matOriginal.rows; ++y)
      for (int x = 0; x < matOriginal.cols; ++x)
        matOriginal.at<Vec3b>(y, x) =
            Vec3b(200 - y / 3, 80 + x / 5, 180 + y / 8);
  }
  matCurrent = matOriginal.clone();
  updatePreview();
}

void MainWindow::showImage(QLabel *label, const Mat &mat) {
  if (mat.empty())
    return;
  QImage qimg;
  if (mat.channels() == 3) {
    Mat rgb;
    cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
    qimg = QImage(rgb.data, rgb.cols, rgb.rows, static_cast<int>(rgb.step),
                  QImage::Format_RGB888)
               .copy();
  } else {
    qimg = QImage(mat.data, mat.cols, mat.rows, static_cast<int>(mat.step),
                  QImage::Format_Grayscale8)
               .copy();
  }
  label->setPixmap(QPixmap::fromImage(qimg).scaled(
      label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void MainWindow::resizeEvent(QResizeEvent *event) {
  QMainWindow::resizeEvent(event);
  updatePreview();
  if (!matProcessed.empty())
    showImage(lblResult, matProcessed);
}
