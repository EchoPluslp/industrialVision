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

//��֪Shape��PaintScene����
void MyGraphicsView::resizeEvent(QResizeEvent* event)
{
	emit viewResized(event);
}


//��������
void MyGraphicsView::wheelEvent(QWheelEvent* event)
{
	QPoint delta = event->angleDelta();

	QPointF scenePos = mapToScene(event->pos());  // ��ȡ����¼�λ���ڳ����е�����

	if (delta.y() > 0) {
		// �Ŵ�
		scale(1 / m_scale, 1 / m_scale);
		m_scale + SCALE_STEP >= MAX_SCALE ? m_scale = MAX_SCALE : m_scale += SCALE_STEP;
		setTransformationAnchor(QGraphicsView::AnchorUnderMouse);  // ����ê��Ϊ���λ��
		setTransform(QTransform::fromScale(m_scale, m_scale), true);  // Ӧ�����ű任
	}
	else {
		// ��С
		scale(1 / m_scale, 1 / m_scale);
		m_scale - SCALE_STEP <= 1.0 ? m_scale = 1.0 : m_scale -= SCALE_STEP;
		setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
		setTransform(QTransform::fromScale(m_scale, m_scale), true);
	}

	centerOn(scenePos);  // ʹ���ź����λ�ñ�������ͼ����
}

