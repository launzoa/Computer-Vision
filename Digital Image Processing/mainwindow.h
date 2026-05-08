#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QComboBox>
#include <QDoubleSpinBox>
#include <QEvent>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMouseEvent>
#include <QPushButton>
#include <QSpinBox>
#include <opencv2/opencv.hpp>

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow() override;

protected:
  bool eventFilter(QObject *watched, QEvent *event) override;
  void resizeEvent(QResizeEvent *event) override;

private slots:
  void onOpenClicked();
  void onApplyClicked();
  void onResetClicked();
  void onOperationChanged(const QString &operation);

private:
  // ── Top bar ──────────────────────────────────────────────────────────
  QPushButton *btnOpen;
  QComboBox *comboOperations;
  QPushButton *btnApply;
  QPushButton *btnReset;

  // ── Info / parameters ────────────────────────────────────────────────
  QLabel *lblDescription;
  QGroupBox *groupParams;

  QLabel *lblScaleHint;
  QDoubleSpinBox *spinScale;

  QLabel *lblKernelHint;
  QSpinBox *spinKernel;

  QLabel *lblThresholdHint;
  QSpinBox *spinThreshold;

  // --- NOVO: Variáveis para a Correção Gamma ---
  QLabel *lblGammaCHint;
  QDoubleSpinBox *spinGammaC;

  QLabel *lblGammaHint;
  QDoubleSpinBox *spinGamma;

  // ── Hover Info Panel ──────────────────────────────────────────────────
  QGroupBox *groupPixelInfo;
  QLineEdit *txtPos;
  QLineEdit *txtRGB;
  QLineEdit *txtHSV;
  QLineEdit *txtGray;

  // ── Image panes ───────────────────────────────────────────────────────
  QLabel *lblOriginal;
  QLabel *lblResult;

  // ── Image data ────────────────────────────────────────────────────────
  cv::Mat matOriginal;
  cv::Mat matCurrent;
  cv::Mat matPreview;
  cv::Mat matProcessed;

  // ── Private helpers ───────────────────────────────────────────────────
  void setupUI();
  void applyStyles();
  void loadDefaultImage();
  void showImage(QLabel *label, const cv::Mat &mat);
  void updateParamPanel(const QString &operation);
  void updatePreview();
};

#endif // MAINWINDOW_H
