#include "bee_circle.h"
#include <QtMath>

bee_circle::bee_circle(QGraphicsItem *parent):
    m_StateFlag(DEFAULT_FLAG_OC)
{
    setAcceptHoverEvents(true);
    setCursor(Qt::ArrowCursor);
    center_circle = QPointF(0,0);
    r = 0;
}

QRectF bee_circle::boundingRect() const
{
    if(!if_create)
        return QRectF(0,0,pixmap_width,pixmap_height);
    else
        return QRectF(center_circle.x()-r-5, center_circle.y()-r-5,2*r+10,2*r+10);
}

QPainterPath bee_circle::shape() const
{
    QPainterPath m_path;
    if(!if_create)
    {
        m_path.addRect(QRectF(0,0,pixmap_width,pixmap_height));
        return m_path;
    }
    else
    {
        m_path.addEllipse(QRectF(center_circle.x()-r-3, center_circle.y()-r-3,2*r+6,2*r+6));
        return m_path;
    }
}

void bee_circle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);     //这个宏是用来把不用到的参数注掉的功能
    Q_UNUSED(widget);
    QPen mPen = QPen(Qt::magenta);
    if(if_hover)
    {
        mPen.setColor(Qt::magenta);
    }
    mPen.setWidth(0);
    painter->setPen(mPen);

    boundingrect = QRectF(center_circle.x()-r,center_circle.y()-r,2*r,2*r);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawEllipse(boundingrect);//绘制圆形

    this->scene()->update();
}

void bee_circle::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    m_startPos= event->pos();
    if(!if_create)
    {
        center_circle = m_startPos;
        pt[0] = QPointF(center_circle.x(), center_circle.y()-r);
        pt[1] = QPointF(center_circle.x()+r, center_circle.y());
        pt[2] = QPointF(center_circle.x(), center_circle.y()+r);
        pt[3] = QPointF(center_circle.x()-r, center_circle.y());
        create_move = true;
    }
    else
    {
        if(event->button() == Qt::LeftButton)
        {
            QRectF rect = QRectF(center_circle.x()-r,center_circle.y()-r,2*r,2*r);
            for(int i = 0;i<4;i++)
            {
                if(m_HandlesList[i+1]->boundingRect().contains(event->pos()))
                {
                    m_StateFlag = SCALE_CIRCLE;
                    break;
                }
            }
            if(m_StateFlag != SCALE_CIRCLE)
            {
                if(rect.contains(m_startPos))
                {
                    m_StateFlag = MOV_CIRCLE;

                }

            }
            scene()->update();//必须要用scene()->update()，不能用update();否则会出现重影
        }
    }
}

void bee_circle::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(create_move)
    {
        qreal delta_x = center_circle.x() - event->pos().x();
        qreal delta_y = center_circle.y() - event->pos().y();

        r = sqrt(delta_x*delta_x + delta_y*delta_y);

        pt[0] = QPointF(center_circle.x(), center_circle.y()-r);
        pt[1] = QPointF(center_circle.x()+r, center_circle.y());
        pt[2] = QPointF(center_circle.x(), center_circle.y()+r);
        pt[3] = QPointF(center_circle.x()-r, center_circle.y());

        if(!if_handleslist_create)
        {
            const int num = 5;
            QPointF point[num] = {center_circle, pt[0],pt[1],pt[2],pt[3]};
            CornerDirction dir[num] = { None ,Top, Right,Bottom,Left};
            for(int i =0;i<num;i++)
            {
                mycorneritem* corner = new mycorneritem(this, point[i], dir[i]);
                m_HandlesList.push_back(corner);
            }
            if_handleslist_create = true;
        }
        m_HandlesList[0]->move(center_circle);
        for(int i =1;i<5;i++)
        {
            m_HandlesList[i]->move(pt[i-1]);
        }
    }
    else
    {
        if(m_StateFlag == MOV_CIRCLE)
        {
            QPointF point = (event->pos() - m_startPos);
            moveBy(point.x(),point.y());
            pt[0] = QPointF(center_circle.x(), center_circle.y()-r);
            pt[1] = QPointF(center_circle.x()+r, center_circle.y());
            pt[2] = QPointF(center_circle.x(), center_circle.y()+r);
            pt[3] = QPointF(center_circle.x()-r, center_circle.y());

        }
        else if(m_StateFlag == SCALE_CIRCLE)
        {
            qreal delta_x = center_circle.x() - event->pos().x();
            qreal delta_y = center_circle.y() - event->pos().y();

            r = sqrt(delta_x*delta_x + delta_y*delta_y);
            pt[0] = QPointF(center_circle.x(), center_circle.y()-r);
            pt[1] = QPointF(center_circle.x()+r, center_circle.y());
            pt[2] = QPointF(center_circle.x(), center_circle.y()+r);
            pt[3] = QPointF(center_circle.x()-r, center_circle.y());

            const int num = 5;
            m_HandlesList[0]->move(center_circle);
            for(int i =1;i<num;i++)
            {
                m_HandlesList[i]->move(pt[i-1]);
            }
        }
    }
    this->scene()->update();
}

void bee_circle::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(create_move&&if_handleslist_create)
    {
        if_create = true;
        create_move = false;
    }
    setCursor(Qt::ArrowCursor);
    m_StateFlag = DEFAULT_FLAG_OC;
    QGraphicsItem::mouseReleaseEvent(event);
    scene()->update();
}

void bee_circle::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{
    if_hover = true;
    scene()->update();
    QGraphicsItem::hoverEnterEvent(event);
}

void bee_circle::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
    if_hover = false;
    scene()->update();
    QGraphicsItem::hoverLeaveEvent(event);
}
