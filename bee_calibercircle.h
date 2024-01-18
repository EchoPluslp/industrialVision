#ifndef BEE_CALIBERCIRCLE_H
#define BEE_CALIBERCIRCLE_H
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
#include "mycorneritem.h"
#include<QtMath>
#include <opencv2/opencv.hpp>
#include "myCCaliperGUI.h"

enum STATE_FLAG_ONECCIRCLE{
    DEFAULT_FLAG_OCIRC=0,
    MOV_CC,
    MOV_CPOINT,
    MOV_RPOINT

};

class bee_calibercircle:public QObject,public QGraphicsItem
{
    Q_OBJECT

public:
    bee_calibercircle(QGraphicsItem *parent = nullptr);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void setcircleize();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void setpixmapwidth(qreal width){pixmap_width = width;}
    void setpixmapheight(qreal height){pixmap_height = height;}
	void setpixmapImage(cv::Mat cvimage) { dstImage = cvimage; }

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

    void fitcircle();

    public slots:
      void  slotSliderValueChanged_MeasureLength(int value);
      void  slotSliderValueChanged_MeasureNums(int value);
      void slotSliderValueChanged_nSetThreshold(int value);
	  void slotSliderValueChanged_nSigma(int value);
      void  slotSliderValueChanged_nSampleDirection(int value);
      void slotSliderValueChanged_nTranslation(int value);

signals:
   void signal_emitstatusValue(int x,int y,int x2,int y2);
private:
    qreal pixmap_width;
    qreal pixmap_height;
    QList<mycorneritem*> m_HandlesList_circle;  //圆端点
    QList<mycorneritem*> m_HandlesList_refct; //可调控矩形端点
    QRectF first_rect;
    QPointF circle_center;
    QPointF first_center;
    QPointF pt[4]; //矩形端点
    QPointF m_startPos;
    STATE_FLAG_ONECCIRCLE m_StateFlag;
    bool if_hover = false;
    bool if_create = false;
    bool create_move = false;
    bool if_handleslist_create = false;
   // qreal width; //矩形宽度
    //qreal height; //矩形高度

	//投影方向
	int nSampleDirection = 0;
	//卡尺数
	int nMeasureNums = 5;
	//宽度
	int nMeasureHeight = 10;
	//高度
	int nMeasureLength = 100;
	//sigma
	int nSigma = 1;
	//阈值
	int nThreshold = 30;

	//边缘极性
	int nTranslation = 0;
	//拟合
	int nFitCircle = 0;
	//圆半径
	int nRadius = 250;

	cv::Mat srcImage;
	Mat dstImage;

	CCaliperCircleGUI circleInstanceGui;
};

#endif // BEE_CALIBERCIRCLE_H
