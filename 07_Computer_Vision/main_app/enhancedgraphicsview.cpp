#include "enhancedgraphicsview.h"

#include <QWheelEvent>
#include <QMenu>
#include <QGraphicsItem>

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

    QGraphicsView::mouseMoveEvent(event);  // 事件传递给父类。QGraphicsView 的默认 mouseMoveEvent 负责处理橡皮筋拖拽过程中的矩形绘制和选择逻辑。如果未调用基类方法，橡皮筋的拖拽过程会被中断。
}

void EnhancedGraphicsView::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        QMenu menu;
        QAction *clearAllAction = menu.addAction("Clear All");
        connect(clearAllAction, &QAction::triggered, this, &EnhancedGraphicsView::clearAll);

        QAction *clearSelectedAction = menu.addAction("Clear Selected");
        connect(clearSelectedAction, &QAction::triggered, this, &EnhancedGraphicsView::clearSelected);

        QAction *noEffectAction = menu.addAction("No Effect");
        connect(noEffectAction, &QAction::triggered, this, &EnhancedGraphicsView::noEffect);

        QAction *blurEffectAction = menu.addAction("Blur Effect");
        connect(blurEffectAction, &QAction::triggered, this, &EnhancedGraphicsView::blurEffect);

        QAction *dropShadowEffectAction = menu.addAction("Drop Shadow Effect");
        connect(dropShadowEffectAction, &QAction::triggered, this, &EnhancedGraphicsView::dropShadowEffect);

        QAction *colorizeEffectAction = menu.addAction("Colorize Effect");
        connect(colorizeEffectAction, &QAction::triggered, this, &EnhancedGraphicsView::colorizeEffect);

        QAction *customEffectAction = menu.addAction("Custom Effect");
        connect(customEffectAction, &QAction::triggered, this, &EnhancedGraphicsView::customEffect);

        menu.exec(event->globalPosition().toPoint());
        event->accept();
    }
    else
    {
        QGraphicsView::mousePressEvent(event);  // 向上传递
    }
}

void EnhancedGraphicsView::clearAll(bool)
{
    scene()->clear();
}

void EnhancedGraphicsView::clearSelected(bool)
{
    while(scene()->selectedItems().count() > 0)
    {
        delete scene()->selectedItems().at(0);
        scene()->selectedItems().removeAt(0);
    }
}

void EnhancedGraphicsView::noEffect(bool)
{
    foreach(QGraphicsItem *item, scene()->selectedItems())
    {
        item->setGraphicsEffect(Q_NULLPTR);
    }
}

#include <QGraphicsBlurEffect>
void EnhancedGraphicsView::blurEffect(bool)
{
    foreach(QGraphicsItem *item, scene()->selectedItems())
    {
        item->setGraphicsEffect(new QGraphicsBlurEffect(this));
    }
}

#include <QGraphicsDropShadowEffect>
void EnhancedGraphicsView::dropShadowEffect(bool)
{
    for (auto i : scene()->selectedItems())
    {
        i->setGraphicsEffect(new QGraphicsDropShadowEffect(this));
    }
}

#include <QGraphicsColorizeEffect>
void EnhancedGraphicsView::colorizeEffect(bool)
{
    for (auto i : scene()->selectedItems())
    {
        i->setGraphicsEffect(new QGraphicsColorizeEffect(this));
    }
}

#include "customgraphicseffect.h"
void EnhancedGraphicsView::customEffect(bool)
{
    for (auto i : scene()->selectedItems())
    {
        i->setGraphicsEffect(new CustomGraphicsEffect(this));
    }
}
