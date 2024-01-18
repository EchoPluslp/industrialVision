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
	setMouseTracking(true);

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

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::fitcircle()
{
	for (int i = CControlLine_List.size() - 1;i >= 0 ;i--)
	{
		
		qgraphicsScene->removeItem(CControlLine_List.at(i));
		qgraphicsScene->update();
	}
    //my_calibercircle->fitcircle();
}

cv::Rect get_IOU(cv::Rect rect1, cv::Rect rect2)
{

	cv::Point center_rect1 = (rect1.tl() + rect1.br()) * 0.5;
	cv::Point center_rect2 = (rect2.tl() + rect2.br()) * 0.5;
	auto verti_dis = abs(center_rect1.x - center_rect2.x);
	auto horiz_dis = abs(center_rect1.y - center_rect2.y);

	auto half_w = (rect1.width + rect2.width) / 2;
	auto half_y = (rect1.height + rect2.height) / 2;


	if (horiz_dis < half_y && verti_dis < half_w) {
		int x_tl, x_br, y_tl, y_br;

		if (rect1.tl().x > rect2.tl().x) {
			if (rect1.br().x > rect2.br().x) {
				x_tl = rect1.tl().x;
				x_br = rect2.br().x;
			}
			else {
				x_tl = rect1.tl().x;
				x_br = rect1.br().x;
			}
		}
		else {
			if (rect1.br().x > rect2.br().x) {
				x_tl = rect2.tl().x;
				x_br = rect2.br().x;
			}
			else {
				x_tl = rect2.tl().x;
				x_br = rect1.br().x;
			}
		}


		if (rect1.tl().y > rect2.tl().y) {
			if (rect1.br().y > rect2.br().y) {
				y_tl = rect1.tl().y;
				y_br = rect2.br().y;
			}
			else {
				y_tl = rect1.tl().y;
				y_br = rect1.br().y;
			}
		}
		else {
			if (rect1.br().y > rect2.br().y) {
				y_tl = rect2.tl().y;
				y_br = rect2.br().y;
			}
			else {
				y_tl = rect2.tl().y;
				y_br = rect1.br().y;
			}
		}

		cv::Point tl(x_tl, y_tl);
		cv::Point br(x_br, y_br);
		return cv::Rect(tl, br);

	}
	return cv::Rect(cv::Point(0, 0), cv::Point(0, 0));

}
QList<QPointF> get_intersection(QPointF pt1, QPointF pt2, QPointF begin, QPointF end)
{
	qreal k1;
	qreal b1;
	qreal k2;
	qreal b2;
	qreal k3;
	qreal b3;

	qreal x1;
	qreal y1;
	qreal x2;
	qreal y2;
	if ((pt1.x() - pt2.x()) != 0)
	{
		k1 = (pt1.y() - pt2.y()) / (pt1.x() - pt2.x());
		b1 = -k1 * pt1.x() + pt1.y();
		k2 = -1 / k1;
		k3 = -1 / k1;
		b2 = -k2 * begin.x() + begin.y();
		b3 = -k3 * end.x() + end.y();

		x1 = (b2 - b1) / (k1 - k2);
		y1 = k1 * x1 + b1;
		x2 = (b3 - b1) / (k1 - k3);
		y2 = k1 * x2 + b1;
	}
	else if (pt1.y() != pt2.y())
	{
		x1 = x2 = pt1.x();
		y1 = begin.y();
		y2 = end.y();
	}
	else
	{
		x1 = x2 = y1 = y2 = 0;
	}

	QList<QPointF> result;
	result.push_back(QPointF(x1, y1));
	result.push_back(QPointF(x2, y2));

	return result;
}


