#ifndef BEE_CALIBERCIRCLE_H
#define BEE_CALIBERCIRCLE_H
#include <QObject>
#include <QWidget>
#include <QMouseEvent>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>
#include <QRect>
#include <QPainter>
#include <QPolygon>
#include <QList>
#include "mycorneritem.h"
#include<QtMath>

enum STATE_FLAG_ONECCIRCLE{
    DEFAULT_FLAG_OCIRC=0,
    MOV_CC,
    MOV_CPOINT,
    MOV_RPOINT

};

class bee_calibercircle:public QObject,public QGraphicsItem
{
public:
    bee_calibercircle(QGraphicsItem *parent = nullptr);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void setcircleize();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void setpixmapwidth(qreal width){pixmap_width = width;}
    void setpixmapheight(qreal height){pixmap_height = height;}
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

private:
    qreal pixmap_width;
    qreal pixmap_height;
    QList<mycorneritem*> m_HandlesList_circle;  //圆端点
    QList<mycorneritem*> m_HandlesList_refct; //可调控矩形端点
    QRectF first_rect;
    QPointF circle_center;
    QPointF first_center;
    QPointF pt[4]; //矩形端点
    QPointF m_startPos;
    STATE_FLAG_ONECCIRCLE m_StateFlag;
    bool if_hover = false;
    bool if_create = false;
    bool create_move = false;
    bool if_handleslist_create = false;
    qreal width; //矩形宽度
    qreal height; //矩形高度
    qreal r;
    int n;  //矩形个数


};

#endif // BEE_CALIBERCIRCLE_H
