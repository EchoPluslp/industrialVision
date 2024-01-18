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
			painter->drawLine(edgepoints[i].x() - 1, edgepoints[i].y() - 1,
				edgepoints[i].x() + 1, edgepoints[i].y() + 1);

			painter->drawLine(edgepoints[i].x() + 1, edgepoints[i].y() - 1,
				edgepoints[i].x() - 1, edgepoints[i].y() + 1);
		}
	}

	if (draw_expect)
	{
		mPen.setColor(Qt::red);
		QBrush brush(Qt::red, Qt::SolidPattern);
		painter->setPen(mPen);
		painter->setBrush(brush);

		for (int i = 0; i < edgeexpectpoints.size(); i++)
		{
			painter->drawLine(edgeexpectpoints[i].x() - 1, edgeexpectpoints[i].y() - 1,
				edgeexpectpoints[i].x() + 1, edgeexpectpoints[i].y() + 1);

			painter->drawLine(edgeexpectpoints[i].x() + 1, edgeexpectpoints[i].y() - 1,
				edgeexpectpoints[i].x() - 1, edgeexpectpoints[i].y() + 1);
		}
	}


	scene()->update();
}

void CControlLine::addedgepoints(QPointF pt)
{
	edgepoints.push_back(pt);
	draw_edge = true;
}

void CControlLine::addedgeexpectpoints(QPointF pt)
{
	edgeexpectpoints.push_back(pt);
	draw_expect = true;
}
