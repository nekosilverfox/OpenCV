#include "histogramchart.h"

#include <QtMath>
#include <QValueAxis>

HistogramChart::HistogramChart(QWidget *parent) : QChartView(parent)
{
    _chart = new QChart();
    _chart->setBackgroundBrush(QBrush(Qt::transparent));
    _chart->legend()->setVisible(false);

    setChart(_chart);

    createAxes();
}

void HistogramChart::updateHistogram(const QImage &image) {
    // 清空旧数据
    qDeleteAll(_areaSeries);
    _areaSeries.clear();
    qDeleteAll(_series);
    _series.clear();

    // 计算各通道直方图数据
    QVector<int> histRed(256, 0), histGreen(256, 0), histBlue(256, 0), histLuminance(256, 0);
    for (int y = 0; y < image.height(); ++y)
    {
        for (int x = 0; x < image.width(); ++x)
        {
            QRgb pixel = image.pixel(x, y);
            int r = qRed(pixel);
            int g = qGreen(pixel);
            int b = qBlue(pixel);
            int luminance = qRound(0.299 * r + 0.587 * g + 0.114 * b); // 亮度计算公式

            histRed[r]++;
            histGreen[g]++;
            histBlue[b]++;
            histLuminance[qBound(0, luminance, 255)]++;
        }
    }

    // 归一化到 [0, 1]
    auto normalize = [](QVector<int> &data) -> QVector<float>
    {
        int max = *std::max_element(data.begin(), data.end());
        if (max == 0) max = 1;
        QVector<float> normalized;
        for (int val : data) {
            normalized.push_back(static_cast<float>(val) / max);
        }
        return normalized;
    };

    QVector<float> redNorm       = normalize(histRed);
    QVector<float> greenNorm     = normalize(histGreen);
    QVector<float> blueNorm      = normalize(histBlue);
    QVector<float> luminanceNorm = normalize(histLuminance);

    // 添加通道曲线（Lightroom 风格叠加）
    addChannelSeries(redNorm,       QColor(255, 0, 0, 150),     "Red");
    addChannelSeries(greenNorm,     QColor(0, 255, 0, 150),     "Green");
    addChannelSeries(blueNorm,      QColor(0, 0, 255, 150),     "Blue");
    addChannelSeries(luminanceNorm, QColor(200, 200, 200, 100), "Luminance");

    _chart->update();
}

void HistogramChart::addChannelSeries(const QVector<float> &data, const QColor &color, const QString &name)
{
    QLineSeries *series = new QLineSeries();
    series->setName(name);
    for (int i = 0; i < data.size(); ++i)
    {
        *series << QPointF(i, data[i]);
    }

    // 创建填充区域
    QAreaSeries *area = new QAreaSeries(series);
    area->setColor(color);
    area->setBorderColor(color.darker(100));  // .darker() 亮度降低
    area->setOpacity(0.5); // 透明度设置

    _series.append(series);
    _areaSeries.append(area);
    _chart->addSeries(area);
}

void HistogramChart::createAxes()
{
    // X 轴（0-255）
    QValueAxis *axisX = new QValueAxis();
    axisX->setRange(0, 255);
    axisX->setLabelFormat("%d");
    axisX->setTitleText("Pixel Value");
    axisX->setGridLineVisible(false);

    // Y 轴（0-1）
    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, 1.0);
    axisY->setTitleText("Normalized Frequency");
    axisY->setGridLineVisible(false);

    _chart->addAxis(axisX, Qt::AlignBottom);
    _chart->addAxis(axisY, Qt::AlignLeft);

    foreach (QAbstractSeries *series, _chart->series())
    {
        series->attachAxis(axisX);
        series->attachAxis(axisY);
    }
}
