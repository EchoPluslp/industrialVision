#include "industrialVision.h"


// �����ﶨ��ȫ�ֱ���
 Dimension dimensions;
 std::mutex mtx;
 std::condition_variable Setcv;
 bool readyToModify;
 int m_rotateIndexInt = 0;
 int defaultRotateIndexValue = 0;
industrialVision::industrialVision(QWidget *parent)
	: QMainWindow(parent)
{
    industrialVision::ui.setupUi(this);

	QFont myFont;
	//�������ִ�С
	//myFont.setPointSize(30);
	//������������
	setFont(myFont);

	ui.menuBar->setStyleSheet("QMenu::item{padding:5px 15px;margin:1px 2px;color:#222222;font - size:40px;}");
	ui.lcdNumber->setDigitCount(8);
	ui.lcdNumber->setSegmentStyle(QLCDNumber::Flat);
	ui.lcdNumber->display(TimeRecord->toString("hh:mm:ss"));

	//�������
	m_pcMyCamera = new CMvCamera;

	//����̶߳���ʵ����
	m_cameraThread = new CameraThread();

	//�����̶߳���ʵ����
	m_processingThread = new ProcessingThread();

	connect(m_processingThread, &ProcessingThread::signal_newPixmap, this, &industrialVision::slot_displayPixmap, Qt::BlockingQueuedConnection);
	
	connect(m_processingThread, &ProcessingThread::signal_modelPictureReadFlag, this, &industrialVision::slot_modelPictureReadFlag, Qt::BlockingQueuedConnection);

	connect(m_processingThread, &ProcessingThread::signal_patternResult, this, &industrialVision::slot_get_patternResult, Qt::BlockingQueuedConnection);

	connect(this, &industrialVision::singal_sendPatternImage, m_processingThread, &ProcessingThread::slot_recievePatternImage, Qt::QueuedConnection);
	
	connect(this, &industrialVision::singal_sendPatternImageWithMaskEllipse, m_processingThread, &ProcessingThread::slot_recievePatternImageWithMask, Qt::QueuedConnection);

	connect(this, &industrialVision::singal_sendPatternImageWithMaskPolygon, m_processingThread, &ProcessingThread::slot_recievePatternImageWithPolygonMask, Qt::QueuedConnection);


	connect(this, &industrialVision::signal_processThread_pattren, m_processingThread, &ProcessingThread::slot_processThread_Pattren, Qt::QueuedConnection);

	connect(this, &industrialVision::send_Grade, m_processingThread, &ProcessingThread::set_Grade, Qt::QueuedConnection);

	connect(this, &industrialVision::openSourceArea, m_processingThread, &ProcessingThread::slot_setSourceArea, Qt::QueuedConnection);
	
	connect(this, &industrialVision::sendInfo_shapeMatch_Value, m_processingThread, &ProcessingThread::get_Info_From_industrial, Qt::QueuedConnection);

	connect(this, &industrialVision::sendInfo_shapeMatch_pictureInfo, m_processingThread, &ProcessingThread::get_Info_From_industrial_pictureInfo, Qt::QueuedConnection);


	connect(this, &industrialVision::sendInfo_shapeMatch_CIRCLE, m_processingThread, &ProcessingThread::get_Info_From_industrial_circle, Qt::QueuedConnection);

	connect(&TransmitSignals::GetInstance(), &TransmitSignals::create_once_pattern, m_processingThread, &ProcessingThread::slot_processThread_Pattren);
	
	connect(&TransmitSignals::GetInstance(), &TransmitSignals::sendToIndustrString, this, &industrialVision::addTextBrower);

	connect(this, &industrialVision::sendResultToServer, &TransmitSignals::GetInstance(), &TransmitSignals::send_pattern_result);
    

	/////////////////////
	//auto action_shizixian = new QAction();
	//action_shizixian->setText("ʮ����");
	//action_shizixian->setFont(QFont(tr("����"), 17, QFont::Bold, false));//����26�ţ��Ӵ֣�б����

	//connect(action_shizixian, &QAction::triggered,
	//	this, &industrialVision::openShiShiPiPei);



	action_RestoreWindow = new QAction();
	action_RestoreWindow->setText("ʵʱ��ʾ");
	action_RestoreWindow->setFont(QFont(tr("����"), 40, QFont::Bold, false));
	ui.menuBar->addAction(action_RestoreWindow);
	connect(action_RestoreWindow, &QAction::triggered,
		this, &industrialVision::restoreWindow);

	action_SetModelFile = new QAction();
	action_SetModelFile->setFont(QFont(tr("����"), 60, QFont::Bold, false));//����26�ţ��Ӵ֣�б����
    action_SetModelFile->setText("���̵���");
    ui.menuBar->addAction(action_SetModelFile);
    connect(action_SetModelFile, &QAction::triggered,
        this, &industrialVision::setModelXMLFile);
	action_SetModelFile->setEnabled(true);

	 
	action_SetAttributes = new QAction();
	action_SetAttributes->setText("ͼ������");
	action_SetAttributes->setFont(QFont(tr("����"),40, QFont::Bold, false));
	connect(action_SetAttributes, &QAction::triggered,
		this, &industrialVision::action_AttributesSet);

	action_password = new QAction();
	action_password->setText("��������");
	action_password->setFont(QFont(tr("����"), 40, QFont::Bold, false));
	connect(action_password, &QAction::triggered,
		this, &industrialVision::actionPasswordAction);

	 action_setLogoPath = new QAction();
	action_setLogoPath->setText("��־����");
	action_setLogoPath->setFont(QFont(tr("����"), 40, QFont::Bold, false));
	connect(action_setLogoPath, &QAction::triggered,
		this, &industrialVision::actionLogAndPathAction);

	SettingMenus = new QMenu("&����", ui.menuBar);
	ui.menuBar->addMenu(SettingMenus);
	
	auto action_userSwitch = new QAction();
	action_userSwitch->setText("�û��л�");
	action_userSwitch->setFont(QFont(tr("����"), 40, QFont::Bold, false));
	ui.menuBar->addAction(action_userSwitch);
	connect(action_userSwitch, &QAction::triggered,
		this, &industrialVision::actionuserSwitch);

	auto action_helpInfo = new QAction();
    action_helpInfo->setText("����");
    action_helpInfo->setFont(QFont(tr("����"),40, QFont::Bold, false));
	ui.menuBar->addAction(action_helpInfo);
	connect(action_helpInfo, &QAction::triggered,
		this, &industrialVision::helpInfo); 

    connect(this, &industrialVision::sendResultToServer, &TransmitSignals::GetInstance(), &TransmitSignals::send_pattern_result);
    connect(this, &industrialVision::SLOTAppendText, this, &industrialVision::addTextBrower);

	connect(timer, &QTimer::timeout, this, &industrialVision::updateTime);

	connect(&connectValual, &connectServer::send_createServerCallBack, this, &industrialVision::receive_ServerCreateInfo);
	
	connect(ui.openGLWidget, &MyGLWidget::rotateIndexValueChanged, this, &industrialVision::getRotateValue,Qt::QueuedConnection);
	
	connect(this, &industrialVision::setdefultCamare, ui.openGLWidget, &MyGLWidget::setMouseClickFlag, Qt::QueuedConnection);

	
    //���������
	connectValual.create_server();
	ui.pushButton_manualOperation->setEnabled(false);

    //���ñ���ɫ 
	ui.menuBar->setStyleSheet("color: rgb(255, 255, 255); background-color: rgba(26, 45, 77, 1);");
	setButtonClickLimits(false);
	//��ɫ������Ͻ�ͼ��
    setWindowTitle("V-Gp System V1.0");
	setWindowIcon(QIcon("icon.ico"));
     AppendText("ϵͳ�����ɹ�",Green);
}


 void industrialVision::click_continuousOperation() {
     
     AppendText("�������������ť",Green);
	 if (m_bOpenDevice) {
		 AppendText("����Ѿ�����", Red);
		 QMessageBox::critical(this, "������Ϣ", "����Ѿ�����");
		 return;
	 }


	 	 
	 if (!DisplayWindowInitial()) {
		 AppendText("���״̬�쳣", Red);
		 return;
	 }
	 //���ð�ťȨ��
	 setButtonClickLimits(true);

	 if (!m_bOpenDevice)
	 {
		 m_bOpenDevice = true;
	 }

		 if (m_pcMyCamera->IsDeviceConnected())
		 {
			 m_pcMyCamera->StartGrabbing();   //��������ɼ�
			 if (!m_cameraThread->isRunning())
			 {
				 m_cameraThread->setSwitchFlag(true);
				 m_cameraThread->start();
			 }
			 if (!m_processingThread->isRunning())
			 {
				 m_processingThread->setSwitchFlag(true);
				 m_processingThread->start();
			 }
		 }
		 Setinitializationparameters();
         //��ȡ����ͺ�
			 // ch:ö�������������豸 | en:Enumerate all devices within subnet
		 int nRet = CMvCamera::EnumDevices(MV_GIGE_DEVICE , &m_stDevList);

		 // ch:��ֵ���뵽��Ϣ�б���в���ʾ���� | en:Add value to the information list box and display
		 for (unsigned int i = 0; i < m_stDevList.nDeviceNum; i++)
		 {
			 MV_CC_DEVICE_INFO* pDeviceInfo = m_stDevList.pDeviceInfo[i];
			 if (NULL == pDeviceInfo)
			 {
				 continue;
			 }

			 wchar_t* pUserName = NULL;
			 if (pDeviceInfo->nTLayerType == MV_GIGE_DEVICE)
			 {
				 int nIp1 = ((m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp & 0xff000000) >> 24);
				 int nIp2 = ((m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp & 0x00ff0000) >> 16);
				 int nIp3 = ((m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp & 0x0000ff00) >> 8);
				 int nIp4 = (m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp & 0x000000ff);
				 if (strcmp("", (char*)pDeviceInfo->SpecialInfo.stGigEInfo.chUserDefinedName) != 0)
				 {
					 DWORD dwLenUserName = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(pDeviceInfo->SpecialInfo.stGigEInfo.chUserDefinedName), -1, NULL, 0);
					 pUserName = new wchar_t[dwLenUserName];
					 MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(pDeviceInfo->SpecialInfo.stGigEInfo.chUserDefinedName), -1, pUserName, dwLenUserName);
				 }
				 else
				 {
					 char strUserName[256] = { 0 };
					 sprintf_s(strUserName, 256, "%s %s (%s)", pDeviceInfo->SpecialInfo.stGigEInfo.chManufacturerName,
						 pDeviceInfo->SpecialInfo.stGigEInfo.chModelName,
						 pDeviceInfo->SpecialInfo.stGigEInfo.chSerialNumber);
					 DWORD dwLenUserName = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(strUserName), -1, NULL, 0);
					 pUserName = new wchar_t[dwLenUserName];
					 MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(strUserName), -1, pUserName, dwLenUserName);
				 }
				 ui.lineEdit_2->setText(QString::fromStdWString(pUserName));
			 }
			 else
			 {
				 AppendText("�޷�ʶ����豸", Red);
				 QMessageBox::critical(this, "������Ϣ", "�޷�ʶ����豸");
			 }
		 }

		 //��ʱ����ʼ��ʱ
		 timer->start(1000);
		 m_bStartGrabbing = true;
}

