#ifndef COPY_MAKE_BORDER_PLUGIN_H
#define COPY_MAKE_BORDER_PLUGIN_H

#include "cv_plugin_interface.h"
#include "copy_make_border_plugin_global.h"
#include "ui_plugin.h"

namespace Ui {
class PluginGui;
}

class COPY_MAKE_BORDER_EXPORT CopyMakeBorder_Plugin: public QObject, public CvPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.computervision.cvplugininterface")
    Q_INTERFACES(CvPluginInterface)

public:
    CopyMakeBorder_Plugin();
    ~CopyMakeBorder_Plugin();

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

#endif // COPY_MAKE_BORDER_PLUGIN_H
