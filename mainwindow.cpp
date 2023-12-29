#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,state_flag_maindow(CHOOSE_NULL)
{

    ui->setupUi(this);
	InitializeMeasureTrackbar();

    this->ImageItem = nullptr;

    qgraphicsScene = new QGraphicsScene();
    qgraphicsScene->setBackgroundBrush(Qt::white);
    qgraphicsScene->setItemIndexMethod(QGraphicsScene::NoIndex);

    ui->graphicsView->setContextMenuPolicy(Qt::CustomContextMenu);

    ui->graphicsView->setScene(qgraphicsScene);
      ui->toolBar->setMovable(false);
      ui->toolBar->setIconSize(QSize(60,60));
      ui->toolBar->setFloatable(false); // 设置工具栏不可浮动
      ui->toolBar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

void MainWindow::InitializeMeasureTrackbar()
{
    //qt 滑动条初始化
	//   //宽度
    ui->slider_nMeasureLength->setMaximum(500);
	ui->slider_nMeasureLength->setMinimum(1);
    ui->slider_nMeasureLength->setTickPosition(QSlider::TicksRight);

    //卡尺数
    ui->slider_nMeasureNums->setValue(10);
    ui->slider_nMeasureNums->setMaximum(99);
	ui->slider_nMeasureNums->setMinimum(3);
	ui->slider_nMeasureNums->setTickPosition(QSlider::TicksLeft);

    //宽度 
    //ui->splider_nMeasureHeight
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::updateLabelValue(int value)
{
	QString text = QString::number(value); // 将整数值转换为字符串
   // 更新标签的文本
    ui->value_nMeasureLength->setText(text);

}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Delete)
    {
        switch(state_flag_maindow)
        {
        case CHOOSE_RECT:
        {
            qgraphicsScene->removeItem(my_rect);
            delete my_rect;
            state_flag_maindow = CHOOSE_PICTURE;

            break;
        }
        case CHOOSE_CIRCLE:
        {
            qgraphicsScene->removeItem(my_circle);
            delete my_circle;
            state_flag_maindow = CHOOSE_PICTURE;
            break;
        }
        case CHOOSE_CONCENCIRCLE:
        {
            qgraphicsScene->removeItem(my_concencircle);
            delete my_concencircle;
            state_flag_maindow = CHOOSE_PICTURE;
            break;
        }case CHOOSE_POLYGON:
        {
            qgraphicsScene->removeItem(my_polygon);
            delete my_polygon;
            state_flag_maindow = CHOOSE_PICTURE;
            break;
        }
        case CALIBERLINE:
        {
            qgraphicsScene->removeItem(my_caliberline);
            delete my_caliberline;
            state_flag_maindow = CHOOSE_PICTURE;
            break;
        }
        case CALIBERCIRCLE:
        {
            qgraphicsScene->removeItem(my_calibercircle);
            delete my_calibercircle;
            state_flag_maindow = CHOOSE_PICTURE;
            break;
        }
        case RINGEXPANSION:
        {
            qgraphicsScene->removeItem(my_ringexpansion);
            delete my_ringexpansion;
            state_flag_maindow = CHOOSE_PICTURE;
            break;
        }
        }
    }

    qgraphicsScene->update();

}


void MainWindow::on_action_choosepicture_triggered()
{
    emit getImageFromCamera("shapeMatch");
    //todo 获取图片
 /*   fileName = QFileDialog::getOpenFileName(this,QStringLiteral("选择图片"),"F:",QStringLiteral("图片文件(*png *jpg *bmp)"));
    if(fileName == nullptr)
    {
        return;
    }
    qgraphicsScene->clear();
    QImage image = QImage(fileName);
    qgraphicsScene->setSceneRect(0, 0, image.width(), image.height());
    ImageItem = qgraphicsScene->addPixmap(QPixmap::fromImage(image));
    qgraphicsScene->update();
    QRectF bounds = qgraphicsScene->itemsBoundingRect();
    bounds.setWidth(bounds.width());
    bounds.setHeight(bounds.height());
    ui->graphicsView->fitInView(bounds, Qt::KeepAspectRatio);
    state_flag_maindow = CHOOSE_PICTURE;*/
}

void MainWindow::on_action_rect_triggered()
{
  //  if(state_flag_maindow != CHOOSE_PICTURE)
  //      return;
  //  else
   // {
        state_flag_maindow = CHOOSE_RECT;
        my_rect = new bee_rect(nullptr,false);
        my_rect->setpixmapwidth(ImageItem->pixmap().width()*ImageItem->scale());
        my_rect->setpixmapheight(ImageItem->pixmap().height()*ImageItem->scale());
        my_rect->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
        this->qgraphicsScene->addItem(my_rect);
        ui->graphicsView->setScene(this->qgraphicsScene);
        qDebug() << "rect";
        //将现在的图片大小传给去
        
   // }
}

