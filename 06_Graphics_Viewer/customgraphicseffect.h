#ifndef CUSTOMGRAPHICSEFFECT_H
#define CUSTOMGRAPHICSEFFECT_H

#include <QObject>
#include <QGraphicsEffect>
#include <QPainter>

class CustomGraphicsEffect : public QGraphicsEffect
{
    Q_OBJECT
public:
    explicit CustomGraphicsEffect(QObject *parent = nullptr);

protected:
    void draw(QPainter *painter) override;

signals:
};

#endif // CUSTOMGRAPHICSEFFECT_H
