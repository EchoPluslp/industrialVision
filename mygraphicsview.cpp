#include "mygraphicsview.h"



#define MAX_SCALE 3
#define MIN_SCALE 0.5
#define SCALE_STEP 0.03


MyGraphicsView::MyGraphicsView(QWidget* parent) : QGraphicsView(parent)
{
	setAlignment(Qt::AlignCenter);
	setRenderHint(QPainter::Antialiasing);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
	//setDragMode(QGraphicsView::ScrollHandDrag);
	m_scale = 1.0;
}

QPoint MyGraphicsView::returnMouseInView()
{
	return graphicPos;
}

//告知Shape和PaintScene缩放
void MyGraphicsView::resizeEvent(QResizeEvent* event)
{
	emit viewResized(event);
}


//滚轮缩放
void MyGraphicsView::wheelEvent(QWheelEvent* event)
{

	QPoint delta = event->angleDelta();
	QPointF scenePos = mapToScene(event->pos());
	
	qreal scaleFactor = 1.0 + (delta.y() > 0 ? SCALE_STEP : -SCALE_STEP);
	qreal newScale = m_scale * scaleFactor;

	if (newScale > MAX_SCALE) {
		scaleFactor = MAX_SCALE ;
	}
	else if (newScale < MIN_SCALE) {
		scaleFactor = MIN_SCALE ;
	}

	if (scaleFactor == MAX_SCALE || scaleFactor == MIN_SCALE)
	{
		return;
	}
	scale(scaleFactor, scaleFactor);

	centerOn(scenePos);
	update();
	
	m_scale = transform().m11();
}



