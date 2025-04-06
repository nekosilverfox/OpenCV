#include "widget.h"
#include "ui_widget.h"

#include <QImage>
#include <QFile>
#include <OpenColorIO/OpenColorIO.h>

namespace OCIO = OCIO_NAMESPACE;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    qDebug() << "OCIO 版本：" << OCIO::GetVersion();

    // 0. 加载一张图像
    QString filename = "/Users/fox/Desktop/IMG_9056.JPG";
    QImage input(filename);

    // 1. 获取 OpenColorIO 配置
    QString configPath = "/Users/fox/雪狸的文件/Programma/OpenCV/TEST_OpenColorIO/spi-vfx.ocio";
    if (!QFile::exists(configPath))
    {
        qDebug() << "OCIO 配置文件未找到！";
        return;
    }
    OCIO::ConstConfigRcPtr config = OCIO::Config::CreateFromFile(configPath.toStdString().c_str());
    if (!config) qDebug() << "Config do not load";

    OCIO::ConstContextRcPtr currentContext = config->getCurrentContext();
    qDebug() << "搜索路径：" << currentContext->getSearchPath();

    OCIO::ConstProcessorRcPtr processor = config->getProcessor(
        "lnf",   // 输入空间（场景线性）
        "vd8"        // 输出空间（sRGB）
        );
    qDebug() << "当前工作颜色空间：" << currentContext->getStringVar("working_space");

    // 获取颜色空间集合
    OCIO::ConstColorSpaceSetRcPtr colorSpaceSet = config->getColorSpaces(nullptr);

    // 提取颜色空间名称
    std::vector<std::string> colorSpaces;
    for (int i = 0; i < colorSpaceSet->getNumColorSpaces(); ++i) {
        OCIO::ConstColorSpaceRcPtr cs = colorSpaceSet->getColorSpaceByIndex(i);
        colorSpaces.push_back(cs->getName());
    }
    qDebug() << "可用的颜色空间：";
    for (const auto &cs : colorSpaces) {
        qDebug() << " - " << cs.c_str();
    }

    // 2. 创建 OpenColorIO 的 CPU 处理器
    OCIO::ConstCPUProcessorRcPtr cpuProcessor = processor->getDefaultCPUProcessor();

    // 3. 将 QImage 转换为 RGB 数据
    qDebug() << "3. 将 QImage 转换为 RGB 数据";
    QImage output = input.convertToFormat(QImage::Format_RGBA32FPx4);
    float *pixels = reinterpret_cast<float*>(output.bits());


    // 4. 应用颜色转换
    qDebug() << "4. 应用颜色转换";
    OCIO::PackedImageDesc desc(
        pixels,                     // 像素数据指针
        output.width(),             // 图像宽度
        output.height(),            // 图像高度
        4,                          // 通道数（RGBA 为 4）
        OCIO::BIT_DEPTH_F32,        // 数据类型（32 位浮点）
        sizeof(float) * 4,          // 通道步长（每个像素 4 个 float）
        sizeof(float) * 4 * output.width(),  // 行步长
        0                           // 平面步长（0 表示连续存储）
        );
    qDebug() << "-》应用颜色转换完成";
    cpuProcessor->apply(desc);

    // 在 Widget 类中显示图像
    ui->imageLabel->setPixmap(QPixmap::fromImage(output));
    ui->imageLabel->show();
}

Widget::~Widget()
{
    delete ui;
}
