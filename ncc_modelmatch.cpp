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

	//�˵����½�
	QMenuBar* qMenubar = this->menuBar();
	QMenu* qMenuProject = qMenubar->addMenu("����");
	QAction* qActionCreate = qMenuProject->addAction("�½�����");

	connect(qActionCreate, &QAction::triggered, this, &NCCMainWindow::on_action_CreateProject);
	connect(this, &NCCMainWindow::saveOnceOrderSuccess, this, &NCCMainWindow::saveOnceOrderItem);
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

QString NCCMainWindow::scanDirectory(const QString& directoryPath)
{
	QDir dir(directoryPath);
	if (!dir.exists()) {
		qDebug() << "The directory does not exist.";
		return "";
	}

	// ���ù�������ֻ��ȡһ���ļ���
	dir.setFilter(QDir::NoDotAndDotDot | QDir::Dirs);

	// ��ȡһ���ļ����б�
	QFileInfoList folderList = dir.entryInfoList();
	if (folderList.size()==0)
	{
		QString newMaxFolderPath = directoryPath + "/" + "0" + "/";

		return newMaxFolderPath;
	}


	// ʹ��һ���ַ����б����洢�ļ�������
	QStringList folderNames;
	foreach(QFileInfo folderInfo, folderList) {
		folderNames << folderInfo.fileName();
	}


	// �����ļ��������е����ֵ������һ
	auto maxElement = std::max_element(folderNames.begin(), folderNames.end(),
		[](const QString& a, const QString& b) { return a.toInt() < b.toInt(); });

	bool ok;
	int maxNumber = maxElement->toInt(&ok);
	if (!ok) {
		QString newMaxFolderPath = directoryPath + "/" + "0" + "/";
		return newMaxFolderPath;
	}

	int newMaxNumber = maxNumber + 1;
	QString newMaxFolderName = QString::number(newMaxNumber);

	QString newMaxFolderPath = directoryPath +"/"+ newMaxFolderName + "/";

	return newMaxFolderPath;
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
		QString nn = item->text();
		if (item->text().contains(name))
		{
			return i;
		}
	}
	return -1;
}

cv::Mat NCCMainWindow::QImageToMat(QImage& qimage)
{
	cv::Mat mat;
	switch (qimage.format())
	{
	case QImage::Format_ARGB32:
	case QImage::Format_RGB32:
	case QImage::Format_ARGB32_Premultiplied:
		mat = cv::Mat(qimage.height(), qimage.width(), CV_8UC4, (void*)qimage.constBits(), qimage.bytesPerLine());
		break;
	case QImage::Format_RGB888:
		mat = cv::Mat(qimage.height(), qimage.width(), CV_8UC3, (void*)qimage.constBits(), qimage.bytesPerLine());
		//cv::cvtColor(mat, mat, CV_BGR2RGB);
		break;
	case QImage::Format_Indexed8:
		mat = cv::Mat(qimage.height(), qimage.width(), CV_8UC1, (void*)qimage.constBits(), qimage.bytesPerLine());
		break;
	}
	return mat;
	
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
	//����ǰrect����id
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
	//ɾ����ǰitem
	ui->listWidget->takeItem(currentRow);

	qgraphicsScene->removeItem(source_rect_List->at(currentRow));
	source_rect_List->removeAt(currentRow);

	qgraphicsScene->update();

}
    
//void NCCMainWindow::on_action_ringexpansion_triggered() {
//
//}