bool industrialVision::nativeEvent(const QByteArray& eventType, void* message, long* result)
{
	MSG* msg = reinterpret_cast<MSG*>(message);
	switch (msg->message)
	{
	case WM_GETMINMAXINFO:
	{
		//���QT�ޱ߿�������ڸ�Windows����������
		if (this->isMaximized())
		{
			int index = QApplication::desktop()->screenNumber(this);
			const QRect rc = QApplication::desktop()->availableGeometry(index);
			MINMAXINFO* p = (MINMAXINFO*)(msg->lParam);
			p->ptMaxPosition.x = 0;
			p->ptMaxPosition.y = 0;
			p->ptMaxSize.x = rc.width();
			p->ptMaxSize.y = rc.height();
			*result = ::DefWindowProc(msg->hwnd, msg->message, msg->wParam, msg->lParam);
			return true;
		}
	}
	}
	return QMainWindow::nativeEvent(eventType, message, result);
}

void industrialVision::sendImgToVisualContinue(QString data)
{ 
    visualTemplate_Control = !visualTemplate_Control;
}

void industrialVision::click_createServer()
{
	connectValual.show();

} 

void industrialVision::receive_ServerCreateInfo(QString flag)
{
	if (flag == "TRUE")
	{
		AppendText("����˿����ɹ�", Green);
		return;
	}
	AppendText("������Ѿ��������߿���ʧ��", Red);
}

//void industrialVision::click_editVision()
//{
//    connect(&createModelItem, &createModel::getImageFromCamera, this, &industrialVision::getImageOneFrame,Qt::UniqueConnection);
//
	//connect(this, &industrialVision::cameraTovisualTemplate,&createModelItem, &createModel::sendImgToFileController, Qt::UniqueConnection);
//
//    connect(&createModelItem, &createModel::sendXMLPath , this, &industrialVision::getXMLPATH, Qt::UniqueConnection);
//
//	connect(createModelItem.fileController2D, &FileController::sendImageToPattern, m_processingThread, &ProcessingThread::slot_processMatchPicture, Qt::UniqueConnection);
//	
//	connect(createModelItem.fileController2D, &FileController::sendImageToPatternWithMask, m_processingThread, &ProcessingThread::slot_processMatchPictureWithMask, Qt::UniqueConnection);
//
//	connect(m_processingThread, &ProcessingThread::QPointSendtoFileControl,createModelItem.fileController2D, &FileController::slot_receiveDrawPoint, Qt::UniqueConnection);
//
//
//    createModelItem.show();
//    //����xml�ļ�
//    AppendText("���Ӿ�ģ�����",Green);
//}

//�µ�ģ��ƥ��~,����״ƥ���ģ��ƥ���ں�����
void industrialVision::click_editVision()
{
	connect(this, &industrialVision::cameraTovisualTemplate, &nccmatchWindow, &NCCMainWindow::sendImgToControllerShape, Qt::UniqueConnection);

	connect(&nccmatchWindow, &NCCMainWindow::getImageFromCamera, this, &industrialVision::getImageOneFrame, Qt::UniqueConnection);
	
	connect(&nccmatchWindow, &NCCMainWindow::sendImageToPattern, m_processingThread, &ProcessingThread::slot_processMatchPicture, Qt::UniqueConnection);

	connect(m_processingThread, &ProcessingThread::QPointSendtoFileControl,&nccmatchWindow, &NCCMainWindow::slot_receiveDrawPoint, Qt::UniqueConnection);

	//����ini·����������
    connect(&nccmatchWindow, &NCCMainWindow::sendINIPath , this, &industrialVision::getXMLPATH, Qt::UniqueConnection);

	nccmatchWindow.show();
    //����xml�ļ�
    AppendText("���Ӿ�ģ�����",Green);
}


void industrialVision::getImageOneFrame(QString imageType) {
	// ����һ����ʱ��
	QTimer timer;
	timer.setSingleShot(true); // ����Ϊ���δ���

	// ���Ӷ�ʱ����timeout�źŵ�һ���ۺ������òۺ����ڶ�ʱ����ʱʱ����
	QObject::connect(&timer, &QTimer::timeout, [&]() {
		if (imageType == "patternMatch")
		{
		emit cameraTovisualTemplate( QImage(), QString());
		}
		else {
			emit cameraTovisualTemplateShape(QImage());

		}
		});

	timer.start(1000); // ������ʱ�������ó�ʱʱ��Ϊ1��


	while (m_imageVector_1.isEmpty())
	{
		QCoreApplication::processEvents(); // ����Qt�¼�����
	}
	//��ʱ��ֹͣ
	timer.stop();
	timer.deleteLater();

	cv::Mat tempMat;
	 m_imageVector_1.at(0).copyTo(tempMat);
	myImageToModel = Mat2QImage(tempMat);
	int wid = myImageToModel.width();
	int height = myImageToModel.height();

 /// /////////////////////
	  
    char   imagePath[256];
	time_t nowtime;
	time(&nowtime); 
    sprintf_s(imagePath, 256, "Image_w%d_h%d_fn%03d.bmp", myImageToModel.width(), myImageToModel.height(), nowtime);
	
	myImageToModel.setText("name", QString::fromStdString(imagePath));

	QTextCodec* codec = QTextCodec::codecForName("GB2312");
	QTextCodec::setCodecForLocale(codec);
	
	QString dirpath = QApplication::applicationDirPath() + "/model/";
	string str = codec->fromUnicode(dirpath).data();


    string path = str + "/" + imagePath;
	QString ModelPath = QString(QString::fromLocal8Bit(path.c_str()));
	 if (imageType == "patternMatch")
	 {
		 emit cameraTovisualTemplate(myImageToModel, ModelPath);
	 }
	 else {
		 emit cameraTovisualTemplateShape(myImageToModel);

	 }
}

void industrialVision::createOncePattern()
{		
	if (resultPointF.x()==1&& resultPointF.y()==1)
	{
		AppendText("����ʾ����������ƥ�����,ƥ��ɹ�", Green);
		return;
	}
	else {
		AppendText("�����󡿴�������ƥ�����,ƥ��ʧ��", Red);

	}
		  if (resultPointF.x() != -m_width && resultPointF.y() != -m_height)
      {
          AppendText("����ʾ����������ƥ�����,ƥ��ɹ�",Green);
		  char xxx[10];
		  char yyy[10];
		  sprintf(xxx, "%.1f", finall_Total_Result.ptCenter.x);
		  sprintf(yyy, "%.1f", finall_Total_Result.ptCenter.y);

          QString  resultFont  = "x:����";

          resultFont.append(QString::fromLocal8Bit(xxx));
		  resultFont.append(",");
		  resultFont.append("y:����");
		  resultFont.append(QString::fromLocal8Bit(yyy));
		  resultFont.append(".");
		  resultFont.append("ƥ����ʱ:");
		  resultFont.append(QString::number(matchTime_total));

		  AppendText(resultFont,Green);
		  total_count++;

          OK_count++;
          ui.texstBrowser_titleStatus->clear();
          ui.texstBrowser_titleStatus->setPixmap(QPixmap("Image/industory/OK.png"));
	  }
      else {
          AppendText("�����󡿴�������ƥ�����,ƥ��ʧ��",Red);
          ui.texstBrowser_titleStatus->clear();
		  total_count++;

		  ui.texstBrowser_titleStatus->setPixmap(QPixmap("Image/industory/NG.png"));
      }
      ui.run_time_edit_count->clear();
	  QString setCount(QString::number(OK_count));
      setCount.append("/");
      setCount.append(QString::number(total_count));
      ui.run_time_edit_count->setText(setCount);
}

