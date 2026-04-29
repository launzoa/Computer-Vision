#include "mainwindow.h"
#include "src/operations.h"

#include <QApplication>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QImage>
#include <QPixmap>
#include <QStatusBar>
#include <QVBoxLayout>

using namespace cv;

// ─────────────────────────────────────────────────────────────────────────────
// Helper: one-line description shown below the combo box
// ─────────────────────────────────────────────────────────────────────────────

static QString operationDescription(const QString &op) {
  static const QMap<QString, QString> descriptions = {
      {"Gray Scale", "Converts the image to grayscale using luminance weights "
                     "(R·0.299 + G·0.587 + B·0.114)."},
      {"Mean (pixel-wise)",
       "Reduces salt-and-pepper noise by averaging 10 independently "
       "noised copies of the image, pixel by pixel."},
      {"Nearest Neighbor",
       "Rescales the image by snapping each output pixel to its nearest "
       "source pixel. Fast, but produces a blocky result at low scales."},
      {"Bilinear",
       "Rescales the image by interpolating between the four nearest "
       "source pixels — smoother than nearest neighbor."},
      {"Histogram Equalization",
       "Enhances global contrast by redistributing pixel intensities "
       "uniformly across the 0–255 range (applied on a grayscale version)."},
      {"Mean Convolution",
       "Adds 10 % salt-and-pepper noise, then smooths it with a uniform "
       "averaging kernel of the chosen size."},
  };
  return descriptions.value(op, QString());
}

// ─────────────────────────────────────────────────────────────────────────────
// Constructor / Destructor
// ─────────────────────────────────────────────────────────────────────────────

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  setupUI();
  applyStyles();
  loadDefaultImage();
}

MainWindow::~MainWindow() = default;

// ─────────────────────────────────────────────────────────────────────────────
// UI setup
// ─────────────────────────────────────────────────────────────────────────────

void MainWindow::setupUI() {
  setWindowTitle("Image Filtering");
  resize(1050, 660);

  auto *central = new QWidget(this);
  setCentralWidget(central);

  // ── Top bar ───────────────────────────────────────────────────────────
  btnOpen = new QPushButton("📂  Open Image");
  btnOpen->setFixedHeight(34);
  btnOpen->setObjectName("btnOpen");

  comboOperations = new QComboBox();
  comboOperations->setFixedHeight(34);
  comboOperations->addItems({
      "Gray Scale",
      "Mean (pixel-wise)",
      "Nearest Neighbor",
      "Bilinear",
      "Histogram Equalization",
      "Mean Convolution",
  });

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

  // ── Description label ─────────────────────────────────────────────────
  lblDescription = new QLabel();
  lblDescription->setWordWrap(true);
  lblDescription->setObjectName("lblDescription");

  // ── Parameter panel ───────────────────────────────────────────────────
  groupParams = new QGroupBox("Parameters");
  groupParams->setObjectName("groupParams");

  lblScaleHint = new QLabel("Scale factor:");
  spinScale = new QDoubleSpinBox();
  spinScale->setRange(0.10, 4.00);
  spinScale->setSingleStep(0.10);
  spinScale->setValue(0.50);
  spinScale->setDecimals(2);
  spinScale->setSuffix("×");
  spinScale->setFixedWidth(90);

  lblKernelHint = new QLabel("Kernel size:");
  spinKernel = new QSpinBox();
  spinKernel->setRange(3, 15);
  spinKernel->setSingleStep(2); // stays odd
  spinKernel->setValue(3);
  spinKernel->setSuffix(" px");
  spinKernel->setFixedWidth(76);

  // Snap to odd on manual input
  connect(spinKernel, QOverload<int>::of(&QSpinBox::valueChanged), this,
          [this](int v) {
            if (v % 2 == 0)
              spinKernel->setValue(v + 1);
          });

  auto *paramLayout = new QHBoxLayout(groupParams);
  paramLayout->setContentsMargins(12, 4, 12, 4);
  paramLayout->addWidget(lblScaleHint);
  paramLayout->addWidget(spinScale);
  paramLayout->addSpacing(20);
  paramLayout->addWidget(lblKernelHint);
  paramLayout->addWidget(spinKernel);
  paramLayout->addStretch();

  // ── Image panes ───────────────────────────────────────────────────────
  lblOriginal = new QLabel("Open an image to get started.");
  lblOriginal->setAlignment(Qt::AlignCenter);
  lblOriginal->setWordWrap(true);
  lblOriginal->setObjectName("imagePane");
  lblOriginal->setMinimumSize(420, 400);

  auto *lblArrow = new QLabel("→");
  lblArrow->setAlignment(Qt::AlignCenter);
  lblArrow->setFont(QFont("Arial", 40, QFont::Bold));
  lblArrow->setObjectName("arrow");
  lblArrow->setFixedWidth(60);

  lblResult =
      new QLabel("Result will appear here\nafter applying an operation.");
  lblResult->setAlignment(Qt::AlignCenter);
  lblResult->setWordWrap(true);
  lblResult->setObjectName("imagePane");
  lblResult->setMinimumSize(420, 400);

  auto *imagesRow = new QHBoxLayout();
  imagesRow->addWidget(lblOriginal, 1);
  imagesRow->addWidget(lblArrow, 0);
  imagesRow->addWidget(lblResult, 1);

  // ── Main layout ───────────────────────────────────────────────────────
  auto *mainLayout = new QVBoxLayout(central);
  mainLayout->setContentsMargins(16, 14, 16, 14);
  mainLayout->setSpacing(10);
  mainLayout->addLayout(topBar);
  mainLayout->addWidget(lblDescription);
  mainLayout->addWidget(groupParams);
  mainLayout->addLayout(imagesRow, 1);

  statusBar()->showMessage("Ready.");

  // ── Signals ───────────────────────────────────────────────────────────
  connect(btnOpen, &QPushButton::clicked, this, &MainWindow::onOpenClicked);
  connect(btnApply, &QPushButton::clicked, this, &MainWindow::onApplyClicked);
  connect(btnReset, &QPushButton::clicked, this, &MainWindow::onResetClicked);
  connect(comboOperations, &QComboBox::currentTextChanged, this,
          &MainWindow::onOperationChanged);

  // Initialise description and param panel for the default selection
  onOperationChanged(comboOperations->currentText());
}

