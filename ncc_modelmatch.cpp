#include "ncc_modelmatch.h"
#include "ui_ncc_MainWindow.h"

using namespace std;

NCCMainWindow::NCCMainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::ncc_MainWindow)
	, state_flag_maindow(CHOOSE_NULL)
{    
	ui->setupUi(this);
	//QTextCodec* codec = QTextCodec::codecForName("UTF-8");//或者"GBK",不分大小写
	//QTextCodec::setCodecForLocale(codec);

	this->ImageItem = nullptr;

	qgraphicsScene = new QGraphicsScene();
	qgraphicsScene->setBackgroundBrush(Qt::white);
	qgraphicsScene->setItemIndexMethod(QGraphicsScene::NoIndex);

	ui->graphicsView->setContextMenuPolicy(Qt::CustomContextMenu);

	ui->graphicsView->setScene(qgraphicsScene);

	ui->toolBar->setMovable(false);
	ui->toolBar->setIconSize(QSize(60, 60));
	ui->toolBar->setFloatable(false); // 设置工具栏不可浮动
	ui->toolBar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	ui->toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	
	source_rect_List = new QList<QGraphicsItem*>();
	//pattern_rect_List = new QList<QGraphicsItem*>();


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

	//信号,连接完成处理
}

NCCMainWindow::~NCCMainWindow()
{
	delete ui;
}


void NCCMainWindow::keyPressEvent(QKeyEvent* event)
{
	/*if (event->key() == Qt::Key_Delete)
	{
		switch (state_flag_maindow)
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
	}*/

	qgraphicsScene->update();

}

//返回指定name的index
int NCCMainWindow::getListItem(QString name)
{
	if (source_rect_List->size()==0)
	{
		return -1;
	}
	for (int i = 0;i< ui->listWidget->count();i++)
	{
		QListWidgetItem* item = ui->listWidget->item(i);
		if (item->text().contains(name))
		{
			return i;
		}
	}
	return -1;
}


//发送获得图片请求数据
void NCCMainWindow::on_action_choosepicture_triggered()
{
	//fileName = QFileDialog::getOpenFileName(this, QStringLiteral("选择图片"), "F:", QStringLiteral("图片文件(*png *jpg *bmp)"));
	//if (fileName == nullptr)
	//{
	//	return;
	//}
	//qgraphicsScene->clear();
	//QImage image = QImage(fileName);
	//qgraphicsScene->setSceneRect(0, 0, image.width(), image.height());

	//ImageItem = qgraphicsScene->addPixmap(QPixmap::fromImage(image));

	//qgraphicsScene->update();
	//QRectF bounds = qgraphicsScene->itemsBoundingRect();
	//bounds.setWidth(bounds.width());
	//bounds.setHeight(bounds.height());
	//ui->graphicsView->fitInView(bounds, Qt::KeepAspectRatio);
	//state_flag_maindow = CHOOSE_PICTURE;

	emit getImageFromCamera("patternMatch");
}

//搜索区域
void NCCMainWindow::on_action_rect_triggered()
{
	index_source = source_rect_List->size()+1;
	state_flag_maindow = CHOOSE_SOURCE_RECT;
	 source_rect_info = new bee_rect(nullptr, false,false, index_source);
	source_rect_info->setpixmapwidth(ImageItem->pixmap().width() * ImageItem->scale());
	source_rect_info->setpixmapheight(ImageItem->pixmap().height() * ImageItem->scale());
	source_rect_info->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);

	this->qgraphicsScene->addItem(source_rect_info);
	ui->graphicsView->setScene(this->qgraphicsScene);
	//保存当前index,并添加到list中
	source_rect_info->current_roi_index = index_source;

	connect(source_rect_info, &bee_rect::create_RECT, this, &NCCMainWindow::createRECT);

}

//特征区域设置 ,矩形
void NCCMainWindow::on_action_rotaterect_triggered()
{
	index_pattern = source_rect_List->size() + 1;
	state_flag_maindow = CHOOSE_RECT;
	ncc_patten_rect_info =  new bee_rect(nullptr, false, true, index_pattern);
	ncc_patten_rect_info->setpixmapwidth(ImageItem->pixmap().width() * ImageItem->scale());
	ncc_patten_rect_info->setpixmapheight(ImageItem->pixmap().height() * ImageItem->scale());
	ncc_patten_rect_info->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
	//ncc_patten_rect_info->if_ncc_modelShape = true;
	//给当前rec t设置id
	this->qgraphicsScene->addItem(ncc_patten_rect_info);
	ui->graphicsView->setScene(this->qgraphicsScene);

	//保存当前index,并添加到list中
	ncc_patten_rect_info->current_roi_index = index_pattern;
	connect(ncc_patten_rect_info, &bee_rect::create_RECT, this, &NCCMainWindow::createRECT);

}