void industrialVision::addTextBrower(QString text,QString flag)
{
    if (flag == Green)
    {
		ui.textBrowser_record->setTextColor(Qt::green);
    }
    else if(flag == Red) {
		ui.textBrowser_record->setTextColor(Qt::red);	                                             
	}
	else {
		ui.textBrowser_record->setTextColor(Qt::gray);
	}

    QDateTime datetime = QDateTime::currentDateTime();
        QString now = datetime.toString("yyyy-MM-dd HH:mm:ss.zzz");
    ui.textBrowser_record->append(now);
    ui.textBrowser_record->append(text);
    ui.textBrowser_record->append("............");
    
	QString settingPath = QCoreApplication::applicationDirPath() + "/setting.ini";
	QSettings* settings = new QSettings(settingPath, QSettings::IniFormat);
	settings->beginGroup("Idus");
	//
	QString logDirectory = settings->value("logDirectory").toString();
    if (logDirectory.isEmpty()) {
        logDirectory = QCoreApplication::applicationDirPath();
    }
    QDir dir;
	if (!dir.exists(logDirectory))  
	{
		bool res = dir.mkpath(logDirectory);
		if (res != true)
		{
			ui.textBrowser_record->append("����ʾ��log�ļ��д���ʧ�ܡ�\r\n");
			return;
		}
	}
	QString str_now = datetime.toString("yyyy_MM_dd_hh");
	QString filename = logDirectory + "\\" + "idav_" + str_now + ".log";
	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
	{
		ui.textBrowser_record->append("����ʾ����־�ļ���ʧ�ܡ�\r\n"+ file.fileName());
		return;//���ļ�ʧ��
	}
	file.write(now.toUtf8()+"\r\n");
	file.write(text.toUtf8()+"\r\n");
	file.close();
}

void industrialVision::getXMLPATH(QString xmlPath)
{
    AppendText("��ȡxml·��:"+ xmlPath,Green);
    m_xmlpath = xmlPath;
	if (read_info_from_ini(m_xmlpath)) {
	AppendText("����xmlģ��ɹ�" + xmlPath, Green);
	}
	else {
		if (!m_processingThread->getmodelAndRealSclar())
		{
			AppendText("XML ģ��ͼ�뵱ǰ���չʾ��ͼƬ������һ��,�޷�ƥ��" + xmlPath, Gray);
			return;
		}
		AppendText("����xmlģ��ʧ��" + xmlPath, Red);
	}
}
void industrialVision::AppendText(const QString& text,QString flag)
{
    emit SLOTAppendText(text, flag);
}

void industrialVision::restoreWindow()
{
    if (smallwindow_flag) {
		smallwindow_flag = false;
        smallwindow_button_click();
        return;
    }
    smallwindow_flag = true;
	for (int i = 0; i < ui.verticalLayout_4->count(); ++i) {
		QWidget* w = ui.verticalLayout_4->itemAt(i)->widget();
		if (w != NULL)
			w->setVisible(small_window_flag);
	}
	for (int i = 0; i < ui.horizontalLayout->count(); ++i) {
		QWidget* w = ui.horizontalLayout->itemAt(i)->widget();
		if (w != NULL)
			w->setVisible(small_window_flag);
	}
	ui.list_widget->setVisible(small_window_flag);
    ui.widget->setVisible(small_window_flag);
    //
	QList<QAction*> actions = ui.menuBar->actions();
	for (int i = 0; i < actions.size(); i++)
	{
		QAction* actionItem = actions.at(i);
		if (actionItem->text() == "�˳�ʵʱ��ʾ")
		{
			actionItem->setText("ʵʱ��ʾ");
			continue;
		}
		actionItem->setVisible(small_window_flag);
	}
    ui.textEdit_2->setVisible(small_window_flag);
	ui.textBrowser_record->setVisible(small_window_flag);

    ///��������
	small_window_flag = !small_window_flag;
	//���ô��ڴ�С
	
	setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
	setMinimumSize(0, 0);
	//setGeometry(QApplication::desktop()->availableGeometry());
	
	setWindowFlags(Qt::WindowMinimizeButtonHint |
                   Qt::WindowMaximizeButtonHint |Qt::WindowCloseButtonHint);
	//setWindowState(Qt::WindowFullScreen);
	//setWindowState(Qt::WindowMinimized);
	//setWindowState(Qt::WindowMaximized);
	setGeometry(originalGeometry);

    show();
     //������Ƶչʾ����ı���
   // setFixedSize(this->width(), this->height());

}

bool industrialVision::compareAspectRatio(QSize smallPicture)
{
	//�ȴ�ȡ���߳�ִ��,֮�˺����m_wideth��m_height
	if (smallPicture.width()<smallPicture.height())
	{
		if (m_width < m_height) { return true; }
		else
		{
			return false;
		}
	}
	else if (smallPicture.width() > smallPicture.height()) {
		if (m_width > m_height) { return true; }
		else
		{
			return false;
		}
	}
	else
	{
		return true;
	}
	return false;
}