void MainWindow::applyStyles() {
  setStyleSheet(R"(
        QMainWindow, QWidget {
            background-color: #F4F6F8;
            font-family: 'Segoe UI', Arial, sans-serif;
            font-size: 13px;
            color: #2C3E50;
        }

        /* ── Buttons ─────────────────────────────────── */
        QPushButton {
            border: 1px solid #BDC3C7;
            border-radius: 6px;
            padding: 0 14px;
            background: #FFFFFF;
            color: #2C3E50;
        }
        QPushButton:hover   { background: #EAF0FB; border-color: #3498DB; }
        QPushButton:pressed { background: #D6EAF8; }

        QPushButton#btnApply {
            background: #2980B9;
            color: #FFFFFF;
            border: none;
            font-weight: bold;
        }
        QPushButton#btnApply:hover   { background: #3498DB; color: #FFFFFF; }
        QPushButton#btnApply:pressed { background: #1F618D; color: #FFFFFF; }
        QPushButton#btnApply:disabled { background: #95A5A6; color: #FFFFFF; }

        QPushButton#btnReset {
            background: #ECF0F1;
            color: #555555;
            border: 1px solid #BDC3C7;
        }
        QPushButton#btnReset:hover { background: #D5DBDB; color: #2C3E50; }

        /* ── Combo box ───────────────────────────────── */
        QComboBox {
            border: 1px solid #BDC3C7;
            border-radius: 6px;
            padding: 0 10px;
            background: #FFFFFF;
            color: #2C3E50;
        }
        QComboBox:hover { border-color: #3498DB; }
        QComboBox::drop-down { border: none; width: 20px; }

        /* Dropdown list */
        QComboBox QAbstractItemView {
            background: #FFFFFF;
            color: #2C3E50;
            selection-background-color: #3498DB;
            selection-color: #FFFFFF;
            border: 1px solid #BDC3C7;
            outline: none;
        }

        /* ── Labels ──────────────────────────────────── */
        QLabel { color: #2C3E50; background: transparent; }

        QLabel#lblDescription {
            color: #7F8C8D;
            font-style: italic;
            padding: 0 2px;
        }

        QLabel#imagePane {
            background-color: #FFFFFF;
            border: 2px solid #D5DBE1;
            border-radius: 12px;
            color: #95A5A6;
            font-size: 12px;
        }

        QLabel#arrow { color: #BDC3C7; }

        /* ── Parameter group box ─────────────────────── */
        QGroupBox#groupParams {
            border: 1px solid #D5DBE1;
            border-radius: 6px;
            margin-top: 6px;
            background: #FFFFFF;
            color: #2C3E50;
        }
        QGroupBox#groupParams::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 6px;
            color: #5D6D7E;
            font-weight: bold;
        }

        /* ── Spin boxes ──────────────────────────────── */
        QDoubleSpinBox, QSpinBox {
            border: 1px solid #BDC3C7;
            border-radius: 4px;
            padding: 2px 4px;
            background: #FFFFFF;
            color: #2C3E50;
        }
        QDoubleSpinBox:focus, QSpinBox:focus { border-color: #3498DB; }

        /* ── Status bar ──────────────────────────────── */
        QStatusBar { color: #7F8C8D; font-size: 12px; }
    )");
}