//���浱ǰ����
//void NCCMainWindow::on_action_ringexpansion_triggered()
//{
//	int item = 0;
//	//�жϵ�ǰ��������ı�־
//	int pattern_index = getListItem("��������");
//	if (pattern_index ==0)
//	{
//		//û��������������
//		return;
//	}
//	int pattern_rect_index = getListItem("����");
//	int pattern_polygon_index = getListItem("�����");
//	int pattern_circle_index = getListItem("Բ��");
//
//	if (pattern_rect_index != -1)
//	{
//		item = 1;
//	}else if (pattern_polygon_index != -1)
//	{
//		item = 2;
//	}else if (pattern_circle_index!=-1)
//	{
//		item = 3;
//	}
//		//���Ϊ
//		QString path = QFileDialog::getSaveFileName(nullptr,
//			tr("Open File"),
//			"",
//			tr("ini Files(*.ini)"));
//
//		if (path.isEmpty())
//		{
//			return;
//		}
//	QSettings* settings = new QSettings(path, QSettings::IniFormat);
//
//	QString groupinfo("pattern_info");
//	settings->beginGroup(groupinfo);
//	settings->setValue("pattern_info_item", item);
//	settings->setValue("source_width", ImageItem->pixmap().width());
//	settings->setValue("source_height", ImageItem->pixmap().height());
//
//	int source_rect_index = getListItem("��������");
//	bee_rect* source_rect_info_item;
//	if (source_rect_index == -1)
//	{
//		source_rect_info_item = new bee_rect();
//		source_rect_info_item->m_rect.setX(0);
//		source_rect_info_item->m_rect.setY(0);
//		source_rect_info_item->m_rect.setWidth(ImageItem->pixmap().width() * ImageItem->scale());
//		source_rect_info_item->m_rect.setHeight(ImageItem->pixmap().height() * ImageItem->scale());
//	}
//	else {
//		source_rect_info_item = (bee_rect*)source_rect_List->at(source_rect_index);
//	}
//
//	//���淶Χͼ��Ϣ
//	settings->setValue("source_rect_info.x", QString::number(source_rect_info_item->m_rect.x()));
//	settings->setValue("source_rect_info.y", QString::number(source_rect_info_item->m_rect.y()));
//	settings->setValue("source_rect_info.width", QString::number(source_rect_info_item->m_rect.width()));
//	settings->setValue("source_rect_info.height", QString::number(source_rect_info_item->m_rect.height()));
//	settings->setValue("source_rect_image_info", QString(fileName));
//
//	//��������ͼ��Ϣ
//	if (pattern_rect_index != -1)
//	{
//		bee_rect* ncc_patten_rect_info_item = (bee_rect*)source_rect_List->at(pattern_rect_index);
//
//		if (ncc_patten_rect_info_item->if_create)
//		{
//		
//		settings->setValue("pattern_rect_info.x", QString::number(ncc_patten_rect_info_item->m_rect.x()));
//		settings->setValue("pattern_rect_info.y", QString::number(ncc_patten_rect_info_item->m_rect.y()));
//		settings->setValue("pattern_rect_info.width", QString::number(ncc_patten_rect_info_item->m_rect.width()));
//		settings->setValue("pattern_rect_info.height", QString::number(ncc_patten_rect_info_item->m_rect.height()));
//		}
//	}	//�������ε�������Ϣ
//	else if (pattern_polygon_index != -1)
//	{
//		bee_polygon* ncc_patten_polygon_info_item = (bee_polygon*)source_rect_List->at(pattern_polygon_index);
//
//		if (ncc_patten_polygon_info_item->if_create)
//		{
//		int num = ncc_patten_polygon_info_item->num;
//		settings->setValue("pattern_polygon_num", QString::number(num));
//
//		QVector<QPointF> pp_item = ncc_patten_polygon_info_item->pp;
//		for (int i = 0; i < num; i++)
//		{
//			QString itemX("pattern_polygon_count_X_");
//			itemX.append(QString::number(i));
//			settings->setValue(itemX, QString::number(pp_item.at(i).x()));
//
//			QString itemY("pattern_polygon_count_Y_");
//			itemY.append(QString::number(i));
//			int y = pp_item.at(i).y();
//			settings->setValue(itemY, QString::number(pp_item.at(i).y()));
//			}
//		 }
//	}else if (pattern_circle_index!=-1)
//	{
//
//	}
//	//���������
//	int source_point_index = getListItem("�����");
//
//	if (source_point_index != -1)
//	{
//		bee_point* ncc_patten_point_info_item = (bee_point*)source_rect_List->at(source_point_index);
//
//	if (ncc_patten_point_info_item->if_create)
//	{
//		QList<mycorneritem*> my_caliberlineItem = ncc_patten_point_info_item->m_HandlesList;
//		mycorneritem*  pointCount = my_caliberlineItem.at(0);
//		double x = pointCount->m_point.x();
//		double y = pointCount->m_point.y();
//		QString itemoutPoint("pattern_Output_Point_Count");
//		QString itemoutPointX("pattern_Output_Point_X");
//		QString itemoutPointY("pattern_Output_Point_Y");
//
//		settings->setValue(itemoutPoint, QString::number(1));
//		settings->setValue(itemoutPointX, QString::number(x));
//		settings->setValue(itemoutPointY, QString::number(y));
//	}	
//	}
//	settings->endGroup();
//
//	delete settings;
//	
//	
//	//���浱ǰͼƬ
//	QString dirpath = QApplication::applicationDirPath() + "/model/";
//	QDir dir(dirpath);
//	if (!dir.exists())
//	{
//		//�������򴴽�
//		dir.mkdir(dirpath); //ֻ����һ����Ŀ¼�������뱣֤�ϼ�Ŀ¼����
//	}
//
//	QString fullpath = dirpath + fileName;
//
//	//���浱ǰ��ͼƬ
//	QImage matSrcImage = ImageItem->pixmap().toImage();
//	matSrcImage.save(fullpath, "BMP", 100);
//
//	QMessageBox::warning(0, "֪ͨ", "����ɹ�");
//	//���͵�ǰ���ĵ�·����ǰ��·��
//	emit sendINIPath(path);
//}

