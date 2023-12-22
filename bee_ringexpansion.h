#ifndef BEE_RINGEXPANSION_H
#define BEE_RINGEXPANSION_H
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

enum STATE_FLAG_ONERINGEX{
    DEFAULT_FLAG_ORE=0,
    MOV_R1,
    MOV_R2,
    MOV_PT1,
    MOV_PT2,
    MOV_STARTSPAN,
    MOV_ENDSPAN,
    MOV_RINGEXPANSION
};

class bee_ringexpansion:public QObject,public QGraphicsItem
{
public:
    bee_ringexpansion(QGraphicsItem *parent = nullptr);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void setringsize();
    void setpixmapwidth(qreal width){pixmap_width = width;}
    void setpixmapheight(qreal height){pixmap_height = height;}

    QPointF getconcenter(){return concenter;}
    qreal getr1(){return r1;}
    qreal getr2(){return r2;}
    qreal getstartspan(){return startspan;}
    qreal getenspan(){return endspan;}
    qreal gettheta(){return theta;}
    qreal getwidth(){return width;}
    QRectF boundingRect() const;
    QPainterPath shape() const;
    QPainterPath getshape();
    QRectF boundingRect1;
    QRectF boundingRect2;

private:
    qreal pixmap_width;
    qreal pixmap_height;
    STATE_FLAG_ONERINGEX m_StateFlag;
    QPointF m_startPos;
    QPointF concenter;
    qreal r1;
    qreal r2;
    qreal startspan;
    qreal endspan;
    qreal middlespan;
    qreal theta;
    qreal width;
    QPointF pt1[3];
    QPointF pt2[3];
    QPointF ptmove[2];
    QList<mycorneritem*> m_HandlesList_angle;
    QList<mycorneritem*> m_HandlesList_r1r2;
    QList<mycorneritem*> m_HandlesList_pt1pt2;
    bool if_hover = false;
    bool if_create = false;
    bool create_move;
    bool if_handleslist_create = false;
    bool flag = false;
};

#endif
