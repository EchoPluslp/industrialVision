#include "bee_ringexpansion.h"
#include <QtMath>

bee_ringexpansion::bee_ringexpansion(QGraphicsItem *parent):
    m_StateFlag(DEFAULT_FLAG_ORE)
{
    setAcceptHoverEvents(true);
    setCursor(Qt::ArrowCursor);
    concenter = QPointF(0,0);
    r1 = 1;
    r2 = 2;
    startspan = 0;
    endspan = 2*M_PI;
    middlespan = startspan + theta/2;
    theta = startspan - middlespan;
    boundingRect1 = QRectF(concenter.x()-r1, concenter.y()-r1,2*r1,2*r1);
    boundingRect2 = QRectF(concenter.x()-r2, concenter.y()-r2,2*r2,2*r2);
}

QRectF bee_ringexpansion::boundingRect() const
{
    if(!if_create)
        return QRectF(0,0,pixmap_width,pixmap_height);
    else
        return QRectF(concenter.x()-r2-3, concenter.y()-r2-3,2*r2+6,2*r2+6);
}

QPainterPath bee_ringexpansion::shape() const
{
    QPainterPath m_path;
    if(!if_create)
    {
        m_path.addRect(QRectF(0,0,pixmap_width,pixmap_height));
        return m_path;
    }
    else
    {

//        QPainterPath m_path2;
        QPainterPath m_path3;
        qreal angle = (startspan - middlespan)*180/M_PI;
        while(angle > 180)
            angle -= 360;
        while(angle < -180)
            angle += 360;
//        m_path.moveTo(pt1[0]);
//        m_path.arcTo(QRectF(concenter.x()-r1, concenter.y()-r1,2*r1,2*r1),-startspan*180/M_PI,2*angle);
//        m_path.lineTo(concenter);
//        m_path.lineTo(pt1[0]);
//        m_path.closeSubpath();

//        m_path2.moveTo(pt2[0]);
//        m_path2.arcTo(QRectF(concenter.x()-r2, concenter.y()-r2,2*r2,2*r2),-startspan*180/M_PI,2*angle);
//        m_path2.lineTo(concenter);
//        m_path2.lineTo(pt2[0]);
//        m_path2.closeSubpath();

        m_path.arcMoveTo(QRectF(concenter.x()-r1, concenter.y()-r1,2*r1,2*r1),-startspan*180/M_PI);
        m_path.arcTo(QRectF(concenter.x()-r1, concenter.y()-r1,2*r1,2*r1),-startspan*180/M_PI,2*theta/16);
        m_path.lineTo((pt2[2]));
        m_path.arcTo(QRectF(concenter.x()-r2, concenter.y()-r2,2*r2,2*r2),-endspan*180/M_PI,-2*theta/16);
        m_path.lineTo(pt1[0]);


        m_path3.addRect(m_HandlesList_angle[0]->boundingRect());
        m_path3.addRect(m_HandlesList_angle[1]->boundingRect());
        m_path3.addRect(m_HandlesList_r1r2[0]->boundingRect());
        m_path3.addRect(m_HandlesList_r1r2[1]->boundingRect());
        m_path3.addRect(m_HandlesList_pt1pt2[0]->boundingRect());
        m_path3.addRect(m_HandlesList_pt1pt2[1]->boundingRect());
        return m_path.united(m_path3);
    }
}

QPainterPath bee_ringexpansion::getshape()
{
    QPainterPath m_path;
    if(!if_create)
    {
        m_path.addRect(QRectF(0,0,pixmap_width,pixmap_height));
        return m_path;
    }
    else
    {

//        QPainterPath m_path2;
//        QPainterPath m_path3;
        qreal angle = (startspan - middlespan)*180/M_PI;
        while(angle > 180)
            angle -= 360;
        while(angle < -180)
            angle += 360;
//        m_path.moveTo(pt1[0]);
//        m_path.arcTo(QRectF(concenter.x()-r1, concenter.y()-r1,2*r1,2*r1),-startspan*180/M_PI,2*angle);
//        m_path.lineTo(concenter);
//        m_path.lineTo(pt1[0]);
//        m_path.closeSubpath();

//        m_path2.moveTo(pt2[0]);
//        m_path2.arcTo(QRectF(concenter.x()-r2, concenter.y()-r2,2*r2,2*r2),-startspan*180/M_PI,2*angle);
//        m_path2.lineTo(concenter);
//        m_path2.lineTo(pt2[0]);
//        m_path2.closeSubpath();

//        m_path3.moveTo(pt1[0]);
        m_path.arcMoveTo(QRectF(concenter.x()-r1, concenter.y()-r1,2*r1,2*r1),-startspan*180/M_PI);
        m_path.arcTo(QRectF(concenter.x()-r1, concenter.y()-r1,2*r1,2*r1),-startspan*180/M_PI,2*theta/16);
        m_path.lineTo((pt2[2]));
        m_path.arcTo(QRectF(concenter.x()-r2, concenter.y()-r2,2*r2,2*r2),-endspan*180/M_PI,-2*theta/16);
        m_path.lineTo(pt1[0]);
        return m_path;
    }
}

