#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QtWidgets>

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

QT_BEGIN_NAMESPACE
namespace Ui { class ncc_MainWindow; }
QT_END_NAMESPACE


enum STATE_FLAG_MAINWINDOW {
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

class NCCMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	NCCMainWindow(QWidget* parent = nullptr);
	~NCCMainWindow();

private slots:
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

protected:
	virtual void keyPressEvent(QKeyEvent* event);

private:
	Ui::ncc_MainWindow* ui;
	bee_rect* my_rect;
	bee_circle* my_circle;
	bee_polygon* my_polygon;
	bee_concencircle* my_concencircle;
	bee_caliberline* my_caliberline;
	bee_calibercircle* my_calibercircle;
	bee_ringexpansion* my_ringexpansion;
	QGraphicsScene* qgraphicsScene;
	STATE_FLAG_MAINWINDOW state_flag_maindow;
	QGraphicsPixmapItem* ImageItem;

	QString fileName;
};
#endif // MAINWINDOW_H
