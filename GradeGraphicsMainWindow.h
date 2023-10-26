#pragma once
#include "preheader.h"
#include "paintView.h"
#include "MainPaintScene.h"
#include "ui_GradeGraphicsMainWindow.h"

namespace Ui {
    class GradeGraphicsMainWindow;
}

class GradeGraphicsMainWindow : public QWidget
{
    Q_OBJECT

public:
  //  explicit GradeGraphicsMainWindow(QWidget* parent = 0);
     GradeGraphicsMainWindow(QWidget* parent,QImage image);

    void initBackPicture(QImage image);



    ~GradeGraphicsMainWindow();

private slots:

    void on_pushButton_linePattern_click();

    void on_pushButton_circlePattern_click();

    void on_pushButton_garyPattern_click();

    void on_pushButton_patternRange_click();

    void on_pushButton_select_click();

    void on_pushButton_mask_click();


signals:
    void changeCurrentShape(DrawBaseShape shape);
    void updateUserItemList(DrawBaseShape shape);


public slots:
    void updateUseredItemMainWindow(DrawBaseShape shape);


private:
    Ui::GradeGraphicsMainWindow* ui;
public:
    MainPaintScene* _mainPaintScene;
    void resizeEvent(QResizeEvent* event);
    bool checkGetImage = false;
};

