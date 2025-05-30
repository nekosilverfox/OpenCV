#include "copy_make_border_plugin.h"
#include "ui_plugin.h"

CopyMakeBorder_Plugin::CopyMakeBorder_Plugin()
{
    // Insert initialization codes here ...
}

CopyMakeBorder_Plugin::~CopyMakeBorder_Plugin()
{
    // Insert cleanup codes here ...
}

QString CopyMakeBorder_Plugin::title()
{
    return this->metaObject()->className();
}

QString CopyMakeBorder_Plugin::version()
{
    return "1.0.0";
}

QString CopyMakeBorder_Plugin::description()
{
    return "A <b>Template</b> plugin";
}

QString CopyMakeBorder_Plugin::help()
{
    return "This is a <b>Template</b> plugin. Clone and use it to create new plugins.";
}

void CopyMakeBorder_Plugin::setupUi(QWidget *parent)
{
    qDebug() << "Setup ui" << title();
    ui = new Ui::PluginGui;
    ui->setupUi(parent);

    QStringList items;
    items.append("BORDER_CONSTANT");
    items.append("BORDER_REPLICATE");
    items.append("BORDER_REFLECT");
    items.append("BORDER_WRAP");
    items.append("BORDER_REFLECT_101");
    ui->cbBorderType->addItems(items);

    connect(ui->cbBorderType, &QComboBox::currentIndexChanged, this, [=](){
        qDebug() << "emit CvPluginInterface::updateNeeded";
        emit CvPluginInterface::updateNeeded();
    });
}

void CopyMakeBorder_Plugin::processImage(const cv::Mat &inputImage, cv::Mat &outputImage)
{
    int top = inputImage.rows / 2;
    int bot = inputImage.rows / 2;
    int left  = inputImage.cols / 2;
    int right = inputImage.cols / 2;

    cv::copyMakeBorder(inputImage, outputImage,
                       top, bot, left, right,
                       ui->cbBorderType->currentIndex());
}




