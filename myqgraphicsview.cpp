#include "myqgraphicsview.h"
#include "qdebug.h"
#include "QApplication"

#include <QtMath>

myqgraphicsview::myqgraphicsview(QWidget *parent):
    m_scaled(1)
{
    setRenderHint(QPainter::Antialiasing);
    centerOn(0, 0);
    setDragMode(ScrollHandDrag);
    viewport()->setCursor(Qt::ArrowCursor);
}

void myqgraphicsview::wheelEvent(QWheelEvent *event)
{
    // 当前放缩倍数;
    int wheelDeltaValue = event->delta();
    // 向上滚动，放大;
    if (wheelDeltaValue > 0)
    {
        this->scale(1.05, 1.05);
        m_scaled*=1.05;
    }
    // 向下滚动，缩小;
    else
    {
//        if (m_scaled < 0.5)
//        {
//            return;
//        }
        this->scale(1/1.05, 1/1.05);
        m_scaled/=1.05;
    }
}

void myqgraphicsview::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);  //将event传入scene
}


void myqgraphicsview::drawBackground(QPainter * painter, const QRectF & rect)
{
    //绘制灰白棋盘图像背景
    int wid = this->geometry().width();
    int hei = this->geometry().height();
    QPointF m_ptCenter = this->mapToScene(wid / 2, hei / 2);
    QPixmap pix(wid, hei);
    QPainter pter(&pix);
    QColor clr_white(Qt::white);
    QColor clr_gray(240, 240, 240, 240);
    int spacing = 15;
    QColor useColor;
    for (int i = 0; i <= floor(wid / spacing); i++)
    {
        for (int j = 0; j <= floor(hei / spacing); j++)
        {
            useColor = ((i + j) % 2 == 0 ? clr_white : clr_gray);
            pter.fillRect(i*spacing, j*spacing, spacing, spacing, useColor);
        }
    }
    painter->drawImage(rect, pix.toImage());
}

void myqgraphicsview::zoomIn()
{
    this->scale(1.05, 1.05);
    m_scaled*=1.05;
}

void myqgraphicsview::zoomOut()
{
//    if (m_scaled < 0.5)
//    {
//        return;
//    }
    this->scale(1/1.05, 1/1.05);
    m_scaled/=1.05;
}

void myqgraphicsview::resetZoom()
{
    resetMatrix();
    m_scaled = 1.0;
}


void myqgraphicsview::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mouseMoveEvent(event);
}

void myqgraphicsview::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
    viewport()->setCursor(Qt::ArrowCursor);
}

void myqgraphicsview::enterEvent(QEvent *event)
{
    viewport()->setCursor(Qt::ArrowCursor);
}