//多边形
void NCCMainWindow::on_action_polygon_triggered()
{
	int polygon_pattern = source_rect_List->size() + 1;

	state_flag_maindow = CHOOSE_POLYGON;
	source_polygon_info = new bee_polygon(nullptr, polygon_pattern);
	source_polygon_info->setpixmapwidth(ImageItem->pixmap().width() * ImageItem->scale());
	source_polygon_info->setpixmapheight(ImageItem->pixmap().height() * ImageItem->scale());
	this->qgraphicsScene->addItem(source_polygon_info);
	ui->graphicsView->setScene(this->qgraphicsScene);

	connect(source_polygon_info, &bee_polygon::create_RECT, this, &NCCMainWindow::createRECT);

}


void NCCMainWindow::on_action_circle_triggered()
{
	int circle_pattern = source_rect_List->size() + 1;

	state_flag_maindow = CHOOSE_CIRCLE;
	source_circle_info = new bee_circle(nullptr, circle_pattern);
	source_circle_info->setpixmapwidth(ImageItem->pixmap().width() * ImageItem->scale()); //设置图片的长宽为绘制时的区域长宽
	source_circle_info->setpixmapheight(ImageItem->pixmap().height() * ImageItem->scale());
	this->qgraphicsScene->addItem(source_circle_info);
	ui->graphicsView->setScene(this->qgraphicsScene);

	connect(source_circle_info, &bee_circle::create_RECT, this, &NCCMainWindow::createRECT);

}

//删除按钮点击事件
void NCCMainWindow::on_action_concircle_triggered()
{
	int listRow = ui->listWidget->currentRow();
	if (listRow<0)
	{
		//未选择删除按钮
		return;
	}
	QListWidgetItem* currentItem = ui->listWidget->currentItem();
	int currentRow = ui->listWidget->currentRow();

	ui->listWidget->takeItem(currentRow);
	qgraphicsScene->removeItem(source_rect_List->at(currentRow));
	source_rect_List->removeAt(currentRow);

	//qgraphicsScene->removeItem(source_rect_info);
	//qgraphicsScene->removeItem(ncc_patten_rect_info);
	//qgraphicsScene->removeItem(source_polygon_info);
	//qgraphicsScene->removeItem(source_circle_info);

	//
	////搜索区域
	//source_rect_info = nullptr;
	////特征区域,矩形
	//ncc_patten_rect_info = nullptr;
	////特征区域多边形
	//source_polygon_info = nullptr;
	////清空圆区域
	//source_circle_info = nullptr;

	qgraphicsScene->update();

}
    
//void NCCMainWindow::on_action_ringexpansion_triggered() {
//
//}

