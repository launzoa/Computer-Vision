#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QComboBox>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QSpinBox>
#include <opencv2/opencv.hpp>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onOpenClicked();
    void onApplyClicked();
    void onResetClicked();
    void onOperationChanged(const QString &operation);

private:
    // ── Top bar ──────────────────────────────────────────────────────────
    QPushButton *btnOpen;
    QComboBox   *comboOperations;
    QPushButton *btnApply;
    QPushButton *btnReset;

    // ── Info / parameters ────────────────────────────────────────────────
    QLabel         *lblDescription;
    QGroupBox      *groupParams;
    QLabel         *lblScaleHint;
    QDoubleSpinBox *spinScale;
    QLabel         *lblKernelHint;
    QSpinBox       *spinKernel;

    // ── Image panes ───────────────────────────────────────────────────────
    QLabel *lblOriginal;
    QLabel *lblResult;

    // ── Image data ────────────────────────────────────────────────────────
    cv::Mat matOriginal;
    cv::Mat matProcessed;

    // ── Private helpers ───────────────────────────────────────────────────
    void setupUI();
    void applyStyles();
    void loadDefaultImage();
    void showImage(QLabel *label, const cv::Mat &mat);
    void updateParamPanel(const QString &operation);

    void resizeEvent(QResizeEvent *event) override;
};

#endif // MAINWINDOW_H
