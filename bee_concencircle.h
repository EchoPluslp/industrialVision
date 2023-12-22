#ifndef BEE_CONCENCIRCLE_H
#define BEE_CONCENCIRCLE_H
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

enum STATE_FLAG_ONECONCENCIRCLE{
    DEFAULT_FLAG_OCC=0,
    SCALE_CIRCLE1,
    SCALE_CIRCLE2,
    MOV_CONCENCIRCLE,//标记当前为鼠标拖动图片移动状态
};


class bee_concencircle:public QObject,public QGraphicsItem
{
public:
    bee_concencircle(QGraphicsItem *parent = nullptr);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void setpixmapwidth(qreal width){pixmap_width = width;}
    void setpixmapheight(qreal height){pixmap_height = height;}

    QPointF getconcenter(){return center_concircle;}
    QPointF gettop1(){return pt1[0];}
    QPointF gettop2(){return pt2[0];}
    qreal getr1(){return r1;}
    qreal getr2(){return r2;}
    QRectF boundingRect() const;
    QPainterPath shape() const;
    QRectF boundingRect1;
    QRectF boundingRect2;

private:
    qreal pixmap_width;
    qreal pixmap_height;
    STATE_FLAG_ONECONCENCIRCLE m_StateFlag;
    QPointF m_startPos;
    bool is_chosse=false;
    QPointF center_concircle;
    qreal r1; //内圆半径
    qreal r2; //外圆半径
    QPointF pt1[4]; //园的上下左右四个点 顺序0123 上右下左
    QPointF pt2[4];
    QList<mycorneritem*> m_HandlesList1;
    QList<mycorneritem*> m_HandlesList2;
    bool if_hover = false;
    bool if_create = false;
    bool create_move;
    bool if_handleslist_create = false;
};

#endif // BEE_CONCENCIRCLE_H
