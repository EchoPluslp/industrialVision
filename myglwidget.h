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
	void paintEvent(QPaintEvent*);   //��дpaintEvent
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);//�ƶ�
	void mouseReleaseEvent(QMouseEvent* event);//����ͷ�
	void mouseDoubleClickEvent(QMouseEvent* event);//˫����ʼ��Ϊ��ʼ

private:
	QPixmap myPixmap;
	QPixmap myPixmap_scale;

	QString myText;
	QRect myPaintArea;         //�滭����
	int m_width;            //ͼƬ��
	int m_height;            //ͼƬ��



	double factor = 1.0;//�Ŵ���С�ı���
	int XPtInterval = 0;    //ƽ��X���ֵ
	int YPtInterval = 0;    //ƽ��Y���ֵ
	bool Pressed = false;           //����Ƿ���ѹ
	QPoint oldPos;          //�ɵ����λ��

	
	void wheelEvent(QWheelEvent* e);     //��껬���¼�
	int action;          //����(�Ŵ�,��С,�ƶ�...)

	QAction* crosshair; //ʮ����
	bool drawCrossHair_Flag = false;
	void drawcrossHair();

	QAction* rotate;    //��ת90

	QAction* restore; //��ԭͼƬ
	QMenu* RightButtonMenu; //�Ҽ��˵�
	const 	std::vector<int> angleArc = { 0,90,180,270 };
public slots:
	void crosshair_Flag();
	void rotate_Flag();
	void restore_Flag();
signals:
	void valueChanged(int value);
};
#endif // MYGLWIDGET_H
