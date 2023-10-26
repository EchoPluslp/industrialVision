#ifndef MAINPAINTSCENEE_H
#define MAINPAINTSCENEE_H

#include <QGraphicsScene>

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
class QMenu;
class QPointF;
class QFont;
class QColor;
QT_END_NAMESPACE
#include "drawtool.h"
#include "GraphicsLineItem.h"

//class DrawTool;

class MainPaintScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit MainPaintScene(QObject* parent = 0);
    ~MainPaintScene();
    QGraphicsPixmapItem* _backGroundItem = nullptr; 

    void mouseEvent(QGraphicsSceneMouseEvent* mouseEvent);
    void InsertPicture(QString path);
    void SetBackGroundImage(QImage image);

    void setCurrentColor(QColor);

public slots:
    void SetCurrentShape(DrawBaseShape shape);
    void getDrawToolInfo(DrawBaseShape shape);

signals:
    void itemSelected(QGraphicsItem* item);
    void rectItemFinallMainScene(DrawBaseShape shape);

private:
    void keyPressEvent(QKeyEvent* event);
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
    QColor _color = QColor(255, 88, 80, 255);
};

#endif // MAINPAINTSCENEE_H
