#include "bee_caliberline.h"
#include <QtMath>
bee_caliberline::bee_caliberline(QGraphicsItem *parent)
{
    nMeasureNums = 5;
	length = 0;

    setAcceptHoverEvents(true);
    setCursor(Qt::ArrowCursor);
	m_plineCaliperGUI = new CLineCaliperGUI();

}
qreal getdistance_2(QPointF pt1, QPointF pt2)
{
	qreal dis = sqrt((pt1.x() - pt2.x()) * (pt1.x() - pt2.x()) +
		(pt1.y() - pt2.y()) * (pt1.y() - pt2.y()));
	return dis;
}

QPointF getRotatePoint_2(QPointF ptCenter, QPointF ptIn, qreal angle)
{
	double dx = ptCenter.x();
	double dy = ptCenter.y();
	double x = ptIn.x();
	double y = ptIn.y();
	double xx;
	double yy;

	xx = (x - dx) * cos(angle * M_PI / 180) - (y - dy) * sin(angle * M_PI / 180) + dx;
	yy = (x - dx) * sin(angle * M_PI / 180) + (y - dy) * cos(angle * M_PI / 180) + dy;

	return QPointF(xx, yy);
}

QRectF bee_caliberline::boundingRect() const
{
	if (!if_create)
	{
		return QRectF(0, 0, pixmap_width, pixmap_height);
	}
	else
	{
		QVector<QPointF> vpt;
		vpt.append(pt_begin);
		vpt.append(pp[0]);
		vpt.append(pp[1]);
		vpt.append(pt_end);
		vpt.append(pp[2]);
		vpt.append(pp[3]);
		vpt.append(pt_begin);

		QRectF re = QPolygonF(vpt).boundingRect();
		return QRectF(re.topLeft().x() - 5,
			re.topLeft().y() - 5,
			re.width() + 10,
			re.height() + 10);
	}
}

QPainterPath bee_caliberline::shape() const
{
    QPainterPath m_path;
    if(!if_create)
    {
        m_path.addRect(QRectF(0,0,pixmap_width,pixmap_height));
        return m_path;
    }
    else
    {
        QPainterPath m_path2;
        QPointF pt_n[4];
        QPointF pp_n[4];
        QVector<QPointF> vpt;
        QPointF center_n;
        qreal angle = atan(k)*180/M_PI;

        center_n = pt_begin;
        pt_n[0] = QPointF(center_n.x(),center_n.y()-height/2);
        pp_n[0] = getRotatePoint_2(center_n,pt_n[0],angle);
        pt_n[3] = QPointF(center_n.x(),center_n.y()+height/2);
        pp_n[3] = getRotatePoint_2(center_n,pt_n[3],angle);
        center_n = pt_end;
        pt_n[1] = QPointF(center_n.x(),center_n.y()-height/2);
        pp_n[1] = getRotatePoint_2(center_n,pt_n[1],angle);
        pt_n[2] = QPointF(center_n.x(),center_n.y()+height/2);
        pp_n[2] = getRotatePoint_2(center_n,pt_n[2],angle);
        for(int i=0;i<4;i++)
        {
            m_path2.addRect(m_HandlesList_refct[i]->boundingRect());
            vpt.append(pp_n[i]);
        }
        vpt.append(pp_n[0]);
        m_path.addPolygon(QPolygonF(vpt));
        m_path.closeSubpath();
        m_path2.addRect(m_HandlesList_line[0]->boundingRect());
        m_path2.addRect(m_HandlesList_line[1]->boundingRect());


        return m_path.united(m_path2);

    }

}

void bee_caliberline::setlinsize()
{
	m_HandlesList_line[0]->move(pt_begin);
	m_HandlesList_line[1]->move(pt_end);

	k = (pt_end.y() - pt_begin.y()) / (pt_end.x() - pt_begin.x());
	qreal angle = atan(k) * 180 / M_PI;

	first_center = pt_begin + (pt_end - pt_begin) / nMeasureNums;
	pt[0] = QPointF(first_center.x() - width / 2, first_center.y() - height / 2);
	pt[1] = QPointF(first_center.x() + width / 2, first_center.y() - height / 2);
	pt[2] = QPointF(first_center.x() + width / 2, first_center.y() + height / 2);
	pt[3] = QPointF(first_center.x() - width / 2, first_center.y() + height / 2);
	QVector<QPointF> vpt;
	for (int i = 0; i < 4; i++)
	{
		pp[i] = getRotatePoint_2(first_center, pt[i], angle);
		vpt.append(pp[i]);
		m_HandlesList_refct[i]->move(pp[i]);
	}
	vpt.append(pp[0]);
	first_poly = QPolygonF(vpt);
	first_center_top = (pp[0] + pp[1]) / 2;
	first_center_bottom = (pp[2] + pp[3]) / 2;
	length = getdistance_2(pt_begin, pt_end);
}

