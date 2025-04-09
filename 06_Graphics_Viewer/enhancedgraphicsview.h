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
    void mousePressEvent(QMouseEvent *event);

private slots:
    void clearAll(bool);
    void clearSelected(bool);
    void noEffect(bool);
    void blurEffect(bool);
    void dropShadowEffect(bool);
    void colorizeEffect(bool);
    void customEffect(bool);

signals:

private:
    QPointF sceneMousePos;
};

#endif // ENHANCEDGRAPHICSVIEW_H
