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

void MyGraphicsView::drawBackground(QPainter* painter, const QRectF& rect)
{
	//»æÖÆ»Ò°×ÆåÅÌÍ¼Ïñ±³¾°
	int wid = this->geometry().width();
	int hei = this->geometry().height();
	QPointF m_ptCenter = this->mapToScene(wid / 2, hei / 2);
	QPixmap pix(wid, hei);
	QPainter pter(&pix);
	QColor clr_white(Qt::white);
	QColor clr_gray(240, 240, 240, 240);
	int spacing = 15;
	QColor useColor;
	for (int i = 0; i <= floor(wid / spacing); i++)
	{
		for (int j = 0; j <= floor(hei / spacing); j++)
		{
			useColor = ((i + j) % 2 == 0 ? clr_white : clr_gray);
			pter.fillRect(i * spacing, j * spacing, spacing, spacing, useColor);
		}
	}
	painter->drawImage(rect, pix.toImage());

}

//¸æÖªShapeºÍPaintSceneËõ·Å
void MyGraphicsView::resizeEvent(QResizeEvent* event)
{
	emit viewResized(event);
}


//¹öÂÖËõ·Å
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



