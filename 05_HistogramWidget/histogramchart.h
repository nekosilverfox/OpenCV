#ifndef HISTOGRAMCHART_H
#define HISTOGRAMCHART_H

#include <QObject>
#include <QChartView>
#include <QAreaSeries>
#include <QLineSeries>
#include <QImage>


class HistogramChart : public QChartView
{
    Q_OBJECT
public:
    explicit HistogramChart(QWidget *parent = nullptr);

    void updateHistogram(const QImage &image);

private:
    void createAxes();
    void addChannelSeries(const QVector<float> &data, const QColor &color, const QString &name);

    QChart* _chart;
    QVector<QLineSeries*> _series;
    QVector<QAreaSeries*> _areaSeries;
};

#endif // HISTOGRAMCHART_H
