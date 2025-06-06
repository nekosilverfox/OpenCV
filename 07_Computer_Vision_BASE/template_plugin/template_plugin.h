#ifndef TEMPLATE_PLUGIN_H
#define TEMPLATE_PLUGIN_H

#include "template_plugin_global.h"
#include "cv_plugin_interface.h"

namespace Ui {
    class PluginGui;
}

// 确保插件类仅通过 CvPluginInterface 间接继承 QObject
class TEMPLATE_PLUGINSHARED_EXPORT Template_Plugin: public CvPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.computervision.cvplugininterface")
    Q_INTERFACES(CvPluginInterface)

public:
    Template_Plugin();
    ~Template_Plugin();

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

private:
    Ui::PluginGui *ui;

};

#endif // TEMPLATE_PLUGIN_H
