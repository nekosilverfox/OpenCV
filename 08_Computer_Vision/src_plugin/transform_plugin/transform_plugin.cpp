#include "transform_plugin.h"

#include "ui_plugin.h"

Transform_Plugin::Transform_Plugin()
{
    // Insert initialization codes here ...
}

Transform_Plugin::~Transform_Plugin()
{
    // Insert cleanup codes here ...
}

QString Transform_Plugin::title()
{
    return this->metaObject()->className();
}

QString Transform_Plugin::version()
{
    return "1.0.0";
}

QString Transform_Plugin::description()
{
    return "Performs different transformations available in OpenCV";
}

QString Transform_Plugin::help()
{
    return "This is a plugin that performs different filters available in OpenCV";
}

void Transform_Plugin::setupUi(QWidget *parent)
{
    ui = new Ui::PluginGui;
    ui->setupUi(parent);

    ui->borderTypeCombo->addItems(
                QStringList()
                << "BORDER_CONSTANT"
                << "BORDER_REPLICATE"
                << "BORDER_REFLECT"
                << "BORDER_WRAP"
                << "BORDER_REFLECT_101");

    ui->interpolationCombo->addItems(
                QStringList()
                << "INTER_NEAREST"
                << "INTER_CUBIC"
                << "INTER_AREA"
                << "INTER_LANCZOS4");

    connect(ui->resizeHalfRadio,    &QRadioButton::toggled, this, [=](){emit CvPluginInterface::updateNeeded();});
    connect(ui->resizeDoubleRadio,  &QRadioButton::toggled, this, [=](){emit CvPluginInterface::updateNeeded();});
    connect(ui->remapRadio,         &QRadioButton::toggled, this, [=](){emit CvPluginInterface::updateNeeded();});
    connect(ui->affineRadio,        &QRadioButton::toggled, this, [=](){emit CvPluginInterface::updateNeeded();});
    connect(ui->perspectiveRadio,   &QRadioButton::toggled, this, [=](){emit CvPluginInterface::updateNeeded();});
    connect(ui->borderTypeCombo,    &QComboBox::currentIndexChanged, this, [=](){emit CvPluginInterface::updateNeeded();});
    connect(ui->interpolationCombo, &QComboBox::currentIndexChanged, this, [=](){emit CvPluginInterface::updateNeeded();});
}

void Transform_Plugin::processImage(const cv::Mat &inputImage, cv::Mat &outputImage)
{
    using namespace cv;
    if(ui->resizeHalfRadio->isChecked())
    {
        resize(inputImage,
               outputImage,
               Size(),
               0.5,
               0.5,
               ui->interpolationCombo->currentIndex());
    }
    else if(ui->resizeDoubleRadio->isChecked())
    {
        resize(inputImage,
               outputImage,
               Size(),
               2.0,
               2.0,
               ui->interpolationCombo->currentIndex());
    }
    else if(ui->affineRadio->isChecked())
    {
        Point2f triangleA[3];
        Point2f triangleB[3];

        triangleA[0] = Point2f(0, 0);
        triangleA[1] = Point2f(inputImage.cols - 1, 0);
        triangleA[2] = Point2f(0, inputImage.rows - 1);

        triangleB[0] = Point2f(inputImage.cols*0.0, inputImage.rows*0.33);
        triangleB[1] = Point2f(inputImage.cols*0.85, inputImage.rows*0.25);
        triangleB[2] = Point2f(inputImage.cols*0.15, inputImage.rows*0.7);

        Mat affineMat = getAffineTransform( triangleA, triangleB );

        warpAffine( inputImage,
                    outputImage,
                    affineMat,
                    inputImage.size(),
                    ui->interpolationCombo->currentIndex(),
                    ui->borderTypeCombo->currentIndex());
    }
    else if(ui->perspectiveRadio->isChecked())
    {
        std::vector<Point2f> cornersA(4);
        std::vector<Point2f> cornersB(4);

        cornersA[0] = Point2f(0, 0);
        cornersA[1] = Point2f(inputImage.cols, 0);
        cornersA[2] = Point2f(inputImage.cols, inputImage.rows);
        cornersA[3] = Point2f(0, inputImage.rows);

        cornersB[0] = Point2f(inputImage.cols*0.25, 0);
        cornersB[1] = Point2f(inputImage.cols * 0.90, 0);
        cornersB[2] = Point2f(inputImage.cols, inputImage.rows);
        cornersB[3] = Point2f(0, inputImage.rows * 0.80);

        Mat homo = findHomography(cornersA, cornersB, RANSAC);
        warpPerspective(inputImage,
                        outputImage,
                        homo,
                        inputImage.size(),
                        ui->interpolationCombo->currentIndex(),
                        ui->borderTypeCombo->currentIndex()); // do perspective transformation
    }
    else if(ui->remapRadio->isChecked())
    {
        cvtColor(inputImage, outputImage, CV_32FC(1));;
        Mat mapX, mapY;
        mapX.create(inputImage.size(), CV_32FC(1));
        mapY.create(inputImage.size(), CV_32FC(1));

        Point2f center(inputImage.cols/2,
                       inputImage.rows/2);

        for(int i=0; i<inputImage.rows; i++)
            for(int j=0; j<inputImage.cols; j++)
            {
                double x = j - center.x;
                double y = i - center.y;

                x = x*x/500;
                /* y = y; */

                mapX.at<float>(i,j) = x + center.x;
                mapY.at<float>(i,j) = y + center.y;
            }

        remap(inputImage,
              outputImage,
              mapX,
              mapY,
              INTER_LANCZOS4,
              BORDER_CONSTANT);

    }
}

