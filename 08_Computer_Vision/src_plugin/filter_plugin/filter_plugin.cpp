#include "filter_plugin.h"

#include "ui_plugin.h"
#include "cvplugininterface.h"

#define BILATERAL_FILTER_PAGE           0
#define BLUR_FILTER_PAGE                1
#define BOX_FILTER_PAGE                 2
#define GAUSSIAN_FILTER_PAGE            3
#define MEDIAN_FILTER_PAGE              4
#define FILTER2D_PAGE                   5
#define DERIVATIVES_PAGE                6
#define MORPH_PAGE                      7

Filter_Plugin::Filter_Plugin()
{
    // Insert initialization codes here ...
}

Filter_Plugin::~Filter_Plugin()
{
    // Insert cleanup codes here ...
}

QString Filter_Plugin::title()
{
    return this->metaObject()->className();
}

QString Filter_Plugin::version()
{
    return "1.0.0";
}

QString Filter_Plugin::description()
{
    return "Performs different filters available in OpenCV";
}

QString Filter_Plugin::help()
{
    return "This is a plugin that performs different filters available in OpenCV";
}
void Filter_Plugin::setupUi(QWidget *parent)
{
    ui = new Ui::PluginGui;
    ui->setupUi(parent);

    ui->mainTabs->setCurrentIndex(0);

    connect(ui->mainTabs, &QTabWidget::currentChanged, this, [=](){emit CvPluginInterface::updateNeeded();});

    connect(ui->bilateralDiaSpin, &QSpinBox::valueChanged, this, [=](){emit CvPluginInterface::updateNeeded();});
    connect(ui->bilateralSigmaColorSpin, &QDoubleSpinBox::valueChanged, this, [=](){emit CvPluginInterface::updateNeeded();});
    connect(ui->bilateralSigmaSpaceSpin, &QDoubleSpinBox::valueChanged, this, [=](){emit CvPluginInterface::updateNeeded();});

    connect(ui->blurKernelSizeSpinBox, &QSpinBox::valueChanged, this, [=](){emit CvPluginInterface::updateNeeded();});
    connect(ui->blurAnchoXSpin, &QSpinBox::valueChanged, this, [=](){emit CvPluginInterface::updateNeeded();});
    connect(ui->blurAnchoYSpin, &QSpinBox::valueChanged, this, [=](){emit CvPluginInterface::updateNeeded();});

    connect(ui->boxKernelSizeSpinBox, &QSpinBox::valueChanged, this, [=](){emit CvPluginInterface::updateNeeded();});
    connect(ui->boxDepthSpin, &QSpinBox::valueChanged, this,  [=](){emit CvPluginInterface::updateNeeded();});
    connect(ui->boxAnchoXSpin, &QSpinBox::valueChanged, this, [=](){emit CvPluginInterface::updateNeeded();});
    connect(ui->boxAnchoYSpin, &QSpinBox::valueChanged, this, [=](){emit CvPluginInterface::updateNeeded();});
    connect(ui->boxNormalCheck, &QCheckBox::toggled, this,    [=](){emit CvPluginInterface::updateNeeded();});

    connect(ui->gaussKernelSizeSpinBox, &QSpinBox::valueChanged, this,
            [=](int arg1){if(arg1 % 2 == 1){emit CvPluginInterface::updateNeeded();}});
    connect(ui->gaussSigmaXSpin, &QDoubleSpinBox::valueChanged, this, [=](){emit CvPluginInterface::updateNeeded();});
    connect(ui->gaussSigmaYSpin, &QDoubleSpinBox::valueChanged, this, [=](){emit CvPluginInterface::updateNeeded();});

    connect(ui->medianApertureSpin, &QSpinBox::valueChanged, this,
            [=](int arg1){if(arg1 % 2 == 1){emit CvPluginInterface::updateNeeded();}});

    connect(ui->derivSobelRadio,  &QCheckBox::toggled, this, [=](){emit CvPluginInterface::updateNeeded();});
    connect(ui->derivScharrRadio, &QCheckBox::toggled, this, [=](){emit CvPluginInterface::updateNeeded();});
    connect(ui->derivLaplacRadio, &QCheckBox::toggled, this, [=](){emit CvPluginInterface::updateNeeded();});
    connect(ui->derivDeltaSpin, &QDoubleSpinBox::valueChanged, this, [=](){emit CvPluginInterface::updateNeeded();});
    connect(ui->derivScaleSpin, &QDoubleSpinBox::valueChanged, this, [=](){emit CvPluginInterface::updateNeeded();});

    ui->morphShapesCombo->addItems(
                QStringList() << "MORPH_RECT" << "MORPH_CROSS" << "MORPH_ELLIPSE");
    ui->morphTypesCombo->addItems(
                QStringList() << "MORPH_ERODE" << "MORPH_DILATE" << "MORPH_OPEN" << "MORPH_CLOSE"
                << "MORPH_GRADIENT" << "MORPH_TOPHAT" << "MORPH_BLACKHAT");
    connect(ui->morphDilateRadio, &QCheckBox::toggled, this, [=](){emit CvPluginInterface::updateNeeded();});
    connect(ui->morphErodeRadio,  &QCheckBox::toggled, this, [=](){emit CvPluginInterface::updateNeeded();});
    connect(ui->morphMorphRadio,  &QCheckBox::toggled, this, [=](){emit CvPluginInterface::updateNeeded();});
    connect(ui->morphIterSpin, &QSpinBox::valueChanged, this, [=](){emit CvPluginInterface::updateNeeded();});
    connect(ui->morphShapesCombo, &QComboBox::currentIndexChanged, this, [=](){emit CvPluginInterface::updateNeeded();});
    connect(ui->morphTypesCombo,  &QComboBox::currentIndexChanged, this, [=](){emit CvPluginInterface::updateNeeded();});
}

