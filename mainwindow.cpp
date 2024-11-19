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
	  ui->toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

	  ui->action_choosepicture->setFont(QFont("Microsoft YaHei", 15, QFont::Bold));
	  ui->action_zoomin->setFont(QFont("Microsoft YaHei", 15, QFont::Bold));
	  ui->action_zoomout->setFont(QFont("Microsoft YaHei", 15, QFont::Bold));
	  ui->action_1_to_1->setFont(QFont("Microsoft YaHei", 15, QFont::Bold));
	  ui->action_fitwin->setFont(QFont("Microsoft YaHei", 15, QFont::Bold));
	  ui->action_rect->setFont(QFont("Microsoft YaHei", 15, QFont::Bold));
	  ui->action_rotaterect->setFont(QFont("Microsoft YaHei", 15, QFont::Bold));
	  ui->action_circle->setFont(QFont("Microsoft YaHei", 15, QFont::Bold));
	  ui->action_concircle->setFont(QFont("Microsoft YaHei", 15, QFont::Bold));
	  ui->action_ringexpansion->setFont(QFont("Microsoft YaHei", 15, QFont::Bold));
	  ui->action_caliberline->setFont(QFont("Microsoft YaHei", 15, QFont::Bold));
	  ui->action_calibercircle->setFont(QFont("Microsoft YaHei", 15, QFont::Bold));
	  ui->action_polygon->setFont(QFont("Microsoft YaHei", 15, QFont::Bold));


}


void MainWindow::InitializeMeasureTrackbar()
{    

}

void MainWindow::findIntersection(cv::Point p_1, cv::Point p_2, cv::Point p_3, cv::Point p_4, cv::Point2f& intersection)
{
	// 确保两条直线不平行
	//if (line1.m == line2.m) {
	//	qDebug() << "平行无交点.";
	//	return;
	//}

	// line1's cpmponent
	double X1 = p_2.x - p_1.x;//b1
	double Y1 = p_2.y - p_1.y;//a1
	// line2's cpmponent
	double X2 = p_4.x - p_3.x;//b2
	double Y2 = p_4.y - p_3.y;//a2
	// distance of 1,2
	double X21 = p_3.x - p_1.x;
	double Y21 = p_3.y - p_1.y;
	// determinant
	double D = Y1 * X2 - Y2 * X1;// a1b2-a2b1
	// 
	if (D == 0) return ;
	// cross point
	intersection.x = (X1 * X2 * Y21 + Y1 * X2 * p_1.x - Y2 * X1 * p_3.x) / D;
	// on screen y is down increased ! 
	intersection.y = -(Y1 * Y2 * X21 + X1 * Y2 * p_1.y - X2 * Y1 * p_3.y) / D;
	// segments intersect.
	if ((abs(intersection.x - p_1.x - X1 / 2) <= abs(X1 / 2)) &&
		(abs(intersection.y - p_1.y - Y1 / 2) <= abs(Y1 / 2)) &&
		(abs(intersection.x - p_3.x - X2 / 2) <= abs(X2 / 2)) &&
		(abs(intersection.y - p_3.y - Y2 / 2) <= abs(Y2 / 2)))
	{

	}
	return ;
}

