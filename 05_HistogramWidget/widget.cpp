#include "widget.h"
#include "ui_widget.h"

#include "histogramchart.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);


    HistogramChart *chart = new HistogramChart(this);
    chart->setRenderHint(QPainter::Antialiasing);
    chart->resize(600, 300);
    chart->show();

    // 加载图像并更新直方图
    QImage image("/Users/fox/Desktop/IMG_20250314_034840.jpg");
    if (!image.isNull()) {
        chart->updateHistogram(image);
    }

}

Widget::~Widget()
{
    delete ui;
}
