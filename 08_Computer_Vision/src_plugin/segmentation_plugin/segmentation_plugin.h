#ifndef SEGMENTATION_PLUGIN_H
#define SEGMENTATION_PLUGIN_H

#include "segmentation_plugin_global.h"
#include "cvplugininterface.h"

namespace Ui {
    class PluginGui;
}

class SEGMENTATION_PLUGINSHARED_EXPORT Segmentation_Plugin: public CvPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.computervision.cvplugininterface")
    Q_INTERFACES(CvPluginInterface)
public:
    Segmentation_Plugin();
    ~Segmentation_Plugin();

    QString title();
    QString version();
    QString description();
    QString help();
    void setupUi(QWidget *parent);
    void processImage(const cv::Mat &inputImage, cv::Mat &outputImage);

signals:
    void updateNeeded();
    void errorMessage(QString msg);
    void infoMessage(QString msg);

private slots:

private:
    Ui::PluginGui *ui;

};

#endif // SEGMENTATION_PLUGIN_H
