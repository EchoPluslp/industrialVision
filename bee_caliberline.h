#ifndef BEE_CALIBERLINE_H
#define BEE_CALIBERLINE_H
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
#include <opencv2/opencv.hpp>
#include "myCCaliperGUI.h"

enum STATE_FLAG_ONELINE{
    DEFAULT_FLAG_OL=0,

    MOV_RECT_PT,
    MOV_ALL,
    MOV_LINE_BEGIN,
    MOV_LINE_END

};


class bee_caliberline:public QObject,public QGraphicsItem
{
    Q_OBJECT
public:
    bee_caliberline(QGraphicsItem *parent = nullptr);
    ~bee_caliberline()
    {
		if (m_plineCaliperGUI)
		{
			delete m_plineCaliperGUI;
		}
    }
    QRectF boundingRect() const;
    QPainterPath shape() const; 
    void setlinsize();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void setpixmapwidth(qreal width){pixmap_width = width;}
    void setpixmapheight(qreal height){pixmap_height = height;}
	void setpixmapImage(cv::Mat cvimage) { dstImage = cvimage; }
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);


	qreal getwidth() { return width; }
	qreal getheight() { return height; }
	qreal getlength() { return length; }
	QPointF getbeginpoint() {
        return pt_begin;
    }
	QPointF getendpoint() { return pt_end; }
	bool if_create = false;

	void currentIndex(int index) { currentIndexs = index; };
    //设置卡尺数
    void setnMeasureNums(int value) { nMeasureNums = value; };
	//设置阈值
	void setnthresholdValue(int value) { nthresholdValue = value; };
	//设置投影方向
	void setnSampleDirection(int value) { nSampleDirection = value; };
    //设置开始坐标-用于保存
    void setSaveStartPoint(cv::Point point_start_Item) { pt_begin_cv2 = point_start_Item; };
	//设置结束坐标-用于保存
	void setEndStartPoint(cv::Point point_end_Item) { pt_end_cv2 = point_end_Item; };
    //设置roi-用于保存
    void setRoi(cv::Rect roi_item) { roi = roi_item; };
    //设置线的开始坐标_用于保存
	void setLineStartPoint(cv::Point point_start_line_item) {pt_begin_line = point_start_line_item;};
    //设置线的结束坐标_用于保存
	void setLineEndPoint(cv::Point point_end_line_item) {pt_end_line = point_end_line_item;}
public slots:
    void slotSliderValueChanged_MeasureNums_line(int value);
    void slotSliderValueChanged_SetThreshold_line(int value);
    void slotSliderValueChanged_SetSampleDirection_line(int value);
signals:
	//卡尺个数
	   //阈值
	   //边缘极性
    void sign_currentLine_Param(int nMeasureNums, int nthresholdValue,int nSampleDirection,int currentIndexs);

private:
    qreal pixmap_width;
    qreal pixmap_height;
    QList<mycorneritem*> m_HandlesList_line;  //直线端点
    QList<mycorneritem*> m_HandlesList_refct; //可调控矩形端点
    bool if_create_move = false;
    bool if_handleslist_create = false;
    QRectF  first_rect; //可调控矩形
    QPolygonF first_poly; //可调控矩形旋转后
    QPointF first_center;
    QPointF pt[4]; //矩形端点
    QPointF pp[4]; //旋转后端点
    QPointF m_startPos;
    STATE_FLAG_ONELINE m_StateFlag;
    bool if_hover = false;
    QPointF pt_begin; //直线起点
    QPointF pt_end;  //直线终点
    QPointF first_center_top; //pp[0] 与 pp[1]终点
    QPointF first_center_bottom; // pp[2] 与 pp[3]终点
    qreal width; //矩形宽度
    qreal height; //矩形高度
	qreal length;
    qreal k; //直线斜率
    QPointF m_lastPos;
    cv::Mat dstImage;
    //当前线的次序
    int currentIndexs;


    
public:
	//卡尺个数
	int nMeasureNums = 5;  
    //阈值
    int nthresholdValue = 5;
    //边缘方向
    int nSampleDirection = 1;


	CLineCaliperGUI* m_plineCaliperGUI;

	//开始坐标
	cv::Point pt_begin_cv2;
	//结束坐标
	cv::Point pt_end_cv2;
	//roi坐标
	cv::Rect roi;
    //pt_begin_line 线的开始坐标
    cv::Point pt_begin_line;
	//pt_end_line 线的结束坐标
    cv::Point pt_end_line;

};

#endif // BEE_CALIBERLINE_H
