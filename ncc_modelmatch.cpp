#include "ncc_modelmatch.h"
#include "ui_ncc_MainWindow.h"

using namespace std;

NCCMainWindow::NCCMainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::ncc_MainWindow)
	, state_flag_maindow(CHOOSE_NULL)
{    
	ui->setupUi(this);
	//QTextCodec* codec = QTextCodec::codecForName("UTF-8");//����"GBK",���ִ�Сд
	//QTextCodec::setCodecForLocale(codec);

	this->ImageItem = nullptr;

	qgraphicsScene = new QGraphicsScene();
	qgraphicsScene->setBackgroundBrush(Qt::white);
	qgraphicsScene->setItemIndexMethod(QGraphicsScene::NoIndex);

	ui->graphicsView->setContextMenuPolicy(Qt::CustomContextMenu);

	ui->graphicsView->setScene(qgraphicsScene);

	ui->toolBar->setMovable(false);
	ui->toolBar->setIconSize(QSize(60, 60));
	ui->toolBar->setFloatable(false); // ���ù��������ɸ���
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

	//�ź�,������ɴ���
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

//����ָ��name��index
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


//���ͻ��ͼƬ��������
void NCCMainWindow::on_action_choosepicture_triggered()
{
	//fileName = QFileDialog::getOpenFileName(this, QStringLiteral("ѡ��ͼƬ"), "F:", QStringLiteral("ͼƬ�ļ�(*png *jpg *bmp)"));
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

//��������
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
	//���浱ǰindex,����ӵ�list��
	source_rect_info->current_roi_index = index_source;

	connect(source_rect_info, &bee_rect::create_RECT, this, &NCCMainWindow::createRECT);

}

//������������ ,����
void NCCMainWindow::on_action_rotaterect_triggered()
{
	index_pattern = source_rect_List->size() + 1;
	state_flag_maindow = CHOOSE_RECT;
	ncc_patten_rect_info =  new bee_rect(nullptr, false, true, index_pattern);
	ncc_patten_rect_info->setpixmapwidth(ImageItem->pixmap().width() * ImageItem->scale());
	ncc_patten_rect_info->setpixmapheight(ImageItem->pixmap().height() * ImageItem->scale());
	ncc_patten_rect_info->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
	//ncc_patten_rect_info->if_ncc_modelShape = true;
	//����ǰrec t����id
	this->qgraphicsScene->addItem(ncc_patten_rect_info);
	ui->graphicsView->setScene(this->qgraphicsScene);

	//���浱ǰindex,����ӵ�list��
	ncc_patten_rect_info->current_roi_index = index_pattern;
	connect(ncc_patten_rect_info, &bee_rect::create_RECT, this, &NCCMainWindow::createRECT);

}

//�����
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
	source_circle_info->setpixmapwidth(ImageItem->pixmap().width() * ImageItem->scale()); //����ͼƬ�ĳ���Ϊ����ʱ�����򳤿�
	source_circle_info->setpixmapheight(ImageItem->pixmap().height() * ImageItem->scale());
	this->qgraphicsScene->addItem(source_circle_info);
	ui->graphicsView->setScene(this->qgraphicsScene);

	connect(source_circle_info, &bee_circle::create_RECT, this, &NCCMainWindow::createRECT);

}

