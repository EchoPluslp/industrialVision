#pragma once
#include "preheader.h"
#include "GraphicsRectItem.h"
//extern QList<GraphicsItem*> choose_List_Item;    //extern 关键字再次声明这个全局变量


class GraphicsEllipseItem : public GraphicsRectItem
{
public:
    GraphicsEllipseItem(const QRect& rect, QGraphicsItem* parent);
    QPainterPath shape() const;
protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
};
