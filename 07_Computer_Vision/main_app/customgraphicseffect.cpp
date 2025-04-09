#include "customgraphicseffect.h"

CustomGraphicsEffect::CustomGraphicsEffect(QObject *parent)
    : QGraphicsEffect {parent}
{}

/**
 * @brief CustomGraphicsEffect::draw 简单的阈值滤镜，根据像素灰度值将其设为纯黑或纯白
 * @param painter
 */
void CustomGraphicsEffect::draw(QPainter *painter)
{
    /*
     * 图像转换为灰度图（QImage::Format_Grayscale8），
     * 所以每个像素在内存中的表示已经不再是 RGB 值，
     * 而是一个单独的灰度值（一个字节），
     * 其取值范围是 0 到 255
     */
    QImage image = sourcePixmap().toImage();
    image = image.convertToFormat(QImage::Format_Grayscale8);

    for(int i=0; i<image.sizeInBytes(); i++)
        image.bits()[i] = (image.bits()[i] < 100) ? 0 : 255;

    painter->drawPixmap(0, 0, QPixmap::fromImage(image));
}
