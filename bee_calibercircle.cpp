#include "bee_calibercircle.h"

bee_calibercircle::bee_calibercircle(QGraphicsItem *parent):
    m_StateFlag(DEFAULT_FLAG_OCIRC)
{
    r=0;
    circle_center = QPointF(0,0);
    setAcceptHoverEvents(true);
    setCursor(Qt::ArrowCursor);
}

qreal getdistance(QPointF pt1, QPointF pt2)
{
    qreal dis = sqrt((pt1.x()-pt2.x())*(pt1.x()-pt2.x()) + (pt1.y()-pt2.y())*(pt1.y()-pt2.y()));
    return dis;
}

QPointF getRotatePoint(QPointF ptCenter, QPointF ptIn, qreal angle)
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

QRectF bee_calibercircle::boundingRect() const
{
    if(!if_create)
        return QRectF(0, 0, pixmap_width, pixmap_height);
    else
        return QRectF(circle_center.x()-r- nMeasureLength -3,circle_center.y()-r- nMeasureLength -3,2*(r+ nMeasureLength +3),2*(r+ nMeasureLength +3));
}

QPainterPath bee_calibercircle::shape() const
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
        QPainterPath m_path3;

        m_path.addEllipse(circle_center.x() -r,circle_center.y()-r,2*r,2*r);
        qreal angle_dis = 360/ nMeasureNums;
        qreal angle = -90;
        QVector<QPointF> vpt;
        QPointF pp[4];
        for(int i=0;i< nMeasureNums;i++)
        {
            QPointF center = QPointF(circle_center.x()+r*cos(angle/180*M_PI),circle_center.y()+r*sin(angle/180*M_PI));
            pp[0] = QPointF(center.x()- nMeasureHeight /2,center.y()- nMeasureLength /2); //LT
            pp[1] = QPointF(center.x()+ nMeasureHeight /2,center.y()- nMeasureLength /2); //RT
            pp[2] = QPointF(center.x()+ nMeasureHeight /2,center.y()+ nMeasureLength /2); //RB
            pp[3] = QPointF(center.x()- nMeasureHeight /2,center.y()+ nMeasureLength /2); //LB

            for(int j=0;j<4;j++)
            {
                pp[j] = getRotatePoint(center,pp[j],(angle+90));
                vpt.append(pp[j]);
            }
            vpt.append(pp[0]);
            m_path2.addPolygon(QPolygonF(vpt));
            vpt.clear();
            angle += angle_dis;
        }
        for(int i=0;i<4;i++)
        {
            m_path3.addRect(m_HandlesList_refct[i]->boundingRect());
            m_path3.addRect(m_HandlesList_circle[i]->boundingRect());
        }
        return m_path.united(m_path2).united(m_path3);

    }

}

void bee_calibercircle::setcircleize()
{
    m_HandlesList_circle[0]->move(circle_center);
    m_HandlesList_circle[1]->move(QPointF(circle_center.x()+r,circle_center.y()));
    m_HandlesList_circle[2]->move(QPointF(circle_center.x(),circle_center.y()+r));
    m_HandlesList_circle[3]->move(QPointF(circle_center.x()-r,circle_center.y()));

    first_center = QPointF(circle_center.x(),circle_center.y()-r);
    pt[0] = QPointF(first_center.x()- nMeasureHeight /2,first_center.y()- nMeasureLength /2); //LT
    pt[1] = QPointF(first_center.x()+ nMeasureHeight /2,first_center.y()- nMeasureLength /2); //RT
    pt[2] = QPointF(first_center.x()+ nMeasureHeight /2,first_center.y()+ nMeasureLength /2); //RB
    pt[3] = QPointF(first_center.x()- nMeasureHeight /2,first_center.y()+ nMeasureLength /2); //LB
    for(int i=0;i<4;i++)
        m_HandlesList_refct[i]->move(pt[i]);
    first_rect = QRectF(pt[0].x(),pt[0].y(), nMeasureHeight, nMeasureLength);
}