void MainWindow::on_action_rotaterect_triggered()
{

        state_flag_maindow = CHOOSE_RECT;
        my_rect = new bee_rect(nullptr,true);
        my_rect->setpixmapwidth(ImageItem->pixmap().width()*ImageItem->scale());
        my_rect->setpixmapheight(ImageItem->pixmap().height()*ImageItem->scale());
        my_rect->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
        this->qgraphicsScene->addItem(my_rect);
        ui->graphicsView->setScene(this->qgraphicsScene);

}

void MainWindow::on_action_polygon_triggered()
{

        state_flag_maindow = CHOOSE_POLYGON;
        my_polygon = new bee_polygon();
        my_polygon->setpixmapwidth(ImageItem->pixmap().width()*ImageItem->scale());
        my_polygon->setpixmapheight(ImageItem->pixmap().height()*ImageItem->scale());
        this->qgraphicsScene->addItem(my_polygon);
        ui->graphicsView->setScene(this->qgraphicsScene);

}

void MainWindow::on_action_circle_triggered()
{

        state_flag_maindow = CHOOSE_CIRCLE;
        my_circle = new bee_circle();
        my_circle->setpixmapwidth(ImageItem->pixmap().width()*ImageItem->scale()); //设置图片的长宽为绘制时的区域长宽
        my_circle->setpixmapheight(ImageItem->pixmap().height()*ImageItem->scale());
        this->qgraphicsScene->addItem(my_circle);
        ui->graphicsView->setScene(this->qgraphicsScene);
        qDebug() << "circle";

}

void MainWindow::on_action_concircle_triggered()
{

        state_flag_maindow = CHOOSE_CONCENCIRCLE;
        my_concencircle = new bee_concencircle();
        my_concencircle->setpixmapwidth(ImageItem->pixmap().width()*ImageItem->scale());
        my_concencircle->setpixmapheight(ImageItem->pixmap().height()*ImageItem->scale());
        this->qgraphicsScene->addItem(my_concencircle);
        ui->graphicsView->setScene(this->qgraphicsScene);

}

void MainWindow::on_action_ringexpansion_triggered()
{

        state_flag_maindow = RINGEXPANSION;
        my_ringexpansion = new bee_ringexpansion();
        my_ringexpansion->setpixmapwidth(ImageItem->pixmap().width()*ImageItem->scale());
        my_ringexpansion->setpixmapheight(ImageItem->pixmap().height()*ImageItem->scale());
        this->qgraphicsScene->addItem(my_ringexpansion);
        ui->graphicsView->setScene(this->qgraphicsScene);

}


void MainWindow::on_action_caliberline_triggered()
{

        state_flag_maindow = CALIBERLINE;
        my_caliberline = new bee_caliberline();
        my_caliberline->setpixmapwidth(ImageItem->pixmap().width()*ImageItem->scale());
        my_caliberline->setpixmapheight(ImageItem->pixmap().height()*ImageItem->scale());
        this->qgraphicsScene->addItem(my_caliberline);
        ui->graphicsView->setScene(this->qgraphicsScene);

}

void MainWindow::on_action_calibercircle_triggered()
{
        state_flag_maindow = CALIBERCIRCLE;
        my_calibercircle = new bee_calibercircle();
        my_calibercircle->setpixmapwidth(ImageItem->pixmap().width()*ImageItem->scale());
        my_calibercircle->setpixmapheight(ImageItem->pixmap().height()*ImageItem->scale());
		Mat frame = QImageToCvMat(ImageItem->pixmap().toImage(), true);

        my_calibercircle->setpixmapImage(frame);

        this->qgraphicsScene->addItem(my_calibercircle);
        ui->graphicsView->setScene(this->qgraphicsScene);
		connect(ui->slider_nMeasureLength, &QSlider::valueChanged, my_calibercircle, &bee_calibercircle::slotSliderValueChanged_MeasureLength);
		connect(ui->slider_nMeasureNums, &QSlider::valueChanged, my_calibercircle, &bee_calibercircle::slotSliderValueChanged_MeasureNums);

}

void MainWindow::on_action_zoomin_triggered()
{
    ui->graphicsView->zoomIn();
}

void MainWindow::on_action_zoomout_triggered()
{

     ui->graphicsView->zoomOut();
}

void MainWindow::on_action_fitwin_triggered()
{
    if(!this->ImageItem)
        return;

    ui->graphicsView->fitInView(this->ImageItem,  Qt::KeepAspectRatio);
}

void MainWindow::on_action_1_to_1_triggered()
{
    ui->graphicsView->resetZoom();
}

void MainWindow::sendImgToControllerShape(QImage image)
{
	 fileName = image.text("name");

 
	qgraphicsScene->clear();
	qgraphicsScene->setSceneRect(0, 0, image.width(), image.height());
	ImageItem = qgraphicsScene->addPixmap(QPixmap::fromImage(image));
	qgraphicsScene->update();

	QRectF bounds = qgraphicsScene->itemsBoundingRect();
	bounds.setWidth(bounds.width());
	bounds.setHeight(bounds.height());
	ui->graphicsView->fitInView(bounds, Qt::KeepAspectRatio);
	state_flag_maindow = CHOOSE_PICTURE; 
}

