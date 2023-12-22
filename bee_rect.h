#ifndef BEE_RECT_H
#define BEE_RECT_H
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

enum STATE_FLAG_ONERECT
{
    DEFAULT_FLAG_OR=0,

    MOV_OAT0,//标记当前为用户按下矩形的左上点，qt[0]
    MOV_OAT1,//标记当前为用户按下矩形的左下点，qt[3]
    MOV_OAT2,//标记当前为用户按下矩形的右上点，qt[1]
    MOV_OAT3,//标记当前为用户按下矩形的右下点，qt[2]

    MOV_RECT_OR,//标记当前为鼠标拖动图片移动状态
    ROTATE_OR//标记当前为旋转状态
};

class bee_rect:public QAbstractGraphicsShapeItem
{
public:
    bee_rect(QGraphicsItem *parent = nullptr, bool flag=false);
    void setRectSize(QRectF mrect,bool bResetRotateCenter = true);
    QPointF getRotatePoint(QPointF ptCenter, QPointF ptIn, qreal angle);//获取旋转后的点
    QRectF  boundingRect() const;
    QPainterPath shape() const;
    QPolygonF getpoly(){return m_poly;}

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void SetRotate(qreal RotateAngle,QPointF ptCenter=QPointF(-999,-999));
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void setpixmapwidth(qreal width){pixmap_width = width;}
    void setpixmapheight(qreal height){pixmap_height = height;}



private:
    qreal pixmap_width;
    qreal pixmap_height;
    QPointF m_startPos;
    QPointF pt[4]; //矩形四个点
    QPointF pp[4]; //多边形的四个点
    int n_flag;    //选中哪一个点进行拖动
    STATE_FLAG_ONERECT m_StateFlag;
    bool if_hover = false;//是否被选中
    QList<mycorneritem*> m_HandlesList;//角的列表
    bool m_bRotate;
    QRectF m_rect;         //旋转前的矩形
    QPolygonF m_poly;     //旋转后的多边形
    qreal rotate_angle;   //旋转角度
    QPointF rotate_center; //旋转中心
    bool if_rotate = false; //是否为旋转矩形
    bool if_create = false;
    bool if_handleslist_create = false;
    bool create_move = false;
};

#endif // BEE_RECT_H
