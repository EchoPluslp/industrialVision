#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H
#pragma execution_character_set("utf-8")

#include <QGraphicsView>

class GraphicsView : public QGraphicsView
{
	Q_OBJECT
public:
	explicit GraphicsView(QWidget* parent = nullptr);
};

#endif // GRAPHICSVIEW_H