void Filter_Plugin::processImage(const cv::Mat &inputImage, cv::Mat &outputImage)
{
    using namespace cv;
    Matx33f f2dkernel;

    switch(ui->mainTabs->currentIndex())
    {

    case BILATERAL_FILTER_PAGE:
        bilateralFilter(inputImage,
                        outputImage,
                        ui->bilateralDiaSpin->value(),
                        ui->bilateralSigmaColorSpin->value(),
                        ui->bilateralSigmaSpaceSpin->value());
        break;

    case BLUR_FILTER_PAGE:
        blur(inputImage,
             outputImage,
             Size(ui->blurKernelSizeSpinBox->value(),
                  ui->blurKernelSizeSpinBox->value()),
             Point(ui->blurAnchoXSpin->value(),
                   ui->blurAnchoYSpin->value()));
        break;

    case BOX_FILTER_PAGE:
        boxFilter(inputImage,
                  outputImage,
                  ui->boxDepthSpin->value(),
                  Size(ui->boxKernelSizeSpinBox->value(),
                       ui->boxKernelSizeSpinBox->value()),
                  Point(ui->boxAnchoXSpin->value(),
                        ui->boxAnchoYSpin->value()),
                  ui->boxNormalCheck->isChecked());

        // replace with sqrBoxFilter
        break;

    case GAUSSIAN_FILTER_PAGE:
        GaussianBlur(inputImage,
                     outputImage,
                     Size(ui->gaussKernelSizeSpinBox->value(),
                          ui->gaussKernelSizeSpinBox->value()),
                     ui->gaussSigmaXSpin->value(),
                     ui->gaussSigmaYSpin->value());
        break;

    case MEDIAN_FILTER_PAGE:
        medianBlur(inputImage,
                   outputImage,
                   ui->medianApertureSpin->value());
        break;

    case FILTER2D_PAGE:
        f2dkernel = Matx33f(0, +1.5, 0,
                            +1.5, -6, +1.5,
                            0, +1.5, 0);

        filter2D(inputImage,
                 outputImage,
                 -1, // Output should have same depth as source
                 f2dkernel,
                 Point(-1,-1));
        break;

    case DERIVATIVES_PAGE:
        if(ui->derivSobelRadio->isChecked())
            Sobel(inputImage, outputImage, -1, 1, 1, 3, ui->derivScaleSpin->value(), ui->derivDeltaSpin->value());
        else if(ui->derivScharrRadio->isChecked())
            Scharr(inputImage, outputImage, -1, 1, 0, ui->derivScaleSpin->value(), ui->derivDeltaSpin->value());
        else if(ui->derivLaplacRadio->isChecked())
            Laplacian(inputImage, outputImage, -1, 3, ui->derivScaleSpin->value(), ui->derivDeltaSpin->value());
        break;

    case MORPH_PAGE:
        if(ui->morphErodeRadio->isChecked())
        {
            erode(inputImage,
                  outputImage,
                  getStructuringElement(ui->morphShapesCombo->currentIndex(),
                                        Size(5,5)),
                  Point(-1,-1),
                  ui->morphIterSpin->value());
        }
        else if(ui->morphDilateRadio->isChecked())
        {
            dilate(inputImage,
                   outputImage,
                   getStructuringElement(ui->morphShapesCombo->currentIndex(),
                                         Size(5,5)),
                   Point(-1,-1),
                   ui->morphIterSpin->value());
        }
        else if(ui->morphMorphRadio->isChecked())
        {
            morphologyEx(inputImage,
                         outputImage,
                         ui->morphTypesCombo->currentIndex(),
                         getStructuringElement(ui->morphShapesCombo->currentIndex(),
                                               Size(5,5)),
                         Point(-1,-1),
                         ui->morphIterSpin->value());
        }
        break;

    }
}
