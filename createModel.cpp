#include "createModel.h"
#include "ui_createModel.h"

#define SAFE_DELETE(p) { if (p) { delete (p);     (p) = nullptr; } }

createModel::createModel(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::createModel)
{
    ui->setupUi(this);

    setWindowTitle(tr("模板设置"));
    windowTitleItem = windowTitle();
    QWidget* c = this->takeCentralWidget();
    if (c) delete c;

    undoStack2D = new QUndoStack(this);

    statusBar = new StatusBar(this);
    this->setStatusBar(statusBar);

    //效果预览
     execPatternAction = new ExecPatternAction(this);
    connect(execPatternAction, &ExecPatternAction::triggered,
        this, &createModel::onExecPatternAction);

    //采集图片
     getImageAction = new GetImageAction(this);
    connect(getImageAction, &GetImageAction::triggered,
        this, &createModel::onGetImageTriggered);

    //保存
     saveAction = new SaveAction(this);
    connect(saveAction, &SaveAction::triggered,
        this, &createModel::onSaveTriggered);

	//另保存
     saveAsAction = new SaveAsAction(this);
    connect(saveAsAction, &SaveAsAction::triggered,
        this, &createModel::onSaveAsTriggered);

    //关闭当前文件
     closeFileAction = new CloseFileAction(this);
    connect(closeFileAction, &CloseFileAction::triggered,
        this, &createModel::onCloseFileTriggered);

    //导入模板
     importAction = new ImportAction(this);
    connect(importAction, &ImportAction::triggered,
        this, &createModel::onImportTriggered);

    //矩形
     rectModeAction = new RectModeAction(this);
    connect(rectModeAction, &RectModeAction::triggered,
        this, &createModel::onRectModeTriggered);

    //输出点
	 setPointAction = new SetPointAction(this);
	connect(setPointAction, &SetPointAction::triggered,
		this, &createModel::onCirclePenModeTriggered);

    //椭圆
     ellipseModeAction = new EllipseModeAction(this);
    connect(ellipseModeAction, &EllipseModeAction::triggered,
        this, &createModel::onEllipseModeTriggered);

    //删除操作
     deleteLabelAction = new DeleteLabelAction(this);
    connect(deleteLabelAction, &DeleteLabelAction::triggered,
        [=]() {
            emit labelsDockWidget->deleteCurrLabel(labelsDockWidget->currLabelName());

			paintScene2D->updateShapes(labelController2D, fileController2D->getCurrImageName());

        });
    //多边形
     polygonModeAction = new PolygonModeAction(this);
    connect(polygonModeAction, &PolygonModeAction::triggered,
        this, &createModel::onPolygonModeTriggered);


    //方形掩膜
    //auto squarePenModeAction = new SquarePenModeAction(this);
    //connect(squarePenModeAction, &SquarePenModeAction::triggered,
    //    this, &createModel::onSquarePenModeTriggered);

    //圆形掩膜
    //auto circlePenModeAction = new CirclePenModeAction(this);
    //connect(circlePenModeAction, &CirclePenModeAction::triggered,
    //    this, &createModel::onCirclePenModeTriggered);
    //选择
     curseModeAction = new CurseModeAction(this);
    connect(curseModeAction, &CurseModeAction::triggered,
        this, &createModel::onCurseModeTriggered);

    //搜索区域
	 searchAreaRectActionItem = new searchAreaRectAction(this);
	connect(searchAreaRectActionItem, &searchAreaRectAction::triggered,
		this, &createModel::onSearchAreaRectTriggered);
    
    //特征匹配
	featureMatchingRectActionItem = new featureMatchingRectAction(this);
	connect(featureMatchingRectActionItem, &featureMatchingRectAction::triggered,
		this, &createModel::onFeatureMatchingTriggered);

    //撤销
     undoAction = new UndoAction(this);
    connect(undoAction, &MagnifyAction::triggered,
        this, &createModel::onUndoTriggered, Qt::ConnectionType::DirectConnection);
    
    //帮助
		 helpActionitem = new helpAction(this);
	connect(helpActionitem, &helpAction::triggered,
		this, &createModel::helpActionTriggered, Qt::ConnectionType::DirectConnection);


    QActionGroup* drawingActionGroup = new QActionGroup(this);
    //选择
    drawingActionGroup->addAction(curseModeAction);
    //矩形
   // drawingActionGroup->addAction(rectModeAction);
    //椭圆
    drawingActionGroup->addAction(ellipseModeAction);
    //多边形
    drawingActionGroup->addAction(polygonModeAction);
    //曲线
  //  drawingActionGroup->addAction(curveModeAction);
   // drawingActionGroup->addAction(squarePenModeAction);
   // drawingActionGroup->addAction(circlePenModeAction);

    curseModeAction->setCheckable(true);
    curseModeAction->setChecked(true);
   // circlePenModeAction->setCheckable(true);
    //squarePenModeAction->setCheckable(true);
    rectModeAction->setCheckable(true);
    ellipseModeAction->setCheckable(true);
    //curveModeAction->setCheckable(true);
    polygonModeAction->setCheckable(true);
    //twoDModeAction->setCheckable(true);
   // twoDModeAction->setChecked(true);
  //  magnifyAction->setCheckable(true);

    // set up and connect menu bar
    menuBar = new MenuBar(this);
    QMenu* menus[3];
    menus[0] = new QMenu(tr("文件"), menuBar);
    menus[1] = new QMenu(tr("工具"), menuBar);
    menus[2] = new QMenu(tr("帮助"), menuBar);
    // menus[3] = new QMenu("&Help", menuBar);
    //修改地址
    for (int i = 0; i < 3; i++) {
        menuBar->addMenu(menus[i]);
    }
	menuBar->setStyleSheet("font:14px; color: rgb(255, 255, 255); background-color: rgba(26, 45, 77, 1);");

  //  menus[0]->addAction(openFileAction);
    menus[0]->addSeparator();
	menus[0]->addAction(importAction);
	menus[0]->addSeparator();


    menus[0]->addAction(saveAction);
	menus[0]->addSeparator();

	menus[0]->addAction(saveAsAction);
	menus[0]->addSeparator();

    menus[0]->addAction(closeFileAction);
	menus[0]->addSeparator();

	menus[1]->addSeparator();
    menus[1]->addAction(searchAreaRectActionItem);
	menus[1]->addSeparator();
	menus[1]->addAction(featureMatchingRectActionItem);
    menus[1]->addSeparator();
	menus[1]->addAction(setPointAction);
    menus[1]->addSeparator();
	menus[1]->addAction(curseModeAction);
	menus[1]->addSeparator();
    menus[1]->addAction(undoAction);
    menus[1]->addSeparator();
    menus[1]->addAction(deleteLabelAction);

	menus[2]->addAction(helpActionitem);

    this->setMenuBar(menuBar);

    
    // set up tool bar
    toolBar = new ToolBar(this);
    toolBar->setIconSize(QSize(45, 45));
	toolBar->setStyleSheet("font:bold 20px;color: rgb(255, 255, 255);background-color: rgba(35, 65, 114, 1);");
    setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

	toolBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	toolBar->addSeparator();
	//导入
	toolBar->addAction(importAction);
	toolBar->addSeparator();
	QWidget* test1 = new QWidget();
	test1->setStyleSheet("color: rgb(255, 255, 255); background-color: rgba(35, 65, 114, 1);");
	test1->setFixedSize(QSize(20, 20));

	toolBar->addWidget(test1);
	toolBar->addSeparator();

    //采集图片
    toolBar->addAction(getImageAction);
     //搜索区域
    toolBar->addSeparator();
	toolBar->addAction(searchAreaRectActionItem);
	toolBar->addSeparator();

    //特征匹配
	//创建下拉菜单
	QMenu* menuSelection = new QMenu(this);
	menuSelection->addAction(featureMatchingRectActionItem);
	menuSelection->addAction(polygonModeAction);
	menuSelection->addAction(ellipseModeAction);

	aBtn = new QToolButton(this);
	aBtn->setPopupMode(QToolButton::InstantPopup);
	aBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	//aBtn->setDefaultAction(ui->actSelPopMenu);
	aBtn->setMenu(menuSelection);
	aBtn->setText(tr("特征区域"));
	aBtn->setIcon(QIcon("./Image/Icon/square_pen_mode"));
	toolBar->addWidget(aBtn);
	toolBar->addSeparator();

   //输出点
   toolBar->addAction(setPointAction);
   toolBar->addSeparator();

    //放大镜
	//toolBar->addAction(magnifyAction);
	//toolBar->addSeparator();

    //撤销
	toolBar->addAction(undoAction);
	toolBar->addSeparator();
	toolBar->addAction(deleteLabelAction);
	toolBar->addSeparator();
    //选择
	toolBar->addAction(curseModeAction);
//	toolBar->addSeparator();
    //矩形
//	toolBar->addAction(rectModeAction);
    //椭圆
	toolBar->addSeparator();

    //多边形
	//toolBar->addAction(polygonModeAction);
	toolBar->addSeparator();
    //掩膜
	//toolBar->addAction(circlePenModeAction);
	//toolBar->addAction(squarePenModeAction);
    QWidget* test3 = new QWidget();
    test3->setStyleSheet("color: rgb(255, 255, 255); background-color: rgba(35, 65, 114, 1);");
    test3->setFixedSize(QSize(20, 20));
    
	toolBar->addWidget(test3);
	//test->move(0, 0);

    //toolBar->addAction(nextImageAction);
   // toolBar->addAction(prevImageAction);
    //toolBar->addAction(redoAction);
    toolBar->addSeparator();
    //toolBar->addAction(curveModeAction);
    //toolBar->addAction(openFileAction);
   // toolBar->addAction(openDirAction);
    toolBar->addAction(saveAction);
	toolBar->addSeparator();
    toolBar->addAction(saveAsAction);
	toolBar->addSeparator();
	toolBar->addAction(execPatternAction);
	toolBar->addSeparator();
    toolBar->addAction(closeFileAction);
    toolBar->addSeparator();

  //  toolBar->addAction(twoDModeAction);
    this->addToolBar(Qt::TopToolBarArea, toolBar);
    toolBar->setMovable(false);// 设置工具栏不可移动
	toolBar->setFloatable(false); // 设置工具栏不可浮动
	toolBar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    // Init 2D
    labelTypeDockWidget2D = new LabelTypeDockWidget();
    labelsDockWidget2D = new LabelsDockWidget();
    fileListDockWidget2D = new FileListDockWidget();


    graphicsView2D = new MyGraphicsView();

    paintScene2D = new PaintScene();

    graphicsView2D->setScene(paintScene2D);
    graphicsView2D->fitInView(paintScene2D->sceneRect());
    graphicsView2D->setMouseTracking(true);

    connect(graphicsView2D, &MyGraphicsView::viewResized,
        [&]() {
            paintScene2D->changeCurrImage(fileController2D->getImage(fileController2D->getCurrImageName()));
            paintScene2D->updateShapes(labelController2D, fileController2D->getCurrImageName());
        });
    connect(paintScene2D, &PaintScene::scenePos,
        statusBar, &StatusBar::setScenePos);
    connect(paintScene2D, &PaintScene::mouseTrackingOn,
        [=]() {graphicsView2D->setMouseTracking(true); });
    connect(paintScene2D, &PaintScene::mouseTrackingOff,
        [=]() {graphicsView2D->setMouseTracking(false); });
    connect(paintScene2D, &PaintScene::drawFinished,
        this, &createModel::drawFinishedSlot, Qt::ConnectionType::DirectConnection);

	/*   connect(radiusSlider, &QSlider::valueChanged,
		   paintScene2D, &PaintScene::setRadius);*/
    //设置初始画笔半径
    //radiusSlider->setValue(20);

    fileController2D = new FileController(this);

    connect(fileController2D, &FileController::fileAdded,
        fileListDockWidget2D, &FileListDockWidget::fileAdded);
    connect(fileController2D, &FileController::updateFiles,
        fileListDockWidget2D, &FileListDockWidget::openFileFinished);

    connect(fileController2D, &FileController::modelFilePATH,
        this, &createModel::sendXMLPathToIndustria);

    connect(fileController2D, &FileController::currImageChanged,
        [&](const QString& imageName) {
            paintScene2D->changeCurrImage(fileController2D->getImage(imageName));
            paintScene2D->updateShapes(labelController2D, imageName);
        });
    connect(fileController2D, &FileController::currImageChanged,
        [&](const QString& imageName) {
            labelsDockWidget2D->setLabelList(labelController2D->getLabelsOfImage(imageName));
        });

    connect(fileController2D, &FileController::fileRemoved,
        fileListDockWidget2D, &FileListDockWidget::fileClosed);

    labelController2D = new LabelController(this);

    connect(labelController2D, &LabelController::labelCreated,
        labelsDockWidget2D, &LabelsDockWidget::labelCreated);
    connect(labelController2D, &LabelController::labelCreated,
        [&]() {  paintScene2D->updateShapes(labelController2D, fileController2D->getCurrImageName());  });
    connect(labelController2D, &LabelController::labelDeleted,
        labelsDockWidget2D, &LabelsDockWidget::labelDeleted);
    connect(labelController2D, &LabelController::typeCreated,
        labelTypeDockWidget2D, &LabelTypeDockWidget::typeCreated);
    connect(labelController2D, &LabelController::typeDeleted,
        labelTypeDockWidget2D, &LabelTypeDockWidget::typeDeleted);
    connect(labelsDockWidget2D, &LabelsDockWidget::deleteCurrLabel,
        [&]() {
            QString currLabelName = labelsDockWidget2D->currLabelName();
            if (currLabelName == "")
                return;
            QStringList l = currLabelName.split("_");
            int id = l.last().toInt();
            l.pop_back();
            QString type = l.join("");
            DeleteCurrLabelCommand* deleteCurrLabelCommand = new DeleteCurrLabelCommand(labelController2D, type, id);
            undoStack2D->push(deleteCurrLabelCommand);
        });


    connect(paintScene2D, &PaintScene::itemSelected,
        [&](QGraphicsItem* item) {
            Label* label = labelController2D->findParentLabelOfItem(item);
            labelsDockWidget2D->labelSelected(label->getType() + "_" + QString::number(label->getId()));
        });

    connect(fileListDockWidget2D, &FileListDockWidget::itemClicked,
        [&](const QString& imageName) {
            SwitchImageCommand* switchImageCommand = new SwitchImageCommand(fileController2D->getCurrImageName(), imageName, fileController2D);
            undoStack2D->push(switchImageCommand);
        });

    connect(fileController2D, &FileController::currImageChanged,
        fileListDockWidget2D, &FileListDockWidget::selectItem);



    drawFinishedDialog2D = new DrawFinishedDialog(this);

    connect(drawFinishedDialog2D, &DrawFinishedDialog::sendTypeName,
        [&](QString& typeName) {
            QUndoCommand* createLabel = new CreateLabelCommand(typeName, areaController2D->takeArea(), labelController2D);
            undoStack2D->push(createLabel);
        });


    areaController2D = new AreaController(this);

    labelTypeDockWidget3D = new LabelTypeDockWidget();

    labelsDockWidget3D = new LabelsDockWidget();

    fileListDockWidget3D = new FileListDockWidget();

    centralWidget3D = new QWidget();
    graphicsViewMain = new MyGraphicsView(centralWidget3D);
    paintSceneMain = new PaintScene(centralWidget3D);



    graphicsViewMain->setScene(paintSceneMain);
    graphicsViewMain->fitInView(paintSceneMain->sceneRect());
    graphicsViewMain->setMouseTracking(true);


    graphicsViewLeft = new MyGraphicsView(centralWidget3D);
    paintSceneLeft = new PaintScene(centralWidget3D);

    graphicsViewLeft->setScene(paintSceneLeft);
    graphicsViewLeft->fitInView(paintSceneLeft->sceneRect());


    graphicsViewVert = new MyGraphicsView(centralWidget3D);
    paintSceneVert = new PaintScene(centralWidget3D);

    graphicsViewVert->setScene(paintSceneVert);
    graphicsViewVert->fitInView(paintSceneVert->sceneRect());



    graphicsViewMain->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    graphicsViewLeft->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    graphicsViewVert->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);



    drawFinishedButton = new QPushButton("Create Label");
    connect(drawFinishedButton, &QPushButton::clicked,
        this, &createModel::onDrawFinishedButtonClicked);



    //Layout
    QHBoxLayout* hl1 = new QHBoxLayout();
    hl1->setSpacing(10);
    hl1->setMargin(0);
    hl1->addWidget(graphicsViewMain, 2);
    hl1->addWidget(graphicsViewLeft, 1);
    QHBoxLayout* hl2 = new QHBoxLayout();
    hl2->setSpacing(10);
    hl2->setMargin(0);
    hl2->addWidget(graphicsViewVert, 2);
    hl2->addWidget(drawFinishedButton, 1);
    QVBoxLayout* vl = new QVBoxLayout();
    vl->addLayout(hl1, 2);
    vl->addLayout(hl2, 1);
    vl->setSpacing(10);
    vl->setMargin(0);


    connect(paintSceneMain, &PaintScene::scenePos,
        statusBar, &StatusBar::setScenePos);

    connect(paintSceneMain, &PaintScene::mouseTrackingOn,
        [=]() {graphicsViewMain->setMouseTracking(true); });
    connect(paintSceneMain, &PaintScene::mouseTrackingOff,
        [=]() {graphicsViewMain->setMouseTracking(false); });

    connect(paintSceneMain, &PaintScene::drawFinished,
        this, &createModel::drawFinishedSlot, Qt::ConnectionType::DirectConnection);


    //connect(radiusSlider, &QSlider::valueChanged,
    //    paintSceneMain, &PaintScene::setRadius);

    magnifying2D = new MagnifyingGlass();

    connect(paintScene2D, &PaintScene::scenePos,
        magnifying2D, &MagnifyingGlass::setDisplayPos);
    magnifying2D->setPaintScene(paintScene2D);
    


   // addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, labelTypeDockWidget2D);
    addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, labelsDockWidget2D);
  //  addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, fileListDockWidget2D);
    on2DModeTriggered();

	//白色填充左上角图标
	QPixmap pixmap(100, 100);
	pixmap.fill(Qt::white);
	setWindowIcon(QIcon(pixmap));
}