void bee_caliberline::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option)
		Q_UNUSED(widget)
		if (!if_create)
		{
			QPen mPen(Qt::blue);
			mPen.setWidth(0);
			painter->setPen(mPen);
			painter->drawLine(pt_begin, pt_end);
		}
		else
		{
			QPen mPen(Qt::blue);
			if (if_hover)
			{
				mPen.setColor(Qt::green);
			}
			QPointF begin = pt_begin;
			QPointF end = pt_end;
			qreal angle;
			if (begin.x() <= end.x())
			{
				angle = atan(k) + M_PI;
			}
			else
			{
				angle = atan(k);
			}

			qreal ExtRefArrowDegrees = M_PI / 8;
			qreal arrow_length = getdistance_2(pt_begin, pt_end) / 20;

			QPointF P1 = QPointF(end.x() + arrow_length * cos(angle - ExtRefArrowDegrees),
				end.y() + arrow_length * sin(angle - ExtRefArrowDegrees));

			QPointF P2 = QPointF(end.x() + arrow_length * cos(angle + ExtRefArrowDegrees),
				end.y() + arrow_length * sin(angle + ExtRefArrowDegrees));
			mPen.setWidth(0);
			painter->setPen(mPen);
			painter->setRenderHint(QPainter::Antialiasing, true);
			painter->drawLine(begin, end);
			painter->drawLine(P1, end);
			painter->drawLine(end, P2);
			painter->drawPolygon(first_poly);

			QPointF pt_n[4];
			QPointF pp_n[4];
			QVector<QPointF> vpt;
			QPointF center_n;
			angle = atan(k) * 180 / M_PI;
			for (int i = 1; i < nMeasureNums - 1; i++)
			{
				center_n = pt_begin + (pt_end - pt_begin) / nMeasureNums * (i + 1);
				pt_n[0] = QPointF(center_n.x() - width / 2, center_n.y() - height / 2);
				pt_n[1] = QPointF(center_n.x() + width / 2, center_n.y() - height / 2);
				pt_n[2] = QPointF(center_n.x() + width / 2, center_n.y() + height / 2);
				pt_n[3] = QPointF(center_n.x() - width / 2, center_n.y() + height / 2);

				for (int i = 0; i < 4; i++)
				{
					pp_n[i] = getRotatePoint_2(center_n, pt_n[i], angle);
					vpt.append(pp_n[i]);
				}
				vpt.append(pp_n[0]);
				painter->drawPolygon(QPolygonF(vpt));
				vpt.clear();
			}
			QPointF end_1 = pp[0] - (pp[0] - pp[3]) / 4;
			QPointF end_2 = pp[1] - (pp[1] - pp[2]) / 4;
			QPointF end_dis = end_2 - end_1;

			if (begin.x() <= end.x())
				angle = atan(k) + M_PI / 2;
			else
				angle = atan(k) - M_PI / 2;

			qreal arrow_length_2 = height / 10;
			QPointF p_11 = QPointF(end_1.x() + arrow_length_2 * cos(angle - ExtRefArrowDegrees),
				end_1.y() + arrow_length_2 * sin(angle - ExtRefArrowDegrees));

			QPointF p_12 = QPointF(end_1.x() + arrow_length_2 * cos(angle + ExtRefArrowDegrees),
				end_1.y() + arrow_length_2 * sin(angle + ExtRefArrowDegrees));
			QPointF p_21 = p_11 + end_dis;
			QPointF p_22 = p_12 + end_dis;

			painter->drawLine(p_11, end_1);
			painter->drawLine(end_1, p_12);
			painter->drawLine(p_21, end_2);
			painter->drawLine(end_2, p_22);
		}


	scene()->update();
}

void bee_caliberline::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		if (!if_create)
		{
			pt_begin = event->pos();
			pt_end = event->pos();
			if_create_move = true;
		}
		else
		{
			m_startPos = event->pos();
			if (m_HandlesList_line[0]->contains(event->pos()))
			{
				m_StateFlag = MOV_LINE_BEGIN;
			}
			else if (m_HandlesList_line[1]->contains(event->pos()))
			{
				m_StateFlag = MOV_LINE_END;
			}
			for (int i = 0; i < 4; i++)
			{
				if (m_HandlesList_refct[i]->contains(event->pos()))
				{
					m_StateFlag = MOV_RECT_PT;
				}
			}
			if (this->boundingRect().contains(m_startPos) && m_StateFlag == DEFAULT_FLAG_OL)
				m_StateFlag = MOV_ALL;

			if (if_hover)
			{
				emit sign_currentLine_Param(nMeasureNums, nthresholdValue, nSampleDirection, currentIndexs);
			}
		}
	}
	scene()->update();
}

