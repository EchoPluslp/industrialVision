#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QWidget>
#include <QGraphicsView>
#include <QWheelEvent>
#include "paintscene.h"
#pragma execution_character_set("utf-8")


// 重写QGraphicsView以支持滚轮事件和缩放
class MyGraphicsView : public QGraphicsView
{
	Q_OBJECT
		double m_scale = 1.0;
	QPoint graphicPos;
public:
	explicit MyGraphicsView(QWidget* parent = nullptr);
	QPoint returnMouseInView();

protected:
	void resizeEvent(QResizeEvent* event) override;
	void wheelEvent(QWheelEvent* event) override;
	
	//void mousePressEvent(QMouseEvent* event);
	//void mouseMoveEvent(QMouseEvent* event);
	//void mouseReleaseEvent(QMouseEvent* event);



signals:
	void viewResized(QResizeEvent*);

public slots:

};

#endif // MYGRAPHICSVIEW_H
