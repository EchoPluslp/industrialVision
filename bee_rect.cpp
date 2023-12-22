#include "bee_rect.h"
#include <QtMath>
#include <QDebug>
#include <QGraphicsItem>
#include <QDir>
#include <QCoreApplication>
#include <QtMath>

bee_rect::bee_rect(QGraphicsItem *parent, bool flag):
    rotate_angle(0),
    m_StateFlag(DEFAULT_FLAG_OR)
{
    setAcceptHoverEvents(true);
    setCursor(Qt::ArrowCursor);
    if_rotate = flag;
    m_rect = QRectF(0,0,0,0);
}

void bee_rect::setRectSize(QRectF mrect, bool bResetRotateCenter)
{
    if(!if_create)
    {
        if(!if_rotate)
        {
            for(int i=0;i<4;i++)
            {
                m_HandlesList[i]->move(pt[i]);
            }
        }
        else
        {
            for(int i=0;i<4;i++)
            {
                m_HandlesList[i]->move(pt[i]);
            }
            m_HandlesList[4]->move((pt[0]+pt[1])/2);
        }
    }
    else
    {
        m_rect = mrect;
        if(bResetRotateCenter)
        {
            rotate_center.setX(m_rect.x()+m_rect.width()/2);
            rotate_center.setY(m_rect.y()+m_rect.height()/2);
        }


        QVector<QPointF> vpt;
        pp[0] = getRotatePoint(rotate_center,pt[0],rotate_angle);
        vpt.append(pp[0]);
        pp[1] = getRotatePoint(rotate_center,pt[1],rotate_angle);
        vpt.append(pp[1]);
        pp[2] = getRotatePoint(rotate_center,pt[2],rotate_angle);
        vpt.append(pp[2]);
        pp[3] = getRotatePoint(rotate_center,pt[3],rotate_angle);
        vpt.append(pp[3]);
        vpt.append(pp[0]);

        if(if_rotate)
        {
            m_HandlesList[4]->move((pp[0]+pp[1])/2);
        }
        for(int i =0;i<4;i++)
        {
            m_HandlesList[i]->move(pp[i]);
        }

        m_poly = QPolygonF(vpt);
    }
}

QRectF bee_rect::boundingRect() const
{
    if(!if_create)
    {
        return QRectF(0,0,pixmap_width,pixmap_height);
    }
    else
    {
        QRectF boundingRectF = m_poly.boundingRect();
        return QRectF(boundingRectF.x()-10,boundingRectF.y()-10,boundingRectF.width()+20,boundingRectF.height()+20);
    }
}

QPainterPath bee_rect::shape() const
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
        m_path.addPolygon(m_poly);
        if(if_rotate)
        {
            for (int i=0;i<5;i++)
            {
                m_path2.addRect(m_HandlesList[i]->boundingRect());
            }
        }
        else
        {
            for (int i=0;i<4;i++)
                m_path2.addRect(m_HandlesList[i]->boundingRect());
        }
        return m_path.united(m_path2);
    }

}

void bee_rect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    QPen mPen = QPen(Qt::magenta);
    if(if_hover)
    {
        mPen.setColor(Qt::magenta);
    }

    mPen.setWidth(0);
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(mPen);

    if(if_create)
    {
        painter->drawPolygon(m_poly);
        QPointF begin = (pp[0]+pp[1])/2;
        qreal x ;
        qreal y ;
        QPointF ppcenter = (pp[0]+pp[2])/2;
        qreal r = sqrt((begin.x()-ppcenter.x())*(begin.x()-ppcenter.x()) + (begin.y()-ppcenter.y())*(begin.y()-ppcenter.y()));
        qreal distance = r/3;
        x = (r+distance)/r*(begin.x()-ppcenter.x())+ppcenter.x();
        y = (r+distance)/r*(begin.y()-ppcenter.y())+ppcenter.y();

        QPointF end = QPointF(x,y);


        qreal k = (end.y()-begin.y())/(end.x()-begin.x());
        qreal angle;
        angle = atan(k);
        qreal ExtRefArrowDegrees = M_PI/6; //绘制箭头

    }
    else
    {
        painter->drawPolygon(m_rect);
    }

}