QPixmap industrialVision::MatToPixmap(const cv::Mat cvMat)
{
	QImage myImage = QImage((const unsigned char*)(cvMat.data), cvMat.cols, cvMat.rows, QImage::Format_Indexed8);   //�Ҷ�ͼ

//QSize��ʾͼ���ڽ����ϵ���ʾ�ߴ�    
	return QPixmap::fromImage(myImage).scaled(QSize(myImage.width(), myImage.height()), Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

QPoint industrialVision::getCenterPointFromCircle(QList<QPoint> listCircle)
{
	if (listCircle.isEmpty())
	{
		return QPoint();
	}
	int min_x = listCircle.at(0).x();
	int min_y = listCircle.at(0).y();
	int max_x = listCircle.at(0).x();
	int max_y = listCircle.at(0).y();

	for (const QPoint& point : listCircle) {
		if (point.x() < min_x) {
			min_x = point.x();
		}
		else if (point.x() > max_x) {
			max_x = point.x();
		}

		if (point.y() < min_y) {
			min_y = point.y();
		}
		else if (point.y() > max_y) {
			max_y = point.y();
		}
	}

	return QPoint(min_x + (max_x - min_x) / 2, min_y + (max_y - min_y) / 2);
}

void industrialVision::setCURRENT_ROLE(QString currentROle)
{
	this->CURRENT_ROLE = currentROle;
	//���������ӵ�ǰȨ��
	QString title(windowTitle());
	title.append("[" + CURRENT_ROLE + "]");
	setWindowTitle(title);
	SettingMenus->clear();
	//�������ý���,����Ա�Ͳ���Ա�ֿ�
	if (CURRENT_ROLE == ROLE_ADMIN)
	{
		//ͼ������                                                                                                                                                                                                                   
		SettingMenus->addAction(action_setLogoPath);
		//��־����
		SettingMenus->addAction(action_SetAttributes);
	}
	//��������
	SettingMenus->addAction(action_password);
}

//��������ť
void industrialVision::openShiShiPiPei()
{
    if (sourceAreaOn) {
		AppendText("����������", Green);
		//ui.pushButton->setStyleSheet("/* ֤ȯ */ QPushButton::hover { background-color: #1450C7; } position: absolute; left: 1px; top: 67px; width: 190px; height: 61px; opacity: 1; /* ����/4 ҳǩѡ��ɫ */ background: #285790; color: rgb(255, 0, 0); box-sizing: border-box; border: 1px solid ; border-image: linear-gradient(180deg, rgba(35,102,211,0.00) 0%, #3797FE 100%) 1;");
    }
    else {
		AppendText("��������ر�", Red);
		//ui.pushButton->setStyleSheet("/* ֤ȯ */ QPushButton::hover { background-color: #1450C7; } position: absolute; left: 1px; top: 67px; width: 190px; height: 61px; opacity: 1; /* ����/4 ҳǩѡ��ɫ */ background: #285790; color: rgb(255, 255, 255); box-sizing: border-box; border: 1px solid ; border-image: linear-gradient(180deg, rgba(35,102,211,0.00) 0%, #3797FE 100%) 1;");
    }
	
	emit openSourceArea(sourceAreaOn);
	sourceAreaOn = !sourceAreaOn;
}

void industrialVision::openshizixian()
{
    shizixian = !shizixian;
}

void industrialVision::setModelXMLFile()
{
        QString path = QFileDialog::getOpenFileName(nullptr, tr("Open File"), ".",
			tr("�����ļ�(*ini)"));
    if (!path.isEmpty()) {
		QFileInfo fileInfo(path);
			 if (fileInfo.suffix().toLower() == "ini"){
			// �ļ���׺������ini����������
			if (!read_info_from_ini(path))
			{
				QMessageBox::warning(nullptr, tr("Path"),
					tr("INIģ��ͼ�뵱ǰ���չʾ��ͼƬ������һ��,�޷�ƥ��"));
			}
			else {
				QMessageBox::warning(nullptr, tr("Path"),
					tr("INIģ��ͼ����ɹ�"));
			}
		}
		else {
			QMessageBox::warning(nullptr, tr("Path"),
				tr("ģ�����."));
		}
    }
    else {
        QMessageBox::warning(nullptr, tr("Path"),
            tr("δѡ��INIģ��."));
    }
}

void industrialVision::rotatePicture()
{
    rotate_image = !rotate_image;
}

//���浱ǰ״̬ΪĬ��״̬������ ,�����´δ�
void industrialVision::click_manualOperation()
{
	
	emit setdefultCamare(defaultCamcare);
	//����������������޸Ļ��߿��Ը���
	ui.gain_edit->setDisabled(defaultCamcare);
	ui.framerate_edit->setDisabled(defaultCamcare);
	ui.grade_edit->setDisabled(defaultCamcare);
	ui.exposure_edit->setDisabled(defaultCamcare);

	if (defaultCamcare)
	{
		ui.gain_edit->setStyleSheet("background-color: grey;");
		ui.framerate_edit->setStyleSheet("background-color: grey;");
		ui.grade_edit->setStyleSheet("background-color: grey;");
		ui.exposure_edit->setStyleSheet("background-color: grey;");	
		ui.lineEdit_2->setStyleSheet("background-color: grey;");
		ui.width_edit->setStyleSheet("background-color: grey;");
		ui.height_edit->setStyleSheet("background-color: grey;");
		ui.pixelformat_edit->setStyleSheet("background-color: grey;");

		//��������ɫ�޸�Ϊ��ɫ
		ui.pushButton_manualOperation->setStyleSheet("/* ֤ȯ */ QPushButton::hover { background-color: #1450C7; } position: absolute; left: 1px; top: 67px; width: 190px; height: 61px; opacity: 1; /* ����/4 ҳǩѡ��ɫ */ background: #285790; color: rgb(255, 0, 0); box-sizing: border-box; border: 1px solid ; border-image: linear-gradient(180deg, rgba(35,102,211,0.00) 0%, #3797FE 100%) 1;");
		ui.pushButton_manualOperation->setText("����״̬");

		//����״̬,���õ�ǰ��ת����ΪĬ�Ϸ���
		defaultRotateIndexValue = m_rotateIndexInt;
	}
	else {
		ui.gain_edit->setStyleSheet("background-color: #10171F;");
		ui.framerate_edit->setStyleSheet("background-color: #10171F;");
		ui.grade_edit->setStyleSheet("background-color: #10171F;");
		ui.exposure_edit->setStyleSheet("background-color: #10171F;");
		ui.lineEdit_2->setStyleSheet("background-color: #10171F;");
		ui.width_edit->setStyleSheet("background-color: #10171F;");
		ui.height_edit->setStyleSheet("background-color: #10171F;");
		ui.pixelformat_edit->setStyleSheet("background-color: #10171F;");

		//��������ɫ�޸�ΪĬ����ɫ
		ui.pushButton_manualOperation->setStyleSheet("/* ֤ȯ */ QPushButton::hover { background-color: #1450C7; } position: absolute; left: 1px; top: 67px; width: 190px; height: 61px; opacity: 1; /* ����/4 ҳǩѡ��ɫ */ background: #285790; color: rgb(255, 255, 255); box-sizing: border-box; border: 1px solid ; border-image: linear-gradient(180deg, rgba(35,102,211,0.00) 0%, #3797FE 100%) 1;");
		ui.pushButton_manualOperation->setText("����״̬");
	}
	defaultCamcare = !defaultCamcare;
}


bool industrialVision::getPatternInfoFromXML(QString path)
{
    QFile xml_MODEL_FILE(path);         //��Ҫ�򿪵��ļ�
    if (!xml_MODEL_FILE.exists()) {
        AppendText("ģ���ļ�������",Red);
        return false;
    }
    QDomDocument doc;
    QRectF areaNode;
    QRectF patternArea;
    cv::Rect srcRect;
    QString pattern_Path;
	QList<QPoint> QpointList;
	int currentPattern_OBJ;
    if (!doc.setContent(&xml_MODEL_FILE))
    {
        xml_MODEL_FILE.close();
        return false;
    }
    xml_MODEL_FILE.close();
	//���°Ѹ�������ÿ�
	resetParameters();
	//��ȡxml�ļ�����
    QDomElement root = doc.documentElement();
    QDomElement elem = root.cloneNode(true).toElement();
    for (int i = 0; i < elem.childNodes().count(); i++) {
        auto typeElem = elem.childNodes().at(i).toElement();
		int childcount = typeElem.childNodes().count();
        for (int j = 0; j < childcount; j++) {
            auto labelElem = typeElem.childNodes().at(j).toElement();
			 
            if (labelElem.tagName() == "Label") {

                Label* label = new Label;
                //��ǰtype����
				QString typeName = typeElem.attribute("Type");
                Shape* currentShape = label->createFromElementToPattern(labelElem);
                QSize small_Picture = currentShape->getCurrSize();
				//�жϵ�ǰͼ��ģ��ͼ�Ƿ����һ��
				if (!compareAspectRatio(small_Picture))
				{
					//����һ��
					m_processingThread->setmodelAndRealSclar(false);
					return false;							
				}																
				//һ��	
				m_processingThread->setmodelAndRealSclar(true); 
                if (typeName.contains("��������")) {
					areaNode = currentShape->getItem()->boundingRect();
					areaNodeREAL_size.setX(((double)areaNode.x() / small_Picture.width()) * m_width);
					areaNodeREAL_size.setY(((double)areaNode.y() / small_Picture.height()) * m_height);
					areaNodeREAL_size.setWidth(((double)areaNode.width() / small_Picture.width()) * m_width);
					areaNodeREAL_size.setHeight(((double)areaNode.height() / small_Picture.height()) * m_height);
					
					areaNodeREAL_size.setX(areaNodeREAL_size.x() < 0 ? 0 : areaNodeREAL_size.x());
					areaNodeREAL_size.setY(areaNodeREAL_size.y() < 0 ? 0 : areaNodeREAL_size.y());

					if (areaNodeREAL_size.x() + areaNodeREAL_size.width() > m_width)
					{
						areaNodeREAL_size.setWidth(m_width - areaNodeREAL_size.x());
					}
					if (areaNodeREAL_size.y() + areaNodeREAL_size.height() > m_height)
					{
						areaNodeREAL_size.setHeight(m_height - areaNodeREAL_size.y());

					}
				}
                else if (typeName.contains("��������")) {
					//�жϵ�ǰ������������������
					currentPattern_OBJ = currentShape->getType();
					
					if (currentPattern_OBJ == Shape::Rect || currentPattern_OBJ == Shape::Ellipse)
					{
					currentShape->getItem();
					patternArea = currentShape->getItem()->boundingRect();
					patternAreaREAL_size_rect.setX(((double)patternArea.x() / small_Picture.width()) * m_width);
					patternAreaREAL_size_rect.setY(((double)patternArea.y() / small_Picture.height()) * m_height);
					patternAreaREAL_size_rect.setWidth(((double)patternArea.width() / small_Picture.width()) * m_width);
					patternAreaREAL_size_rect.setHeight(((double)patternArea.height() / small_Picture.height()) * m_height);
					
					patternAreaREAL_size_rect.setX(patternAreaREAL_size_rect.x() < 0 ? 0 : patternAreaREAL_size_rect.x());
					patternAreaREAL_size_rect.setY(patternAreaREAL_size_rect.y() < 0 ? 0 : patternAreaREAL_size_rect.y());

					Area* m_area_item = new Area;
					//��patternImageName��ֵ���ڶ�ȡ����ͼ
					 pattern_Path = m_area_item->getFileName(labelElem.firstChildElement("Area")).trimmed();
					 SAFE_DELETE(m_area_item);
					
					 if (patternAreaREAL_size_rect.x() + patternAreaREAL_size_rect.width() > m_width)
					 {
						 patternAreaREAL_size_rect.setWidth(m_width - patternAreaREAL_size_rect.x());
					 }
					 if (patternAreaREAL_size_rect.y() + patternAreaREAL_size_rect.height() > m_height)
					 {
						 patternAreaREAL_size_rect.setHeight(m_height - patternAreaREAL_size_rect.y());
					 }
				}
					else if (currentPattern_OBJ == Shape::Polygon)
					{
						
						patternAreaREAL_size_polygon.clear();
						Area* m_area_item = new Area;
						//��patternImageName��ֵ���ڶ�ȡ����ͼ
						pattern_Path = m_area_item->getFileName(labelElem.firstChildElement("Area")).trimmed();
						SAFE_DELETE(m_area_item);

						MyGraphicsPolygonItem* currItem = (MyGraphicsPolygonItem*)currentShape->getItem();
						double ratio = double(m_width) / double(currentShape->m_currSize.width());

						QPolygonF prev = currItem->polygon();
						for (int i = 0; i < prev.count(); i++) {
							auto point = prev.at(i);
							patternAreaREAL_size_polygon << QPointF(point * ratio);
						}
					}
			}
				else if (typeName.contains("�����")) {
					//�ҵ�����㲢�һ�ȡ����������
					QDomElement elemPoint = labelElem.firstChildElement("Area");
					auto imageNodes = elemPoint.childNodes();
					for (int i = 0; i < imageNodes.count(); i++) {
						auto imageElem = imageNodes.at(i).toElement();

						auto shapeElemPoint = imageElem.firstChildElement("Shape");
						if (shapeElemPoint.isNull())
							return false;
						auto itemElem = shapeElemPoint.firstChildElement("Item");
						auto pointNodes = itemElem.childNodes();
						for (int count = 0; count < pointNodes.count(); count++) {
							QDomElement pointElem = pointNodes.at(count).toElement();
							QPoint point = QPoint(pointElem.attribute("X", "0").toInt(), pointElem.attribute("Y", "0").toInt());
							
							QpointList.append(point);
						}
					}
						if (!QpointList.isEmpty())
						{
							//�����������������
							centerPoint = getCenterPointFromCircle(QpointList);
							centerPoint.setX(((double)centerPoint.x() / small_Picture.width()) * m_width);
							centerPoint.setY(((double)centerPoint.y() / small_Picture.height()) * m_height);
							if (currentPattern_OBJ == Shape::Ellipse||currentPattern_OBJ == Shape::Rect )
							{
								patternRectCenterPoint.setX(patternAreaREAL_size_rect.x() + (patternAreaREAL_size_rect.width() / 2));
								patternRectCenterPoint.setY(patternAreaREAL_size_rect.y() + (patternAreaREAL_size_rect.height() / 2));
							}else if (currentPattern_OBJ == Shape::Polygon)
							{
								// ��QPolygonF�еĵ�����ת��Ϊvector<cv::Point>
								std::vector<cv::Point> points;
								for (const QPointF& point : patternAreaREAL_size_polygon) {
									points.emplace_back(static_cast<int>(point.x()), static_cast<int>(point.y()));
								}
								points.pop_back();

								cv::Rect boundingRect = cv::boundingRect(points);
								patternRectCenterPoint.setX(boundingRect.x + (boundingRect.width / 2));
								patternRectCenterPoint.setY(boundingRect.y + (boundingRect.height / 2));
							}

						}
					}
				}
			}
        }

    if (pattern_Path.isEmpty())
    {
		AppendText("���������ȡ����,����ģ���������ģ��ͼ",Red);
        return false;
    }
   if (areaNodeREAL_size.x()<0|| areaNodeREAL_size.y()<0)
   {
	   AppendText("���������������,����������", Red);
	   return false;
   }
   //��Χ��
   srcQRect.setX(areaNodeREAL_size.x());
   srcQRect.setY(areaNodeREAL_size.y());
   srcQRect.setWidth(areaNodeREAL_size.width());
   srcQRect.setHeight(areaNodeREAL_size.height());
   //û�����÷�Χ��,��Ĭ��ȫͼ
   if (srcQRect.x() == 0 && srcQRect.y() == 0 && srcQRect.width() == 0 && srcQRect.height() == 0) {
	   srcQRect.setWidth(m_width);
	   srcQRect.setHeight(m_height);
   }

   if(currentPattern_OBJ == Shape::Rect){
	if (patternAreaREAL_size_rect.x() < 0 || patternAreaREAL_size_rect.y() < 0)
		{
	   AppendText("���������������,����������", Red);
	   return false;
		}
		//���͸�peocessingthread�߳� ·��,ƥ��ģ�����ĵ�����,��Χͼ���ĵ�����
	m_processingThread->setShapeType(1);

		emit singal_sendPatternImage(pattern_Path, patternAreaREAL_size_rect,srcQRect, centerPoint, patternRectCenterPoint);
		return true;
	}else if (currentPattern_OBJ == Shape::Ellipse)
	{
		m_processingThread->setShapeType(0);

		emit singal_sendPatternImageWithMaskEllipse(pattern_Path, patternAreaREAL_size_rect, srcQRect, centerPoint, patternRectCenterPoint);
		return true;
	}
	else if (currentPattern_OBJ == Shape::Polygon)
   {
	   
	   m_processingThread->setShapeType(0);
	   emit singal_sendPatternImageWithMaskPolygon(pattern_Path, patternAreaREAL_size_polygon, srcQRect, centerPoint, patternRectCenterPoint);
	   return true;
   }

   return false;
}

void industrialVision::click_stopOperation() {
     if (FALSE == m_bOpenDevice || FALSE == m_bStartGrabbing || NULL == m_pcMyCamera)
     { 
         return;
     }
     int nRet = m_pcMyCamera->StopGrabbing();
	 timer->stop();
	
   m_cameraThread->setSwitchFlag(false);
	m_processingThread->setSwitchFlag(false);

	 m_bOpenDevice = false;
	 m_bStartGrabbing = FALSE;
	 return;
 }




industrialVision::~industrialVision()
{

	//����ر�ʱ��״̬
	SaveInitializationParameters();
	if (!open_mv)
		return;
    CloseDevice();

}

int industrialVision::GetTopLayer(Mat* matTempl, int iMinDstLength)
{
    int iTopLayer = 0;
    int iMinReduceArea = iMinDstLength * iMinDstLength;
    int iArea = matTempl->cols * matTempl->rows;
    while (iArea > iMinReduceArea)
    {
        iArea /= 4;
        iTopLayer++;
    }
    return iTopLayer;
}

bool industrialVision::DisplayWindowInitial()
{
	//��ʼ������
	//m_deviceNum = 0;
	m_bOpenDevice = false;

	//��ʼ�����
	memset(&m_stDevList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));
	//ö�������������豸
	int nRet = CMvCamera::EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &m_stDevList);
	if (MV_OK != nRet)
	{
		QMessageBox::warning(this, "����", "��������Ƿ��������ӣ�");
		return false;
	}
	 int m_deviceNum = m_stDevList.nDeviceNum;
	//if (m_deviceNum !=1)
	//{
	//	QMessageBox::warning(this, "����", "��������Ƿ��������ӣ�");
	//	return false;
	//}

		 //��ȡ�ĸ����
	 QString settingPath = QCoreApplication::applicationDirPath() + "/setting.ini";
	 QSettings settings(settingPath, QSettings::IniFormat);
	 // ��ȡ���з���
	 QStringList groups = settings.childGroups();
	 settings.beginGroup("Idus");

	 int cameraIndex = settings.value("cameraIndex").toInt();

		MV_CC_DEVICE_INFO* pDeviceInfo = m_stDevList.pDeviceInfo[cameraIndex];
		if(!m_pcMyCamera->IsDeviceConnected()){
			int nRet = m_pcMyCamera->Open(pDeviceInfo);   //�����
			if (MV_OK != nRet)
			{
				delete m_pcMyCamera;
				m_pcMyCamera = NULL;
				QMessageBox::warning(this, "����", "���豸ʧ�ܣ�");
				return false;
			}
			m_cameraThread->setCameraPtr(m_pcMyCamera);
			m_cameraThread->setCameraIndex(0);
			m_processingThread->setThreadId(0);
			open_mv = true;
			return	true;
		}
	
	return false;
}