// ─────────────────────────────────────────────────────────────────────────────
// Slots
// ─────────────────────────────────────────────────────────────────────────────

void MainWindow::onOpenClicked() {
  const QString path = QFileDialog::getOpenFileName(
      this, "Open Image", "",
      "Images (*.png *.jpg *.jpeg *.bmp *.tiff *.tif);;All Files (*)");

  if (path.isEmpty())
    return;

  Mat img = cv::imread(path.toStdString());
  if (img.empty()) {
    statusBar()->showMessage("Error: could not read " + path);
    return;
  }

  matOriginal = img;
  matProcessed = Mat();

  showImage(lblOriginal, matOriginal);
  lblResult->clear();
  lblResult->setText("Result will appear here\nafter applying an operation.");
  statusBar()->showMessage("Loaded: " + path);
}

void MainWindow::onApplyClicked() {
  if (matOriginal.empty()) {
    statusBar()->showMessage("No image loaded — please open one first.");
    return;
  }

  const QString op = comboOperations->currentText();
  statusBar()->showMessage("Applying "
                           " + op + "
                           " …");
  btnApply->setEnabled(false);
  QApplication::processEvents(); // let the status bar paint before we block

  if (op == "Gray Scale") {
    matProcessed = ImageFiltering::gray_scale(matOriginal);

  } else if (op == "Mean (pixel-wise)") {
    matProcessed = ImageFiltering::mean_pixelwise(matOriginal);

  } else if (op == "Nearest Neighbor") {
    matProcessed =
        ImageFiltering::nearest_neighbor(matOriginal, spinScale->value());

  } else if (op == "Bilinear") {
    matProcessed = ImageFiltering::bilinear(matOriginal, spinScale->value());

  } else if (op == "Histogram Equalization") {
    Mat gray = ImageFiltering::gray_scale(matOriginal);
    matProcessed = ImageFiltering::histogram_equalization(gray);

  } else if (op == "Mean Convolution") {
    // First add noise, then denoise — that is the canonical demo
    Mat noisy = ImageFiltering::salt_and_pepper(matOriginal, 0.10);
    matProcessed = ImageFiltering::mean_convolution(noisy, spinKernel->value());
  }

  showImage(lblResult, matProcessed);
  btnApply->setEnabled(true);
  statusBar()->showMessage("Done: "
                           " + op + "
                           " applied.");
}

void MainWindow::onResetClicked() {
  matProcessed = Mat();
  lblResult->clear();
  lblResult->setText("Result will appear here\nafter applying an operation.");
  statusBar()->showMessage("Result cleared.");
}

void MainWindow::onOperationChanged(const QString &operation) {
  lblDescription->setText(operationDescription(operation));
  updateParamPanel(operation);
}

// ─────────────────────────────────────────────────────────────────────────────
// Helpers
// ─────────────────────────────────────────────────────────────────────────────

void MainWindow::updateParamPanel(const QString &operation) {
  const bool needsScale =
      (operation == "Nearest Neighbor" || operation == "Bilinear");
  const bool needsKernel = (operation == "Mean Convolution");

  lblScaleHint->setVisible(needsScale);
  spinScale->setVisible(needsScale);
  lblKernelHint->setVisible(needsKernel);
  spinKernel->setVisible(needsKernel);
  groupParams->setVisible(needsScale || needsKernel);
}

void MainWindow::loadDefaultImage() {
  matOriginal = cv::imread("placeholder.jpg");

  if (matOriginal.empty()) {
    // Synthetic colour-gradient placeholder so the UI is never blank
    matOriginal = Mat(400, 400, CV_8UC3);
    for (int y = 0; y < matOriginal.rows; ++y)
      for (int x = 0; x < matOriginal.cols; ++x)
        matOriginal.at<Vec3b>(y, x) =
            Vec3b(200 - y / 3, 80 + x / 5, 180 + y / 8);
  }

  showImage(lblOriginal, matOriginal);
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
  if (!matOriginal.empty())
    showImage(lblOriginal, matOriginal);
  if (!matProcessed.empty())
    showImage(lblResult, matProcessed);
}