createModel::~createModel()
{
    delete ui;
    magnifying2D->close();
    SAFE_DELETE(magnifying2D);
    SAFE_DELETE(graphicsView2D)
    SAFE_DELETE(paintScene2D);
}

void createModel::closeEvent(QCloseEvent* event)
{
}


void createModel::keyPressEvent(QKeyEvent* event)
{
	switch (event->key()) {
		// 处理 Backspace 或 Delete 键的按下事件

	case Qt::Key_Backspace:
    {
		undoStack->undo();
		break;

    }
	case Qt::Key_Delete:
    {
		emit labelsDockWidget->deleteCurrLabel(labelsDockWidget->currLabelName());
		paintScene2D->updateShapes(labelController2D, fileController2D->getCurrImageName());
		break;
    }
    default:
		// 调用基类的处理函数处理其他键盘按键
		QMainWindow::keyPressEvent(event);
		break;
	}
}


void createModel::onOpenFileTriggered() {
    fileController->openFile(labelController);
}

void createModel::onOpenDirTriggered() {
    fileController->openDir(labelController);
}

void createModel::onSaveAsTriggered() {
    fileController->saveAsFile(labelController);
}

void createModel::onCloseFileTriggered() {
    //判断是否关闭
	QMessageBox::StandardButton reply;
	reply = QMessageBox::question(this, tr("确认关闭"), tr("确定要关闭文件吗?"), QMessageBox::Yes | QMessageBox::No);

	if (reply == QMessageBox::Yes) {
		// 用户选择了"是"
		CloseImageCommand* closeImageCommand = new CloseImageCommand(fileController);
		undoStack->push(closeImageCommand);
		labelController->deleteAllLabel();
	}
	else {
		// 用户选择了"否"或者关闭对话框
	}
}

