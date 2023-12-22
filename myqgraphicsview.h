#ifndef MYQGRAPHICSVIEW_H
#define MYQGRAPHICSVIEW_H
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QWidget>

class myqgraphicsview:public QGraphicsView
{
public:
    myqgraphicsview(QWidget *parent);
    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void enterEvent(QEvent *event);


    void drawBackground(QPainter * painter, const QRectF & rect);

public:
    void zoomIn();
    void zoomOut();
    void resetZoom();
private:
    qreal m_scaled;
};

#endif // MYQGRAPHICSVIEW_H
