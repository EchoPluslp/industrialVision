#ifndef SHAPE_H
#define SHAPE_H
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QDebug>
#include <QPen>
#include <QBrush>
#include <QDomDocument>


/*
 * ����������ά��������Ļ���
 * ͳһ�ṩ�˻�ͼ����Ľӿڣ���PaintScene��ͼ�������麯��ʵ�ֶ�̬
 */
class Shape
{
public:
	enum Figure {
		Empty,
		Rect,
		Ellipse,
		Polygon,
		Curve,
		CirclePen,
		SquarePen,
		Point
	};
	Shape();
	virtual ~Shape() = 0;
	Shape(const Shape& s);


public:
	Figure m_type;
	//���Լ�¼��ͼ�Ƿ����
	bool m_quit = false;
	//��¼Shape��ǰ����ͼƬ��Size,��ͼƬ����ʱʹShape������Ӧ������
	QSize m_currSize = QSize(0, 0);

	//��ͼ����¼�
	virtual void mousePress(QGraphicsSceneMouseEvent* event) = 0;
	virtual void mouseMove(QGraphicsSceneMouseEvent* event) = 0;
	virtual void mouseRelease(QGraphicsSceneMouseEvent* event) = 0;
	virtual bool quitDrawing(QGraphicsSceneMouseEvent* event) = 0;

	//resize�¼�������Shapeʹ����ͼƬ��Ӧ
	virtual void resizeEvent(QSize newSize) = 0;


	virtual QGraphicsItem* getItem() const = 0;
	virtual QGraphicsItem* copyItem() const = 0;
	virtual Figure getType() const {
		return m_type;
	}

	virtual QSize getCurrSize();

	virtual void setPen(QPen pen, QBrush brush) = 0;
	virtual void setRadius(int r) {}

	//����Shape
	virtual Shape* clone() = 0;

	//�Դ�Shape����DOM�ڵ㣬������Ϣ
	virtual QDomElement getElement(QDomDocument& doc) = 0;
	//��DOM�ڵ�ָ�Shape
	virtual Shape* createFromElement(QDomElement elem) = 0;
	//��ȡ��ǰ�Ľڵ���Ϣ
	virtual QRect getSize() = 0;

};

#endif // SHAPE_H
