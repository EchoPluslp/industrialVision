#include "mygraphicsview.h"



#define MAX_SCALE 3
#define SCALE_STEP 0.05


MyGraphicsView::MyGraphicsView(QWidget* parent) : QGraphicsView(parent)
{
	setAlignment(Qt::AlignCenter);
	setRenderHint(QPainter::Antialiasing);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
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

	QPointF scenePos = mapToScene(event->pos());  // 获取鼠标事件位置在场景中的坐标

	if (delta.y() > 0) {
		// 放大
		scale(1 / m_scale, 1 / m_scale);
		m_scale + SCALE_STEP >= MAX_SCALE ? m_scale = MAX_SCALE : m_scale += SCALE_STEP;
		setTransformationAnchor(QGraphicsView::AnchorUnderMouse);  // 设置锚点为鼠标位置
		setTransform(QTransform::fromScale(m_scale, m_scale), true);  // 应用缩放变换
	}
	else {
		// 缩小
		scale(1 / m_scale, 1 / m_scale);
		m_scale - SCALE_STEP <= 1.0 ? m_scale = 1.0 : m_scale -= SCALE_STEP;
		setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
		setTransform(QTransform::fromScale(m_scale, m_scale), true);
	}

	centerOn(scenePos);  // 使缩放后鼠标位置保持在视图中心
}

