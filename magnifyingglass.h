#ifndef MAGNIFYINGGLASS_H
#define MAGNIFYINGGLASS_H

#include <QWidget>
#include <QDockWidget>
#include <QGraphicsView>
#include "paintscene.h"
#pragma execution_character_set("utf-8")



//�Ŵ�Dock����һ��View��ʾ��ͼScene�����趨����
class MagnifyingGlass : public QDockWidget
{
	Q_OBJECT
		QGraphicsView* view;
	PaintScene* scene;
	// ����
	uint ratio = 3;
	bool showing = false;
public:
	explicit MagnifyingGlass(QWidget* parent = nullptr);
	void setPaintScene(PaintScene* scene);
	bool isShowing();
protected:
	void showEvent(QShowEvent* e);
	void closeEvent(QCloseEvent* e);
signals:

public slots:
	void setDisplayPos(QPoint mousePos);
	void ratioChanged(int value);
};

#endif // MAGNIFYINGGLASS_H
