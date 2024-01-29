#ifndef CONTROL_LINE_H
#define CONTROL_LINE_H

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
#include <QDebug>
#include <QtMath>
#include <opencv2/opencv.hpp>


class CControlLine :public QObject, public QGraphicsItem
{
	
public:
	CControlLine(QGraphicsItem* parent = nullptr);
	QRectF boundingRect() const;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
	void addedgepoints(QPointF pt);
	void addedgeexpectpoints(QPointF pt);
	void setline(QPointF pt1, QPointF pt2) { linestart = pt1; lineend = pt2; }\
	void addcircles(cv::Point2d& pdCenter, double& dRadius);
	~CControlLine()
	{
		qDebug() << "~one_line";
	}

private:
	QPointF linestart;
	QPointF lineend;
	QList<QPointF> edgepoints;
	bool draw_edge = false;
	QList<QPointF> edgeexpectpoints;
	bool draw_expect = false;

	QPointF circle_Center;
	qreal circle_Radius;
	bool draw_circle = false;

};

#endif
