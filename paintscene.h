#ifndef PAINTSCENE_H
#define PAINTSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneEvent>

#include <QWidget>
#include <QtDebug>
#include "shape.h"
#include "rect.h"
#include "ellipse.h"
#include "curve.h"
#include "polygon.h"
#include "circlepen.h"
#include "squarepen.h"
#include "area.h"
#include "labelcontroller.h"
#include "label.h"
#include <QColor>
#include <QPen>
#include <QSharedPointer>
#include <QGraphicsView>
#include <QSet>

/*
 * ��������¼�
 * ��ʾͼƬ
 * �������ƵĶ��󣬲����䷢�͸�Controller
 * ����ֻ�����ͼ��ص���Ϣ
 */

class PaintScene : public QGraphicsScene {
	Q_OBJECT

public:
	// ���ڻ�ͼ��ͼ������
	Shape::Figure currShapeType;

	// ���ڻ��Ƶ�ͼ��
	Shape* currShape;

	// ����ʾ�ı���ͼ
	QGraphicsPixmapItem* currPixmap = nullptr;

	// �趨�ı��뻭ˢ����LabelType����
	QPen currPen;
	QBrush currBrush;
	int radius;


	//�滭
	bool isDrawing;
	//�϶�
	bool isDraging = true;

	bool ifTrackMouse;

	//������
	QGraphicsLineItem* crossLineX, * crossLineY;
	QGraphicsRectItem* squarePenCurse;
	QGraphicsEllipseItem* circlePenCurse;

	
	QGraphicsLineItem* crosshairLinePosX;
	QGraphicsLineItem* crosshairLinePosY;

public:
	PaintScene(QObject* parent = nullptr);
	~PaintScene();



public slots:
	//�趨��������
	void setCurrentShape(Shape::Figure s);
	//�趨������ʽ
	void setPen(QPen pen, QBrush brush);


	QPen& getPen();
	QBrush& getBrush();

	//�ı���ʾ��ͼƬ
	void changeCurrImage(QImage image);
	//������ʾ��ǰͼƬ������Shape
	virtual void updateShapes(LabelController* labelController, const QString& imageName);
	//������ʾĳһShape
	void updateShape(Shape* shape);
	//�趨Բ�Ρ������ʴ�ϸ
	void setRadius(int value);

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent* event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);


signals:
	void mouseTrackingOn();
	void mouseTrackingOff();
	void drawFinished(Shape* shape);
	void scenePos(QPoint);
	void sendCurrPosToThreeview(double, double);
	void viewPos(QPoint);
	void itemSelected(QGraphicsItem*);
};




#endif // PAINTSCENE_H
