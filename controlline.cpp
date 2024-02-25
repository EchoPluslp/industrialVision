#include "controlline.h"
#include "qdebug.h"

CControlLine::CControlLine(QGraphicsItem* parent)
{
	Q_UNUSED(parent)

		linestart = QPointF(0, 0);
	lineend = QPointF(0, 0);
}

QRectF CControlLine::boundingRect() const
{
	return QRectF(linestart, lineend);
}

void CControlLine::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{  
	Q_UNUSED(option)
		Q_UNUSED(widget)

	QPen mPen(Qt::green);
	mPen.setWidth(0);
	painter->setPen(mPen);
	painter->drawLine(linestart, lineend);
	 
	if (draw_edge)
	{
		QBrush brush1(Qt::green, Qt::SolidPattern);
		painter->setBrush(brush1);
		for (int i = 0; i < edgepoints.size(); i++)
		{
			painter->drawLine(edgepoints[i].x(), edgepoints[i].y(),
				edgepoints[i].x(), edgepoints[i].y() );

			painter->drawLine(edgepoints[i].x() , edgepoints[i].y() ,
				edgepoints[i].x(), edgepoints[i].y() );
		}
	}

	if (draw_expect)
	{
		mPen.setWidth(5);

		mPen.setColor(Qt::yellow);
		QBrush brush(Qt::yellow, Qt::SolidPattern);
		painter->setPen(mPen);
		painter->setBrush(brush);
		painter->setRenderHint(QPainter::Antialiasing, true);

		for (int i = 0; i < edgeexpectpoints.size(); i++)
		{
			painter->drawPoint(edgeexpectpoints[i]);
			/*	painter->drawLine(edgeexpectpoints[i].x(), edgeexpectpoints[i].y() ,
					edgeexpectpoints[i].x() , edgeexpectpoints[i].y() );

				painter->drawLine(edgeexpectpoints[i].x() + 1, edgeexpectpoints[i].y() ,
					edgeexpectpoints[i].x() , edgeexpectpoints[i].y() );*/
		}
	}
	if (draw_circle)
	{
		mPen.setWidth(2);
		painter->setPen(mPen);
		painter->setRenderHint(QPainter::Antialiasing, true);
		painter->drawEllipse(circle_Center,circle_Radius,circle_Radius);
	}

	scene()->update();
}

void CControlLine::addedgepoints(QPointF pt)
{
	edgepoints.clear();
	edgepoints.push_back(pt);
	draw_edge = true;
}

void CControlLine::addedgeexpectpoints(QPointF pt)
{
	edgeexpectpoints.clear();
	edgeexpectpoints.push_back(pt);
	draw_expect = true;
}

void CControlLine::addcircles(cv::Point2d& pdCenter, double& dRadius)
{

	circle_Center.setX(pdCenter.x);
	circle_Center.setY(pdCenter.y);
	circle_Radius = dRadius;
	draw_circle = true;

}