void bee_ringexpansion::setringsize()
{

    theta = startspan - middlespan;
    theta = theta*180/M_PI*16;
    while(theta > 180*16)
        theta -= 360*16;
    while(theta < -180*16)
        theta+=360*16;

    r1 = r2 - width;
    pt1[0] = QPointF(concenter.x() + r1*cos(startspan), concenter.y() + r1*sin(startspan));
    pt1[1] = QPointF(concenter.x() + r1*cos(middlespan), concenter.y() + r1*sin(middlespan));
    pt1[2] = QPointF(concenter.x() + r1*cos(endspan), concenter.y() + r1*sin(endspan));
    pt2[0] = QPointF(concenter.x() + r2*cos(startspan), concenter.y() + r2*sin(startspan));
    pt2[1] = QPointF(concenter.x() + r2*cos(middlespan), concenter.y() + r2*sin(middlespan));
    pt2[2] = QPointF(concenter.x() + r2*cos(endspan), concenter.y() + r2*sin(endspan));
    qreal rm = r1/2 + r2/2;
    ptmove[0] = QPointF(concenter.x() + rm*cos(startspan), concenter.y() + rm*sin(startspan));
    ptmove[1] = QPointF(concenter.x() + rm*cos(endspan), concenter.y() + rm*sin(endspan));
    m_HandlesList_r1r2[0]->move(pt1[1]);
    m_HandlesList_r1r2[1]->move(pt2[1]);
    m_HandlesList_angle[0]->move(ptmove[0]);
    m_HandlesList_angle[1]->move(ptmove[1]);
    m_HandlesList_pt1pt2[0]->move(pt2[0]);
    m_HandlesList_pt1pt2[1]->move(pt2[2]);
}

void bee_ringexpansion::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen mPen = QPen(Qt::magenta);
    if(if_hover)
        mPen.setColor(Qt::magenta);
    mPen.setWidth(0);
    painter->setPen(mPen);
    boundingRect1 = QRectF(concenter.x()-r1, concenter.y()-r1,2*r1,2*r1);
    boundingRect2 = QRectF(concenter.x()-r2, concenter.y()-r2,2*r2,2*r2);
    painter->setRenderHint(QPainter::Antialiasing);
    if(!if_create)
    {
        painter->drawEllipse(boundingRect1);//绘制圆形1
        painter->drawEllipse(boundingRect2);//绘制圆形2
        painter->drawLine(pt1[0],pt2[0]);
        painter->drawLine(pt1[2],pt2[2]);
    }else
    {
        qreal startangle = startspan*180/M_PI * 16;
        qreal endangle = endspan*180/M_PI * 16;
        painter->drawArc(boundingRect1,-startangle,2*theta);
        painter->drawArc(boundingRect2,-startangle,2*theta);
        painter->drawLine(pt1[2],pt2[2]);
        painter->setPen(mPen);
        painter->drawLine(pt1[0],pt2[0]);
//        painter->drawPath(this->getshape());
    }

    this->scene()->update();
}

