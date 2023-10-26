#ifndef DRAWTOOL_H
#define DRAWTOOL_H

#include "preheader.h"
#include "MainPaintScene.h"
#include "GraphicsRectItem.h"
#include "GraphicsPixmapItem.h"
#include "GraphicsPathItem.h"
#include "GraphicsArrowItem.h"
#include "GraphicsLineItem.h"
#include <QObject>
QT_BEGIN_NAMESPACE
class QGraphicsScene;
class QGraphicsSceneMouseEvent;
class QMenu;
class QPointF;
class QGraphicsLineItem;
class QFont;
class QGraphicsTextItem;
class QColor;
class QAbstractGraphicsShapeItem;
class PaintView;
QT_END_NAMESPACE

class MainPaintScene;
class GraphicsItem;

class DrawTool : public QObject
{
    Q_OBJECT

public:
    DrawTool(DrawBaseShape shape = DrawBaseShape::selection);

    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event, MainPaintScene* scene);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event, MainPaintScene* scene);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event, MainPaintScene* scene);

    virtual void setPicture(QString path = "") {}

    void setCurrentColor(QColor color)
    {
        _mColor = color;
    }
    QColor _mColor;
    DrawBaseShape m_drawShape;
    static DrawTool* findTool(DrawBaseShape drawShape);
    static QList<DrawTool*> c_tools;
    static QPointF c_down;
    static quint32 c_nDownFlags;
    static QPointF c_last;
    static DrawBaseShape c_drawShape;
    static QList<GraphicsItem*> GraphicsItemList;    //������һ��ָ��ȫ�����ڴ�ĵ�ַ
signals:
    void rectItemFinallInTOOL(DrawBaseShape);
};

class SelectTool : public DrawTool
{
public:
    SelectTool();
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event, MainPaintScene* scene);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event, MainPaintScene* scene);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event, MainPaintScene* scene);
    QSizeF  m_lastSize;
    QGraphicsPathItem* dashRect;
    QGraphicsLineItem* dashLine;
    bool m_hoverSizer;

};

class  RotationTool : public DrawTool
{
public:
    RotationTool();
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event, MainPaintScene* scene);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event, MainPaintScene* scene);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event, MainPaintScene* scene);
    qreal lastAngle;
    QGraphicsPathItem* dashRect;

};


class ImageTool : public DrawTool
{
public:
    ImageTool(DrawBaseShape drawShape);
    virtual void setPicture(QString path = "");
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event, MainPaintScene* scene);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event, MainPaintScene* scene);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event, MainPaintScene* scene);
    GraphicsPixmapItem* item;
    QString _imgPath;
};

class RectTool : public DrawTool
{
    Q_OBJECT
public:
    RectTool(DrawBaseShape drawShape);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event, MainPaintScene* scene);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event, MainPaintScene* scene);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event, MainPaintScene* scene);
    GraphicsRectItem* item;

};

// ��
class PenPathTool : public DrawTool
{
public:
    PenPathTool(DrawBaseShape drawShape);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event, MainPaintScene* scene);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event, MainPaintScene* scene);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event, MainPaintScene* scene);
    QGraphicsPathItem* item;
    QPainterPath* _path;
    bool isDrawing; //������
};

//�߶�
class LineTool : public DrawTool
{
public:
    LineTool(DrawBaseShape drawShape);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event, MainPaintScene* scene);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event, MainPaintScene* scene);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event, MainPaintScene* scene);
    GraphicsLineItem* item;
    bool isDrawing; //������
};

//��ͷ
class ArrowItem : public DrawTool
{
public:
    ArrowItem(DrawBaseShape drawShape);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event, MainPaintScene* scene);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event, MainPaintScene* scene);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event, MainPaintScene* scene);
    GraphicsArrowItem* item;
    bool isDrawing;
};

#endif // MAINPAINTSCENEE_H