void industrialVision::setButtonClickLimits(bool flag)
{
	//ui.pushButton_editVision->setEnabled(flag);
	action_SetModelFile->setEnabled(true);
	ui.pushButton_manualOperation->setEnabled(flag);
	//ui.pushButton->setEnabled(flag);
	ui.pushButton_stopOperation->setEnabled(flag);
}

//ͨ�÷��� ���ص���
void industrialVision::ShowErrorMsg(QString csMessage, int nErrorNum)
{
    QString errorMsg;
    if (nErrorNum == 0)
    {
        errorMsg.append(csMessage);
    }
    else
    {
        errorMsg.append(csMessage);
    }

    switch (nErrorNum)
    {
    case MV_E_HANDLE:           errorMsg += "Error or invalid handle ";                                         break;
    case MV_E_SUPPORT:          errorMsg += "Not supported function ";                                          break;
    case MV_E_BUFOVER:          errorMsg += "Cache is full ";                                                   break;
    case MV_E_CALLORDER:        errorMsg += "Function calling order error ";                                    break;
    case MV_E_PARAMETER:        errorMsg += "Incorrect parameter ";                                             break;
    case MV_E_RESOURCE:         errorMsg += "Applying resource failed ";                                        break;
    case MV_E_NODATA:           errorMsg += "No data ";                                                         break;
    case MV_E_PRECONDITION:     errorMsg += "Precondition error, or running environment changed ";              break;
    case MV_E_VERSION:          errorMsg += "Version mismatches ";                                              break;
    case MV_E_NOENOUGH_BUF:     errorMsg += "Insufficient memory ";                                             break;
    case MV_E_ABNORMAL_IMAGE:   errorMsg += "Abnormal image, maybe incomplete image because of lost packet ";   break;
    case MV_E_UNKNOW:           errorMsg += "Unknown error ";                                                   break;
    case MV_E_GC_GENERIC:       errorMsg += "General error ";                                                   break;
    case MV_E_GC_ACCESS:        errorMsg += "Node accessing condition error ";                                  break;
    case MV_E_ACCESS_DENIED:	errorMsg += "No permission ";                                                   break;
    case MV_E_BUSY:             errorMsg += "Device is busy, or network disconnected ";                         break;
    case MV_E_NETER:            errorMsg += "Network error ";                                                   break;
    }

  // MessageBox(errorMsg, TEXT("PROMPT"), MB_OK | MB_ICONWARNING);
    QMessageBox::critical(this, "������Ϣ", errorMsg);

}

