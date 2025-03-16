#include "widget.h"
#include "ui_widget.h"

#include "libraw.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    // QString path = "/Users/fox/Desktop/IMG_9056.JPG";
    // QString path = "/Users/fox/Downloads/IMG_6961.CR2";
    QString path = "/Users/fox/Desktop/IMG_3677.CR3";
    QString savePath = "/Users/fox/Desktop/LibRaw_test_output.jpg";

    LibRaw* rawProcessor = new LibRaw;
    // qDebug() << "LibRaw 版本:" << libraw_version();


    // 1. 打开文件
    int ret = rawProcessor->open_file(path.toLocal8Bit().data());
    if (LIBRAW_SUCCESS != ret)
    {
        qDebug() << "无法打开文件:" << libraw_strerror(ret);
        return;
    }

    // 设置处理参数
    rawProcessor->imgdata.params.use_auto_wb = 1;      // 启用自动白平衡
    rawProcessor->imgdata.params.use_camera_wb = 1;    // 优先使用相机白平衡
    rawProcessor->imgdata.params.half_size = 0;        // 禁用半尺寸输出（保留全分辨率）
    rawProcessor->imgdata.params.output_bps = 8;       // 输出 8 位/通道
    rawProcessor->imgdata.params.no_auto_bright = 0;   // 启用自动亮度调整
    rawProcessor->imgdata.params.gamm[0] = 1.0;        // Gamma 值（通常设为 6.25 或 4.5）
    rawProcessor->imgdata.params.gamm[1] = 1.0;
    rawProcessor->imgdata.params.user_qual = 3;        // 去马赛克算法：AHD（高质量）
    qDebug() << "1. 打开文件:" << libraw_strerror(ret);



    // 2. 解码 RAW 数据
    if (int ret = rawProcessor->unpack() != LIBRAW_SUCCESS)
    {
        qDebug() << "解码失败:" << libraw_strerror(ret);
        rawProcessor->recycle();
        return;
    }
    qDebug() << "2. 解码:" << libraw_strerror(ret);



    // 3. 处理图像（颜色转换、去马赛克等）
    if (int ret = rawProcessor->dcraw_process() != LIBRAW_SUCCESS)
    {
        qDebug() << "处理失败:" << libraw_strerror(ret);
        rawProcessor->recycle();
        return;
    }
    qDebug() << "3. 处理图像（颜色转换、去马赛克等）:" << libraw_strerror(ret);



    // 4. 获取处理后的图像数据
    libraw_processed_image_t *processed = rawProcessor->dcraw_make_mem_image(&ret);
    if (!processed || ret != LIBRAW_SUCCESS)
    {
        qDebug() << "无法生成图像数据:" << libraw_strerror(ret);
        rawProcessor->recycle();
        return;
    }
    qDebug() << "4. 获取处理后的图像数据:";
    qDebug() << "图像尺寸:" << processed->width << "x" << processed->height;
    qDebug() << "通道数:" << processed->colors;
    qDebug() << "位深:" << processed->bits;

    // 5. 将图像数据转换为 QImage
    // 根据 processed->colors 和 processed->bits 确定格式
    QImage::Format imageFormat;
    if (processed->colors == 3 && processed->bits == 8) {
        imageFormat = QImage::Format_RGB888;  // 8位/通道 RGB
    } else if (processed->colors == 3 && processed->bits == 16) {
        imageFormat = QImage::Format_RGB16;   // 16位/通道 RGB（需手动转换）
    } else {
        qDebug() << "不支持的图像格式，通道数:" << processed->colors << "位深:" << processed->bits;
        return;
    }

    QImage image(processed->data, processed->width, processed->height,
                 processed->width * processed->colors * (processed->bits / 8), // 计算步长
                 imageFormat);
    qDebug() << "5. 将图像数据转换为 QImage:" << libraw_strerror(ret);

    // 6. 显示图像和元数据
    QPixmap pixmap = QPixmap::fromImage(image).scaledToWidth(800);
    ui->imageLabel->setPixmap(pixmap);

    QString metadata;
    metadata += QString("相机型号: %1\n").arg(rawProcessor->imgdata.idata.model);
    metadata += QString("ISO: %1\n").arg(rawProcessor->imgdata.other.iso_speed);
    metadata += QString("光圈: f/%1\n").arg(rawProcessor->imgdata.other.aperture);
    metadata += QString("快门: 1/%1 秒\n").arg(1.0 / rawProcessor->imgdata.other.shutter);

    metadata += QString("镜头型号: %1\n").arg(rawProcessor->imgdata.lens.Lens);
    metadata += QString("焦距: %1mm\n").arg(rawProcessor->imgdata.other.focal_len);
    metadata += QString("拍摄时间: %1\n").arg(rawProcessor->imgdata.other.timestamp);
    // qDebug() << "原始 Exif 数据:" << rawProcessor->imgdata.other;


    ui->lbRawInfo->setText(metadata);
    qDebug() << "6. 显示图像和元数据";


    // 7. 保存为 JPG
    if (!image.save(savePath, "JPG", 90))
    {
        qDebug() << "保存 JPG 失败";
    }
    qDebug() << "7. 保存为 JPG";


    // 清理内存
    LibRaw::dcraw_clear_mem(processed);
    rawProcessor->recycle();
}

Widget::~Widget()
{
    delete ui;
}
