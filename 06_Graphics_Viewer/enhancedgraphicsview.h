#ifndef ENHANCEDGRAPHICSVIEW_H
#define ENHANCEDGRAPHICSVIEW_H

#include <QWidget>
#include <QGraphicsView>

class EnhancedGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit EnhancedGraphicsView(QWidget *parent = nullptr);

protected:
    void wheelEvent(QWheelEvent *event);
    void mouseMoveEvent(QMouseEvent *event);


signals:

private:
    QPointF sceneMousePos;
};

#endif // ENHANCEDGRAPHICSVIEW_H