void bee_caliberline::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	Q_UNUSED(event)
		if (if_create_move)
		{
			pt_end = event->pos();
		}
		else if (m_StateFlag == MOV_LINE_BEGIN)
		{
			pt_begin = event->pos();
			setlinsize();
			scene()->update();
		}
		else if (m_StateFlag == MOV_LINE_END)
		{
			pt_end = event->pos();
			setlinsize();
			scene()->update();
		}
		else if (m_StateFlag == MOV_ALL)
		{
			QPointF point = (event->pos() - m_startPos);
			moveBy(point.x(), point.y());
			setlinsize();
			scene()->update();
		}
		else if (m_StateFlag == MOV_RECT_PT)
		{
			QPointF PT = event->pos();
			QPointF CE = first_center;
			QPointF PT_R = getRotatePoint_2(CE, PT, -atan(k) * 180 / M_PI);
			width = std::abs(PT_R.x() - CE.x()) * 2;
			height = std::abs(PT_R.y() - CE.y()) * 2;
			setlinsize();
			scene()->update();
		}
}

void bee_caliberline::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	Q_UNUSED(event)
		if (if_create_move)
		{
			if (getdistance_2(pt_end, pt_begin) < 10)
				return;
			if_create = true;
			if_create_move = false;
			qreal r = getdistance_2(pt_begin, pt_end);
			width = r / (nMeasureNums + 2);
			height = r / (nMeasureNums + 2);
			if (!if_handleslist_create)
			{
				k = (pt_end.y() - pt_begin.y()) / (pt_end.x() - pt_begin.x());
				first_center = pt_begin + (pt_end - pt_begin) / nMeasureNums;
				pt[0] = QPointF(first_center.x() - width / 2, first_center.y() - height / 2);
				pt[1] = QPointF(first_center.x() + width / 2, first_center.y() - height / 2);
				pt[2] = QPointF(first_center.x() + width / 2, first_center.y() + height / 2);
				pt[3] = QPointF(first_center.x() - width / 2, first_center.y() + height / 2);
				pp[0] = pt[0];
				pp[1] = pt[1];
				pp[2] = pt[2];
				pp[3] = pt[3];
				first_center_top = (pp[0] + pp[1]) / 2;
				first_center_bottom = (pp[2] + pp[3]) / 2;
				const int num1 = 2;
				QPointF point1[num1] = { pt_begin, pt_end };
				CornerDirction dir1[num1] = { None ,None };
				for (int i = 0; i < num1; i++)
				{
					mycorneritem* corner = new mycorneritem(this, point1[i], dir1[i]);
					m_HandlesList_line.push_back(corner);
				}
				const int num2 = 4;
				QPointF point2[num2] = { pp[0], pp[1],pp[2],pp[3] };
				CornerDirction dir2[num2] = { LeftTop ,RightTop,RightBottom,LeftBottom };
				for (int i = 0; i < num2; i++)
				{
					mycorneritem* corner = new mycorneritem(this, point2[i], dir2[i]);
					m_HandlesList_refct.push_back(corner);
				}
				QVector<QPointF> vpt;
				for (int i = 0; i < 4; i++)
				{
					vpt.append(pp[i]);
				}
				vpt.append(pp[0]);
				first_poly = QPolygonF(vpt);
				first_center_top = (pp[0] + pp[1]) / 2;
				first_center_bottom = (pp[2] + pp[3]) / 2;
				setlinsize();
				if_handleslist_create = true;
			}
			else
			{
				setlinsize();
			}
		}
	m_StateFlag = DEFAULT_FLAG_OL;
	scene()->update();
}


void bee_caliberline::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{
    if_hover = true;
    scene()->update();
    QGraphicsItem::hoverEnterEvent(event);
	//发送当前的设置给qt前端界面
}

void bee_caliberline::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
    if_hover = false;
    scene()->update();
    QGraphicsItem::hoverLeaveEvent(event);
}

//边缘阈值
void bee_caliberline::slotSliderValueChanged_SetThreshold_line(int value)
{
    nthresholdValue = value;
}


void bee_caliberline::slotSliderValueChanged_SetSampleDirection_line(int value) {
    //边缘方向
    nSampleDirection = value;
}

void bee_caliberline::fitline()
{

}


void bee_caliberline::slotSliderValueChanged_MeasureNums_line(int value) {
    nMeasureNums = value;
	scene()->update();
}