void MainWindow::fitline()
{
	if (my_caliberline_List.isEmpty())
	{
		return;
	}
	for (int i = 0; i < my_caliberline_List.size();i++) {
		
	bee_caliberline* my_caliberline = my_caliberline_List.at(i);

	m_plineCaliperGUI = new CLineCaliperGUI();
	QPointF teno = my_caliberline->getbeginpoint();
	QPointF pt_begin = my_caliberline->mapToItem(ImageItem, my_caliberline->getbeginpoint());
	QPointF pt_end = my_caliberline->mapToItem(ImageItem, my_caliberline->getendpoint());
	qreal length_new = sqrt((pt_begin.x() - pt_end.x()) * (pt_begin.x() - pt_end.x()) +
		(pt_begin.y() - pt_end.y()) * (pt_begin.y() - pt_end.y()));

	qreal scale = length_new / my_caliberline->getlength();

	cv::Point pt_begin_cv = cv::Point(pt_begin.x(), pt_begin.y());
	cv::Point pt_end_cv = cv::Point(pt_end.x(), pt_end.y());

	QImage image = ImageItem->pixmap().toImage();
	image = image.convertToFormat(QImage::Format_RGB888);
	Mat srcImage = Mat(image.height(),
		image.width(),
		CV_8UC(3),
		image.bits(),
		image.bytesPerLine());

	if (srcImage.empty())
	{
		qDebug() << "Fail to load the image";
		return;
	}
	QRectF bounding = my_caliberline->mapToItem(ImageItem, my_caliberline->boundingRect()).boundingRect();
	cv::Rect bound_caliber(bounding.topLeft().x(),
		bounding.topLeft().y(),
		bounding.width(), bounding.height());

	cv::Rect bound_src(0, 0, image.width(), image.height());
	cv::Rect roi = get_IOU(bound_caliber, bound_src);

	Mat src = srcImage(roi).clone();
	cv::Point pt_begin_cv2 = pt_begin_cv - roi.tl();
	cv::Point pt_end_cv2 = pt_end_cv - roi.tl();
	// todo 针对每个line[ 处理]
	m_plineCaliperGUI->createLineKaChi(src, pt_begin_cv2, pt_end_cv2,
		my_caliberline->getheight() * scale,
		my_caliberline->getwidth() * scale,
		nSigma, my_caliberline->nthresholdValue,
		my_caliberline->nSampleDirection,
		my_caliberline->nMeasureNums);

	Point2d pdLineStart(0, 0), pdLineEnd(0, 0);
	double dAngle = 0;
	m_plineCaliperGUI->lineEdgePointSetsFit(pdLineStart, pdLineEnd, dAngle);

	//完成匹配,画出点
	lineitem = new CControlLine();
	//将item加入list中,方便删除
	CControlLine_List.append(lineitem);

	QPointF linestart = lineitem->mapFromItem(ImageItem,
		QPointF(pdLineStart.x + roi.tl().x,
			pdLineStart.y + roi.tl().y));
	QPointF lineend = lineitem->mapFromItem(ImageItem,
		QPointF(pdLineEnd.x + roi.tl().x,
			pdLineEnd.y + roi.tl().y));
	qDebug() << linestart << lineend;
	QPointF pt_begin_line = lineitem->mapFromItem(ImageItem, pt_begin);
	QPointF pt_end_line = lineitem->mapFromItem(ImageItem, pt_end);

	QList<QPointF> result = get_intersection(linestart, lineend, pt_begin_line, pt_end_line);

	lineitem->setline(result[0], result[1]);

	vector<Point2d> edgepoints = m_plineCaliperGUI->getEdgePoints();
	vector<Point2d> edgeexpectpoints = m_plineCaliperGUI->getExcepetEdgePoints();  //边缘点的信息
	for (size_t i = 0; i < edgepoints.size(); i++)
	{
		QPointF pt = lineitem->mapFromItem(ImageItem, QPointF(edgepoints[i].x + roi.tl().x,
			edgepoints[i].y + roi.tl().y));
		lineitem->addedgepoints(pt);
	}

	for (size_t j = 0; j < edgeexpectpoints.size(); j++)
	{
		QPointF pt = lineitem->mapFromItem(ImageItem,
			QPointF(edgeexpectpoints[j].x + roi.tl().x,
				edgeexpectpoints[j].y + roi.tl().y));
		lineitem->addedgeexpectpoints(pt);
	}
	qgraphicsScene->addItem(lineitem);
	qgraphicsScene->update();

	QPointF center = QPointF(pdLineStart.x, pdLineStart.y) / 2 + QPointF(pdLineEnd.x, pdLineEnd.y) / 2;
	qreal angle = atan2(pdLineStart.y - pdLineEnd.y, pdLineStart.x - pdLineEnd.x) * 180 / M_PI;

	QString str = tr("图像尺寸=(%1,%2),直线中心=(%3,%4),角度=%5").
		arg(ImageItem->pixmap().width()).
		arg(ImageItem->pixmap().height()).
		arg(center.x()).arg(center.y()).
		arg(angle);

	}
}

void MainWindow::slot_ShowLine_Param(int nMeasureNums, int nthresholdValue, int nSampleDirection,int currentIndexs)
{
	currentIndexs_line = currentIndexs;
	ui->spinBox_nMeasureNums_line->setValue(nMeasureNums);
	ui->spinBox_nSetThreshold_line->setValue(nthresholdValue);
	ui->comboBox_edge_polarity->setCurrentIndex(nSampleDirection);
}