void createModel::onNextImageTriggered() {
    fileListDockWidget->nextFile();
}

void createModel::onPrevImageTriggered() {
    fileListDockWidget->prevFile();
}

void createModel::onSetPointTriggered() {
	paintScene->setCurrentShape(Shape::Figure::Empty);
}

void createModel::onRectModeTriggered() {
    paintScene->setCurrentShape(Shape::Figure::Rect);
}

void createModel::onEllipseModeTriggered() {
    paintScene->setCurrentShape(Shape::Figure::Ellipse);
}

void createModel::onPolygonModeTriggered() {
    paintScene->setCurrentShape(Shape::Figure::Polygon);
}

void createModel::onCurveModeTriggered() {
    paintScene->setCurrentShape(Shape::Figure::Curve);
}






void createModel::onSquarePenModeTriggered() {
    paintScene->setCurrentShape(Shape::Figure::SquarePen);
}

void createModel::onCirclePenModeTriggered() {
    paintScene->setCurrentShape(Shape::Figure::CirclePen);
}




void createModel::onCurseModeTriggered()
{
    paintScene->setCurrentShape(Shape::Figure::Empty);
}



void createModel::on2DModeTriggered()
{
    if (currMode == twoD) {
        return;
    }
    labelController = labelController2D;
    areaController = areaController2D;
    fileController = fileController2D;
    paintScene = paintScene2D;
    drawFinishedDialog = drawFinishedDialog2D;
    undoStack = undoStack2D;


    labelsDockWidget2D->show();
    labelsDockWidget2D->setStyleSheet("color: rgb(255, 255, 255); background-color: rgba(26, 45, 77, 1);");
   // labelTypeDockWidget2D->show();
    fileListDockWidget2D->setStyleSheet("color: rgb(255, 255, 255); background-color: rgba(26, 45, 77, 1);");

   // fileListDockWidget2D->show();
   // labelTypeDockWidget = labelTypeDockWidget2D;
    labelsDockWidget = labelsDockWidget2D;
    fileListDockWidget = fileListDockWidget2D;

    takeCentralWidget();
    setCentralWidget(graphicsView2D);
    currMode = twoD;
}


