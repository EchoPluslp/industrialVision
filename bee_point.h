#pragma once
#ifndef BEE_POINT_H
#define BEE_POINT_H


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
enum STATE_FLAG_ONEPOINT {
	DEFAULT_FLAG_OP_POINT = 0,
	MOV_POINT_OP_POINT,
	MOV_POLYGON_OP_POINT,//标记当前为鼠标拖动图片移动状态
};

class bee_point :public QObject, public QGraphicsItem
{
	Q_OBJECT
public:
	bee_point(QGraphicsItem* parent = nullptr,int count = 0);

	QRectF  boundingRect() const;
	QPainterPath shape() const;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
	void mousePressEvent(QGraphicsSceneMouseEvent* event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event);
	void setPolygonsize();
	void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
	void hoverMoveEvent(QGraphicsSceneHoverEvent* event);
	void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);
	QPolygonF getpoly();
	void setpixmapwidth(qreal width) { pixmap_width = width; }
	void setpixmapheight(qreal height) { pixmap_height = height; }


public:
	//qreal pixmap_width;
	//qreal pixmap_height;
	//STATE_FLAG_ONEPOINT m_StateFlag;
	//QPointF m_startPos;
	//bool if_hover = false;
	//bool if_create = false;
	//bool if_handleslist_create = false;
	//bool create_move;
	//// 可移动的角点
	//mycorneritem* corner;
	//QVector<QPointF> pp;

	//测试
	QPolygonF m_poly;
	qreal pixmap_width;
	qreal pixmap_height;
	bool if_create = false;
	bool if_hover;
	QVector<QPointF> pp;
	int num; //多边形点个数
	int num_move;//移动点的下标
	QList<mycorneritem*> m_HandlesList;
	STATE_FLAG_ONEPOINT m_StateFlag;

	QPointF m_firstPos;//多边形起点
	QPointF m_startPos;
	mycorneritem* corner;
	bool m_bpress = false;
	QPointF movepoint;
	int index_value;
signals:
	void create_RECT(int type, int index);
};

#endif // BEE_POINT_H
