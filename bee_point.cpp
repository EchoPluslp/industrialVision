#include "bee_point.h"
bee_point::bee_point(QGraphicsItem* parent,int count) :
	m_StateFlag(DEFAULT_FLAG_OP_POINT)
{
	num = 0;
	num_move = 0;
	if_hover = false;
	pixmap_width = 1;
	pixmap_height = 1;
	setAcceptHoverEvents(true);
	setCursor(Qt::ArrowCursor);
	index_value = count;
}

QRectF bee_point::boundingRect() const
{
	if (!if_create)
		return QRectF(0, 0, pixmap_width, pixmap_height);
	else
		return QRectF(m_poly.boundingRect().topLeft().x() - 2, m_poly.boundingRect().topLeft().y() - 2, m_poly.boundingRect().width() + 4, m_poly.boundingRect().height() + 4);
}

QPainterPath bee_point::shape() const
{
	QPainterPath m_path;
	if (!if_create)
	{
		m_path.addRect(QRectF(0, 0, pixmap_width, pixmap_height));
		return m_path;
	}
	else
	{
		QPainterPath m_path2;
		m_path.addPolygon(m_poly);
		for (int i = 0; i < num; i++)
			m_path2.addRect(m_HandlesList[i]->boundingRect());
		return m_path.united(m_path2);
	}

}

void bee_point::setPolygonsize()
{
	if (if_create)
	{
		if (m_StateFlag == MOV_POINT_OP_POINT)
		{
			m_HandlesList[num_move]->move(pp[num_move]);
			m_poly = QPolygonF(pp);
		}
	}
}

void bee_point::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	if (!if_create && num >= 1)
	{
	/*	QPen mPen(Qt::magenta);
		mPen.setWidth(0);
		painter->setPen(mPen);
		for (int i = 1; i < num; i++)
		{
			painter->drawLine(pp[i - 1], pp[i]);
		}
		painter->drawLine(pp[num - 1], movepoint);
		painter->drawLine(movepoint, pp[0]);*/
	}
	if (if_create)
	{
		//QPen mPen(Qt::magenta);
		//if (if_hover)
		//	mPen.setColor(Qt::magenta);
		//mPen.setWidth(0);
		//painter->setPen(mPen);

		//QBrush brush(QColor(255, 0, 0), Qt::Dense7Pattern); //»­Ë¢
		//painter->setBrush(brush);

		//painter->drawPolygon(m_poly);
	}
}

void bee_point::mousePressEvent(QGraphicsSceneMouseEvent* event)
{

	if (event->button() == event->button() == Qt::LeftButton)
	{
		if (!if_create)
		{
			m_firstPos = event->pos();
			movepoint = event->pos();
			pp.push_back(event->pos());
			num++;
			corner = new mycorneritem(this, pp[num - 1], None);
			m_HandlesList.push_back(corner);
			m_bpress = true;
			//{
		/*	pp.pop_back();
			pp.push_back(m_firstPos);*/
			m_poly = QPolygonF(pp);
			if_create = true;
			emit create_RECT(5, index_value);
		}
		else
		{
			m_startPos = event->pos();
			for (int i = 0; i < num; i++)
			{
				if (m_HandlesList[i]->contains(event->pos()))
				{
					m_StateFlag = MOV_POINT_OP_POINT;
					num_move = i;
				}
			}
			if (m_StateFlag == DEFAULT_FLAG_OP_POINT)
			{
				if (this->shape().contains(m_startPos))
				{
					m_StateFlag = MOV_POLYGON_OP_POINT;
				}
			}
		}
		scene()->update();
	}
	setCursor(Qt::ArrowCursor);
	scene()->update();
}

void bee_point::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	if (!if_create && m_bpress)
	{
		movepoint = event->pos();
	}
	if (if_create)
	{
		if (m_StateFlag == MOV_POINT_OP_POINT)
		{
			pp[num_move] = event->pos();
		}
		else if (m_StateFlag == MOV_POLYGON_OP_POINT)
		{
			QPointF dis = event->pos() - m_startPos;
			moveBy(dis.x(), dis.y());
		}
	}
	setPolygonsize();
	scene()->update();
}


void bee_point::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	if (!if_create)
	{
		m_bpress = false;
		return;
	}
	else
	{
		m_StateFlag = DEFAULT_FLAG_OP_POINT;
	}

	scene()->update();


}

void bee_point::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
	if (!if_create)
	{
		//if (num > 2)
		//{
			pp.pop_back();
			pp.push_back(m_firstPos);
			m_poly = QPolygonF(pp);
			if_create = true;

		//}
		/*else
		{
			pp.pop_back();
			m_HandlesList.pop_back();
			delete corner;
			num -= 1;
		}*/

	}
	setCursor(Qt::ArrowCursor);
	scene()->update();
}


//todo Êä³öµã
void bee_point::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
	if_hover = true;
	scene()->update();
	QGraphicsItem::hoverEnterEvent(event);
}

void bee_point::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
	if (!if_create)
	{
		movepoint = event->pos();
		scene()->update();
	}
}

void bee_point::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
	if_hover = false;
	scene()->update();
	QGraphicsItem::hoverLeaveEvent(event);
}
QPolygonF bee_point::getpoly()
{
	return m_poly;
}