void bee_rect::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button()== Qt::LeftButton)
    {
        m_startPos = event->pos();
        if(!if_create)
        {
            create_move = true;
        }
        else
        {
            if(if_rotate && m_HandlesList[4]->contains(m_startPos))
            {
                QCursor cCursor;
                QPixmap cMap;
                cMap.load(":/res/Icon/rotate.png");
                QSize cSize(20, 20);
                QPixmap cScaleMap = cMap.scaled(cSize, Qt::KeepAspectRatio);
                cCursor = QCursor(cScaleMap);
                setCursor(cCursor);
                m_StateFlag = ROTATE_OR;
            }
            else if(m_HandlesList[0]->contains(m_startPos))
                m_StateFlag = MOV_OAT0;
            else if(m_HandlesList[1]->contains(m_startPos))
                m_StateFlag = MOV_OAT1;
            else if(m_HandlesList[2]->contains(m_startPos))
                m_StateFlag = MOV_OAT2;
            else if(m_HandlesList[3]->contains(m_startPos))
                m_StateFlag = MOV_OAT3;
            else if(m_poly.containsPoint(m_startPos,Qt::WindingFill))
                m_StateFlag = MOV_RECT_OR;
            else
                m_StateFlag = DEFAULT_FLAG_OR;
        }
    }

    scene()->update();
}

void bee_rect::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(create_move)
    {
        if(!if_handleslist_create)
        {
            if(!if_rotate)
            {
                const int num = 4;
                QPointF point[num] = {m_startPos,m_startPos,m_startPos,m_startPos};
                CornerDirction dir[num] = {LeftTop, RightTop,RightBottom,LeftBottom};
                for(int i =0;i<num;i++)
                {
                    pt[i]=point[i];
                    pp[i]=pt[i];
                    mycorneritem* corner = new mycorneritem(this, point[i], dir[i]);
                    m_HandlesList.push_back(corner);
                }
            }
            else
            {
                const int num = 5;
                QPointF point[num] = {m_startPos,m_startPos,m_startPos,m_startPos,m_startPos};
                CornerDirction dir[num] = {LeftTop, RightTop,RightBottom,LeftBottom,Rotation };
                for(int i =0;i<num;i++)
                {
                    if(i<4)
                    {
                        pt[i]=point[i];
                        pp[i]=pt[i];
                    }
                    mycorneritem* corner = new mycorneritem(this, point[i], dir[i]);
                    m_HandlesList.push_back(corner);
                }
            }
            if_handleslist_create = true;
        }

        pt[2] = event->pos();
        pt[1] = QPointF(pt[2].x(), pt[0].y());
        pt[3] = QPointF(pt[0].x(), pt[2].y());
        m_rect.setTopLeft(pt[0]);
        m_rect.setTopRight(pt[1]);
        m_rect.setBottomLeft(pt[3]);
        m_rect.setBottomRight(pt[2]);
        setRectSize(m_rect);
    }
    else
    {
        if(m_StateFlag == ROTATE_OR)
        {
            qreal nRotateAngle = atan2((event->pos().x()-rotate_center.x()),(event->pos().y()-rotate_center.y()))*180/M_PI;
            if(abs(int(180 - nRotateAngle - rotate_angle))%360 <100 || abs(int(180 - nRotateAngle - rotate_angle))%360>260)
                SetRotate(180 - nRotateAngle);
            else
                SetRotate(360 - nRotateAngle);
            setRectSize(m_rect);
        }
        else if(m_StateFlag == MOV_RECT_OR)
        {
            //todo 拖动更新 更新pt[]坐标
            QPointF x = event->pos();
            //相差
            QPointF point = (event->pos() - m_startPos);
            setRectSize(m_rect);
       

            scene()->update();
        }
        else if(m_StateFlag == MOV_OAT0)
        {
            pp[0] = event->pos();
            QPointF centre = QPointF((pp[0].x() + pp[2].x())/2, (pp[0].y() + pp[2].y())/2);
            pt[0] = getRotatePoint(centre, pp[0], -rotate_angle);
            pt[2] = getRotatePoint(centre, pp[2], -rotate_angle);
            pt[1] = QPointF(pt[2].x(), pt[0].y());
            pt[3] = QPointF(pt[0].x(), pt[2].y());

            m_rect.setTopLeft(pt[0]);
            m_rect.setTopRight(pt[1]);
            m_rect.setBottomLeft(pt[3]);
            m_rect.setBottomRight(pt[2]);
            rotate_center=centre;
            m_rect.moveCenter(rotate_center);
            setRectSize(m_rect);
        }
        else if(m_StateFlag == MOV_OAT1)
        {
            pp[1] = event->pos();
            QPointF centre = QPointF((pp[1].x() + pp[3].x())/2, (pp[1].y() + pp[3].y())/2);
            pt[1] = getRotatePoint(centre, pp[1], -rotate_angle);
            pt[3] = getRotatePoint(centre, pp[3], -rotate_angle);
            pt[0] = QPointF(pt[3].x(), pt[1].y());
            pt[2] = QPointF(pt[1].x(), pt[3].y());

            m_rect.setTopLeft(pt[0]);
            m_rect.setTopRight(pt[1]);
            m_rect.setBottomLeft(pt[3]);
            m_rect.setBottomRight(pt[2]);
            rotate_center=centre;
            m_rect.moveCenter(rotate_center);
            setRectSize(m_rect);
        }
        else if(m_StateFlag == MOV_OAT2)
        {
            pp[2] = event->pos();
            QPointF centre = QPointF((pp[2].x() + pp[0].x())/2, (pp[2].y() + pp[0].y())/2);
            pt[2] = getRotatePoint(centre, pp[2], -rotate_angle);
            pt[0] = getRotatePoint(centre, pp[0], -rotate_angle);
            pt[1] = QPointF(pt[2].x(), pt[0].y());
            pt[3] = QPointF(pt[0].x(), pt[2].y());

            m_rect.setTopLeft(pt[0]);
            m_rect.setTopRight(pt[1]);
            m_rect.setBottomLeft(pt[3]);
            m_rect.setBottomRight(pt[2]);
            rotate_center=centre;
            m_rect.moveCenter(rotate_center);
            setRectSize(m_rect);
        }
        else if(m_StateFlag == MOV_OAT3)
        {
            pp[3] = event->pos();
            QPointF centre = QPointF((pp[1].x() + pp[3].x())/2, (pp[1].y() + pp[3].y())/2);
            pt[1] = getRotatePoint(centre, pp[1], -rotate_angle);
            pt[3] = getRotatePoint(centre, pp[3], -rotate_angle);
            pt[0] = QPointF(pt[3].x(), pt[1].y());
            pt[2] = QPointF(pt[1].x(), pt[3].y());

            m_rect.setTopLeft(pt[0]);
            m_rect.setTopRight(pt[1]);
            m_rect.setBottomLeft(pt[3]);
            m_rect.setBottomRight(pt[2]);
            rotate_center=centre;
            m_rect.moveCenter(rotate_center);
            setRectSize(m_rect);
        }
    }
    scene()->update();
}