void bee_ringexpansion::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    m_startPos= event->pos();
    if(!if_create)
    {
        concenter = m_startPos;
        create_move = true;
        startspan = 0;
        endspan = M_PI;
        middlespan = startspan/2 + endspan/2;
        pt1[0] = QPointF(concenter.x() + r1*cos(startspan), concenter.y() + r1*sin(startspan));
        pt1[1] = QPointF(concenter.x() + r1*cos(middlespan), concenter.y() + r1*sin(middlespan));
        pt1[2] = QPointF(concenter.x() + r1*cos(endspan), concenter.y() + r1*sin(endspan));
        pt2[0] = QPointF(concenter.x() + r2*cos(startspan), concenter.y() + r2*sin(startspan));
        pt2[1] = QPointF(concenter.x() + r2*cos(middlespan), concenter.y() + r2*sin(middlespan));
        pt2[2] = QPointF(concenter.x() + r2*cos(endspan), concenter.y() + r2*sin(endspan));  //创建圆环
    }else
    {
        if(event->button() ==event->button()== Qt::LeftButton)
        {
            if(m_HandlesList_r1r2[0]->boundingRect().contains(event->pos()))
            {
                m_StateFlag = MOV_R1;
            }
            else if(m_HandlesList_r1r2[1]->boundingRect().contains(event->pos()) && pt2[0]!=pt2[2])  //当pt2[0] pt2[2]不重合时才能拉动r2
            {
                m_StateFlag = MOV_R2;
            }else if(m_HandlesList_angle[0]->boundingRect().contains(event->pos()))
            {
                m_StateFlag = MOV_STARTSPAN;
            }else if(m_HandlesList_angle[1]->boundingRect().contains(event->pos()))
            {
                m_StateFlag = MOV_ENDSPAN;
            }else if(m_HandlesList_pt1pt2[0]->boundingRect().contains(event->pos()))
            {
                m_StateFlag = MOV_PT1;
            }else if(m_HandlesList_pt1pt2[1]->boundingRect().contains(event->pos()))
            {
                m_StateFlag = MOV_PT2;
            }else if(boundingRect().contains(m_startPos))
            {
                    m_StateFlag = MOV_RINGEXPANSION;
            }
        }
        scene()->update();//必须要用scene()->update()，不能用update();否则会出现重影
    }
}