double MainWindow::findangle(cv::Point p_1, cv::Point p_2, cv::Point p_3, cv::Point p_4)
{

	// 计算两条线的向量
	cv::Point vec1 = p_1 - p_2;
	cv::Point vec2 = p_3 - p_4;

	// 计算向量的模
	double magnitude1 = cv::norm(vec1);
	double magnitude2 = cv::norm(vec2);

	// 计算向量的点积
	double dotProduct = vec1.x * vec2.x + vec1.y * vec2.y;

	// 计算余弦值
	double cosTheta = dotProduct / (magnitude1 * magnitude2);

	// 使用反余弦函数计算夹角（弧度）
	double angleRad = std::acos(std::max(-1.0, std::min(1.0, cosTheta)));

	// 将弧度转换为度数
	double angleDeg = angleRad * 180 / CV_PI;
	return angleDeg;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::fitcircle()
{
	/*for (int i = CControlLine_List.size() - 1;i >= 0 ;i--)
	{
		
		qgraphicsScene->removeItem(CControlLine_List.at(i));
		qgraphicsScene->update();
	}*/
	//把点返回


	my_calibercircle->fitcircle(pdCenter, dRadius);
	lineitem = new CControlLine();
	lineitem->addcircles(pdCenter, dRadius);
	qgraphicsScene->addItem(lineitem);
	CControlLine_List.append(lineitem);

	qgraphicsScene->update();
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
	//每次使用前清空使用
	resultLinePoint.clear();

	for (int i = 0; i < my_caliberline_List.size();i++) {
		
	bee_caliberline* my_caliberline = my_caliberline_List.at(i);

	m_plineCaliperGUI = new CLineCaliperGUI();
	//QPointF teno = my_caliberline->getbeginpoint();
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

	int getheight =  my_caliberline->getheight()* scale;
	int width =  my_caliberline->getwidth()* scale;

	m_plineCaliperGUI->createLineKaChi(src, pt_begin_cv2, pt_end_cv2,
		my_caliberline->getheight() * scale,
		my_caliberline->getwidth() * scale,
		nSigma, my_caliberline->nthresholdValue,
		my_caliberline->nSampleDirection,
		my_caliberline->nMeasureNums);
	//匹配时设置当前的保存信息参数
	{
		my_caliberline->setSaveStartPoint(pt_begin_cv2);
		my_caliberline->setEndStartPoint(pt_end_cv2);
		my_caliberline->setRoi(roi);
	}

	Point2d pdLineStart(0, 0), pdLineEnd(0, 0);
	double dAngle = 0;
	m_plineCaliperGUI->lineEdgePointSetsFit(pdLineStart, pdLineEnd, dAngle);

	cv::Point startPoint(pdLineStart.x + roi.tl().x,
		pdLineStart.y + roi.tl().y);
	cv::Point endPoint(pdLineEnd.x + roi.tl().x,
		pdLineEnd.y + roi.tl().y);
	cv::Mat tempCoun = srcImage.clone();
	cv::circle(tempCoun, startPoint, 9, Scalar(0, 255, 0) , cv::FILLED);
	cv::circle(tempCoun, endPoint, 9, Scalar(0, 255, 0), cv::FILLED);


	//匹配失败：
	// start和end = -1  或者他们相等
	
	//完成匹配,画出点
	lineitem = new CControlLine();
	//将item加入list中,方便删除
	CControlLine_List.append(lineitem);
	//转换到像素坐标

	QPointF linestart = lineitem->mapFromItem(ImageItem,
		QPointF(pdLineStart.x + roi.tl().x,
			pdLineStart.y + roi.tl().y));
	QPointF lineend = lineitem->mapFromItem(ImageItem,
		QPointF(pdLineEnd.x + roi.tl().x,
			pdLineEnd.y + roi.tl().y));
	qDebug() << linestart << lineend;
	qDebug() << pt_begin << pt_end;

	QPointF pt_begin_line = lineitem->mapFromItem(ImageItem, pt_begin);
	QPointF pt_end_line = lineitem->mapFromItem(ImageItem, pt_end);
	//匹配时设置当前的保存信息参数
	{
		my_caliberline->setLineStartPoint(Point(pt_begin_line.x(), pt_begin_line.y()));
		my_caliberline->setLineEndPoint(Point(pt_end_line.x(), pt_end_line.y()));
	}

	vector<cv::Point2f> result = m_plineCaliperGUI->get_intersection(cv::Point2f(linestart.x(), linestart.y()), cv::Point2f(lineend.x(), lineend.y()), cv::Point2f(pt_begin_line.x(), pt_begin_line.y()), cv::Point2f(pt_end_line.x(), pt_end_line.y()));

	lineitem->setline(QPointF(result[0].x, result[0].y), QPointF(result[1].x, result[1].y));

	resultLinePoint.append(cv::Point(result[0].x, result[0].y));

	resultLinePoint.append(cv::Point(result[1].x, result[1].y));

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
		//lineitem->addedgeexpectpoints(pt);
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
	//两条线,计算夹角
	if (resultLinePoint.size() == 4)
	{
//	//计算夹角。        
		double angleDeg = m_plineCaliperGUI->findangle(resultLinePoint.at(0), resultLinePoint.at(1), resultLinePoint.at(2),
			resultLinePoint.at(3));
	//--------------------------计算交点
	Line Line_1 = m_plineCaliperGUI->calculateLine(resultLinePoint.at(0), resultLinePoint.at(1));
	Line Line_2 = m_plineCaliperGUI->calculateLine(resultLinePoint.at(2), resultLinePoint.at(3));

	cv::Point2f Intersection(-1, -1);
	m_plineCaliperGUI->findIntersection(resultLinePoint.at(0), resultLinePoint.at(1), resultLinePoint.at(2), resultLinePoint.at(3), Intersection);
	//输出结果
	QString str = tr(" =(%1,%2),直线交点=(%3,%4),角度=%5").
		arg(ImageItem->pixmap().width()).
		arg(ImageItem->pixmap().height()).
		arg(Intersection.x).arg(Intersection.y).
		arg(angleDeg);

	lineitem->addedgeexpectpoints(QPointF(Intersection.x, Intersection.y));


	ui->statusBar->showMessage(str);
	}else if (resultLinePoint.size() == 10)
	{
		//计算夹角
		 angleDeg_1_newp = m_plineCaliperGUI->findangle(resultLinePoint.at(2), resultLinePoint.at(3), resultLinePoint.at(4), resultLinePoint.at(5));
		 angleDeg_2_newp = m_plineCaliperGUI->findangle(resultLinePoint.at(6), resultLinePoint.at(7), resultLinePoint.at(8), resultLinePoint.at(9));

		//计算交点1		
		// cv::Point2f Intersection_1(-1, -1);
		m_plineCaliperGUI->findIntersection(resultLinePoint.at(2), resultLinePoint.at(3), resultLinePoint.at(4), resultLinePoint.at(5), Intersection_1_newP);
		//计算交点2	
		//cv::Point2f Intersection_2(-1, -1);
		m_plineCaliperGUI->findIntersection(resultLinePoint.at(6), resultLinePoint.at(7), resultLinePoint.at(8), resultLinePoint.at(9), Intersection_2_newP);
		//计算点线距离
		pointToLineDistance_1_newP = m_plineCaliperGUI->pointToLineDistance(Intersection_1_newP, resultLinePoint.at(0), resultLinePoint.at(1));
		pointToLineDistance_2_newP = m_plineCaliperGUI->pointToLineDistance(Intersection_2_newP, resultLinePoint.at(0), resultLinePoint.at(1));
		bool_newP = true;
	}
		
}


void MainWindow::fitAllIndex()
{
	//清理不符合条件的卡尺信息
	for (QList<bee_caliberline*>::iterator it = my_caliberline_List.begin(); it != my_caliberline_List.end();)
	{
		if ((*it)->if_create == false)
		{
			delete* it; // 删除指针指向的对象
			it = my_caliberline_List.erase(it); // 从列表中删除元素，并获取下一个元素的迭代器
		}
		else
		{
			++it; // 继续遍历下一个元素
		}
	}
	for (QList<bee_calibercircle*>::iterator it = my_calibercircle_List.begin(); it != my_calibercircle_List.end();)
	{
		if ((*it)->if_create == false)
		{
			delete* it; // 删除指针指向的对象
			it = my_calibercircle_List.erase(it); // 从列表中删除元素，并获取下一个元素的迭代器
		}
		else
		{
			++it; // 继续遍历下一个元素
		}
	}
	
	//有效卡尺数量
	int linecount = my_caliberline_List.size();
	int circlecount = my_calibercircle_List.size();
	if (linecount==0&& circlecount==0)
	{
		return;
	}
	//线卡尺数量为0,调用圆
	if (linecount == 0 )
	{
		fitcircle();
	}
	//圆卡尺数量为0,调用线
	if (circlecount == 0)
	{
		fitline();
	}
	if (linecount ==1&&circlecount==1)
	{
		fitcircle();
		fitline();
		//获得圆的圆心
		Point2d circleCenter = pdCenter;
		if (circleCenter.x==0&& circleCenter.y==0)
		{
			return;
		}
		QPointF line_mindPoint;
		//获得连线的中点
		if (resultLinePoint.size()==2)
		{
		cv::Point	Point_1 = resultLinePoint.at(0);
		cv::Point	Point_2 = resultLinePoint.at(1);

		line_mindPoint.setX((Point_1.x+ Point_2.x)/2);
		line_mindPoint.setY((Point_1.y + Point_2.y)/2);
		}
		if (line_mindPoint.x()!=0 && line_mindPoint.y()!=0)
		{
			cv::Point resulePoint((circleCenter.x+ line_mindPoint.x())/2,
				(circleCenter.y + line_mindPoint.y()) / 2);
			lineitem->addedgeexpectpoints(QPointF(resulePoint.x, resulePoint.y));

		}
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

//保存按钮点击事件
void MainWindow::saveInfo()
{
	//清理不符合条件的卡尺信息
	for (QList<bee_caliberline*>::iterator it = my_caliberline_List.begin(); it != my_caliberline_List.end();)
	{
		if ((*it)->if_create == false)
		{
			delete* it; // 删除指针指向的对象
			it = my_caliberline_List.erase(it); // 从列表中删除元素，并获取下一个元素的迭代器
		}
		else
		{
			++it; // 继续遍历下一个元素
		}
	}
	for (QList<bee_calibercircle*>::iterator it = my_calibercircle_List.begin(); it != my_calibercircle_List.end();)
	{
		if ((*it)->if_create == false)
		{
			delete* it; // 删除指针指向的对象
			it = my_calibercircle_List.erase(it); // 从列表中删除元素，并获取下一个元素的迭代器
		}
		else
		{
			++it; // 继续遍历下一个元素
		}
	}

	//有效卡尺数量
	int linecount = my_caliberline_List.size();
	int circlecount = my_calibercircle_List.size();

	//另存为
	QString path = QFileDialog::getSaveFileName(nullptr,
		tr("Open File"),
		"",
		tr("ini Files(*.ini)"));

	if (path.isNull())
	{
		return;
	}

	QSettings* settings = new QSettings(path, QSettings::IniFormat);
	QString group("shape_info");
	settings->beginGroup(group);
	settings->setValue("shape_info_item", 1);
	settings->setValue("source_width", ImageItem->pixmap().width());
	settings->setValue("source_height", ImageItem->pixmap().height());
	settings->setValue("shape_info_item", 1);
	if (bool_newP)
	{
		//设置匹配标准参数
		settings->setValue("angleDeg_1_newp", QString::number(angleDeg_1_newp));
		settings->setValue("angleDeg_2_newp", QString::number(angleDeg_2_newp));
		settings->setValue("Intersection_1_newP.x", QString::number(Intersection_1_newP.x));
		settings->setValue("Intersection_1_newP.y", QString::number(Intersection_2_newP.y));
		settings->setValue("Intersection_2_newP.x", QString::number(Intersection_2_newP.x));
		settings->setValue("Intersection_2_newP.y", QString::number(Intersection_2_newP.y));
		settings->setValue("pointToLineDistance_1_newP", QString::number(pointToLineDistance_1_newP));
		settings->setValue("pointToLineDistance_2_newP", QString::number(pointToLineDistance_2_newP));
		bool_newP = false;
		
	}
	
	settings->endGroup();
	
	for (int i = 0; i < my_caliberline_List.size(); i++)
	{
	bee_caliberline* my_Item = my_caliberline_List.at(i);

	//保存线的信息
	QString group("shapeMatch_line_");
	group.append(QString::number(i + 1));

	settings->beginGroup(group);
	settings->setValue("pt_begin_cv2.x", QString::number(my_Item->pt_begin_cv2.x));
	settings->setValue("pt_begin_cv2.y", QString::number(my_Item->pt_begin_cv2.y));
	settings->setValue("pt_end_cv2.x", QString::number(my_Item->pt_end_cv2.x));
	settings->setValue("pt_end_cv2.y", QString::number(my_Item->pt_end_cv2.y));
	settings->setValue("height", QString::number(my_Item->getheight()));
	settings->setValue("width", QString::number(my_Item->getwidth()));
	settings->setValue("nthresholdValue", QString::number(my_Item->nthresholdValue));
	settings->setValue("nSampleDirection", QString::number(my_Item->nSampleDirection));
	settings->setValue("nMeasureNums", QString::number(my_Item->nMeasureNums));
	settings->setValue("roi.x", QString::number(my_Item->roi.x));
	settings->setValue("roi.y", QString::number(my_Item->roi.y));
	settings->setValue("roi.width", QString::number(my_Item->roi.width));
	settings->setValue("roi.height", QString::number(my_Item->roi.height));
	settings->setValue("pt_begin_line.x", QString::number(my_Item->pt_begin_line.x));
	settings->setValue("pt_begin_line.y", QString::number(my_Item->pt_begin_line.y));
	settings->setValue("pt_end_line.x", QString::number(my_Item->pt_end_line.x));
	settings->setValue("pt_end_line.y", QString::number(my_Item->pt_end_line.y));

	settings->endGroup();

	//delete my_Item;
	}
	//保存圆的信息
	for (int i = 0; i < my_calibercircle_List.size(); i++) {
		bee_calibercircle* my_Item = my_calibercircle_List.at(i);
		QString group("shapeMatch_circle_");
		group.append(QString::number(i));
		settings->beginGroup(group);
		settings->setValue("pdCenter.x", QString::number(my_Item->circle_center.x()));
		settings->setValue("pdCenter.y", QString::number(my_Item->circle_center.y()));
		settings->setValue("nRadius", QString::number(my_Item->nRadius));
		settings->setValue("dMeasureLength", QString::number(my_Item->nMeasureLength));
		settings->setValue("dMeasureHeight", QString::number(my_Item->nMeasureHeight));
		settings->setValue("dSigma", QString::number(my_Item->nSigma));
		settings->setValue("nThreshold", QString::number(my_Item->nThreshold));
		settings->setValue("nTranslation", QString::number(my_Item->nTranslation));
		settings->setValue("nMesureNums", QString::number(my_Item->nMeasureNums));
		settings->setValue("nSampleDirection", QString::number(my_Item->nSampleDirection));
		settings->setValue("nCircleSize", QString::number(my_Item->pixmap_width/150));
		settings->setValue("roi.x", QString::number(my_Item->boundingRect().x()));
		settings->setValue("roi.y", QString::number(my_Item->boundingRect().y()));
		settings->setValue("roi.width", QString::number(my_Item->boundingRect().width()));
		settings->setValue("roi.height", QString::number(my_Item->boundingRect().height()));

		settings->endGroup();
	}
	delete settings;
}



void MainWindow::fitline_with_signal(QPixmap image)
{
	
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    //if(event->key() == Qt::Key_Delete)
    //{
    //    switch(state_flag_maindow)
    //    {
    //    case CHOOSE_RECT:
    //    { 
    //        qgraphicsScene->removeItem(my_rect);
    //        delete my_rect;
    //        state_flag_maindow = CHOOSE_PICTURE;

    //        break;
    //    }
    //    case CHOOSE_CIRCLE:
    //    {
    //        qgraphicsScene->removeItem(my_circle);
    //        delete my_circle;
    //        state_flag_maindow = CHOOSE_PICTURE;
    //        break;
    //    }
    //    case CHOOSE_CONCENCIRCLE:
    //    {
    //        qgraphicsScene->removeItem(my_concencircle);
    //        delete my_concencircle;
    //        state_flag_maindow = CHOOSE_PICTURE;
    //        break;
    //    }case CHOOSE_POLYGON:
    //    {
    //        qgraphicsScene->removeItem(my_polygon);
    //        delete my_polygon;
    //        state_flag_maindow = CHOOSE_PICTURE;
    //        break;
    //    }
    //    case CALIBERLINE:
    //    {
    //       // qgraphicsScene->removeItem(my_caliberline);
    //       // delete my_caliberline;
    //        state_flag_maindow = CHOOSE_PICTURE;
    //        break;
    //    }
    //    case CALIBERCIRCLE:
    //    {
    //        qgraphicsScene->removeItem(my_calibercircle);
    //        delete my_calibercircle;
    //        state_flag_maindow = CHOOSE_PICTURE;
    //        break;
    //    }
    //    case RINGEXPANSION:
    //    {
    //        qgraphicsScene->removeItem(my_ringexpansion);
    //        delete my_ringexpansion;
    //        state_flag_maindow = CHOOSE_PICTURE;
    //        break;
    //    }
    //    }
    //}

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
	if (CControlLine_List.size()==0)
	{
		return;
	}
	for (int i = 0;i< CControlLine_List.size();i++)
	{
		CControlLine*  item = CControlLine_List.at(i);
		qgraphicsScene->removeItem(item);
		delete item;
		item = nullptr;
	}
	CControlLine_List.clear();
	/* state_flag_maindow = RINGEXPANSION;
	 my_ringexpansion = new bee_ringexpansion();
	 my_ringexpansion->setpixmapwidth(ImageItem->pixmap().width()*ImageItem->scale());
	 my_ringexpansion->setpixmapheight(ImageItem->pixmap().height()*ImageItem->scale());
	 this->qgraphicsScene->addItem(my_ringexpansion);
	 ui->graphicsView->setScene(this->qgraphicsScene);*/
}

void MainWindow::on_action_caliberline_triggered()
{
	if (ImageItem == nullptr)
	{
		return;
	}
	QPixmap x = ImageItem->pixmap();
	if (x.isNull())
	{
		return;
	}
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
	if (ImageItem == nullptr)
	{
		return;
	}
	QPixmap x = ImageItem->pixmap();
	if (x.isNull())
	{
		return;
	}
        state_flag_maindow = CALIBERCIRCLE;
        my_calibercircle = new bee_calibercircle();
        my_calibercircle->setpixmapwidth(ImageItem->pixmap().width()*ImageItem->scale());
        my_calibercircle->setpixmapheight(ImageItem->pixmap().height()*ImageItem->scale());

		Mat frame = QImageToCvMat(ImageItem->pixmap().toImage(), true);
		cvtColor(frame, frame, COLOR_BGR2GRAY); 
        my_calibercircle->setpixmapImage(frame);

        this->qgraphicsScene->addItem(my_calibercircle);
        ui->graphicsView->setScene(this->qgraphicsScene);

		//目前就只支持一个圆的信息保存
		my_calibercircle_List.append(my_calibercircle);

        //卡尺数
		connect(ui->spinbox_nMeasureNums, QOverload<int>::of(&QSpinBox::valueChanged), my_calibercircle, &bee_calibercircle::slotSliderValueChanged_MeasureNums);

        //边缘方向
		connect(ui->spinBox_nSampleDirection, QOverload<int>::of(&QSpinBox::valueChanged), my_calibercircle, &bee_calibercircle::slotSliderValueChanged_nSampleDirection);

          //sigma
		connect(ui->spinBox_nsigma, QOverload<int>::of(&QSpinBox::valueChanged), my_calibercircle, &bee_calibercircle::slotSliderValueChanged_nSigma);

        //阈值                  
		connect(ui->spinBox_nThreshold, QOverload<int>::of(&QSpinBox::valueChanged), my_calibercircle, &bee_calibercircle::slotSliderValueChanged_nSetThreshold);
 
		//极性 
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