void createModel::drawFinishedSlot(Shape* shape) {
    if (!fileController->hasCurrImage())
        return;

	Shape* copyedShape = shape->clone();
    if (sender() == paintScene2D) {
        areaController2D->receiveImageShape(fileController->getCurrImageName(), copyedShape);

      //  drawFinishedDialog->setList(labelTypeDockWidget->getItemList());
		//drawFinishedDialog->setShapeFlag(paintScene->currShapeType);
        drawFinishedDialog->emitcurrentShapeType(paintScene->currShapeType);
        //drawFinishedDialog->exec();

    }
    else {
        return;
    }
}

void createModel::onDrawFinishedButtonClicked()
{
 
     //   drawFinishedDialog->setList(labelTypeDockWidget->getItemList());
      
        drawFinishedDialog->exec();
}

void createModel::onMagnifyTriggered()
{
    if (currMode == threeD) {
        magnifying = magnifying3D;
    }
    if (currMode == twoD) {
        magnifying = magnifying2D;
    }
    if (magnifying->isShowing())
        magnifying->close();
    else
        magnifying->show();
}


//从相机获得图片
void createModel::onGetImageTriggered()
{
    emit getImageFromCamera();
	setWindowTitle(windowTitleItem);
}

void createModel::sendImgToFileController(QImage image,QString modePath)
{
    if (image.isNull())
    {
		QMessageBox::warning(nullptr, tr("Path"),
			tr("相机状态异常,未获取到图片"));
        return;
    }
	fileController->getImageFromCamera(image);

   // fileController->getImageFromCamera(image,modePath);
}

