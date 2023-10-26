#ifndef MYGRAPHICSITEMS_H
#define MYGRAPHICSITEMS_H
#pragma execution_character_set("utf-8")

#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsPolygonItem>
#include <QGraphicsPathItem>
#include <QGraphicsScene>

//��дһϵ��GraphicsItem����֧���Զ���������趨���϶���Χ����

class MyGraphicsRectItem : public QGraphicsRectItem
{
public:
	explicit MyGraphicsRectItem(QGraphicsItem* parent = nullptr);
	using QGraphicsRectItem::QGraphicsRectItem;
	void setScenePos(QPointF pos);
protected:
	//�޶��϶���Χq
	QVariant itemChange(GraphicsItemChange change, const QVariant& value);
};

class MyGraphicsEllipseItem : public QGraphicsEllipseItem
{
public:
	explicit MyGraphicsEllipseItem(QGraphicsItem* parent = nullptr);
	using QGraphicsEllipseItem::QGraphicsEllipseItem;
	void setScenePos(QPointF pos);
protected:
	//�޶��϶���Χ
	QVariant itemChange(GraphicsItemChange change, const QVariant& value);
};

class MyGraphicsPolygonItem : public QGraphicsPolygonItem
{
public:
	explicit MyGraphicsPolygonItem(QGraphicsItem* parent = nullptr);
	using QGraphicsPolygonItem::QGraphicsPolygonItem;
	void setScenePos(QPointF pos);
protected:
	//�޶��϶���Χ
	QVariant itemChange(GraphicsItemChange change, const QVariant& value);
};

class MyGraphicsPathItem : public QGraphicsPathItem
{
public:
	explicit MyGraphicsPathItem(QGraphicsItem* parent = nullptr);
	using QGraphicsPathItem::QGraphicsPathItem;
	void setScenePos(QPointF pos);
protected:
	//�޶��϶���Χ
	QVariant itemChange(GraphicsItemChange change, const QVariant& value);
};
#endif // MYGRAPHICSITEMS_H
