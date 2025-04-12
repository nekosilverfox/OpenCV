#include "segmentation_plugin.h"

#include "ui_plugin.h"

Segmentation_Plugin::Segmentation_Plugin()
{
    // Insert initialization codes here ...
}

Segmentation_Plugin::~Segmentation_Plugin()
{
    // Insert cleanup codes here ...
}

QString Segmentation_Plugin::title()
{
    return this->metaObject()->className();
}

QString Segmentation_Plugin::version()
{
    return "1.0.0";
}

QString Segmentation_Plugin::description()
{
    return "";
}

QString Segmentation_Plugin::help()
{
    return "";
}

void Segmentation_Plugin::setupUi(QWidget *parent)
{
    ui = new Ui::PluginGui;
    ui->setupUi(parent);

    ui->threshAdaptiveCombo->addItems(
                QStringList()
                << "ADAPTIVE_THRESH_MEAN_C"
                << "ADAPTIVE_THRESH_GAUSSIAN_C");

    ui->threshTypeCombo->addItems(
                QStringList()
                << "THRESH_BINARY"
                << "THRESH_BINARY_INV"
                << "THRESH_TRUNC"
                << "THRESH_TOZERO"
                << "THRESH_TOZERO_INV");
    connect(ui->threshAdaptiveCheck, &QCheckBox::toggled, this, [=](){emit CvPluginInterface::updateNeeded();});
    connect(ui->threshAdaptiveCombo, &QComboBox::currentIndexChanged, this, [=](){emit CvPluginInterface::updateNeeded();});
    connect(ui->threshTypeCombo,     &QComboBox::currentIndexChanged, this, [=](){emit CvPluginInterface::updateNeeded();});
    connect(ui->threshSlider,    &QSlider::valueChanged, this, [=](){emit CvPluginInterface::updateNeeded();});
    connect(ui->threshMaxSlider, &QSlider::valueChanged, this, [=](){emit CvPluginInterface::updateNeeded();});

}

void Segmentation_Plugin::processImage(const cv::Mat &inputImage, cv::Mat &outputImage)
{
    using namespace cv;

    Mat grayScale;
    cvtColor(inputImage, grayScale, COLOR_BGR2GRAY);

    if(ui->threshAdaptiveCheck->isChecked())
    {
        adaptiveThreshold(grayScale,
                          grayScale,
                          ui->threshMaxSlider->value(),
                          ui->threshAdaptiveCombo->currentIndex(),  // ADAPTIVE_THRESH_GAUSSIAN_C
                          ui->threshTypeCombo->currentIndex(),
                          7,
                          0);
    }
    else
    {
        threshold(grayScale,
                  grayScale,
                  ui->threshSlider->value(),
                  ui->threshMaxSlider->value(),
                  ui->threshTypeCombo->currentIndex());
    }

    cvtColor(grayScale, outputImage, COLOR_GRAY2BGR);
}
