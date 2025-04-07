#include "blinkingwidget.h"

BlinkingWidget::BlinkingWidget(QWidget *parent)
    : QWidget(parent)
{
    blink  = false;
    connect(&blinkTimer, &QTimer::timeout, this, &BlinkingWidget::onBlink);
    blinkTimer.start(500);
}


void BlinkingWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    if(blink)
        painter.fillRect(this->rect(), QBrush(Qt::red));
    else
        painter.fillRect(this->rect(), QBrush(Qt::white));
}

void BlinkingWidget::onBlink()
{
    blink = !blink;
    this->update();
}