int industrialVision::GetExposureTime()
{
	MVCC_FLOATVALUE stFloatValue = { 0 };

	int nRet = m_pcMyCamera->GetFloatValue("ExposureTime", &stFloatValue);
	if (MV_OK != nRet)
	{
		return nRet;
	}
	m_dExposureEdit = stFloatValue.fCurValue / 100.0;
    ui.exposure_edit->clear();
    ui.exposure_edit->setText(QString::number(m_dExposureEdit,'f', 0));

	return MV_OK;
}

int industrialVision::SetExposureTime()
{
	m_pcMyCamera->SetEnumValue("ExposureAuto", MV_EXPOSURE_AUTO_MODE_OFF);

	//������ʾ��ֵ�ǳ�����100��
    m_dExposureEdit = ui.exposure_edit->text().toFloat() * 100;
	return m_pcMyCamera->SetFloatValue("ExposureTime", (float)m_dExposureEdit);
}

int industrialVision::GetGain()
{
	MVCC_FLOATVALUE stFloatValue = { 0 };

	int nRet = m_pcMyCamera->GetFloatValue("Gain", &stFloatValue);
	if (MV_OK != nRet)
	{
		return nRet;
	}
	m_dGainEdit = stFloatValue.fCurValue;
	ui.gain_edit->clear();
    ui.gain_edit->setText(QString::number(m_dGainEdit, 'f', 1));
	return MV_OK;
}

int industrialVision::SetGain()
{
	// ch:��������ǰ�Ȱ��Զ�����رգ�ʧ�����践��
 //en:Set Gain after Auto Gain is turned off, this failure does not need to return
	m_pcMyCamera->SetEnumValue("GainAuto", 0);
    m_dGainEdit = ui.gain_edit->text().toFloat();
	return m_pcMyCamera->SetFloatValue("Gain", (float)m_dGainEdit);
}

int industrialVision::GetFrameRate()
{
	MVCC_FLOATVALUE stFloatValue = { 0 };

	int nRet = m_pcMyCamera->GetFloatValue("ResultingFrameRate", &stFloatValue);
	if (MV_OK != nRet)
	{
		return nRet;
	}
	m_dFrameRateEdit = stFloatValue.fCurValue;
	ui.framerate_edit->clear();
	ui.framerate_edit->setText(QString::number(m_dFrameRateEdit, 'f', 6));
	return MV_OK;
}

int industrialVision::SetFrameRate()
{
	int nRet = m_pcMyCamera->SetBoolValue("AcquisitionFrameRateEnable", true);
	if (MV_OK != nRet)
	{
		return nRet;
	}
    double dblValue  = ui.framerate_edit->text().toDouble();
	 m_dFrameRateEdit = static_cast<int>(dblValue);

	return m_pcMyCamera->SetFloatValue("AcquisitionFrameRate", m_dFrameRateEdit);
}


int industrialVision::GetPixelFormat()
{
	MVCC_ENUMVALUE stEnumValue = { 0 };
	MVCC_ENUMENTRY stPixelFormatInfo = { 0 };

	int nRet = m_pcMyCamera->GetEnumValue("PixelFormat", &stEnumValue);
	if (MV_OK != nRet)
	{
		return nRet;
	}

	stPixelFormatInfo.nValue = stEnumValue.nCurValue;
	nRet = m_pcMyCamera->GetEnumEntrySymbolic("PixelFormat", &stPixelFormatInfo);
	if (MV_OK != nRet)
	{
		return nRet;
	}

	strcpy_s(m_chPixelFormat, MV_MAX_SYMBOLIC_LEN, stPixelFormatInfo.chSymbolic);
	ui.pixelformat_edit->clear();
	ui.pixelformat_edit->setText(m_chPixelFormat);

	return MV_OK;
}

int industrialVision::GetWidth()
{
    _MVCC_INTVALUE_EX_T stIntValue = { 0 };

	int nRet = m_pcMyCamera->GetIntValue("Width", &stIntValue);
	if (MV_OK != nRet)
	{
		return nRet;
	}
	const_m_width = stIntValue.nCurValue;
	m_width = stIntValue.nCurValue;
	ui.width_edit->clear();
	ui.width_edit->setText(QString::number(m_width, 'f', 0));
	return MV_OK;
}

int industrialVision::GetHeight()
{
	_MVCC_INTVALUE_EX_T stIntValue = { 0 };

	int nRet = m_pcMyCamera->GetIntValue("Height", &stIntValue);
	if (MV_OK != nRet)
	{
		return nRet;
	}
	const_m_width = stIntValue.nCurValue;
	m_height = stIntValue.nCurValue;
	ui.height_edit->clear();
	ui.height_edit->setText(QString::number(m_height, 'f', 0));
	return MV_OK;
}

void industrialVision::OnBnClickedSetParameterButton()
{

	bool bIsSetSucceed = true;
	int nRet = SetExposureTime();
	if (nRet != MV_OK)
	{
		bIsSetSucceed = false;
		AppendText("�����ع�ʱ��ʧ��", Red);

	}
	nRet = SetGain();
	if (nRet != MV_OK)
	{
		bIsSetSucceed = false;
		AppendText("��������ʧ��", Red);
	}
	nRet = SetFrameRate();
	if (nRet != MV_OK)
	{
		bIsSetSucceed = false;
		AppendText("����֡��ʧ��", Red);

	}

	if (true == bIsSetSucceed)
	{
		AppendText("�������óɹ�",Green);
	}
	//�÷�
	QString String_m_grade = ui.grade_edit->text();
	double ba = String_m_grade.toDouble();//QString ת��Ϊ char*  

	if (0 < ba && ba < 100)
	{ //��0-1��Χ��
		String_m_grade = QString::number(ba, 'f', 0);
	}
	else
	{ //��ΪĬ��ֵ  
		String_m_grade = QString::number(m_dScore, 'f', 0);
	}
	ui.grade_edit->clear();
	ui.grade_edit->setText(String_m_grade);
	emit send_Grade(String_m_grade);

}

void industrialVision::Setinitializationparameters()
{
	//  //��ȡ�ϴιر�ʱ��״̬
	QString settingPath = QCoreApplication::applicationDirPath() + "/setting.ini";
	QSettings* settings = new QSettings(settingPath, QSettings::IniFormat);
	settings->beginGroup("Idus");
	//�ع�
    QString exposure = settings->value("m_dExposureEdit").toString();
    if (exposure.isEmpty())
    {
        //�������ȡ����
        GetExposureTime();
    }
    else {
		ui.exposure_edit->clear();
		ui.exposure_edit->setText(exposure);
    }
    //����
	QString m_dGainEdit = settings->value("m_dGainEdit").toString();
	if (m_dGainEdit.isEmpty())
	{
		//�������ȡ����
        GetGain();
	}
	else {
		ui.gain_edit->clear();
		ui.gain_edit->setText(m_dGainEdit);
	}
    //֡��
	QString m_dFrameRateEdit = settings->value("m_dFrameRateEdit").toString();
	if (m_dFrameRateEdit.isEmpty())
	{
		//�������ȡ����
        GetFrameRate();
	}
	else {
		ui.framerate_edit->clear();
		ui.framerate_edit->setText(m_dFrameRateEdit);
	}

	//�÷�
	QString String_m_grade = settings->value("m_grade").toString();
	int ba = String_m_grade.toInt();//QString ת��Ϊ char*  

	if (0< ba && ba  < 100)
	{ //��0-1��Χ��
		String_m_grade = QString::number(ba, 'f', 0);
	}
	else
	{ //��ΪĬ��ֵ  
		String_m_grade = QString::number(m_dScore, 'f', 0);
	}


	ui.grade_edit->clear();
	ui.grade_edit->setText(String_m_grade);
	emit send_Grade(String_m_grade);
	//���ظ�ʽ
	QString m_chPixelFormat = settings->value("m_chPixelFormat").toString();
	if (m_chPixelFormat.isEmpty())
	{
		//�������ȡ����
        GetPixelFormat();
	}
	else {
		ui.pixelformat_edit->clear();
		ui.pixelformat_edit->setText(m_chPixelFormat);
	}
	//��
        GetWidth();
	//��
        GetHeight();
	
		//������ת
		double m_rotateIndex = settings->value("m_rotateIndex", "0").toDouble();
		 m_rotateIndexInt = static_cast<int>(m_rotateIndex);
		m_cameraThread->setRotateIndex(m_rotateIndexInt);

			defaultRotateIndexValue = m_rotateIndexInt;
		
		if (m_rotateIndexInt == 1 || m_rotateIndexInt == 3)
		{
			//���������ֵ
			int temp = m_width;
			m_width = m_height;
			m_height = temp;
		}
		//xml·��
		QString m_xmlFilePath = settings->value("m_xmlFilePath").toString();
		//if (m_xmlFilePath.isEmpty())
		//{
		//	AppendText("xmlĬ��·��Ϊ��,������ģ���ļ�·��", Red);
		//}
		//else {
		//	//getXMLPATH(m_xmlFilePath);
		//	AppendText("����·����ȡxml�ļ����", Green);
		//}

    //���ò���
    OnBnClickedSetParameterButton();
	settings->endGroup();
	delete settings;
}