//���浱ǰ˳��
void NCCMainWindow::on_action_ringexpansion_triggered() {
	//�������ݴ洢��ָ���ļ�������
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
	int pattern_circle_index = getListItem("Բ��");

	if (pattern_rect_index != -1)
	{
		item = 1;
	}else if (pattern_polygon_index != -1)
	{
		item = 2;
	}else if (pattern_circle_index!=-1)
	{
		item = 3;
	}
		//���Ϊ�ļ���·����
	QString filePathIn_ = scanDirectory(folderPathChoose);
	if (filePathIn_.isEmpty())
	{
		return;
	}

	QDir dir;
	dir.mkpath(filePathIn_);
	QSettings* settings = new QSettings(filePathIn_+"/model.ini", QSettings::IniFormat);
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
	bee_rect* ncc_patten_rect_info_item;
	//��������ͼ��Ϣ
	if (pattern_rect_index != -1)
	{
		ncc_patten_rect_info_item = (bee_rect*)source_rect_List->at(pattern_rect_index);

		if (ncc_patten_rect_info_item->if_create)
		{
		
		settings->setValue("pattern_rect_info.x", QString::number(ncc_patten_rect_info_item->m_rect.x()));
		settings->setValue("pattern_rect_info.y", QString::number(ncc_patten_rect_info_item->m_rect.y()));
		settings->setValue("pattern_rect_info.width", QString::number(ncc_patten_rect_info_item->m_rect.width()));
		settings->setValue("pattern_rect_info.height", QString::number(ncc_patten_rect_info_item->m_rect.height()));
		settings->setValue("pattern_rect_info_midPoint.width", QString::number(ncc_patten_rect_info_item->m_rect.x()+ ncc_patten_rect_info_item->m_rect.width()/2));
		settings->setValue("pattern_rect_info_midPoint.height", QString::number(ncc_patten_rect_info_item->m_rect.y()+ ncc_patten_rect_info_item->m_rect.height()/2));
		}
	}	//�������ε�������Ϣ
	else if (pattern_polygon_index != -1)
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
	}else if (pattern_circle_index!=-1)
	{

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
	if (ncc_patten_rect_info_item==nullptr)
	{
		return;
	}
	QString fullpath = filePathIn_ + "model.bmp";
	QRect region(ncc_patten_rect_info_item->m_rect.x(), ncc_patten_rect_info_item->m_rect.y(), ncc_patten_rect_info_item->m_rect.width(), 
		ncc_patten_rect_info_item->m_rect.height());

	//���浱ǰ��ͼƬ

	QImage matSrcImage = ImageItem->pixmap().toImage().copy(region);
	matSrcImage.save(fullpath, "BMP", 100);

	QMessageBox::warning(0, "֪ͨ", "����ɹ�");
	//���͵�ǰ���ĵ�·����ǰ��·��
	//emit sendINIPath(filePathIn_);
	
	//����ɹ�
	emit saveOnceOrderSuccess();
	
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
	int pattern_index = getListItem("��������");
	if (pattern_index == -1)
	{
		//û��������������
		return;
	}
	//�����������
		int source_rect_index = getListItem("��������");
		bee_rect* source_rect_info_item;
	if (source_rect_index == -1)
	{
		//û��ѡ����������,Ĭ��ȫͼ
		source_x = 0;
		source_y = 0;
		source_width = really_imageItem_width;
		source_height = really_imageItem_height;
	}else{
		source_rect_info_item = (bee_rect*)source_rect_List->at(source_rect_index);

		 source_x = source_rect_info_item->m_rect.x() == 0 ? 0 : source_rect_info_item->m_rect.x();
		 source_y = source_rect_info_item->m_rect.y() == 0 ? 0 : source_rect_info_item->m_rect.y();
		 source_width = source_rect_info_item->m_rect.width() == 0 ? really_imageItem_width : source_rect_info_item->m_rect.width();
		 source_height = source_rect_info_item->m_rect.height() == 0 ? really_imageItem_height : source_rect_info_item->m_rect.height();
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

	//��ȡ��������
	int pattern_index_rect = getListItem("����");
	if (pattern_index_rect != -1)
	{
		bee_rect*  pattern_rect_info_item = (bee_rect*)source_rect_List->at(pattern_index_rect);

		double ncc_pattern_x = pattern_rect_info_item->m_rect.x() == 0 ? 0 : pattern_rect_info_item->m_rect.x();
		double ncc_pattern_y = pattern_rect_info_item->m_rect.y() == 0 ? 0 : pattern_rect_info_item->m_rect.y();
		double ncc_pattern_width = pattern_rect_info_item->m_rect.width() == 0 ? really_imageItem_width : pattern_rect_info_item->m_rect.width();
		double ncc_pattern_height = pattern_rect_info_item->m_rect.height() == 0 ? really_imageItem_height : pattern_rect_info_item->m_rect.height();
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
		return;
	}


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
	fileName = ModelPath;
	qgraphicsScene->clear();
	qgraphicsScene->setSceneRect(0, 0, image.width(), image.height());
	ImageItem = qgraphicsScene->addPixmap(QPixmap::fromImage(image));
	qgraphicsScene->update();

	QRectF bounds = qgraphicsScene->itemsBoundingRect();
	bounds.setWidth(bounds.width());
	bounds.setHeight(bounds.height());
	ui->graphicsView->fitInView(bounds, Qt::KeepAspectRatio);
	state_flag_maindow = CHOOSE_PICTURE;
	ImageItem_IMAGE = image.copy();
}

//
void NCCMainWindow::slot_receiveDrawPoint(QPoint resultPoint, int totalModelTime)
{
	QPixmap image = ImageItem->pixmap();
	int sourceX = 0;
	int sourceY = 0;
	if (source_rect_info!=nullptr)
	{
		sourceX = source_rect_info->m_rect.x();
		sourceY = source_rect_info->m_rect.y();
	}
	int x = resultPoint.x() + sourceX;
	int y = resultPoint.y() + sourceY;
	Mat matImag = QImageToMat(image.toImage());


	QString str = tr("size(%1,%2),centerPoint(%3,%4),totaltime(%5)").
		arg(ImageItem->pixmap().width()).
		arg(ImageItem->pixmap().height()).
		arg(x).arg(y).arg(totalModelTime);
	ui->statusbar->showMessage(str);

	Mat showImage = matImag.clone();
	cv::circle(showImage, cv::Point(x, y), 8, cv::Scalar(0, 255, 0), -1);
	//����puttext
	std::string text = str.toStdString();
	int font_face = cv::FONT_HERSHEY_COMPLEX;
	double font_scale = 1;
	int thickness = 2;
	int baseline = 0;
	//��ȡ�ı���ĳ���
	cv::Size text_size = cv::getTextSize(text, font_face, font_scale, thickness, &baseline);
	
	//�ı�λ��
	Point origin;
	origin.x = x- text_size.width / 2;
	origin.y = y + text_size.height +50 ;
	
	//��������
	Scalar color = Scalar(0, 0, 255);//������ɫ
	int lineType = 8;//�����߶�����
	bool bottomLoftOrigin = false;//������Զ��

	//�����ı�
	putText(showImage, text, origin, font_face, font_scale, color, thickness, lineType, bottomLoftOrigin);

	cv::resize(showImage, showImage, Size(showImage.cols / 2, showImage.rows / 2), 0, 0);


	cv::imshow("TestResult", showImage);


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

void NCCMainWindow::on_action_CreateProject()
{
	//todo �������ֽ���
	// �����ļ���ѡ��Ի���
	QFileDialog dialog(nullptr, "Select or Create Folder", QCoreApplication::applicationDirPath());
	dialog.setFileMode(QFileDialog::Directory);
	dialog.setOption(QFileDialog::ShowDirsOnly, true);

	// ��ʾ�Ի��򲢻�ȡ�û�ѡ����ļ���·��
	if (dialog.exec() == QDialog::Accepted)
	{
		folderPathChoose = dialog.selectedFiles().first();
		//QDir dir(folderPathChoose);
		//QString folderName = dir.dirName();
	}
	else
	{
		qDebug() << "No folder selected.";
		return;
	}

}



void NCCMainWindow::on_deleteOrder_click()
{

}

void NCCMainWindow::saveOnceOrderItem()
{
	//���ʣ�µ�
	qgraphicsScene->clear();
	//���������Ϣ
		if (source_rect_List) {
			// ����б��е���
			source_rect_List->clear();

			//// ɾ���б���
			//delete source_rect_List;

			//// ��ָ������Ϊnullptr
			//source_rect_List = nullptr;
		}
	//����Ҳ�list
		ui->listWidget->clear();

	//����ini�ļ���Ϣ
	


}