void bee_rect::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(create_move&&if_handleslist_create)
    {
        if_create = true;
        create_move = false;
        m_rect.setTopLeft(pt[0]);
        m_rect.setTopRight(pt[1]);
        m_rect.setBottomLeft(pt[3]);
        m_rect.setBottomRight(pt[2]);
        setRectSize(m_rect);
    }

    setCursor(Qt::ArrowCursor);
    m_StateFlag = DEFAULT_FLAG_OR;
    QGraphicsItem::mouseReleaseEvent(event);
    scene()->update();
}

void bee_rect::SetRotate(qreal RotateAngle, QPointF ptCenter)
{
    m_bRotate = true;
    if(ptCenter.x()==-999 && ptCenter.y()==-999)
    {
        rotate_center = QPointF(m_rect.x()+m_rect.width()/2,m_rect.y()+m_rect.height()/2);
    }
    else
    {
        rotate_center = ptCenter;

    }
    rotate_angle = RotateAngle;
    this->update();
}

QPointF bee_rect::getRotatePoint(QPointF ptCenter, QPointF ptIn, qreal angle)
{
    double dx = ptCenter.x();
    double dy = ptCenter.y();
    double x = ptIn.x();
    double y = ptIn.y();
    double xx,yy;
    xx = (x-dx)*cos(angle*M_PI/180)-(y-dy)*sin(angle*M_PI/180)+dx;
    yy = (x-dx)*sin(angle*M_PI/180)+(y-dy)*cos(angle*M_PI/180)+dy;

    return QPointF(xx,yy);
}

void bee_rect::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{
    if_hover = true;
    scene()->update();
    QGraphicsItem::hoverEnterEvent(event);
}

void bee_rect::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
    if_hover = false;
    scene()->update();
    QGraphicsItem::hoverLeaveEvent(event);
}