void bee_ringexpansion::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    qreal r = sqrt((concenter.x() - event->pos().x())*(concenter.x() - event->pos().x()) + (concenter.y() - event->pos().y())*(concenter.y() - event->pos().y()));
    if(create_move)
    {
        r2 = r;
        r1 = r/2;
        width = r2-r1;
        pt1[0] = QPointF(concenter.x() + r1*cos(startspan), concenter.y() + r1*sin(startspan));
        pt1[1] = QPointF(concenter.x() + r1*cos(middlespan), concenter.y() + r1*sin(middlespan));
        pt1[2] = QPointF(concenter.x() + r1*cos(endspan), concenter.y() + r1*sin(endspan));
        pt2[0] = QPointF(concenter.x() + r2*cos(startspan), concenter.y() + r2*sin(startspan));
        pt2[1] = QPointF(concenter.x() + r2*cos(middlespan), concenter.y() + r2*sin(middlespan));
        pt2[2] = QPointF(concenter.x() + r2*cos(endspan), concenter.y() + r2*sin(endspan));

        if(!if_handleslist_create) //创建端点item
        {
            mycorneritem* corner_r1 = new mycorneritem(this, pt1[1], None);
            m_HandlesList_r1r2.push_back(corner_r1);
            mycorneritem* corner_r2 = new mycorneritem(this, pt1[2], None);
            m_HandlesList_r1r2.push_back(corner_r2);

            qreal rm = r1/2 + r2/2;
            mycorneritem* corner_startspan = new mycorneritem(this, QPointF(concenter.x() + rm*cos(startspan), concenter.y() + rm*sin(startspan)), None);
            m_HandlesList_angle.push_back(corner_startspan);
            mycorneritem* corner_endspan = new mycorneritem(this, QPointF(concenter.x() + rm*cos(endspan), concenter.y() + rm*sin(endspan)), None);
            m_HandlesList_angle.push_back(corner_endspan);

            mycorneritem* corner_pt1 = new mycorneritem(this, pt2[0], None);
            m_HandlesList_pt1pt2.push_back(corner_pt1);
            mycorneritem* corner_pt2 = new mycorneritem(this, pt2[2], None);
            m_HandlesList_pt1pt2.push_back(corner_pt2);
            if_handleslist_create = true;

        }
        qreal rm = r1/2 + r2/2;
        ptmove[0] = QPointF(concenter.x() + rm*cos(startspan), concenter.y() + rm*sin(startspan));
        ptmove[1] = QPointF(concenter.x() + rm*cos(endspan), concenter.y() + rm*sin(endspan));
        m_HandlesList_r1r2[0]->move(pt1[1]);
        m_HandlesList_r1r2[1]->move(pt2[1]);
        m_HandlesList_angle[0]->move(ptmove[0]);
        m_HandlesList_angle[1]->move(ptmove[1]);
        m_HandlesList_pt1pt2[0]->move(pt2[0]);
        m_HandlesList_pt1pt2[1]->move(pt2[2]);
        setringsize();

    }else
    {
        qreal r = sqrt((concenter.x() - event->pos().x())*(concenter.x() - event->pos().x()) + (concenter.y() - event->pos().y())*(concenter.y() - event->pos().y()));
        qreal angle = atan2(event->pos().y()-concenter.y(),event->pos().x()-concenter.x());
        if(m_StateFlag == MOV_R1 && r2 - r > 2)
        {
            r1 = r;
            width = r2-r1;
            middlespan = startspan/2 + endspan/2;
            setringsize();
        }else if(m_StateFlag == MOV_R2)  //三点确定圆心
        {
            pt2[1] = event->pos();
            double a = pt2[0].x() - pt2[1].x();
            double b = pt2[0].y() - pt2[1].y();
            double c = pt2[0].x() - pt2[2].x();
            double d = pt2[0].y() - pt2[2].y();
            double e = ((pt2[0].x()*pt2[0].x()-pt2[1].x()*pt2[1].x())-(pt2[1].y()*pt2[1].y()-pt2[0].y()*pt2[0].y()))/2;
            double f = ((pt2[0].x()*pt2[0].x()-pt2[2].x()*pt2[2].x())-(pt2[2].y()*pt2[2].y()-pt2[0].y()*pt2[0].y()))/2;
            qreal x = (e*d - b*f)/(a*d - b*c);
            qreal y = (a*f - e*c)/(a*d - b*c);
            concenter = QPointF(x,y);
            r2 = sqrt((concenter.x() - pt2[0].x())*(concenter.x() - pt2[0].x()) + (concenter.y() - pt2[0].y())*(concenter.y() - pt2[0].y()));
            qreal new_start = atan2(pt2[0].y() - concenter.y(), pt2[0].x() - concenter.x());
            qreal new_end = atan2(pt2[2].y() - concenter.y(), pt2[2].x() - concenter.x());
            if(std::abs(new_start - startspan)>= 2*M_PI-0.1)
            {
                startspan = new_start + 2*M_PI;
            }

            else
                startspan = new_start;
            if(std::abs(new_end - endspan)>= 2*M_PI-0.1)
            {
                endspan = new_end + 2*M_PI;
            }
            else
                endspan = new_end;

            middlespan = startspan/2+endspan/2;
            setringsize();
        }else if(m_StateFlag == MOV_STARTSPAN)
        {
            if(std::abs(startspan - angle)<M_PI)
                startspan = angle;
            else
                startspan = angle + 2*M_PI;

            middlespan = startspan/2 + endspan/2;
            setringsize();
        }else if(m_StateFlag == MOV_ENDSPAN)
        {
            if(std::abs(endspan - angle)<M_PI)
                endspan = angle;
            else
                endspan = angle + 2*M_PI;
            middlespan = startspan/2 + endspan/2;
            setringsize();
        }else if(m_StateFlag == MOV_PT1)
        {
            pt2[0] = event->pos();
            qreal distance = sqrt((pt2[0].x() - pt2[2].x()) *(pt2[0].x() - pt2[2].x()) + (pt2[0].y() - pt2[2].y())*(pt2[0].y() - pt2[2].y()));
            if(distance > 2*r2)
                r2 = distance/2 + 1;

            if(std::abs(pt2[0].x() - pt2[2].x())<0.01)
            {
                qreal y0 = pt2[0].y()/2 + pt2[1].y()/2;
                qreal y1 = pt2[0].y()/2 + pt2[1].y()/2;
                qreal deltay = (y0-pt2[0].y()) * (y0-pt2[0].y());
                qreal deltax = sqrt(r2*r2 - deltay);
                qreal x0 = pt2[2].x() - deltax;
                qreal x1 = pt2[2].x() + deltax;
                if(concenter.x() > pt2[0].x())
                    concenter = QPointF(x1,y1);
                else
                    concenter = QPointF(x0,y0);
            }else
            {
                qreal C1 = (pt2[2].x()*pt2[2].x() + pt2[2].y()*pt2[2].y() - pt2[0].x()*pt2[0].x() - pt2[0].y()*pt2[0].y())/2/(pt2[2].x() - pt2[0].x());
                qreal C2 = (pt2[2].y() - pt2[0].y())/(pt2[2].x() - pt2[0].x());
                qreal A = 1+ C2*C2;
                qreal B = 2*(pt2[0].x() - C1)*C2 -2*pt2[0].y();
                qreal C = (pt2[0].x() - C1)*(pt2[0].x() - C1) + pt2[0].y()*pt2[0].y() - r2*r2;
                qreal y0 = (-B + sqrt(B*B - 4 * A * C)) / 2 / A;
                qreal y1 = (-B - sqrt(B*B - 4 * A * C)) / 2 / A;
                qreal x0 = x0 = C1 - C2 * y0;
                qreal x1 = x1 = C1 - C2 * y1;
                if(concenter.y()>pt2[0].y())
                    concenter = QPointF(x0,y0);
                else
                    concenter = QPointF(x1,y1);
            }
            r2 = sqrt((concenter.x() - pt2[2].x())*(concenter.x() - pt2[2].x()) + (concenter.y() - pt2[2].y())*(concenter.y() - pt2[2].y()));
            startspan = atan2(pt2[0].y() - concenter.y(), pt2[0].x() - concenter.x());
            endspan = atan2(pt2[2].y() - concenter.y(), pt2[2].x() - concenter.x());
            middlespan = startspan/2 + endspan/2;
            setringsize();
        }else if(m_StateFlag == MOV_PT2)
        {
            pt2[2] = event->pos();
            qreal distance = sqrt((pt2[0].x() - pt2[2].x()) *(pt2[0].x() - pt2[2].x()) + (pt2[0].y() - pt2[2].y())*(pt2[0].y() - pt2[2].y()));
            if(distance > 2*r2)
                r2 = distance/2 + 1;
            if(std::abs(pt2[0].x() - pt2[2].x())<0.01)
            {
                qreal y0 = pt2[0].y()/2 + pt2[1].y()/2;
                qreal y1 = pt2[0].y()/2 + pt2[1].y()/2;
                qreal deltay = (y0-pt2[0].y()) * (y0-pt2[0].y());
                qreal deltax = sqrt(r2*r2 - deltay);
                qreal x0 = pt2[2].x() - deltax;
                qreal x1 = pt2[2].x() + deltax;
                if(concenter.x() > pt2[0].x())
                    concenter = QPointF(x1,y1);
                else
                    concenter = QPointF(x0,y0);
            }else
            {
                qreal C1 = (pt2[2].x()*pt2[2].x() + pt2[2].y()*pt2[2].y() - pt2[0].x()*pt2[0].x() - pt2[0].y()*pt2[0].y())/2/(pt2[2].x() - pt2[0].x());
                qreal C2 = (pt2[2].y() - pt2[0].y())/(pt2[2].x() - pt2[0].x());
                qreal A = 1+ C2*C2;
                qreal B = 2*(pt2[0].x() - C1)*C2 -2*pt2[0].y();
                qreal C = (pt2[0].x() - C1)*(pt2[0].x() - C1) + pt2[0].y()*pt2[0].y() - r2*r2;
                qreal y0 = (-B + sqrt(B*B - 4 * A * C)) / 2 / A;
                qreal y1 = (-B - sqrt(B*B - 4 * A * C)) / 2 / A;
                qreal x0 = x0 = C1 - C2 * y0;
                qreal x1 = x1 = C1 - C2 * y1;

                if(concenter.y()>pt2[0].y())
                    concenter = QPointF(x0,y0);
                else
                    concenter = QPointF(x1,y1);
            }
            r2 = sqrt((concenter.x() - pt2[0].x())*(concenter.x() - pt2[0].x()) + (concenter.y() - pt2[0].y())*(concenter.y() - pt2[0].y()));
            startspan = atan2(pt2[0].y() - concenter.y(), pt2[0].x() - concenter.x());
            endspan = atan2(pt2[2].y() - concenter.y(), pt2[2].x() - concenter.x());
            middlespan = startspan/2 + endspan/2;
            setringsize();
        }else if(m_StateFlag == MOV_RINGEXPANSION)
        {
            QPointF point = (event->pos() - m_startPos);
            moveBy(point.x(),point.y());
        }
    }
    this->scene()->update();
}

void bee_ringexpansion::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(create_move&&if_handleslist_create)
    {
        if_create = true;
        create_move = false;
    }
    setCursor(Qt::ArrowCursor);
    m_StateFlag = DEFAULT_FLAG_ORE;
    scene()->update();
}

void bee_ringexpansion::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{
    if_hover = true;
    scene()->update();
    QGraphicsItem::hoverEnterEvent(event);
}

void bee_ringexpansion::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
    if_hover = false;
    scene()->update();
    QGraphicsItem::hoverLeaveEvent(event);
}