void bee_calibercircle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(!if_create)
    {
        QPen mPen(Qt::magenta);//绘制箭头
        mPen.setWidth(0);
        painter->setPen(mPen);
        painter->drawEllipse(circle_center,r,r);
    }
    else
    {
        QPen mPen(Qt::magenta);
        if(if_hover)
            mPen.setColor(Qt::magenta);
        mPen.setWidth(0);
        painter->setPen(mPen);
        qreal angle_dis = 360/ nMeasureNums;
        qreal angle = -90;
        QVector<QPointF> vpt;
        for(int i=0;i< nMeasureNums;i++)
        {
            QPointF center = QPointF(circle_center.x()+r*cos(angle/180*M_PI),circle_center.y()+r*sin(angle/180*M_PI));
//            QPointF center = QPointF(circle_center.x(),circle_center.y()-r);
            QPointF pp[4];
            pp[0] = QPointF(center.x()- nMeasureHeight /2,center.y()- nMeasureLength /2); //LT
            pp[1] = QPointF(center.x()+ nMeasureHeight /2,center.y()- nMeasureLength /2); //RT
            pp[2] = QPointF(center.x()+ nMeasureHeight /2,center.y()+ nMeasureLength /2); //RB
            pp[3] = QPointF(center.x()- nMeasureHeight /2,center.y()+ nMeasureLength /2); //LB

            for(int j=0;j<4;j++)
            {
                pp[j] = getRotatePoint(center,pp[j],(angle+90));
                vpt.append(pp[j]);
            }
            vpt.append(pp[0]);
            painter->drawPolygon(QPolygonF(vpt));
            vpt.clear();
            angle += angle_dis;
        }

        QPointF end_1 = pt[0] - (pt[0] - pt[3])/4;
        QPointF end_2 = pt[1] - (pt[1] - pt[2])/4;
        QPointF end_dis = end_2-end_1;

        angle = M_PI/2;
        qreal ExtRefArrowDegrees = M_PI/6;

        qreal arrow_length_2 = nMeasureLength /8;
        QPointF p_11 = QPointF(end_1.x()+arrow_length_2*cos(angle-ExtRefArrowDegrees), end_1.y()+arrow_length_2*sin(angle-ExtRefArrowDegrees));
        QPointF p_12 = QPointF(end_1.x()+arrow_length_2*cos(angle+ExtRefArrowDegrees), end_1.y()+arrow_length_2*sin(angle+ExtRefArrowDegrees));
        QPointF p_21 = p_11+end_dis;
        QPointF p_22 = p_12+end_dis;

        painter->drawLine(p_11,end_1);
        painter->drawLine(end_1,p_12);
        painter->drawLine(p_21,end_2);
        painter->drawLine(end_2,p_22);
        painter->drawEllipse(circle_center,r,r);
//        painter->drawEllipse(circle_center.x()-r,circle_center.y()-r,2*r,2*r);
    }
    scene()->update();
}

void bee_calibercircle::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

    if(event->button() ==event->button()== Qt::LeftButton)
    {
        if(!if_create)
        {
            circle_center = event->pos();
            create_move=true;
        }
        else
        {
            m_startPos= event->pos();
            for(int i=1;i<4;i++) //移动圆环半径
            {
                if(m_HandlesList_circle[i]->contains(event->pos()))
                {
                    m_StateFlag = MOV_CPOINT;
                    return;
                }
            }
            for(int i=0;i<4;i++) //移动卡尺 
            {
                if(m_HandlesList_refct[i]->contains(event->pos()))
                {
                    m_StateFlag = MOV_RPOINT;
                    return;
                }
            }
            if(this->boundingRect().contains(m_startPos) && m_StateFlag == DEFAULT_FLAG_OCIRC)
                m_StateFlag = MOV_CC;
        }
    }
    scene()->update();//必须要用scene()->update()，不能用update();否则会出现重影
}

void bee_calibercircle::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(create_move)
    {
        r = getdistance(circle_center,event->pos());

        circleInstanceGui.createCircleKaChi(srcImage, cv::Point2d(circle_center.x(), circle_center.y()), r, nMeasureLength, nMeasureHeight, nSigma,
			nThreshold, nTranslation, nMeasureNums, nSampleDirection);
    }else if(m_StateFlag == MOV_CPOINT)//移动圆环半径
    {
        
        r = getdistance(circle_center,event->pos());
        setcircleize();
        scene()->update();
		circleInstanceGui.AdjustCaliper(srcImage, Point2d(-4, -4), r, nMeasureLength, nMeasureHeight,
			1, nThreshold, nTranslation, nMeasureNums, nSampleDirection);
    }else if(m_StateFlag == MOV_CC) //中心区域
    {
        //移动中心
        //修改m_中心坐标

        QPointF point = (event->pos() - m_startPos);
        moveBy(point.x(), point.y());
        setcircleize();
        scene()->update();
        //移动圆形卡尺圆心
		circleInstanceGui.AdjustCaliper(srcImage, Point2d(event->pos().x(), event->pos().y()), nRadius, nMeasureLength, nMeasureHeight,
			1, nThreshold, nTranslation, nMeasureNums, nSampleDirection);
        
    }else if(m_StateFlag == MOV_RPOINT)//移动卡尺的长宽
    {
        QPointF PT = event->pos();
        QPointF CE = first_center;
        //宽度
        nMeasureHeight = std::abs(PT.x() - CE.x())*2;
        //高度
        nMeasureLength = std::abs(PT.y() -CE.y())*2;
        setcircleize();
        scene()->update();
        //修改移动卡尺的长宽参数
		circleInstanceGui.AdjustCaliper(srcImage, Point2d(-3, -3), nRadius, nMeasureLength, nMeasureHeight,
			1, nThreshold, nTranslation, nMeasureNums, nSampleDirection);
		circleInstanceGui.AdjustCaliper(srcImage, Point2d(-2, -2), nRadius, nMeasureLength, nMeasureHeight,
			1, nThreshold, nTranslation, nMeasureNums, nSampleDirection);
    }
}