//保存当前数据
void NCCMainWindow::on_action_ringexpansion_triggered()
{
	int item = 0;
	//判断当前特征区域的标志
	int pattern_index = getListItem("特征区域");
	if (pattern_index ==0)
	{
		//没有设置特征区域
		return;
	}
	int pattern_rect_index = getListItem("矩形");
	int pattern_polygon_index = getListItem("多边形");
	if (pattern_rect_index != -1)
	{
		item = 1;
	}else if (pattern_polygon_index != -1)
	{
		item = 2;
	}
		//另存为
		QString path = QFileDialog::getSaveFileName(nullptr,
			tr("Open File"),
			"",
			tr("ini Files(*.ini)"));

	QSettings* settings = new QSettings(path, QSettings::IniFormat);

	QString groupinfo("pattern_info");
	settings->beginGroup(groupinfo);
	settings->setValue("pattern_info_item", item);
	settings->setValue("source_width", ImageItem->pixmap().width());
	settings->setValue("source_height", ImageItem->pixmap().height());

	int source_rect_index = getListItem("搜索区域");
	bee_rect* source_rect_info_item;
	if (source_rect_index == -1)
	{
		source_rect_info_item = new bee_rect();
		source_rect_info_item->m_rect.setX(0);
		source_rect_info_item->m_rect.setY(0);
		source_rect_info_item->m_rect.setWidth(ImageItem->pixmap().width() * ImageItem->scale());
		source_rect_info_item->m_rect.setHeight(ImageItem->pixmap().height() * ImageItem->scale());
	}
	else {
		source_rect_info_item = (bee_rect*)source_rect_List->at(source_rect_index);
	}

	//保存范围图信息
	settings->setValue("source_rect_info.x", QString::number(source_rect_info_item->m_rect.x()));
	settings->setValue("source_rect_info.y", QString::number(source_rect_info_item->m_rect.y()));
	settings->setValue("source_rect_info.width", QString::number(source_rect_info_item->m_rect.width()));
	settings->setValue("source_rect_info.height", QString::number(source_rect_info_item->m_rect.height()));
	settings->setValue("source_rect_image_info", QString(fileName));

	//保存特征图信息
	if (pattern_rect_index != -1)
	{
		bee_rect* ncc_patten_rect_info_item = (bee_rect*)source_rect_List->at(pattern_rect_index);

		if (ncc_patten_rect_info_item->if_create)
		{
		
		settings->setValue("pattern_rect_info.x", QString::number(ncc_patten_rect_info_item->m_rect.x()));
		settings->setValue("pattern_rect_info.y", QString::number(ncc_patten_rect_info_item->m_rect.y()));
		settings->setValue("pattern_rect_info.width", QString::number(ncc_patten_rect_info_item->m_rect.width()));
		settings->setValue("pattern_rect_info.height", QString::number(ncc_patten_rect_info_item->m_rect.height()));
		}
	}

	//保存多边形的特征信息
	if (pattern_polygon_index != -1)
	{
		bee_polygon* ncc_patten_polygon_info_item = (bee_polygon*)source_rect_List->at(pattern_polygon_index);

		if (ncc_patten_polygon_info_item->if_create)
		{
		int num = ncc_patten_polygon_info_item->num;
		settings->setValue("pattern_polygon_num", QString::number(num));

		QVector<QPointF> pp_item = ncc_patten_polygon_info_item->pp;
		for (int i = 0; i < num; i++)
		{
			QString itemX("pattern_polygon_count_X_");
			itemX.append(QString::number(i));
			settings->setValue(itemX, QString::number(pp_item.at(i).x()));

			QString itemY("pattern_polygon_count_Y_");
			itemY.append(QString::number(i));
			int y = pp_item.at(i).y();
			settings->setValue(itemY, QString::number(pp_item.at(i).y()));
			}
		 }
	}
	//保存输出点
	int source_point_index = getListItem("输出点");

	if (source_point_index != -1)
	{
		bee_point* ncc_patten_point_info_item = (bee_point*)source_rect_List->at(source_point_index);

	if (ncc_patten_point_info_item->if_create)
	{
		QList<mycorneritem*> my_caliberlineItem = ncc_patten_point_info_item->m_HandlesList;
		mycorneritem*  pointCount = my_caliberlineItem.at(0);
		double x = pointCount->m_point.x();
		double y = pointCount->m_point.y();
		QString itemoutPoint("pattern_Output_Point_Count");
		QString itemoutPointX("pattern_Output_Point_X");
		QString itemoutPointY("pattern_Output_Point_Y");

		settings->setValue(itemoutPoint, QString::number(1));
		settings->setValue(itemoutPointX, QString::number(x));
		settings->setValue(itemoutPointY, QString::number(y));
	}	
	}
	settings->endGroup();

	delete settings;
	
	
	//保存当前图片
	QString dirpath = QApplication::applicationDirPath() + "/model/";
	QDir dir(dirpath);
	if (!dir.exists())
	{
		//不存在则创建
		dir.mkdir(dirpath); //只创建一级子目录，即必须保证上级目录存在
	}

	QString fullpath = dirpath + fileName;

	//保存当前的图片
	QImage matSrcImage = ImageItem->pixmap().toImage();
	matSrcImage.save(fullpath, "BMP", 100);

	QMessageBox::warning(0, "通知", "保存成功");
	//发送当前的文档路径给前端路径
	emit sendINIPath(path);
}


//效果预览按钮
void NCCMainWindow::on_action_caliberline_triggered()
{
	int really_imageItem_width = ImageItem->pixmap().width() * ImageItem->scale() -1;
	int really_imageItem_height = ImageItem->pixmap().height() * ImageItem->scale() -1;
	double source_x = 0;
	double source_y = 0;
	double source_width = really_imageItem_width;
	double source_height;
	//获取两个画好的roi区域,用于匹配
	if (source_rect_info != nullptr)
	{
		 source_x = source_rect_info->m_rect.x() == 0 ? 0 : source_rect_info->m_rect.x();
		 source_y = source_rect_info->m_rect.y() == 0 ? 0 : source_rect_info->m_rect.y();
		 source_width = source_rect_info->m_rect.width() == 0 ? really_imageItem_width : source_rect_info->m_rect.width();
		 source_height = source_rect_info->m_rect.height() == 0 ? really_imageItem_height : source_rect_info->m_rect.height();
	}
	//判断source 是否超过图片的范围
	if (source_x + source_width > really_imageItem_width)
	{
		source_width = really_imageItem_width - source_x;
	}
	if (source_y + source_height > really_imageItem_height)
	{
		source_height = really_imageItem_height - source_y;
	}

	if (ncc_patten_rect_info==nullptr)
	{
		return;
	}
	//获取特征区域
	double ncc_pattern_x = ncc_patten_rect_info->m_rect.x() == 0 ? 0 : ncc_patten_rect_info->m_rect.x();
	double ncc_pattern_y = ncc_patten_rect_info->m_rect.y() == 0 ? 0 : ncc_patten_rect_info->m_rect.y();
	double ncc_pattern_width = ncc_patten_rect_info->m_rect.width() == 0 ? really_imageItem_width : ncc_patten_rect_info->m_rect.width();
	double ncc_pattern_height = ncc_patten_rect_info->m_rect.height() == 0 ? really_imageItem_height : ncc_patten_rect_info->m_rect.height();

	//判断ncc pattern 是否超过图片的范围
	if (ncc_pattern_x + ncc_pattern_width > really_imageItem_width)
	{
		ncc_pattern_width = really_imageItem_width - ncc_pattern_x;
	}
	if (ncc_pattern_y + ncc_pattern_height > really_imageItem_height)
	{
		ncc_pattern_height = really_imageItem_height - ncc_pattern_y;
	}

	//得到坐标获取对应的Qimage
	QImage source_Item = ImageItem->pixmap().copy(QRect(source_x, source_y, source_width, source_height)).toImage();
	QImage  ncc_pattern_Item = ImageItem->pixmap().copy(QRect(ncc_pattern_x, ncc_pattern_y, ncc_pattern_width, ncc_pattern_height)).toImage();

	//发送给processThread 线程 用于匹配
	emit sendImageToPattern(ncc_pattern_Item, source_Item);

}