void createModel::onExecPatternAction()
{
    //执行模板匹配
    fileController->onExecPattern(labelController);
}

void createModel::sendXMLPathToIndustria(QString path)
{
    emit sendXMLPath(path);
}

void createModel::onSearchAreaRectTriggered()
{
	paintScene->setCurrentShape(Shape::Figure::searchAreaRect);

}

void createModel::onFeatureMatchingTriggered()
{
	paintScene->setCurrentShape(Shape::Figure::featureMatchingRect);

}


void createModel::onImportTriggered()
{

    fileController->importFromFile(labelController);
    QString item = fileController->returnImportFilepath();
    QString title(windowTitleItem);
    title.append("[" + item + "]");
    setWindowTitle(title);
}





void createModel::onSaveTriggered()
{
    fileController->saveFile(labelController);
}


void createModel::onUndoTriggered()
{
    undoStack->undo();
}

void createModel::onRedoTriggered()
{
    undoStack->redo();
}
void createModel::helpActionTriggered() {

}

void createModel::setcreateModelItem_language(QString language)
{
    drawFinishedDialog->language = language;
	if (language=="zh")
	{
		//效果预览
		execPatternAction->setText("效果预览");
		//采集图片
		getImageAction->setText("采集图片");
		//保存
		saveAction->setText("保存");
		//另存为
		saveAsAction->setText("另存为");
		//关闭当前文件
		 closeFileAction->setText("关闭当前文件");
		//导入模板
		importAction->setText("导入模板");
		//矩形
		rectModeAction->setText("矩形");
		//输出点
		setPointAction->setText("输出点");
		//椭圆
		ellipseModeAction->setText("椭圆");
		//删除操作
		deleteLabelAction->setText("删除操作");
		//多边形
		polygonModeAction->setText("多边形");
		//选择
		curseModeAction->setText("选择");
		//搜索区域
		searchAreaRectActionItem->setText("搜索区域");
		//特征区域
		featureMatchingRectActionItem->setText("特征区域");
		//撤销
		undoAction->setText("撤销");
		//帮助
		helpActionitem->setText("帮助");

        aBtn->setText("特征区域");
        labelsDockWidget->setWindowTitle("已选操作");
	}else if (language == "en")
	{
		//效果预览
		execPatternAction->setText("Effect preview");
		//采集图片
		getImageAction->setText("Collecting");
		//保存
		saveAction->setText("Save");
		//另存为
		saveAsAction->setText("SaveAs");
		//关闭当前文件
		closeFileAction->setText("CloseFile");
		//导入模板
		importAction->setText("Import");
		//矩形
		rectModeAction->setText("Rectangle");
		//输出点
		setPointAction->setText("Output point");
		//椭圆
		ellipseModeAction->setText("Ellipse");
		//删除操作
		deleteLabelAction->setText("Delete");
		//多边形
		polygonModeAction->setText("Polygon");
		//选择
		curseModeAction->setText("Choice");
		//搜索区域
		searchAreaRectActionItem->setText("SearchArea");
		//特征区域
		featureMatchingRectActionItem->setText("Rectangle");
		aBtn->setText("CharacteristicArea");
		//撤销
		undoAction->setText("Revoke");
		//帮助
		helpActionitem->setText("Help");
        //已选操作
		labelsDockWidget->setWindowTitle("SelectedAction");

	}else if (language == "es")
	{
		//效果预览
		execPatternAction->setText("Vista previa del efecto");
		//采集图片
		getImageAction->setText("Recogida");
		//保存
		saveAction->setText("Salvar");
		//另存为
		saveAsAction->setText("SaveAs");
		//关闭当前文件
		closeFileAction->setText("Guardar como");
		//导入模板
		importAction->setText("Importación");
		//矩形
		rectModeAction->setText("Rectangular");
		//输出点
		setPointAction->setText("Punto de salida");
		//椭圆
		ellipseModeAction->setText("Elíptico");
		//删除操作
		deleteLabelAction->setText("Eliminar");
		//多边形
		polygonModeAction->setText("Polígono");
		//选择
		curseModeAction->setText("Selección");
		//搜索区域
		searchAreaRectActionItem->setText("Área de búsqueda");
		//特征区域
		featureMatchingRectActionItem->setText("Rectángulos");
		aBtn->setText("Zona característica");
		//撤销
		undoAction->setText("Revocación");
		//帮助
		helpActionitem->setText("Ayudar");
		//已选操作
		labelsDockWidget->setWindowTitle("Seleccionada");
	}
}