void industrialVision::SaveInitializationParameters()
{
	//����ر�ʱ��״̬
	QString settingPath = QCoreApplication::applicationDirPath() + "/setting.ini";
	QSettings* settings = new QSettings(settingPath, QSettings::IniFormat);
	settings->beginGroup("Idus");
	settings->setValue("m_dExposureEdit", ui.exposure_edit->text());
	settings->setValue("m_dGainEdit", ui.gain_edit->text());
	settings->setValue("m_dFrameRateEdit", ui.framerate_edit->text());
	settings->setValue("m_grade", ui.grade_edit->text());
	settings->setValue("pixelformat_edit", ui.pixelformat_edit->text());
	settings->setValue("m_width", ui.width_edit->text());
	settings->setValue("m_height", ui.height_edit->text());
	settings->setValue("m_xmlFilePath", m_xmlpath); 
	settings->setValue("m_rotateIndex", m_cameraThread->getRotateIndex());

	settings->endGroup();
	delete settings;

}

void industrialVision::smallwindow_button_click()
{
    
	for (int i = 0; i < ui.verticalLayout_4->count(); ++i) {
		QWidget* w = ui.verticalLayout_4->itemAt(i)->widget();
		if (w != NULL) 
			w->setVisible(small_window_flag);
	}
	for (int i = 0; i < ui.horizontalLayout->count(); ++i) {
		QWidget* w = ui.horizontalLayout->itemAt(i)->widget();
		if (w != NULL)
			w->setVisible(small_window_flag);
	}
    ui.widget->setVisible(small_window_flag);
    
    ui.list_widget->setVisible(small_window_flag);

        //�����л����ó�true
	QList<QAction*> actions = ui.menuBar->actions();
	for (int i = 0; i < actions.size(); i++)
	{
		QAction* actionItem = actions.at(i);
		if (actionItem->text() == "ʵʱ��ʾ")
		{                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
			actionItem->setText("�˳�ʵʱ��ʾ");
            continue;
		}
        actionItem->setVisible(small_window_flag);
	}
	ui.textEdit_2->setVisible(small_window_flag);
	ui.textBrowser_record->setVisible(small_window_flag);
	// ��ȡ�����浱ǰ���ڵ�λ�úʹ�С
	originalGeometry = geometry();

	setGeometry(50, 50, 200, 133);
	// ���ô��ڿ��϶�������С����������С������С
	setMinimumSize(200, 133);  // ������СΪ200x200
	setMaximumSize(800, 533);  // �������Ϊ800x533

	// ������Ƶչʾ����ı���
    small_window_flag = !small_window_flag;
    setWindowFlags(Qt::WindowStaysOnTopHint);
    show();
}
                                                      
//todo,,, ʵʱ��Χ��
// �߼����ý���
void industrialVision::slot_displayPixmap(QPixmap newPixmap, int index)
{
	if (newPixmap.isNull())
	{
		qDebug() << "newPixmap is a null image!" << "   index:" << index;
		return;
	}
	if (index == 0)
	{
		//ui.openGLWidget->set
		qreal dpr = devicePixelRatioF();
		//ui.openGLWidget->paintEvent();
		ui.openGLWidget->setPixmap(newPixmap, "");   //ʹ��OpenGLWidget��ʾ
	}
}

void industrialVision::slot_get_patternResult(QPointF resultPoint,int matchTime)
{
    resultPointF.setX(resultPoint.x());
	resultPointF.setY(resultPoint.y());
    matchTime_total = matchTime;
	createOncePattern();
}

void industrialVision::click_shapeMatch()
{

	connect(this, &industrialVision::cameraTovisualTemplateShape, &shapeMainWindow, &MainWindow::sendImgToControllerShape, Qt::UniqueConnection);
	
	connect(&shapeMainWindow, &MainWindow::getImageFromCamera, this, &industrialVision::getImageOneFrame, Qt::UniqueConnection);
	
	shapeMainWindow.show();
	AppendText("����״ģ�����", Green);

}

//��������
void industrialVision::action_AttributesSet()
{
	//����·��,����ļ�S
	logoset.show();
}


void industrialVision::helpInfo()
{
}

void industrialVision::updateTime()
{
	*TimeRecord = TimeRecord->addSecs(1);
	ui.lcdNumber->display(TimeRecord->toString("hh:mm:ss"));
}

void industrialVision::slot_modelPictureReadFlag()
{
	AppendText("ģ��ͼ��ȡ�����д���,����������ģ��ͼ,����ģ��ͼ·������", Red);
}

void industrialVision::getRotateValue(int x)
{
	AppendText("��תͼ��...", Green);
	m_cameraThread->setRotateIndex(x);
	//����չʾͼƬ�ĳ���,����ģ���ȡ�ı�������
	
		//���������ֵ
		int temp = m_width;
		m_width = m_height;
		m_height = temp;
	
	if(!m_xmlpath.isEmpty()){
	if (!read_info_from_ini(m_xmlpath)) {
		if (!m_processingThread->getmodelAndRealSclar()) {
			AppendText("INIģ��ͼ�뵱ǰ���չʾ��ͼƬ������һ��,�޷�ƥ��" + m_xmlpath, Gray);
			return;
		}
	}
	}
}

void industrialVision::actionPasswordAction()
{
	//���������ý���
	passwordSetItem.setcurrentRole(CURRENT_ROLE);
	passwordSetItem.show();
}

//�л��û�
void industrialVision::actionuserSwitch()
{
	emit sign_switchLogin();
	//close();
}

void industrialVision::actionLogAndPathAction()
{
	logoPathItem.show();

}


int industrialVision::CloseDevice()
{
    m_bThreadState = FALSE;
    if (m_pcMyCamera)
    {
        m_pcMyCamera->Close();
        delete m_pcMyCamera;
        m_pcMyCamera = NULL;
    }

    m_bStartGrabbing = FALSE;
    m_bOpenDevice = FALSE;

    if (m_pSaveImageBuf)
    {
        free(m_pSaveImageBuf);
        m_pSaveImageBuf = NULL;
    }
    m_nSaveImageBufSize = 0;

    return MV_OK;
}

void industrialVision::reinitialize() {
	//������־
	AppendText("�л��û���ť���", Green);
	setWindowTitle("V-Gp System V1.0");
	//���¼������ý�
	//�������ñ���
}

