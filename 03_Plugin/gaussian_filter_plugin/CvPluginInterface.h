/**
 * 插件接口 Interface
 */

#ifndef CVPLUGININTERFACE_H
#define CVPLUGININTERFACE_H

#include <QObject>
#include <QString>
#include "opencv2/opencv.hpp"

class CvPluginInterface
{
public:
    virtual ~CvPluginInterface() {}

    virtual QString description() = 0;  // 返回插件说明
    virtual void processImage(const cv::Mat &inputImage, cv::Mat &outputImage) = 0;
};

#define CVPLUGININTERFACE_IID "com.amin.cvplugininterface"  // 一个独一无二的字符串，采用类似包名格式
Q_DECLARE_INTERFACE(CvPluginInterface, CVPLUGININTERFACE_IID)  // 宏将我们的类定义为接口。不包含这个宏，Qt将无法将我们的类识别为插件接口

#endif // CVPLUGININTERFACE_H
