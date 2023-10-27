#pragma once
#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QOpenGLWidget>
#include <QPainter>
#include <QtGui/QOpenGLFunctions>
#include <QtGui/QMatrix4x4>
#include <QtGui/QWheelEvent>
#include  <QtMath>
#include<QMenu>
#pragma execution_character_set("utf-8")
extern int m_rotateIndexInt;
class MyGLWidget : public QOpenGLWidget
{
	Q_OBJECT

public:
	MyGLWidget(QWidget* parent);
	MyGLWidget();
	~MyGLWidget();

	void setPixmap(QPixmap pixmap, QString text);

protected:
	void paintEvent(QPaintEvent*);   //重写paintEvent
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);//移动
	void mouseReleaseEvent(QMouseEvent* event);//鼠标释放
	void mouseDoubleClickEvent(QMouseEvent* event);//双击初始化为开始

private:
	QPixmap myPixmap;
	QPixmap myPixmap_scale;

	QString myText;
	QRect myPaintArea;         //绘画区域
	int m_width;            //图片宽
	int m_height;            //图片高



	double factor = 1.0;//放大缩小的倍数
	int XPtInterval = 0;    //平移X轴的值
	int YPtInterval = 0;    //平移Y轴的值
	bool Pressed = false;           //鼠标是否被摁压
	QPoint oldPos;          //旧的鼠标位置

	
	void wheelEvent(QWheelEvent* e);     //鼠标滑轮事件
	int action;          //动作(放大,缩小,移动...)

	QAction* crosshair; //十字线
	bool drawCrossHair_Flag = false;
	void drawcrossHair();

	QAction* rotate;    //旋转90

	QAction* restore; //还原图片
	QMenu* RightButtonMenu; //右键菜单
	const 	std::vector<int> angleArc = { 0,90,180,270 };
public slots:
	void crosshair_Flag();
	void rotate_Flag();
	void restore_Flag();
signals:
	void valueChanged(int value);
};
#endif // MYGLWIDGET_H