//输出点
void NCCMainWindow::on_action_calibercircle_triggered()
{
	int point_pattern = source_rect_List->size() + 1;


	state_flag_maindow = CALIBERCIRCLE;
	source_point_info = new bee_point(nullptr, point_pattern);
	source_point_info->setpixmapwidth(ImageItem->pixmap().width() * ImageItem->scale());
	source_point_info->setpixmapheight(ImageItem->pixmap().height() * ImageItem->scale());
	
	this->qgraphicsScene->addItem(source_point_info);
	ui->graphicsView->setScene(this->qgraphicsScene);

	connect(source_point_info, &bee_point::create_RECT, this, &NCCMainWindow::createRECT);

}

void NCCMainWindow::on_action_zoomin_triggered()
{
	ui->graphicsView->zoomIn();
}

void NCCMainWindow::on_action_zoomout_triggered()
{

	ui->graphicsView->zoomOut();
}

void NCCMainWindow::on_action_fitwin_triggered()
{
	if (!this->ImageItem)
		return;

	ui->graphicsView->fitInView(this->ImageItem, Qt::KeepAspectRatio);
}

void NCCMainWindow::on_action_1_to_1_triggered()
{
	ui->graphicsView->resetZoom();
}

void NCCMainWindow::sendImgToControllerShape(QImage image,QString ModelPath)
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

//
void NCCMainWindow::slot_receiveDrawPoint(QPoint resultPoint, int totalModelTime)
{
	QPixmap image = ImageItem->pixmap();
	QPainter painter(&image);
	int sourceX = 0;
	int sourceY = 0;
	if (source_rect_info!=nullptr)
	{
		sourceX = source_rect_info->m_rect.x();
		sourceY = source_rect_info->m_rect.y();
	}
	int x = resultPoint.x() + sourceX;
	int y = resultPoint.y() + sourceY;
	painter.setPen(QPen(Qt::green, 10));

	painter.drawPoint(x,y);//在image中(0,0)为左上角，宽20、高20的矩形内画圆
	QString str = tr("size=(%1,%2),centerPoint(%3,%4),totaltime(%5)").
		arg(ImageItem->pixmap().width()).
		arg(ImageItem->pixmap().height()).
		arg(x).arg(y).arg(totalModelTime);
	ui->statusbar->showMessage(str);

	qgraphicsScene->update();
}

void NCCMainWindow::createRECT(int type, int index)
{
	if (type==1)
	{
		QString itemValue = "搜索区域-";
		itemValue.append(QString::number(index));
		ui->listWidget->addItem(itemValue);
		source_rect_List->append(source_rect_info);
	}
	else if(type == 2) {
		QString itemValue = "特征区域-矩形-";
		itemValue.append(QString::number(index));
		ui->listWidget->addItem(itemValue);
		source_rect_List->append(ncc_patten_rect_info);
	}
	else if (type == 3) {
		QString itemValue = "特征区域-多边形-";
		itemValue.append(QString::number(index));
		ui->listWidget->addItem(itemValue);
		source_rect_List->append(source_polygon_info);
	}
	else if (type == 4) {
		QString itemValue = "特征区域-圆形-";
		itemValue.append(QString::number(index));
		ui->listWidget->addItem(itemValue);
		source_rect_List->append(source_circle_info);
	}
	else if (type == 5) {
		QString itemValue = "输出点-";
		itemValue.append(QString::number(index));
		ui->listWidget->addItem(itemValue);
		source_rect_List->append(source_point_info);
	}
}