void MainWindow::slot_index_nMeasureNums(int value)
{
	my_caliberline_List.at(currentIndexs_line)->setnMeasureNums(value);
}

void MainWindow::slot_index_nSetThreshold(int value)
{
	my_caliberline_List.at(currentIndexs_line)->setnthresholdValue(value);

}

void MainWindow::slot_index_SampleDirection(int value)
{
	my_caliberline_List.at(currentIndexs_line)->setnSampleDirection(value);

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
           // qgraphicsScene->removeItem(my_caliberline);
           // delete my_caliberline;
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
		bee_caliberline* my_caliberline = new bee_caliberline();
        my_caliberline->setpixmapwidth(ImageItem->pixmap().width()*ImageItem->scale());
        my_caliberline->setpixmapheight(ImageItem->pixmap().height()*ImageItem->scale());
        this->qgraphicsScene->addItem(my_caliberline);
        ui->graphicsView->setScene(this->qgraphicsScene);
        Mat frame = QImageToCvMat(ImageItem->pixmap().toImage(), true);
		cvtColor(frame, frame, COLOR_BGR2GRAY);
        my_caliberline->setpixmapImage(frame);

		my_caliberline_List.append(my_caliberline);
		my_caliberline->currentIndex(my_caliberline_List.size()-1);
		connect(my_caliberline, &bee_caliberline::sign_currentLine_Param, this, &MainWindow::slot_ShowLine_Param);

		//卡尺数
		connect(ui->spinBox_nMeasureNums_line, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::slot_index_nMeasureNums);
		//阈值数量
		connect(ui->spinBox_nSetThreshold_line, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::slot_index_nSetThreshold);
		//边缘方向
		connect(ui->comboBox_edge_polarity, QOverload<int>::of(&QComboBox::currentIndexChanged) , this, &MainWindow::slot_index_SampleDirection);

}

void MainWindow::on_action_calibercircle_triggered()
{
        state_flag_maindow = CALIBERCIRCLE;
        my_calibercircle = new bee_calibercircle();
        my_calibercircle->setpixmapwidth(ImageItem->pixmap().width()*ImageItem->scale());
        my_calibercircle->setpixmapheight(ImageItem->pixmap().height()*ImageItem->scale());
		Mat frame = QImageToCvMat(ImageItem->pixmap().toImage(), true);
		cvtColor(frame, frame, COLOR_BGR2GRAY); 
        my_calibercircle->setpixmapImage(frame);
        this->qgraphicsScene->addItem(my_calibercircle);
        ui->graphicsView->setScene(this->qgraphicsScene);

        //卡尺数
		connect(ui->spinbox_nMeasureNums, QOverload<int>::of(&QSpinBox::valueChanged), my_calibercircle, &bee_calibercircle::slotSliderValueChanged_MeasureNums);

        //边缘方向
  //      connect(ui->horizontalSlider_nSampleDirection, &QSlider::valueChanged, my_calibercircle, &bee_calibercircle::slotSliderValueChanged_nSampleDirection);
		connect(ui->spinBox_nSampleDirection, QOverload<int>::of(&QSpinBox::valueChanged), my_calibercircle, &bee_calibercircle::slotSliderValueChanged_nSampleDirection);

          //sigma
//      connect(ui->horizontalSlider_nSigma, &QSlider::valueChanged, my_calibercircle, &bee_calibercircle::slotSliderValueChanged_nSigma);
		connect(ui->spinBox_nsigma, QOverload<int>::of(&QSpinBox::valueChanged), my_calibercircle, &bee_calibercircle::slotSliderValueChanged_nSigma);

        //阈值
//      connect(ui->horizontalSlider_nTranslation, &QSlider::valueChanged, my_calibercircle, &bee_calibercircle::slotSliderValueChanged_nSetThreshold);
		connect(ui->spinBox_nThreshold, QOverload<int>::of(&QSpinBox::valueChanged), my_calibercircle, &bee_calibercircle::slotSliderValueChanged_nSetThreshold);

// 
     //极性 
  //      connect(ui->horizontalSlider_nSetThreshold, &QSlider::valueChanged, my_calibercircle, &bee_calibercircle::slotSliderValueChanged_nTranslation);
        connect(ui->comboBox_nTranslation, QOverload<int>::of(&QComboBox::currentIndexChanged), my_calibercircle, &bee_calibercircle::slotSliderValueChanged_nTranslation);
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
	
	my_caliberline_List.clear();
}

