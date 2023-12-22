#include "bee_concencircle.h"

#include <QtMath>

bee_concencircle::bee_concencircle(QGraphicsItem *parent):
    m_StateFlag(DEFAULT_FLAG_OCC)
{
    setAcceptHoverEvents(true);
    setCursor(Qt::ArrowCursor);
    center_concircle = QPointF(0,0);
    boundingRect1 = QRectF(center_concircle.x()-r1, center_concircle.y()-r1,2*r1,2*r1);
    boundingRect2 = QRectF(center_concircle.x()-r2, center_concircle.y()-r2,2*r2,2*r2);
    r1 = 0;
    r2 = 0;

}

QRectF bee_concencircle::boundingRect() const
{
    if(!if_create)
        return QRectF(0,0,pixmap_width,pixmap_height);
    else
        return QRectF(center_concircle.x()-r2-3, center_concircle.y()-r2-3,2*r2+6,2*r2+6);
}

QPainterPath bee_concencircle::shape() const
{
    QPainterPath m_path;
    if(!if_create)
    {
        m_path.addRect(QRectF(0,0,pixmap_width,pixmap_height));
        return m_path;
    }
    else
    {
        m_path.addEllipse(QRectF(center_concircle.x()-r2-3, center_concircle.y()-r2-3,2*r2+6,2*r2+6));
        return m_path;
    }

}

void bee_concencircle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    QPen mPen = QPen(Qt::magenta);
    if(if_hover)
        mPen.setColor(Qt::magenta);
    mPen.setWidth(0);
    painter->setPen(mPen);
    boundingRect1 = QRectF(center_concircle.x()-r1, center_concircle.y()-r1,2*r1,2*r1);
    boundingRect2 = QRectF(center_concircle.x()-r2, center_concircle.y()-r2,2*r2,2*r2);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawEllipse(boundingRect1);//绘制圆形1
    painter->drawEllipse(boundingRect2);//绘制圆形2

    this->scene()->update();
}

void bee_concencircle::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    m_startPos= event->pos();
    if(!if_create)
    {
        center_concircle = m_startPos;
        create_move = true;
        pt1[0] = QPointF(center_concircle.x(), center_concircle.y()-r1);
        pt1[1] = QPointF(center_concircle.x()+r1, center_concircle.y());
        pt1[2] = QPointF(center_concircle.x(), center_concircle.y()+r1);
        pt1[3] = QPointF(center_concircle.x()-r1, center_concircle.y());
        pt2[0] = QPointF(center_concircle.x(), center_concircle.y()-r2);
        pt2[1] = QPointF(center_concircle.x()+r2, center_concircle.y());
        pt2[2] = QPointF(center_concircle.x(), center_concircle.y()+r2);
        pt2[3] = QPointF(center_concircle.x()-r2, center_concircle.y());
    }else
    {
        if(event->button()== Qt::LeftButton)
        {
            for(int i = 0;i<4;i++)
            {
                if(m_HandlesList1[i+1]->contains(event->pos()))
                {
                    m_StateFlag = SCALE_CIRCLE1;
                    break;
                }
                if(m_HandlesList2[i+1]->contains(event->pos()))
                {
                    m_StateFlag = SCALE_CIRCLE2;
                    break;
                }
            }
            if(m_StateFlag != SCALE_CIRCLE1 && m_StateFlag != SCALE_CIRCLE2)
                if(boundingRect().contains(m_startPos))
                {
                    m_StateFlag = MOV_CONCENCIRCLE;

                }

            scene()->update();//必须要用scene()->update()，不能用update();否则会出现重影
        }
    }
}

void bee_concencircle::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    qreal r = sqrt((center_concircle.x() - event->pos().x())*(center_concircle.x() - event->pos().x()) + (center_concircle.y() - event->pos().y())*(center_concircle.y() - event->pos().y()));
    if(create_move)
    {
        r2 = r;
        r1 = r/2;
        pt1[0] = QPointF(center_concircle.x(), center_concircle.y()-r1);
        pt1[1] = QPointF(center_concircle.x()+r1, center_concircle.y());
        pt1[2] = QPointF(center_concircle.x(), center_concircle.y()+r1);
        pt1[3] = QPointF(center_concircle.x()-r1, center_concircle.y());

        pt2[0] = QPointF(center_concircle.x(), center_concircle.y()-r2);
        pt2[1] = QPointF(center_concircle.x()+r2, center_concircle.y());
        pt2[2] = QPointF(center_concircle.x(), center_concircle.y()+r2);
        pt2[3] = QPointF(center_concircle.x()-r2, center_concircle.y());

        if(!if_handleslist_create)
        {
            const int num = 5;
            QPointF point1[num] = {center_concircle, pt1[0],pt1[1],pt1[2],pt1[3]};
            CornerDirction dir[num] = { None ,Top, Right,Bottom,Left};
            for(int i =0;i<num;i++)
            {
                mycorneritem* corner = new mycorneritem(this, point1[i], dir[i]);
                m_HandlesList1.push_back(corner);
            }
            QPointF point2[num] = {center_concircle, pt2[0],pt2[1],pt2[2],pt2[3]};
            for(int i =0;i<num;i++)
            {
                mycorneritem* corner = new mycorneritem(this, point2[i], dir[i]);
                m_HandlesList2.push_back(corner);
            }
            if_handleslist_create = true;
        }

        m_HandlesList1[0]->move(center_concircle);
        m_HandlesList2[0]->move(center_concircle);
        for(int i =1;i<5;i++)
        {
            m_HandlesList1[i]->move(pt1[i-1]);
            m_HandlesList2[i]->move(pt2[i-1]);
        }
    }
    else
    {
        if(m_StateFlag == MOV_CONCENCIRCLE)
        {
            QPointF point = (event->pos() - m_startPos);
            moveBy(point.x(),point.y());
        }
        else if(m_StateFlag == SCALE_CIRCLE1 && r2-r>=5)
        {
            r1 = r;
            pt1[0] = QPointF(center_concircle.x(), center_concircle.y()-r);
            pt1[1] = QPointF(center_concircle.x()+r, center_concircle.y());
            pt1[2] = QPointF(center_concircle.x(), center_concircle.y()+r);
            pt1[3] = QPointF(center_concircle.x()-r, center_concircle.y());

            m_HandlesList1[0]->move(center_concircle);
            for(int i =1;i<5;i++)
                m_HandlesList1[i]->move(pt1[i-1]);

        }
        else if(m_StateFlag == SCALE_CIRCLE2 && r-r1>=2)
        {
            r2 = r;
            pt2[0] = QPointF(center_concircle.x(), center_concircle.y()-r);
            pt2[1] = QPointF(center_concircle.x()+r, center_concircle.y());
            pt2[2] = QPointF(center_concircle.x(), center_concircle.y()+r);
            pt2[3] = QPointF(center_concircle.x()-r, center_concircle.y());

            m_HandlesList2[0]->move(center_concircle);
            for(int i =1;i<5;i++)
                m_HandlesList2[i]->move(pt2[i-1]);
        }
    }
    this->scene()->update();
}

void bee_concencircle::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(create_move&&if_handleslist_create)
    {
        if_create = true;
        create_move = false;
    }
    setCursor(Qt::ArrowCursor);
    m_StateFlag = DEFAULT_FLAG_OCC;
    QGraphicsItem::mouseReleaseEvent(event);
    scene()->update();
}

void bee_concencircle::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{
    if_hover = true;
    scene()->update();
    QGraphicsItem::hoverEnterEvent(event);
}

void bee_concencircle::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
    if_hover = false;
    scene()->update();
    QGraphicsItem::hoverLeaveEvent(event);
}
