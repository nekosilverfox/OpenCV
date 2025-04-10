#ifndef COPY_MAKE_BORDER_PLUGIN_H
#define COPY_MAKE_BORDER_PLUGIN_H

#include "copy_make_border_plugin_global.h"

#include <QObject>

#include "../cv_plugin_interface/cv_plugin_interface.h"


namespace Ui {
class PluginGui;
}

class COPY_MAKE_BORDER_EXPORT CopyMakeBorder_Plugin: public CvPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.computervision.cvplugininterface")
    Q_INTERFACES(CvPluginInterface)

public:
    CopyMakeBorder_Plugin();
    ~CopyMakeBorder_Plugin();

    QString title() override;
    QString version() override;
    QString description()override;
    QString help() override;
    void setupUi(QWidget *parent) override;
    void processImage(const cv::Mat &inputImage, cv::Mat &outputImage) override;

signals:
    void updateNeeded();
    void errorMessage(QString msg);
    void infoMessage(QString msg);

private:
    Ui::PluginGui *ui;
};

#endif // COPY_MAKE_BORDER_PLUGIN_H