bool industrialVision::read_info_from_ini(QString path)
{
	bool is_shapeMatch = true;
	//��path�ж�ȡini��Ϣ
	//��ȡ�ϴιر�ʱ��״̬
	QString settingPath = path;
	QSettings settings(settingPath, QSettings::IniFormat);
	// ��ȡ���з���
	QStringList groups = settings.childGroups();
	int source_width = 0;
	int source_height = 0;
	foreach(const QString & groupName, groups) {
		settings.beginGroup(groupName);

		//���û��������Ĭ��ֵ
		source_width = settings.value("source_width").toInt();
		source_height = settings.value("source_height").toInt();
		settings.endGroup();
	}
	//settings.beginGroup(groups)

	if (!compareAspectRatio(QSize(source_width, source_height)))
	{
		//����һ��
		m_processingThread->setmodelAndRealSclar(false);
		return false;
	}
	//��ǰͼ��ı�����ģ��ͼƬ�ı���һ��	
	m_processingThread->setmodelAndRealSclar(true);

	//���°Ѹ�������ÿ�
	resetParameters();

	//�жϵ�ǰ�ĵ���ʲô���͵��ĵ���
	foreach(const QString & groupName, groups) {
		if (groupName.contains("shape")){
			is_shapeMatch = true;
		}
		else if (groupName.contains("pattern")){
			is_shapeMatch = false;
		}
	}

	//����ƥ������
	if (is_shapeMatch)
	{
	// ��������
	foreach(const QString & groupName, groups) {
		// �����ض�����
		settings.beginGroup(groupName);
		if (groupName.contains("shape_info"))
		{
			double angleDeg_1_newp = settings.value("angleDeg_1_newp", 0).toDouble();
			double angleDeg_2_newp = settings.value("angleDeg_2_newp", 0).toDouble();
			double Intersection_1_newPX = settings.value("Intersection_1_newP.x", 0).toDouble();
			double Intersection_1_newPY = settings.value("Intersection_1_newP.y", 0).toDouble();
			double Intersection_2_newPX = settings.value("Intersection_2_newP.x", 0).toDouble();
			double Intersection_2_newPY = settings.value("Intersection_2_newP.y", 0).toDouble();
			double pointToLineDistance_1_newP = settings.value("pointToLineDistance_1_newP", 0).toDouble();
			double pointToLineDistance_2_newP = settings.value("pointToLineDistance_2_newP", 0).toDouble();
			emit sendInfo_shapeMatch_pictureInfo(angleDeg_1_newp, angleDeg_1_newp,QPointF(Intersection_1_newPX, Intersection_1_newPY),QPointF(Intersection_2_newPX, Intersection_2_newPY),
				pointToLineDistance_1_newP, pointToLineDistance_2_newP);
		}
		//�ж���Բ����ֱ��
		if (groupName.contains("line"))
		{
		// ��ȡ�����ڵ�����
		double pt_begin_cv2X = settings.value("pt_begin_cv2.x",0).toDouble();
		double pt_begin_cv2Y = settings.value("pt_begin_cv2.y",0).toDouble();

		double pt_end_cv2X = settings.value("pt_end_cv2.x", 0).toDouble();
		double pt_end_cv2Y = settings.value("pt_end_cv2.y", 0).toDouble();

		double height = settings.value("height", 0).toDouble();
		double width = settings.value("width", 0).toDouble();

		int nthresholdValue = settings.value("nthresholdValue", 0).toInt();
		int nSampleDirection = settings.value("nSampleDirection", 0).toInt();
		int nMeasureNums = settings.value("nMeasureNums", 0).toInt();

		double roiX = settings.value("roi.x", 0).toDouble();
		double roiY = settings.value("roi.y", 0).toDouble();
		double roiWidth = settings.value("roi.width", 0).toDouble();
		double roiHeight = settings.value("roi.height", 0).toDouble();

		double pt_start_lineX   = settings.value("pt_begin_line.x", 0).toDouble();
		double pt_start_lineY = settings.value("pt_begin_line.y", 0).toDouble();

		double pt_end_lineX = settings.value("pt_end_line.x", 0).toDouble();
		double pt_end_lineY = settings.value("pt_end_line.y", 0).toDouble();
		//settings.endGroup();

		emit sendInfo_shapeMatch_Value(QPointF(pt_begin_cv2X, pt_begin_cv2Y), QPointF(pt_end_cv2X, pt_end_cv2Y), height, width,
			nthresholdValue, nSampleDirection, nMeasureNums, QRect(roiX, roiY, roiWidth, roiHeight),QPointF(pt_start_lineX, pt_start_lineY),
			QPointF(pt_end_lineX, pt_end_lineY));
		m_xmlpath = path;
		//����Ϊiniģʽ
		m_processingThread->setShape_match(true);
		// �˳�����
	}
		if (groupName.contains("circle"))
	{
		double pdCenterX = settings.value("pdCenter.x", 0).toDouble();
		double pdCenterY = settings.value("pdCenter.y", 0).toDouble();
		double nRadius = settings.value("nRadius", 0).toDouble();

		
		double dMeasureLength = settings.value("dMeasureLength", 0).toDouble();
		double dMeasureHeight = settings.value("dMeasureHeight", 0).toDouble();

		int dSigma = settings.value("dSigma", 0).toInt();
		int nThreshold = settings.value("nThreshold", 0).toInt();
		int nTranslation = settings.value("nTranslation", 0).toInt();
		int nMesureNums = settings.value("nMesureNums", 0).toInt();
		int nCircleSize = settings.value("nCircleSize", 0).toInt();
		int nSampleDirection = settings.value("nSampleDirection", 0).toInt();

		double roiX = settings.value("roi.x", 0).toDouble();
		double roiY = settings.value("roi.y", 0).toDouble();
		double roiWidth = settings.value("roi.width", 0).toDouble();
		double roiHeight = settings.value("roi.height", 0).toDouble();


		emit sendInfo_shapeMatch_CIRCLE(QPointF(pdCenterX, pdCenterY), nRadius, dMeasureLength, dMeasureHeight,
			dSigma, nThreshold, nTranslation, nMesureNums, nCircleSize, nSampleDirection, QRectF(roiX, roiY, roiWidth, roiHeight));
		//settings.endGroup();
		m_xmlpath = path;
		//����Ϊiniģʽ
		m_processingThread->setShape_match(true);

		}
	settings.endGroup();

	}
	return true;

	}

	else {//��ȡģ��ƥ�������
		int currentItem = 0;
		settings.beginGroup("pattern_info");
		if (settings.value("pattern_info_item", 0).toInt() == 1)
		{//�Ǿ���
			currentItem = 1;
		}else if (settings.value("pattern_info_item", 0).toInt() == 2)
		{
			//�Ƕ����
			currentItem = 2;
		}
		double source_rect_infoX = settings.value("source_rect_info.x", 0).toDouble();
		double source_rect_infoY = settings.value("source_rect_info.y", 0).toDouble();
		double source_rect_infoWIDTH = settings.value("source_rect_info.width", 0).toDouble();
		double source_rect_infoHeight = settings.value("source_rect_info.height", 0).toDouble();
	
		//��Χ��
		srcQRect.setX(source_rect_infoX);
		srcQRect.setY(source_rect_infoY);
		srcQRect.setWidth(source_rect_infoWIDTH);
		srcQRect.setHeight(source_rect_infoHeight);
		
		//�ж������
		double pattern_rect_infoX = settings.value("pattern_Output_Point_Count", 0).toDouble();
		QPoint pattern_rect_outPut(0, 0);
		QPoint patternRectCenterPoint(0, 0);

		if (pattern_rect_infoX == 1)
		{
			//�������
			double outputx = settings.value("pattern_Output_Point_X", 0).toDouble();
			double outputy = settings.value("pattern_Output_Point_Y", 0).toDouble();

			pattern_rect_outPut.setX(outputx);
			pattern_rect_outPut.setY(outputy);
		}

		if (currentItem == 1){//���������Ǿ��ο�

		//��������
		QString pattern_image_path = settings.value("source_rect_image_info", 0).toString();

		double pattern_rect_infoX = settings.value("pattern_rect_info.x", 0).toDouble();
		double pattern_rect_infoY = settings.value("pattern_rect_info.y", 0).toDouble();
		double pattern_rect_infoWidth = settings.value("pattern_rect_info.width", 0).toDouble();
		double pattern_rect_infoHeight = settings.value("pattern_rect_info.height", 0).toDouble();

		patternRectCenterPoint.setX(pattern_rect_infoX + pattern_rect_infoWidth / 2);
		patternRectCenterPoint.setY(pattern_rect_infoY + pattern_rect_infoHeight / 2);

		m_processingThread->setShapeType(1);
		//ƴ��Ҫ������Ϣ
				settings.endGroup();  
				emit singal_sendPatternImage(pattern_image_path,QRectF(pattern_rect_infoX, pattern_rect_infoY,
					pattern_rect_infoWidth, pattern_rect_infoHeight),
					srcQRect,
					pattern_rect_outPut, patternRectCenterPoint);

				m_xmlpath = path;

				return true;

		}
	else if (currentItem == 2)
		{
			QString pattern_image_path = settings.value("source_rect_image_info", 0).toString();
			//����shape,����û��mask��Ĥ,0��,1û��
			m_processingThread->setShapeType(0);
			QPolygonF  patternAreaREAL_size_polygon;
			int numCount = settings.value("pattern_polygon_num", 0).toInt();

			for (int i  = 0;i< numCount;i++)
			{
				QString polygonX("pattern_polygon_count_X_");
				polygonX.append(QString::number(i));
				
				QString polygonY("pattern_polygon_count_Y_");
				polygonY.append(QString::number(i));

				double pattern_polygon_countX = settings.value(polygonX, 0).toDouble();
				double pattern_polygon_countY = settings.value(polygonY, 0).toDouble();

				
				patternAreaREAL_size_polygon << QPointF(pattern_polygon_countX, pattern_polygon_countY);
			}

			// ��QPolygonF�еĵ�����ת��Ϊvector<cv::Point>
			std::vector<cv::Point> points;
			for (const QPointF& point : patternAreaREAL_size_polygon) {
				points.emplace_back(static_cast<int>(point.x()), static_cast<int>(point.y()));
			}

			cv::Rect boundingRect = cv::boundingRect(points);
			patternRectCenterPoint.setX(boundingRect.x + (boundingRect.width / 2));
			patternRectCenterPoint.setY(boundingRect.y + (boundingRect.height / 2));

			emit singal_sendPatternImageWithMaskPolygon(pattern_image_path, patternAreaREAL_size_polygon, srcQRect,
				pattern_rect_outPut, patternRectCenterPoint);

			m_xmlpath = path;

			return true;
		}
	}
	return false;
}

void industrialVision::resetParameters()
{
	centerPoint.setX(0);
	centerPoint.setY(0);
	areaNodeREAL_size.setX(0);
	areaNodeREAL_size.setY(0);
	areaNodeREAL_size.setWidth(0);
	areaNodeREAL_size.setHeight(0);
	//����rect
		patternAreaREAL_size_rect.setX(0);
		patternAreaREAL_size_rect.setY(0);
		patternAreaREAL_size_rect.setWidth(0);
		patternAreaREAL_size_rect.setHeight(0);

}

QImage industrialVision::Mat2QImage(const cv::Mat cvImage)
{
    cv::Mat temp = cvImage.clone();

    std::vector<uchar> imgBuf;
    imencode(".bmp", temp, imgBuf);

    QByteArray baImg((char*)imgBuf.data(), static_cast<int>(imgBuf.size()));
    QImage image;
    image.loadFromData(baImg, "BMP");
    return image;
}

