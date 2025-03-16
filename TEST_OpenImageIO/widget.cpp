#include "widget.h"
#include "ui_widget.h"

#include <OpenImageIO/imageio.h>
#include <OpenImageIO/imagebuf.h>
#include <OpenImageIO/imagebufalgo.h>
#include <QLabel>

using namespace OIIO;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    QString filename = "/Users/fox/Desktop/IMG_9056.JPG";

    // 2. 创建 ImageInput 对象
    auto in = OIIO::ImageInput::open(filename.toStdString());
    if (!in)
    {
        qDebug() << "无法打开文件：" << filename;
        return;
    }

    // 3. 获取图像基本信息
    const OIIO::ImageSpec &spec = in->spec();
    qDebug() << "图像宽度：" << spec.width;
    qDebug() << "图像高度：" << spec.height;
    qDebug() << "通道数：" << spec.nchannels;

    // 遍历元数据
    for (const auto &param : spec.extra_attribs) {
        qDebug() << "元数据 -" << param.name().c_str() << ":" << param.get<std::string>();
    }

    // 4. 读取像素数据（示例：仅读取元数据，不处理像素）
    in->close();

    // 5. 显示结果（可选：使用 QLabel 显示图像）
    QLabel label("OpenImageIO 测试成功！");
    label.show();


    // 测试将图像转换为灰度图并保存
    QString output = "/Users/fox/Desktop/GREY_IMG_9056.JPG";
    convertToGrayscale(filename.toStdString(), output.toStdString());

    // 测试缩略图
    QString resizeOutput = "/Users/fox/Desktop/RESIZE_IMG_9056.JPG";
    resizeImage(filename.toStdString(), resizeOutput.toStdString(), 500, 500);
}

Widget::~Widget()
{
    delete ui;
}

/**
 * @brief Widget::convertToGrayscale 将图像转换为灰度图并保存
 * @param inputPath
 * @param outputPath
 */
void Widget::convertToGrayscale(const std::string &inputPath, const std::string &outputPath)
{
    // 读取输入图像
    auto in = ImageInput::open(inputPath);
    if (!in) return;

    const ImageSpec &inSpec = in->spec();
    std::vector<unsigned char> pixels(inSpec.width * inSpec.height * inSpec.nchannels);
    bool success = in->read_image(
        0,                      // subimage（子图像索引）
        0,                      // miplevel（Mipmap 层级）
        0,                      // chbegin（起始通道）
        inSpec.nchannels,         // chend（结束通道：总通道数）
        TypeDesc::UINT8,        // format（目标数据类型）
        pixels.data()           // data（像素数据指针）
        );
    if (!success)
    {
        qDebug() << "读取像素数据失败";
        in->close();
        return;
    }
    in->close();

    // 转换为灰度（简单平均法）
    std::vector<unsigned char> grayPixels(inSpec.width * inSpec.height);
    for (int y = 0; y < inSpec.height; ++y) {
        for (int x = 0; x < inSpec.width; ++x) {
            int idx = (y * inSpec.width + x) * inSpec.nchannels;
            unsigned char r = pixels[idx];
            unsigned char g = pixels[idx + 1];
            unsigned char b = pixels[idx + 2];
            grayPixels[y * inSpec.width + x] = static_cast<unsigned char>((r + g + b) / 3);
        }
    }

    // 保存灰度图
    ImageSpec outSpec(inSpec.width, inSpec.height, 1, TypeDesc::UINT8);
    auto out = ImageOutput::create(outputPath);
    if (!out) return;

    out->open(outputPath, outSpec);
    out->write_image(TypeDesc::UINT8, grayPixels.data());
    out->close();
}

/**
 * @brief Widget::resizeImage 调整图像尺寸（缩略图生成）
 * @param inputPath
 * @param outputPath
 * @param newWidth
 * @param newHeight
 */
void Widget::resizeImage(const std::string &inputPath, const std::string &outputPath, int newWidth, int newHeight)
{
    // 初始化 ImageBuf
    ImageBuf inputBuf(inputPath);
    if (inputBuf.has_error()) {
        qDebug() << "错误：" << inputBuf.geterror().c_str();
        return;
    }

    // 调整尺寸
    ImageBuf outputBuf;

    // 使用默认滤波器（如 "lanczos3"）和 ROI
    // 创建目标 ROI
    ROI dst_roi(0, newWidth, 0, newHeight, 0, 1, 0, inputBuf.nchannels());
    bool success = OIIO::ImageBufAlgo::resize(
        outputBuf,    // 输出图像
        inputBuf,     // 输入图像
        "lanczos3",   // 滤波器名称
        0.0f,         // 滤波器宽度（0 表示自动选择）
        dst_roi,      // 处理区域（默认全图）
        0             // 线程数（0 表示自动）
        );

    // 保存结果
    if (!outputBuf.write(outputPath)) {
        qDebug() << "保存失败：" << outputBuf.geterror().c_str();
    } else {
        qDebug() << "缩略图图像已保存至：" << outputPath.c_str();
    }
}
