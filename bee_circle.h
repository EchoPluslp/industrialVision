#ifndef BEE_CIRCLE_H
#define BEE_CIRCLE_H


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

enum STATE_FLAG_ONECIRCLE{
    DEFAULT_FLAG_OC=0,
    SCALE_CIRCLE,
    MOV_CIRCLE,//标记当前为鼠标拖动图片移动状态
};

class bee_circle:public QObject,public QGraphicsItem
{
public:
    bee_circle(QGraphicsItem *parent = nullptr);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void setpixmapwidth(qreal width){pixmap_width = width;}
    void setpixmapheight(qreal height){pixmap_height = height;}
    QRectF boundingRect() const;
    QPainterPath shape() const;
    QRectF boundingrect;

private:
    qreal pixmap_width;
    qreal pixmap_height;
    QPointF center_circle;
    qreal r;
    STATE_FLAG_ONECIRCLE m_StateFlag;
    QPointF m_startPos;
    QPointF pt[4]; //圆的上下左右四个点 顺序0123对应圆的四个角点(上右下左)
    QList<mycorneritem*> m_HandlesList;
    bool if_hover = false;
    bool if_create = false;
    bool if_handleslist_create = false;
    bool create_move;
    QRectF  first_rect; //可调控矩形

};

#endif // BEE_CIRCLE_H
