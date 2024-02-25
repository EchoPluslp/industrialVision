#ifndef MYCORNERITEM_H
#define MYCORNERITEM_H
#include <QAbstractGraphicsShapeItem>
#include <QGraphicsSceneHoverEvent>
#include <QPainter>
#include <QCursor>
#include <QGraphicsItem>
#include <qdebug.h>
enum CustomType {
    TypeImage = 65536 + 1,
    TypeCorner = 65536 + 2,
    TypeRect,
    TypeRotateRect,
    TypeCircle
};

enum SelectionHandleState
{
    SelectionHandleOff,
    SelectionHandleInactive,
    SelectionHandleActive
};

enum CornerDirction    //鼠标经过时的形状
{
    None = 0,
    LeftTop,
    Top,
    RightTop,
    Right,
    RightBottom,
    Bottom,
    LeftBottom,
    Left,
    Rotation,           //旋转
};
constexpr int CORNER_SIZE = 5;

class mycorneritem:public QAbstractGraphicsShapeItem
{
public:
    explicit mycorneritem(QGraphicsItem *parent, QPointF point, CornerDirction dir, bool control = false);
    virtual ~mycorneritem() {};
    bool hitTest(const QPointF &point);
    CornerDirction getDir() const { return m_Dir; }
    void setState(SelectionHandleState state);
    void move(QPointF point);
    void cornerTranslate(QPointF point);
    inline void setSelectState(bool bState) { m_bSelect = bState; }
    inline bool getSelectState() { return m_bSelect; }
    inline QPointF getPointCenter() { return m_point; }
    qreal getboundingrectwidth();
    virtual QRectF boundingRect() const override;

    virtual int type() const { return TypeCorner; }
    QPointF m_point;

private:
    QBrush m_brush;
    CornerDirction m_Dir;
    SelectionHandleState m_State;
    QCursor m_RotateCursor;  //旋转图标
    double m_scaleFactor;
    bool m_bSelect;   //对由多个点组成的图形，点击被选中的标志
    QPointF m_ptPress, m_ptMove;

protected:
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) Q_DECL_OVERRIDE;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};
#endif // MYCORNERITEM_H
