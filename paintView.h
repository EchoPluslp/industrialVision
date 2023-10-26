#pragma once
#pragma once
#include "preheader.h"
#include "ShapeMimeData.h"
#include <QGraphicsItem>
#include "GraphicsItem.h"

class PaintView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit PaintView(QWidget* parent = 0);
    ~PaintView();

    void ZoomOut();
    void ZoomIn();
    void SetTranslateSpeed(qreal speed);
private:
    Qt::MouseButton _translateButton;  // ƽ�ư�ť
    qreal  _zoomDelta;  // ���ŵ�����
    qreal _scaleValue;  // ����ֵ
    QPoint  _lastMousePos;  // �������µ�λ��
    qreal _translateSpeed;  // ƽ���ٶ�
    bool _bMouseTranslate;

    void zoom(float scaleFactor);
    void paintEvent(QPaintEvent* event);
    void wheelEvent(QWheelEvent* event);
    void keyPressEvent(QKeyEvent* event);
    void translate(QPointF delta);
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void resizeEvent(QResizeEvent* event);
    void contextMenuEvent(QContextMenuEvent* event);

private slots:
    void onAssemblageAction(bool);
    void onRotation1Action(bool);
    void onRotation2Action(bool);
    void onDeleteAction(bool);
    void on_copy();
    void on_paste();
    void on_cut();


};
