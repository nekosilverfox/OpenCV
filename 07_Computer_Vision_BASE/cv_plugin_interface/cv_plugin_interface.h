#ifndef CVPLUGIN_H
#define CVPLUGIN_H

#include <QObject>
#include <QWidget>
#include <QString>
#include <QtPlugin> // 必须包含

#include "opencv2/opencv.hpp"

// 新增导出宏定义
#if defined(MAKE_CVPLUGIN_LIB) // 插件项目需定义此宏
#  define CVPLUGIN_EXPORT Q_DECL_EXPORT
#else
#  define CVPLUGIN_EXPORT Q_DECL_IMPORT
#endif

class CVPLUGIN_EXPORT CvPluginInterface : public QObject
{
    Q_OBJECT

public:
    virtual ~CvPluginInterface() = default;
    virtual QString title() = 0;
    virtual QString version() = 0;
    virtual QString description() = 0;
    virtual QString help() = 0;
    virtual void setupUi(QWidget *parent) = 0;
    virtual void processImage(const cv::Mat &inputImage, cv::Mat &outputImage) = 0;

signals: // 声明信号
    void updateNeeded();
    void infoMessage(const QString& message);
    void errorMessage(const QString& message);
};

#define CVPLUGININTERFACE_IID "com.computervision.cvplugininterface"
Q_DECLARE_INTERFACE(CvPluginInterface, CVPLUGININTERFACE_IID)

#endif // CVPLUGIN_H
