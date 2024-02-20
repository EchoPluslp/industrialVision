#ifndef NCCMAINWINDOW_H
#define NCCMAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QtWidgets>

#include "myqgraphicsview.h"
#include "bee_rect.h"
#include "bee_circle.h"
#include "bee_concencircle.h"
#include "bee_polygon.h"
#include "bee_point.h"
//#include <opencv2/opencv.hpp>
#include <iostream>
#include "bee_caliberline.h"
#include "bee_calibercircle.h"
#include "bee_ringexpansion.h"
#include <QMouseEvent>
#include <QTextCodec>
#pragma execution_character_set("UTF-8")


QT_BEGIN_NAMESPACE
namespace Ui { class ncc_MainWindow; }
QT_END_NAMESPACE

class NCCMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	NCCMainWindow(QWidget* parent = nullptr);
	~NCCMainWindow();
	enum STATE_FLAG_MAINWINDOW {
		CHOOSE_NULL,
		CHOOSE_PICTURE,
		CHOOSE_RECT,
		CHOOSE_SOURCE_RECT,
		CHOOSE_CIRCLE,
		CHOOSE_CONCENCIRCLE,
		CHOOSE_POLYGON,
		CALIBERLINE,
		CALIBERCIRCLE,
		RINGEXPANSION
	};

public slots:
	void on_action_choosepicture_triggered();

	void on_action_rect_triggered();

	void on_action_rotaterect_triggered();

	void on_action_polygon_triggered();

	void on_action_circle_triggered();

	void on_action_concircle_triggered();

	//���浱ǰ����
	void on_action_ringexpansion_triggered();


	void on_action_caliberline_triggered();

	void on_action_calibercircle_triggered();


	void on_action_zoomin_triggered();

	void on_action_zoomout_triggered();

	void on_action_fitwin_triggered();

	void on_action_1_to_1_triggered();

	void sendImgToControllerShape(QImage image,QString ModelPath);
	void slot_receiveDrawPoint(QPoint resultPoint, int totalModelTime);

	void createRECT(int type, int index);

	
signals:
	void getImageFromCamera(QString str);
	void sendImageToPattern(QImage sourceImage, QImage patternImage);
	void sendINIPath(QString iniPath);
protected:
	virtual void keyPressEvent(QKeyEvent* event);

public:
	Ui::ncc_MainWindow* ui;
	bee_circle* source_circle_info;
	bee_concencircle* my_concencircle;
	bee_caliberline* my_caliberline;
	bee_calibercircle* my_calibercircle;
	bee_ringexpansion* my_ringexpansion;
	QGraphicsScene* qgraphicsScene;
	NCCMainWindow::STATE_FLAG_MAINWINDOW state_flag_maindow;
	QGraphicsPixmapItem* ImageItem;

	QString fileName;

	//roi���id��

	//roi�б�
	//����������ο�
	bee_rect* source_rect_info = nullptr;
	int index_source = 0;
	QList<QGraphicsItem*>* source_rect_List;

	//����������ο�
	bee_rect* ncc_patten_rect_info = nullptr;
	int index_pattern = 0;
//	QList<QGraphicsItem*>* pattern_rect_List;


	//����������
	bee_polygon* source_polygon_info = nullptr;
	int index_polygon = 0;
	//�����
	bee_point* source_point_info = nullptr;
	int index_point;

	//������ѡ����,����ʱ�����Ӧ����
	int getListItem(QString name);
};
#endif // MAINWINDOW_H
