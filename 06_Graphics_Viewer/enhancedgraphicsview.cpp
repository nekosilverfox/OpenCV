#include "enhancedgraphicsview.h"

#include <QWheelEvent>

EnhancedGraphicsView::EnhancedGraphicsView(QWidget *parent)
    : QGraphicsView{parent}
{}

void EnhancedGraphicsView::wheelEvent(QWheelEvent *event)
{
    if (event->angleDelta().y() != 0)
    {
        double angleDeltaY = event->angleDelta().y();
        double zoomFactor = qPow(1.0015, angleDeltaY);
        scale(zoomFactor, zoomFactor);

        if(angleDeltaY > 0)
        {
            this->centerOn(sceneMousePos);
            sceneMousePos = this->mapToScene(event->position().toPoint());
        }

        this->viewport()->update();
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void EnhancedGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    sceneMousePos = this->mapToScene(event->pos());
}
