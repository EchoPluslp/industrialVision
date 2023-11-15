#ifndef CREATEMODEL_H_
#define CREATEMODEL_H_
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")// 该指令仅支持VS环境
#endif

#include <QDockWidget>
#include <QMainWindow>
#include <QtDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QImage>
#include <QList>
#include <QImageReader>

#include "menubar.h"
#include "toolbar.h"
#include "myactions.h"
#include "paintscene.h"
#include "statusbar.h"
#include "shape.h"
#include "filelistdockwidget.h"
#include "labelsdockwidget.h"
#include "labeltypedockwidget.h"
#include "filecontroller.h"
#include "labelcontroller.h"
#include "drawfinisheddialog.h"
#include "areacontroller.h"
#include "graphicsview.h"
#include "mygraphicsview.h"
#include "magnifyingglass.h"
#include "mycommand.h"
#include "ui_createModel.h"

namespace Ui {
	class createModel;
}

class createModel : public QMainWindow
{
	Q_OBJECT

public:
	explicit createModel(QWidget* parent = nullptr);
	~createModel();
	void closeEvent(QCloseEvent* event);
public slots:
	void onGetImageTriggered();
	void onOpenFileTriggered();
	void onOpenDirTriggered();
	void onSaveAsTriggered();
	void onCloseFileTriggered();
	void onNextImageTriggered();
	void onPrevImageTriggered();
	void onRectModeTriggered();
	void onEllipseModeTriggered();
	void onPolygonModeTriggered();
	void onCurveModeTriggered();
	void onSquarePenModeTriggered();
	void onCirclePenModeTriggered();
	void onCurseModeTriggered();
	void on2DModeTriggered();
	void drawFinishedSlot(Shape* shape);
	void onDrawFinishedButtonClicked();
	void onMagnifyTriggered();
	void onImportTriggered();
	void onSaveTriggered();
	void onSetPointTriggered();
	void onUndoTriggered();
	void onRedoTriggered();
	void sendImgToFileController(QImage image,QString path);
	void onExecPatternAction();
	void sendXMLPathToIndustria(QString xmlPath);
	void onSearchAreaRectTriggered();

	void onFeatureMatchingTriggered();

public:
	Ui::createModel* ui;

	MenuBar* menuBar;
	ToolBar* toolBar;
	StatusBar* statusBar;
	 QString windowTitleItem;

	DrawFinishedDialog* drawFinishedDialog;
	DrawFinishedDialog* drawFinishedDialog2D;

	AreaController* areaController;
	AreaController* areaController2D;

	FileController* fileController;
	LabelController* labelController;

	FileController* fileController2D;
	LabelController* labelController2D;


	PaintScene* paintScene;
	MyGraphicsView* graphicsView;
	FileListDockWidget* fileListDockWidget;
	LabelsDockWidget* labelsDockWidget;
	LabelTypeDockWidget* labelTypeDockWidget;

	PaintScene* paintScene2D;
	MyGraphicsView* graphicsView2D;
	FileListDockWidget* fileListDockWidget2D;
	LabelsDockWidget* labelsDockWidget2D;
	LabelTypeDockWidget* labelTypeDockWidget2D;

	PaintScene* paintSceneMain;
	MyGraphicsView* graphicsViewMain;
	PaintScene* paintSceneLeft;
	MyGraphicsView* graphicsViewLeft;
	PaintScene* paintSceneVert;
	MyGraphicsView* graphicsViewVert;

	QWidget* centralWidget3D;
	QPushButton* drawFinishedButton;

	FileListDockWidget* fileListDockWidget3D;
	LabelsDockWidget* labelsDockWidget3D;
	LabelTypeDockWidget* labelTypeDockWidget3D;

	MagnifyingGlass* magnifying, * magnifying2D, * magnifying3D;

	QUndoStack* undoStack;
	QUndoStack* undoStack2D;

	enum Mode {
		NoMode,
		threeD,
		twoD
	};
	Mode currMode = NoMode;
signals:
	void getImageFromCamera();
	void sendXMLPath(QString xmlPath);
};

#endif // MAINWINDOW_H
