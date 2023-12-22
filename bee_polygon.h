#ifndef BEE_POLYGON_H
#define BEE_POLYGON_H
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
#include <qdebug.h>

enum STATE_FLAG_ONEPOLYGON{
    DEFAULT_FLAG_OP=0,
    MOV_POINT_OP,
    MOV_POLYGON_OP,//标记当前为鼠标拖动图片移动状态
};

class bee_polygon:public QObject,public QGraphicsItem
{
public:
    bee_polygon(QGraphicsItem *parent = nullptr);

    QRectF  boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void setPolygonsize();
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    QPolygonF getpoly();
    void setpixmapwidth(qreal width){pixmap_width = width;}
    void setpixmapheight(qreal height){pixmap_height = height;}


private:
    QPolygonF m_poly;
    qreal pixmap_width;
    qreal pixmap_height;
    bool if_create = false;
    bool if_hover;
    QVector<QPointF> pp;
    int num; //多边形点个数
    int num_move;//移动点的下标
    QList<mycorneritem*> m_HandlesList;
    QPointF m_firstPos;//多边形起点
    QPointF m_startPos;
    STATE_FLAG_ONEPOLYGON m_StateFlag;
    mycorneritem* corner;
    bool m_bpress = false;
    QPointF movepoint;

};

#endif // BEE_POLYGON_H
