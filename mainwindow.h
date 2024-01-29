#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QtWidgets>
#include "common.h"
#include "myqgraphicsview.h"
#include "bee_rect.h"
#include "bee_circle.h"
#include "bee_concencircle.h"
#include "bee_polygon.h"
//#include <opencv2/opencv.hpp>
#include <iostream>
#include "bee_caliberline.h"
#include "bee_calibercircle.h"
#include "bee_ringexpansion.h"
#include <QMouseEvent>
#include "controlline.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum STATE_FLAG_MAINWINDOW{
    CHOOSE_NULL,
    CHOOSE_PICTURE,
    CHOOSE_RECT,
    CHOOSE_CIRCLE,
    CHOOSE_CONCENCIRCLE,
    CHOOSE_POLYGON,
    CALIBERLINE,
    CALIBERCIRCLE,
    RINGEXPANSION
};



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

	// 计算直线的斜率和截距
    Line calculateLine(const cv::Point& p1, const cv::Point& p2);
	void InitializeMeasureTrackbar();
    void findIntersection(cv::Point p_1, cv::Point p_2, cv::Point p_3, cv::Point p_4,cv::Point2f& intersection);
    double findangle(cv::Point p_1, cv::Point p_2,cv::Point p_3,cv::Point p_4);

    ~MainWindow();

public  slots:
    void on_action_choosepicture_triggered();

    void on_action_rect_triggered();

    void on_action_rotaterect_triggered();

    void on_action_polygon_triggered();

    void on_action_circle_triggered();

    void on_action_concircle_triggered();

    void on_action_ringexpansion_triggered();


    void on_action_caliberline_triggered();

    void on_action_calibercircle_triggered();


    void on_action_zoomin_triggered();

    void on_action_zoomout_triggered();

    void on_action_fitwin_triggered();

    void on_action_1_to_1_triggered();

    void sendImgToControllerShape(QImage image);

    void fitcircle();

    void fitline();

    void slot_ShowLine_Param(int nMeasureNums, int nthresholdValue, int nSampleDirection,int currentindex_line);

    void slot_index_nMeasureNums(int value);

    void slot_index_nSetThreshold(int value);

    void slot_index_SampleDirection(int value);

    void saveInfo();
 
	void fitline_with_signal(QPixmap image);

protected:
   virtual void keyPressEvent(QKeyEvent *event);

 signals:
    void getImageFromCamera(QString matchType);
private:
    Ui::MainWindow *ui;
    bee_rect *my_rect;
    bee_circle *my_circle;
    bee_polygon *my_polygon;
    bee_concencircle *my_concencircle;
    //bee_caliberline *my_caliberline;
    QList<bee_caliberline*> my_caliberline_List;
	QList<bee_calibercircle*> my_calibercircle_List;

    bee_calibercircle *my_calibercircle                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       ;
    bee_ringexpansion *my_ringexpansion;
    QGraphicsScene *qgraphicsScene;
    STATE_FLAG_MAINWINDOW state_flag_maindow;
    QGraphicsPixmapItem *ImageItem;

	CLineCaliperGUI* m_plineCaliperGUI;


    QList<CControlLine*> CControlLine_List;
    QString fileName;
    int nSigma = 1;
    int currentIndexs_line = 0;
	CControlLine* lineitem;
    QList<cv::Point> resultLinePoint;
};
#endif // MAINWINDOW_H
