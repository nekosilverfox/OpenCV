#ifndef TRANSFORM_PLUGIN_H
#define TRANSFORM_PLUGIN_H

#include "transform_plugin_global.h"
#include "cvplugininterface.h"
#include <QObject>
#include <QtMath>

namespace Ui {
    class PluginGui;
}

class TRANSFORM_PLUGINSHARED_EXPORT Transform_Plugin: public CvPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.computervision.cvplugininterface")
    Q_INTERFACES(CvPluginInterface)
public:
    Transform_Plugin();
    ~Transform_Plugin();

    QString title();
    QString version();
    QString description();
    QString help();
    void setupUi(QWidget *parent);
    void processImage(const cv::Mat &inputImage, cv::Mat &outputImage);

private slots:

private:
    Ui::PluginGui *ui;

};

#endif // TRANSFORM_PLUGIN_H