//ɾ����ť����¼�
void NCCMainWindow::on_action_concircle_triggered()
{
	int listRow = ui->listWidget->currentRow();
	if (listRow<0)
	{
		//δѡ��ɾ����ť
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
	////��������
	//source_rect_info = nullptr;
	////��������,����
	//ncc_patten_rect_info = nullptr;
	////������������
	//source_polygon_info = nullptr;
	////���Բ����
	//source_circle_info = nullptr;

	qgraphicsScene->update();

}
    
//void NCCMainWindow::on_action_ringexpansion_triggered() {
//
//}

//���浱ǰ����
void NCCMainWindow::on_action_ringexpansion_triggered()
{
	int item = 0;
	//�жϵ�ǰ��������ı�־
	int pattern_index = getListItem("��������");
	if (pattern_index ==0)
	{
		//û��������������
		return;
	}
	int pattern_rect_index = getListItem("����");
	int pattern_polygon_index = getListItem("�����");
	if (pattern_rect_index != -1)
	{
		item = 1;
	}else if (pattern_polygon_index != -1)
	{
		item = 2;
	}
		//���Ϊ
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

	int source_rect_index = getListItem("��������");
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

	//���淶Χͼ��Ϣ
	settings->setValue("source_rect_info.x", QString::number(source_rect_info_item->m_rect.x()));
	settings->setValue("source_rect_info.y", QString::number(source_rect_info_item->m_rect.y()));
	settings->setValue("source_rect_info.width", QString::number(source_rect_info_item->m_rect.width()));
	settings->setValue("source_rect_info.height", QString::number(source_rect_info_item->m_rect.height()));
	settings->setValue("source_rect_image_info", QString(fileName));

	//��������ͼ��Ϣ
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

	//�������ε�������Ϣ
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
	//���������
	int source_point_index = getListItem("�����");

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
	
	
	//���浱ǰͼƬ
	QString dirpath = QApplication::applicationDirPath() + "/model/";
	QDir dir(dirpath);
	if (!dir.exists())
	{
		//�������򴴽�
		dir.mkdir(dirpath); //ֻ����һ����Ŀ¼�������뱣֤�ϼ�Ŀ¼����
	}

	QString fullpath = dirpath + fileName;

	//���浱ǰ��ͼƬ
	QImage matSrcImage = ImageItem->pixmap().toImage();
	matSrcImage.save(fullpath, "BMP", 100);

	QMessageBox::warning(0, "֪ͨ", "����ɹ�");
	//���͵�ǰ���ĵ�·����ǰ��·��
	emit sendINIPath(path);
}


//Ч��Ԥ����ť
void NCCMainWindow::on_action_caliberline_triggered()
{
	int really_imageItem_width = ImageItem->pixmap().width() * ImageItem->scale() -1;
	int really_imageItem_height = ImageItem->pixmap().height() * ImageItem->scale() -1;
	double source_x = 0;
	double source_y = 0;
	double source_width = really_imageItem_width;
	double source_height;
	//��ȡ�������õ�roi����,����ƥ��
	if (source_rect_info != nullptr)
	{
		 source_x = source_rect_info->m_rect.x() == 0 ? 0 : source_rect_info->m_rect.x();
		 source_y = source_rect_info->m_rect.y() == 0 ? 0 : source_rect_info->m_rect.y();
		 source_width = source_rect_info->m_rect.width() == 0 ? really_imageItem_width : source_rect_info->m_rect.width();
		 source_height = source_rect_info->m_rect.height() == 0 ? really_imageItem_height : source_rect_info->m_rect.height();
	}
	//�ж�source �Ƿ񳬹�ͼƬ�ķ�Χ
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
	//��ȡ��������
	double ncc_pattern_x = ncc_patten_rect_info->m_rect.x() == 0 ? 0 : ncc_patten_rect_info->m_rect.x();
	double ncc_pattern_y = ncc_patten_rect_info->m_rect.y() == 0 ? 0 : ncc_patten_rect_info->m_rect.y();
	double ncc_pattern_width = ncc_patten_rect_info->m_rect.width() == 0 ? really_imageItem_width : ncc_patten_rect_info->m_rect.width();
	double ncc_pattern_height = ncc_patten_rect_info->m_rect.height() == 0 ? really_imageItem_height : ncc_patten_rect_info->m_rect.height();

	//�ж�ncc pattern �Ƿ񳬹�ͼƬ�ķ�Χ
	if (ncc_pattern_x + ncc_pattern_width > really_imageItem_width)
	{
		ncc_pattern_width = really_imageItem_width - ncc_pattern_x;
	}
	if (ncc_pattern_y + ncc_pattern_height > really_imageItem_height)
	{
		ncc_pattern_height = really_imageItem_height - ncc_pattern_y;
	}

	//�õ������ȡ��Ӧ��Qimage
	QImage source_Item = ImageItem->pixmap().copy(QRect(source_x, source_y, source_width, source_height)).toImage();
	QImage  ncc_pattern_Item = ImageItem->pixmap().copy(QRect(ncc_pattern_x, ncc_pattern_y, ncc_pattern_width, ncc_pattern_height)).toImage();

	//���͸�processThread �߳� ����ƥ��
	emit sendImageToPattern(ncc_pattern_Item, source_Item);

}

//�����
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

	painter.drawPoint(x,y);//��image��(0,0)Ϊ���Ͻǣ���20����20�ľ����ڻ�Բ
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
		QString itemValue = "��������-";
		itemValue.append(QString::number(index));
		ui->listWidget->addItem(itemValue);
		source_rect_List->append(source_rect_info);
	}
	else if(type == 2) {
		QString itemValue = "��������-����-";
		itemValue.append(QString::number(index));
		ui->listWidget->addItem(itemValue);
		source_rect_List->append(ncc_patten_rect_info);
	}
	else if (type == 3) {
		QString itemValue = "��������-�����-";
		itemValue.append(QString::number(index));
		ui->listWidget->addItem(itemValue);
		source_rect_List->append(source_polygon_info);
	}
	else if (type == 4) {
		QString itemValue = "��������-Բ��-";
		itemValue.append(QString::number(index));
		ui->listWidget->addItem(itemValue);
		source_rect_List->append(source_circle_info);
	}
	else if (type == 5) {
		QString itemValue = "�����-";
		itemValue.append(QString::number(index));
		ui->listWidget->addItem(itemValue);
		source_rect_List->append(source_point_info);
	}
}
