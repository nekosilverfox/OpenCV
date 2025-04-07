#ifndef BLINKINGWIDGET_H
#define BLINKINGWIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QTimer>

class BlinkingWidget : public QWidget
{
    Q_OBJECT

public:
    BlinkingWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onBlink();

private:
    QTimer blinkTimer;
    bool blink;
};
#endif // BLINKINGWIDGET_H