void bee_calibercircle::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(create_move)
    {
        if(r<2)
            return;
        if_create = true;
        create_move = false;
        r = getdistance(circle_center,event->pos());
        nMeasureHeight = r/4;
        nMeasureLength = r/4;
        if(!if_handleslist_create)
        {

            first_center = QPointF(circle_center.x()+r,circle_center.y()-r);
            pt[0] = QPointF(first_center.x()- nMeasureHeight /2,first_center.y()- nMeasureLength /2); //LT
            pt[1] = QPointF(first_center.x()+ nMeasureHeight /2,first_center.y()- nMeasureLength /2); //RT
            pt[2] = QPointF(first_center.x()+ nMeasureHeight /2,first_center.y()+ nMeasureLength /2); //RB
            pt[3] = QPointF(first_center.x()- nMeasureHeight   /2,first_center.y()+ nMeasureLength /2); //LB


            const int num1 = 4;
            QPointF point1[num1] = {circle_center, QPointF(circle_center.x()+r,circle_center.y()),QPointF(circle_center.x(),circle_center.y()+r),QPointF(circle_center.x()-r,circle_center.y())};
            CornerDirction dir1[num1] = { None ,Left,Bottom,Right};
            for(int i =0;i<num1;i++)
            {
                mycorneritem* corner = new mycorneritem(this, point1[i], dir1[i]);
                m_HandlesList_circle.push_back(corner);
            }
            const int num2 = 4;
            QPointF point2[num2] = {pt[0], pt[1],pt[2],pt[3]};
            CornerDirction dir2[num2] = { LeftTop ,RightTop,RightBottom,LeftBottom};
            for(int i =0;i<num2;i++)
            {
                mycorneritem* corner = new mycorneritem(this, point2[i], dir2[i]);
                m_HandlesList_refct.push_back(corner);
            }
            setcircleize();
            if_handleslist_create = true;
        }
        else
            setcircleize();
    }
    m_StateFlag = DEFAULT_FLAG_OCIRC;
    scene()->update();//必须要用scene()->update()，不能用update();否则会出现重影
}


void bee_calibercircle::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{
    if_hover = true;
    scene()->update();
    QGraphicsItem::hoverEnterEvent(event);
}

void bee_calibercircle::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
    if_hover = false;
    scene()->update();
    QGraphicsItem::hoverLeaveEvent(event);
}

void bee_calibercircle::fitcircle()
{
    Point2d pdCenter(0, 0);
    double dRadius = 0;
    //执行匹配
    circleInstanceGui.circleFitOpt(pdCenter, dRadius, 0);
    dstImage.copyTo(srcImage);
    ///123
    circleInstanceGui.edgePointSetsShow(srcImage, 10, GREED);

    //circle(srcImage, pdCenter, dRadius, GREED);
   // drawMyCross(srcImage, pdCenter, 90, 5, GREED);
   // imshow(WindowHandle, srcImage);
    cout << "Circle info: " << pdCenter << ", Radius: " << dRadius << endl;

    vector<Point2d>vpdEdgePoints;
    vector<double>vdEdgePointsGradient;
    circleInstanceGui.getEdgeSetsInfo(vpdEdgePoints, vdEdgePointsGradient);
    for (int i = 0; i < vpdEdgePoints.size(); i++)
    {
        cout << "Edge Point[" << i << "]: " << vpdEdgePoints[i] << ", gradient: " << vdEdgePointsGradient[i] << endl;
    }
}



void  bee_calibercircle::slotSliderValueChanged_MeasureNums(int value) {
	 nMeasureNums = value;
     //更新 卡尺
	 //setcircleize();
	 scene()->update();
	 dstImage.copyTo(srcImage);
	 circleInstanceGui.AdjustCaliper(srcImage, Point2d(-1, -1), nRadius, nMeasureLength, nMeasureHeight,
		 1, nThreshold, nTranslation, nMeasureNums, nSampleDirection);
}

void bee_calibercircle::slotSliderValueChanged_nSetThreshold(int value)
{
    int nThreshold = value;

	circleInstanceGui.AdjustCaliper(srcImage, Point2d(-5, -5), nRadius, nMeasureLength, nMeasureHeight,
		1, nThreshold, nTranslation, nMeasureNums, nSampleDirection);
}

void  bee_calibercircle::slotSliderValueChanged_MeasureLength(int value) {
	//dstImage.copyTo(srcImage);
	circleInstanceGui.AdjustCaliper(srcImage, Point2d(-2, -2), nRadius, nMeasureLength, nMeasureHeight,
		1, nThreshold, nTranslation, nMeasureNums, nSampleDirection);
}