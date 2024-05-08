﻿#include "industrialVision.h"


// 在这里定义全局变量
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
	//设置文字大小
	//myFont.setPointSize(30);
	//设置文字字体
	setFont(myFont);

	ui.menuBar->setStyleSheet("QMenu::item{padding:5px 15px;margin:1px 2px;color:#222222;font - size:40px;}");
	ui.lcdNumber->setDigitCount(8);
	ui.lcdNumber->setSegmentStyle(QLCDNumber::Flat);
	ui.lcdNumber->display(TimeRecord->toString("hh:mm:ss"));

	//相机对象
	m_pcMyCamera = new CMvCamera;

	//相机线程对象实例化
	m_cameraThread = new CameraThread();

	//处理线程对象实例化
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

	
	connect(&TransmitSignals::GetInstance(), &TransmitSignals::create_once_pattern, m_processingThread, &ProcessingThread::slot_processThread_Pattren);
	
	connect(&TransmitSignals::GetInstance(), &TransmitSignals::sendToIndustrString, this, &industrialVision::addTextBrower);


	connect(this, &industrialVision::sendResultToServer, &TransmitSignals::GetInstance(), &TransmitSignals::send_pattern_result);
    
	/////////////////////
	//auto action_shizixian = new QAction();
	//action_shizixian->setText("十字线");
	//action_shizixian->setFont(QFont(tr("宋体"), 17, QFont::Bold, false));//宋体26号，加粗，斜体字

	//connect(action_shizixian, &QAction::triggered,
	//	this, &industrialVision::openShiShiPiPei);



	action_RestoreWindow = new QAction();
	action_RestoreWindow->setText("实时显示");
	action_RestoreWindow->setFont(QFont(tr("宋体"), 40, QFont::Bold, false));
	ui.menuBar->addAction(action_RestoreWindow);
	connect(action_RestoreWindow, &QAction::triggered,
		this, &industrialVision::restoreWindow);

	action_SetModelFile = new QAction();
	action_SetModelFile->setFont(QFont(tr("宋体"), 60, QFont::Bold, false));//宋体26号，加粗，斜体字
    action_SetModelFile->setText("工程导入");
    ui.menuBar->addAction(action_SetModelFile);
    connect(action_SetModelFile, &QAction::triggered,
        this, &industrialVision::setModelXMLFile);
	action_SetModelFile->setEnabled(true);

	 
	action_SetAttributes = new QAction();
	action_SetAttributes->setText("图标设置");
	action_SetAttributes->setFont(QFont(tr("宋体"),40, QFont::Bold, false));
	connect(action_SetAttributes, &QAction::triggered,
		this, &industrialVision::action_AttributesSet);

	action_password = new QAction();
	action_password->setText("密码设置");
	action_password->setFont(QFont(tr("宋体"), 40, QFont::Bold, false));
	connect(action_password, &QAction::triggered,
		this, &industrialVision::actionPasswordAction);

	 action_setLogoPath = new QAction();
	action_setLogoPath->setText("日志设置");
	action_setLogoPath->setFont(QFont(tr("宋体"), 40, QFont::Bold, false));
	connect(action_setLogoPath, &QAction::triggered,
		this, &industrialVision::actionLogAndPathAction);

	SettingMenus = new QMenu("&设置", ui.menuBar);
	ui.menuBar->addMenu(SettingMenus);
	
	 action_userSwitch = new QAction();
	action_userSwitch->setText("用户切换");
	action_userSwitch->setFont(QFont(tr("宋体"), 40, QFont::Bold, false));
	ui.menuBar->addAction(action_userSwitch);
	connect(action_userSwitch, &QAction::triggered,
		this, &industrialVision::actionuserSwitch);

	 action_helpInfo = new QAction();
    action_helpInfo->setText("帮助");
    action_helpInfo->setFont(QFont(tr("宋体"),40, QFont::Bold, false));
	ui.menuBar->addAction(action_helpInfo);
	connect(action_helpInfo, &QAction::triggered,
		this, &industrialVision::helpInfo); 

	//语言设置
	//中文
	action_language_zh = new QAction();
	action_language_zh->setText("中文");
	action_language_zh->setFont(QFont(tr("宋体"), 40, QFont::Bold, false));
	connect(action_language_zh, &QAction::triggered,
		this, &industrialVision::languageSet_zh);
	
	//英文
	action_language_en = new QAction();
	action_language_en->setText("ENGLISH");
	action_language_en->setFont(QFont(tr("宋体"), 40, QFont::Bold, false));
	connect(action_language_en, &QAction::triggered,
		this, &industrialVision::languageSet_en);
	//西班牙文
	action_language_es = new QAction();
	action_language_es->setText("SPANISH");
	action_language_es->setFont(QFont(tr("宋体"), 40, QFont::Bold, false));
	connect(action_language_es, &QAction::triggered,
		this, &industrialVision::languageSet_es);
	
	LanguageMenus = new QMenu("语言", ui.menuBar);

	LanguageMenus->addAction(action_language_zh);
	LanguageMenus->addAction(action_language_en);
	//LanguageMenus->addAction(action_language_es);
	ui.menuBar->addMenu(LanguageMenus);
	///////

    connect(this, &industrialVision::sendResultToServer, &TransmitSignals::GetInstance(), &TransmitSignals::send_pattern_result);
    connect(this, &industrialVision::SLOTAppendText, this, &industrialVision::addTextBrower);

	connect(timer, &QTimer::timeout, this, &industrialVision::updateTime);

	connect(&connectValual, &connectServer::send_createServerCallBack, this, &industrialVision::receive_ServerCreateInfo);
	
	connect(ui.openGLWidget, &MyGLWidget::rotateIndexValueChanged, this, &industrialVision::getRotateValue,Qt::QueuedConnection);
	
	connect(this, &industrialVision::setdefultCamare, ui.openGLWidget, &MyGLWidget::setMouseClickFlag, Qt::QueuedConnection);
	connect(this, &industrialVision::send_system_language, ui.openGLWidget, &MyGLWidget::set_system_language, Qt::QueuedConnection);

	
    //开启服务端
	connectValual.create_server();
	ui.pushButton_manualOperation->setEnabled(false);

    //设置背景色 
	ui.menuBar->setStyleSheet("color: rgb(255, 255, 255); background-color: rgba(26, 45, 77, 1);");
	setButtonClickLimits(false);
	//白色填充左上角图标
    setWindowTitle("V-Gp System V1.0");
	setWindowIcon(QIcon("icon.ico"));
	setLanguageInfo(setLanguageItem);

	if (setLanguageItem=="zh")
	{
		AppendText("系统启动成功", Green);
	}else if (setLanguageItem == "en")
	{
		AppendText("System startup successful", Green);
	}else if (setLanguageItem == "es")
	{
		AppendText("El sistema arrancó con éxito", Green);
	}
}


 void industrialVision::click_continuousOperation() {
     
     //AppendText("点击连续工作按钮",Green);
	 if (m_bOpenDevice) {
		 if (setLanguageItem == "zh")
		 {
			 AppendText("相机已经启动", Red);
			 QMessageBox::critical(this, "错误信息", "相机已经启动");
		 }else if (setLanguageItem == "en")
		 {
			 AppendText("The camera has been activated", Red);
			 QMessageBox::critical(this, "error message", "The camera has been activated");
		 }
		 else if (setLanguageItem == "es")
		 {
			 AppendText("La Cámara ha arrancado", Red);
			 QMessageBox::critical(this, "Mensaje de error", "La Cámara ha arrancado");
		 }
		 return;
	 }
	 if (!DisplayWindowInitial()){
		 if (setLanguageItem == "zh")
		 {
			 AppendText("相机状态异常", Red);
		 }
		 else if (setLanguageItem == "en")
		 {
			 AppendText("Abnormal camera status", Red);
		 }
		 else if (setLanguageItem == "es")
		 {
			 AppendText("Estado anormal de la Cámara", Red);
		 }
		 return;
	 }
	 //设置按钮权限
	 setButtonClickLimits(true);

	 if (!m_bOpenDevice)
	 {
		 m_bOpenDevice = true;
	 }

		 if (m_pcMyCamera->IsDeviceConnected())
		 {
			 m_pcMyCamera->StartGrabbing();   //开启相机采集
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
         //获取相机型号
			 // ch:枚举子网内所有设备 | en:Enumerate all devices within subnet
		 int nRet = CMvCamera::EnumDevices(MV_GIGE_DEVICE , &m_stDevList);

		 // ch:将值加入到信息列表框中并显示出来 | en:Add value to the information list box and display
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
				 if (setLanguageItem=="zh")
				 {
					 AppendText("无法识别的设备", Red);
					 QMessageBox::critical(this, "错误信息", "无法识别的设备");
				 }else if (setLanguageItem == "en")
				 {
					 AppendText("Unrecognized device", Red);
					 QMessageBox::critical(this, "error message", "Unrecognized device");
				 }
				 else if (setLanguageItem == "es")
				 {
					 AppendText("Dispositivos no reconocibles", Red);
					 QMessageBox::critical(this, "Mensaje de error", "Dispositivos no reconocibles");
				 }
		
			 }
		 }

		 //计时器开始计时
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
		//解决QT无边框窗体最大化遮盖Windows任务栏问题
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
		if (setLanguageItem=="zh")
		{
			AppendText("服务端开启成功", Green);
			return;
		}else if (setLanguageItem == "en")
		{
			AppendText("Successfully opened the server", Green);
			return;

		}
		else if (setLanguageItem == "es")
		{
			AppendText("El lado del servicio se abrió con éxito.", Green);
			return;

		}
	}
	if (setLanguageItem == "zh")
	{
		AppendText("服务端已经开启或者开启失败", Red);
		return;
	}
	else if (setLanguageItem == "en")
	{
		AppendText("The server has already been opened or failed to open it", Red);
		return;

	}
	else if (setLanguageItem == "es")
	{
		AppendText("El servidor ha sido abierto o ha fallado", Red);
		return;

	}

}

void industrialVision::click_editVision()
{
    connect(&createModelItem, &createModel::getImageFromCamera, this, &industrialVision::getImageOneFrame,Qt::UniqueConnection);
    connect(this, &industrialVision::cameraTovisualTemplate,&createModelItem, &createModel::sendImgToFileController, Qt::UniqueConnection);

    connect(&createModelItem, &createModel::sendXMLPath , this, &industrialVision::getXMLPATH, Qt::UniqueConnection);

	connect(createModelItem.fileController2D, &FileController::sendImageToPattern, m_processingThread, &ProcessingThread::slot_processMatchPicture, Qt::UniqueConnection);
	
	connect(createModelItem.fileController2D, &FileController::sendImageToPatternWithMask, m_processingThread, &ProcessingThread::slot_processMatchPictureWithMask, Qt::UniqueConnection);

	connect(m_processingThread, &ProcessingThread::QPointSendtoFileControl,createModelItem.fileController2D, &FileController::slot_receiveDrawPoint, Qt::UniqueConnection);


    createModelItem.show();
    //更新xml文件
	if (setLanguageItem == "zh")
	{
		AppendText("打开视觉模板界面", Green);
		return;
	}
	else if (setLanguageItem == "en")
	{
		AppendText("Open the visual template interface", Green);
		return;
	}
	else if (setLanguageItem == "es")
	{
		AppendText("Abrir la interfaz de la plantilla visual", Green);
		return;
	}
}

void industrialVision::getImageOneFrame() {
	// 创建一个定时器
	QTimer timer;
	timer.setSingleShot(true); // 设置为单次触发

	// 连接定时器的timeout信号到一个槽函数，该槽函数在定时器超时时触发
	QObject::connect(&timer, &QTimer::timeout, [&]() {
		emit cameraTovisualTemplate( QImage(), QString());
		});

	timer.start(1000); // 启动定时器，设置超时时间为1秒


	while (m_imageVector_1.isEmpty())
	{
		QCoreApplication::processEvents(); // 允许Qt事件处理
	}
	//定时器停止
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
     emit cameraTovisualTemplate(myImageToModel, ModelPath);
}

void industrialVision::createOncePattern()
{		
		  if (resultPointF.x() != -m_width && resultPointF.y() != -m_height)
      {
		  if (setLanguageItem == "zh")
		  {
			  AppendText("【提示】触发接受匹配完成,匹配成功", Green);
		  }
		  else if (setLanguageItem == "en")
		  {
			  AppendText("【Tip】 Trigger acceptance matching completed, matching successful", Green);
		  }
		  else if (setLanguageItem == "es")
		  {
			  AppendText("[aviso] se completa el emparejamiento de aceptación del activación, y el emparejamiento es exitoso.", Green);
		  }

		  char xxx[10];
		  char yyy[10];
		  sprintf(xxx, "%.1f", finall_Total_Result.ptCenter.x);
		  sprintf(yyy, "%.1f", finall_Total_Result.ptCenter.y);

          QString  resultFont  = "x:坐标";

          resultFont.append(QString::fromLocal8Bit(xxx));
		  resultFont.append(",");
		  resultFont.append("y:坐标");
		  resultFont.append(QString::fromLocal8Bit(yyy));
		  resultFont.append(".");
		  if (setLanguageItem == "zh")
		  {
			  resultFont.append("--匹配用时:");
		  }
		  else if (setLanguageItem == "en")
		  {
			  resultFont.append("--Matching time:");
		  }
		  else if (setLanguageItem == "es")
		  {
			  resultFont.append("--Tiempo de coincidencia:");
		  }
		  resultFont.append(QString::number(matchTime_total));
		  resultFont.append("ms");

		  AppendText(resultFont,Green);
		  total_count++;

          OK_count++;
          ui.texstBrowser_titleStatus->clear();
          ui.texstBrowser_titleStatus->setPixmap(QPixmap("Image/industory/OK.png"));
	  }
      else {
		  if (setLanguageItem == "zh")
		  {
			  AppendText("【错误】触发接受匹配完成,匹配失败", Red);
		  }
		  else if (setLanguageItem == "en")
		  {
			  AppendText("[Error] Trigger acceptance matching completed, matching failed", Red);
		  }
		  else if (setLanguageItem == "es")
		  {
			  AppendText("[error] activación para aceptar la coincidencia completada, la coincidencia falló", Red);
		  }
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
			ui.textBrowser_record->append("【提示】log文件夹创建失败。\r\n");
			return;
		}
	}
	QString str_now = datetime.toString("yyyy_MM_dd_hh");
	QString filename = logDirectory + "\\" + "idav_" + str_now + ".log";
	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
	{
		ui.textBrowser_record->append("【提示】日志文件打开失败。\r\n"+ file.fileName());
		return;//打开文件失败
	}
	file.write(now.toUtf8()+"\r\n");
	file.write(text.toUtf8()+"\r\n");
	file.close();
}
void industrialVision::getXMLPATH(QString xmlPath)
{
	if (setLanguageItem == "zh")
	{
		AppendText("读取xml路径:" + xmlPath, Green);
	}else if (setLanguageItem == "en")
	{
		AppendText("Read XML path:" + xmlPath, Green);
	}
	else if (setLanguageItem == "es")
	{
		AppendText("Leer la ruta xml:" + xmlPath, Green);
	}                                 

    m_xmlpath = xmlPath;

	if(getPatternInfoFromXML(m_xmlpath)){
	if (setLanguageItem == "zh")
		{
		AppendText("加载xml模板成功" + xmlPath, Green);
		}
	else if (setLanguageItem == "en")
		{
		AppendText("Successfully loaded XML template" + xmlPath, Green);
		}
	else if (setLanguageItem == "es")
		{
		AppendText("Carga exitosa de la plantilla XML" + xmlPath, Green);
		}
	}
	else {
		if (!m_processingThread->getmodelAndRealSclar())
		{
			if (setLanguageItem == "zh")
			{
				AppendText("XML 模板图与当前相机展示的图片比例不一致,无法匹配" + xmlPath, Gray);
			}
			else if (setLanguageItem == "en")
			{
				AppendText("The scale of the XML template image is inconsistent with the image displayed by the current camera and cannot be matched" + xmlPath, Gray);
			}
			else if (setLanguageItem == "es")
			{
				AppendText("La imagen de la plantilla XML no coincide con la escala de la imagen mostrada por la Cámara actual y no puede coincidir" + xmlPath, Gray);
			}
			return;
		}
		if (setLanguageItem == "zh")
		{
			AppendText("加载xml模板失败" + xmlPath, Red);
		}
		else if (setLanguageItem == "en")
		{
			AppendText("Failed to load XML template" + xmlPath, Red);
		}
		else if (setLanguageItem == "es")
		{
			AppendText("Falló la carga de la plantilla XML" + xmlPath, Red);
		}
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
		actionItem->setVisible(small_window_flag);
	}
    ui.textEdit_2->setVisible(small_window_flag);
	ui.textBrowser_record->setVisible(small_window_flag);

    ///其他设置
	small_window_flag = !small_window_flag;
	//设置窗口大小
	
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
     //设置视频展示区域的比例
   // setFixedSize(this->width(), this->height());

}

bool industrialVision::compareAspectRatio(QSize smallPicture)
{
	//等待取流线程执行,之了后才有m_wideth和m_height
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
	QImage myImage = QImage((const unsigned char*)(cvMat.data), cvMat.cols, cvMat.rows, QImage::Format_Indexed8);   //灰度图

//QSize表示图像在界面上的显示尺寸    
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
	//窗口栏增加当前权限
	QString title(windowTitle());
	title.append("[" + CURRENT_ROLE + "]");
	setWindowTitle(title);
	SettingMenus->clear();
	//增加设置界面,管理员和操作员分开
	if (CURRENT_ROLE == ROLE_ADMIN)
	{
		//图标设置                                                                                                                                                                                                                   
		SettingMenus->addAction(action_setLogoPath);
		//日志设置
		SettingMenus->addAction(action_SetAttributes);
	}
	//密码设置
	SettingMenus->addAction(action_password);
}

//搜索区域按钮
void industrialVision::openShiShiPiPei()
{
    if (sourceAreaOn) {


		//AppendText("source aRE", Green);
		//ui.pushButton->setStyleSheet("/* 证券 */ QPushButton::hover { background-color: #1450C7; } position: absolute; left: 1px; top: 67px; width: 190px; height: 61px; opacity: 1; /* 背景/4 页签选中色 */ background: #285790; color: rgb(255, 0, 0); box-sizing: border-box; border: 1px solid ; border-image: linear-gradient(180deg, rgba(35,102,211,0.00) 0%, #3797FE 100%) 1;");
    }
    else {
	//	AppendText("搜索区域关闭", Red);
		//ui.pushButton->setStyleSheet("/* 证券 */ QPushButton::hover { background-color: #1450C7; } position: absolute; left: 1px; top: 67px; width: 190px; height: 61px; opacity: 1; /* 背景/4 页签选中色 */ background: #285790; color: rgb(255, 255, 255); box-sizing: border-box; border: 1px solid ; border-image: linear-gradient(180deg, rgba(35,102,211,0.00) 0%, #3797FE 100%) 1;");
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
            tr("XML Files(*.xml)"));
    if (!path.isEmpty()) {
       /* QFile file(path);
        if (!file.open(QFile::ReadOnly))
            return;*/
		m_xmlpath = path;
		if (getPatternInfoFromXML(m_xmlpath)) {
			if (setLanguageItem == "zh")
			{
				AppendText("加载xml模板成功" + m_xmlpath, Green);
			}
			else if (setLanguageItem == "en")
			{
				AppendText("Successfully loaded XML template" + m_xmlpath, Green);
			}
			else if (setLanguageItem == "es")
			{
				AppendText("Carga exitosa de la plantilla XML" + m_xmlpath, Green);
			}
		}
		else {
			if(!m_processingThread->getmodelAndRealSclar()){
				if (setLanguageItem == "zh")
				{
					AppendText("XML模板图与当前相机展示的图片比例不一致,无法匹配" + m_xmlpath, Gray);
				}
				else if (setLanguageItem == "en")
				{
					AppendText("The scale of the XML template image is inconsistent with the image displayed by the current camera and cannot be matched" + m_xmlpath, Gray);
				}
				else if (setLanguageItem == "es")
				{
					AppendText("La imagen de la plantilla XML no coincide con la escala de la imagen mostrada por la Cámara actual y no puede coincidir" + m_xmlpath, Gray);
				}
			return;
			}
			if (setLanguageItem == "zh")
			{
				AppendText("加载xml模板失败" + m_xmlpath, Red);
			}
			else if (setLanguageItem == "en")
			{
				AppendText("Failed to load XML template" + m_xmlpath, Red);
			}
			else if (setLanguageItem == "es")
			{
				AppendText("Falló la carga de la plantilla XML" + m_xmlpath, Red);
			}
		}
    }
	else {
		if (setLanguageItem == "zh")
		{
			QMessageBox::warning(nullptr, tr("Path"),
				tr("未选择xml模板."));
		}
		else if (setLanguageItem == "en")
		{
			QMessageBox::warning(nullptr, tr("Path"),
				tr("No XML template selected"));
		}
		else if (setLanguageItem == "es")
		{
			QMessageBox::warning(nullptr, tr("Path"),
				tr("No se ha seleccionado ninguna plantilla de texto."));
		}
   
    }
}

void industrialVision::rotatePicture()
{
    rotate_image = !rotate_image;
}

//保存当前状态为默认状态并锁定 ,方便下次打开
void industrialVision::click_manualOperation()
{
	
	emit setdefultCamare(defaultCamcare);
	//设置相机参数不可修改或者可以更改
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

		//将字体颜色修改为红色
		ui.pushButton_manualOperation->setStyleSheet("/* 证券 */ QPushButton::hover { background-color: #1450C7; } position: absolute; left: 1px; top: 67px; width: 190px; height: 61px; opacity: 1; /* 背景/4 页签选中色 */ background: #285790; color: rgb(255, 0, 0); box-sizing: border-box; border: 1px solid ; border-image: linear-gradient(180deg, rgba(35,102,211,0.00) 0%, #3797FE 100%) 1;");
		if (setLanguageItem == "zh")
		{
			ui.pushButton_manualOperation->setText("解锁状态");
			emit send_system_language("zh");
		}
		else if (setLanguageItem == "en")
		{
			ui.pushButton_manualOperation->setText("Unlocking status");
			emit send_system_language("en");

		}
		else if (setLanguageItem == "es")
		{
			ui.pushButton_manualOperation->setText("Estado de desbloqueo");
			emit send_system_language("es");
		}

		//锁定状态,设置当前旋转方向为默认方向
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

		//将字体颜色修改为默认颜色
		ui.pushButton_manualOperation->setStyleSheet("/* 证券 */ QPushButton::hover { background-color: #1450C7; } position: absolute; left: 1px; top: 67px; width: 190px; height: 61px; opacity: 1; /* 背景/4 页签选中色 */ background: #285790; color: rgb(255, 255, 255); box-sizing: border-box; border: 1px solid ; border-image: linear-gradient(180deg, rgba(35,102,211,0.00) 0%, #3797FE 100%) 1;");
		if (setLanguageItem == "zh")
		{
			ui.pushButton_manualOperation->setText("锁定状态");
			emit send_system_language("zh");			
		}
		else if (setLanguageItem == "en")
		{
			ui.pushButton_manualOperation->setText("Locked state");
			emit send_system_language("en");

		}
		else if (setLanguageItem == "es")
		{
			ui.pushButton_manualOperation->setText("Bloqueo");
			emit send_system_language("es");

		}
	}
	defaultCamcare = !defaultCamcare;
}


bool industrialVision::getPatternInfoFromXML(QString path)
{
    QFile xml_MODEL_FILE(path);         //需要打开的文件
    if (!xml_MODEL_FILE.exists()) {
		if (setLanguageItem == "zh")
		{
			AppendText("模板文件不存在", Red);
		}
		else if (setLanguageItem == "en")
		{
			AppendText("The template file does not exist", Red);
		}
		else if (setLanguageItem == "es")
		{
			AppendText("El archivo de plantilla no existe", Red);
		}
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
	//重新把各项参数置空
	resetParameters();
	//读取xml文件操作
    QDomElement root = doc.documentElement();
    QDomElement elem = root.cloneNode(true).toElement();
    for (int i = 0; i < elem.childNodes().count(); i++) {
        auto typeElem = elem.childNodes().at(i).toElement();
		int childcount = typeElem.childNodes().count();
        for (int j = 0; j < childcount; j++) {
            auto labelElem = typeElem.childNodes().at(j).toElement();
			 
            if (labelElem.tagName() == "Label") {

                Label* label = new Label;
                //当前type名字
				QString typeName = typeElem.attribute("Type");
                Shape* currentShape = label->createFromElementToPattern(labelElem);
                QSize small_Picture = currentShape->getCurrSize();
				//判断当前图和模板图是否比例一致
				if (!compareAspectRatio(small_Picture))
				{
					//代表不一致
					m_processingThread->setmodelAndRealSclar(false);
					return false;							
				}																
				//一致	
				m_processingThread->setmodelAndRealSclar(true);
                if (typeName.contains("搜索区域")|| typeName.contains("SearchArea")||
					typeName.contains("Área de búsqueda")) {
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
                else if (typeName.contains("特征区域")|| typeName.contains("CharacteristicArea")||
					typeName.contains("Zona característica")) {
					//判断当前特征区域是哪种类型
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
					//给patternImageName赋值用于读取特征图
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
						//给patternImageName赋值用于读取特征图
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
				else if (typeName.contains("输出点")|| typeName.contains("Output point")||
					typeName.contains("Punto de salida")) {
					//找到输出点并且获取其中心坐标
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
							//获得输出点的中心坐标
							centerPoint = getCenterPointFromCircle(QpointList);
							centerPoint.setX(((double)centerPoint.x() / small_Picture.width()) * m_width);
							centerPoint.setY(((double)centerPoint.y() / small_Picture.height()) * m_height);
							//输出点中心实际的中心坐标
							patternRectCenterPoint.setX(patternAreaREAL_size_rect.x() + (patternAreaREAL_size_rect.width() / 2));
							patternRectCenterPoint.setY(patternAreaREAL_size_rect.y() + (patternAreaREAL_size_rect.height() / 2));
						}
				}
			}
        }
    }

    if (pattern_Path.isEmpty())
    {
		if (setLanguageItem == "zh")
		{
			AppendText("特征区域读取错误,请在模板界面设置模板图", Red);
		}
		else if (setLanguageItem == "en")
		{
			AppendText("Error reading feature area, please set template image in the template interface", Red);
		}
		else if (setLanguageItem == "es")
		{
			AppendText("Error de lectura del área característica, establezca el mapa de la plantilla en la interfaz de la plantilla", Red);
		}
        return false;
    }
   if (areaNodeREAL_size.x()<0|| areaNodeREAL_size.y()<0)
   {
	   if (setLanguageItem == "zh")
	   {
		   AppendText("搜索区域坐标错误,请重新制作", Red);
	   }
	   else if (setLanguageItem == "en")
	   {
		   AppendText("Search area coordinates error, please remake", Red);
	   }
	   else if (setLanguageItem == "es")
	   {
		   AppendText("Error en las coordenadas del área de búsqueda, por favor vuelva a crear", Red);
	   }
	   return false;
   }
   //范围框
   srcQRect.setX(areaNodeREAL_size.x());
   srcQRect.setY(areaNodeREAL_size.y());
   srcQRect.setWidth(areaNodeREAL_size.width());
   srcQRect.setHeight(areaNodeREAL_size.height());
   //没有设置范围框,则默认全图
   if (srcQRect.x() == 0 && srcQRect.y() == 0 && srcQRect.width() == 0 && srcQRect.height() == 0) {
	   srcQRect.setWidth(m_width);
	   srcQRect.setHeight(m_height);
   }

   if(currentPattern_OBJ == Shape::Rect){
	if (patternAreaREAL_size_rect.x() < 0 || patternAreaREAL_size_rect.y() < 0)
		{
	   AppendText("特征区域坐标错误,请重新制作", Red);
	   return false;
		}
		//发送给peocessingthread线程 路径,匹配模板中心点坐标,范围图中心点坐标
	m_processingThread->setShapeType(1);

		emit singal_sendPatternImage(pattern_Path, patternAreaREAL_size_rect,srcQRect, centerPoint, patternRectCenterPoint);
		return true;
	}else if (currentPattern_OBJ == Shape::Ellipse)
	{
		//123
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

	//保存关闭时的状态
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
	//初始化变量
	//m_deviceNum = 0;
	m_bOpenDevice = false;

	//初始化相机
	memset(&m_stDevList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));
	//枚举子网内所有设备
	int nRet = CMvCamera::EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &m_stDevList);
	if (MV_OK != nRet)
	{
		QMessageBox::warning(this, "警告", "请检查相机是否正常连接！");
		return false;
	}
	 int m_deviceNum = m_stDevList.nDeviceNum;
	if (m_deviceNum !=1)
	{
		QMessageBox::warning(this, "警告", "请检查相机是否正常连接！");
		return false;
	}
	for (int i = 0; i < m_deviceNum; i++)
	{
		MV_CC_DEVICE_INFO* pDeviceInfo = m_stDevList.pDeviceInfo[i];
		if(!m_pcMyCamera->IsDeviceConnected()){
			int nRet = m_pcMyCamera->Open(pDeviceInfo);   //打开相机
			if (MV_OK != nRet)
			{
				//delete m_pcMyCamera;
				//m_pcMyCamera = NULL;
				QMessageBox::warning(this, "警告", "打开设备失败！");
				return false;
			}
			m_cameraThread->setCameraPtr(m_pcMyCamera);
			m_cameraThread->setCameraIndex(i);
			m_processingThread->setThreadId(i);
		}
        open_mv = true;
	}
	return true;
}

void industrialVision::setButtonClickLimits(bool flag)
{
	//ui.pushButton_editVision->setEnabled(flag);
	action_SetModelFile->setEnabled(true);
	ui.pushButton_manualOperation->setEnabled(flag);
	//ui.pushButton->setEnabled(flag);
	ui.pushButton_stopOperation->setEnabled(flag);

}

//通用方法 返回弹窗
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
    QMessageBox::critical(this, "错误信息", errorMsg);

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

	//外面显示的值是除以了100的
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
	// ch:设置增益前先把自动增益关闭，失败无需返回
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
		if (setLanguageItem == "zh")
		{
			AppendText("设置曝光时间失败", Red);
		}
		else if (setLanguageItem == "en")
		{
			AppendText("Failed to set exposure time", Red);
		}
		else if (setLanguageItem == "es")
		{
			AppendText("Falló la configuración del tiempo de exposición", Red);
		}
	}
	nRet = SetGain();
	if (nRet != MV_OK)
	{
		bIsSetSucceed = false;
		if (setLanguageItem == "zh")
		{
			AppendText("设置增益失败", Red);
		}
		else if (setLanguageItem == "en")
		{
			AppendText("Set gain failed", Red);
		}
		else if (setLanguageItem == "es")
		{
			AppendText("Falló la configuración de la ganancia", Red);
		}
	}
	nRet = SetFrameRate();
	if (nRet != MV_OK)
	{
		bIsSetSucceed = false;
		if (setLanguageItem == "zh")
		{
			AppendText("设置帧率失败", Red);
		}
		else if (setLanguageItem == "en")
		{
			AppendText("Failed to set frame rate", Red);
		}
		else if (setLanguageItem == "es")
		{
			AppendText("Falló la configuración de la tasa de fotogramas", Red);
		}
	}

	if (true == bIsSetSucceed)
	{
		if (setLanguageItem == "zh")
		{
			AppendText("参数设置成功", Green);
		}
		else if (setLanguageItem == "en")
		{
			AppendText("Parameter setting successful", Green);
		}
		else if (setLanguageItem == "es")
		{
			AppendText("Configuración de parámetros exitosa", Green);
		}
	}
	//得分
	QString String_m_grade = ui.grade_edit->text();
	double ba = String_m_grade.toDouble();//QString 转换为 char*  

	if (0 < ba && ba < 100)
	{ //在0-1范围内
		String_m_grade = QString::number(ba, 'f', 0);
	}
	else
	{ //设为默认值  
		String_m_grade = QString::number(m_dScore, 'f', 0);
	}
	ui.grade_edit->clear();
	ui.grade_edit->setText(String_m_grade);
	emit send_Grade(String_m_grade);

}

void industrialVision::Setinitializationparameters()
{
	//  //读取上次关闭时的状态
	QString settingPath = QCoreApplication::applicationDirPath() + "/setting.ini";
	QSettings* settings = new QSettings(settingPath, QSettings::IniFormat);
	settings->beginGroup("Idus");
	//曝光
    QString exposure = settings->value("m_dExposureEdit").toString();
    if (exposure.isEmpty())
    {
        //从相机获取参数
        GetExposureTime();
    }
    else {
		ui.exposure_edit->clear();
		ui.exposure_edit->setText(exposure);
    }
    //增益
	QString m_dGainEdit = settings->value("m_dGainEdit").toString();
	if (m_dGainEdit.isEmpty())
	{
		//从相机获取参数
        GetGain();
	}
	else {
		ui.gain_edit->clear();
		ui.gain_edit->setText(m_dGainEdit);
	}
    //帧率
	QString m_dFrameRateEdit = settings->value("m_dFrameRateEdit").toString();
	if (m_dFrameRateEdit.isEmpty())
	{
		//从相机获取参数
        GetFrameRate();
	}
	else {
		ui.framerate_edit->clear();
		ui.framerate_edit->setText(m_dFrameRateEdit);
	}

	//得分
	QString String_m_grade = settings->value("m_grade").toString();
	int ba = String_m_grade.toInt();//QString 转换为 char*  

	if (0< ba && ba  < 100)
	{ //在0-1范围内
		String_m_grade = QString::number(ba, 'f', 0);
	}
	else
	{ //设为默认值  
		String_m_grade = QString::number(m_dScore, 'f', 0);
	}


	ui.grade_edit->clear();
	ui.grade_edit->setText(String_m_grade);
	emit send_Grade(String_m_grade);
	//像素格式
	QString m_chPixelFormat = settings->value("m_chPixelFormat").toString();
	if (m_chPixelFormat.isEmpty())
	{
		//从相机获取参数
        GetPixelFormat();
	}
	else {
		ui.pixelformat_edit->clear();
		ui.pixelformat_edit->setText(m_chPixelFormat);
	}
	//宽
        GetWidth();
	//高
        GetHeight();
	
		//设置旋转
		double m_rotateIndex = settings->value("m_rotateIndex", "0").toDouble();
		 m_rotateIndexInt = static_cast<int>(m_rotateIndex);
		m_cameraThread->setRotateIndex(m_rotateIndexInt);

			defaultRotateIndexValue = m_rotateIndexInt;
		
		if (m_rotateIndexInt == 1 || m_rotateIndexInt == 3)
		{
			//交换长宽的值
			int temp = m_width;
			m_width = m_height;
			m_height = temp;
		}
		//xml路径
		QString m_xmlFilePath = settings->value("m_xmlFilePath").toString();
		if (m_xmlFilePath.isEmpty())
		{
			if (setLanguageItem == "zh")
			{
				AppendText("xml默认路径为空,请设置模板文件路径", Green);
			}
			else if (setLanguageItem == "en")
			{
				AppendText("The default path for XML is empty, please set the template file path", Red);
			}
			else if (setLanguageItem == "es")
			{
				AppendText("La ruta predeterminada de XML está vacía, establezca la ruta del archivo de plantilla", Red);
			}
		}
		else {
			getXMLPATH(m_xmlFilePath);
			if (setLanguageItem == "zh")
			{
				AppendText("加载路径读取xml文件完成", Green);
			}
			else if (setLanguageItem == "en")
			{
				AppendText("Loading path reading XML file completed", Green);
			}
			else if (setLanguageItem == "es")
			{
				AppendText("Se completa la ruta de carga para leer el archivo XML", Green);
			}
		}

    //设置参数
    OnBnClickedSetParameterButton();
	settings->endGroup();
	delete settings;
}

void industrialVision::SaveInitializationParameters()
{
	//保存关闭时的状态
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
	settings->setValue("language", setLanguageItem);

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

        //窗口切换设置成true
	QList<QAction*> actions = ui.menuBar->actions();
	for (int i = 0; i < actions.size(); i++)
	{
		QAction* actionItem = actions.at(i);
		if (actionItem->text() == "实时显示"|| actionItem->text() == "Display"|| actionItem->text() == "Exhibición")
		{
            continue;
		}
        actionItem->setVisible(small_window_flag);
	}
	ui.textEdit_2->setVisible(small_window_flag);
	ui.textBrowser_record->setVisible(small_window_flag);
	// 获取并保存当前窗口的位置和大小
	originalGeometry = geometry();

	setGeometry(50, 50, 200, 133);
	// 设置窗口可拖动调整大小，并设置最小和最大大小
	setMinimumSize(200, 133);  // 设置最小为200x200
	setMaximumSize(800, 533);  // 设置最大为800x533

	// 设置视频展示区域的比例
    small_window_flag = !small_window_flag;
    setWindowFlags(Qt::WindowStaysOnTopHint);
    show();
}
                                                      
//todo,,, 实时范围框
// 高级设置界面
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
		ui.openGLWidget->setPixmap(newPixmap, "");   //使用OpenGLWidget显示
	}
}

void industrialVision::slot_get_patternResult(QPointF resultPoint,int matchTime)
{
    resultPointF.setX(resultPoint.x());
	resultPointF.setY(resultPoint.y());
    matchTime_total = matchTime;
	createOncePattern();
}

//属性设置
void industrialVision::action_AttributesSet()
{
	//设置路径,与打开文件S
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
	if (setLanguageItem == "zh")
	{
		AppendText("模板图读取过程中错误,请重新设置模板图,或者模板图路径错误", Red);
	}
	else if (setLanguageItem == "en")
	{
		AppendText("There was an error during the template image reading process. Please reset the template image or the template image path is incorrect", Red);
	}
	else if (setLanguageItem == "es")
	{
		AppendText("Error durante la lectura del mapa de plantillas, vuelva a configurar el mapa de plantillas o el camino del mapa de plantillas es incorrecto", Red);
	}
}

void industrialVision::getRotateValue(int x)
{
	if (setLanguageItem == "zh")
	{
		AppendText("旋转图像...", Green);
	}
	else if (setLanguageItem == "en")
	{
		AppendText("Rotate Image...", Green);
	}
	else if (setLanguageItem == "es")
	{
		AppendText("Imagen giratoria......", Green);
	}
	m_cameraThread->setRotateIndex(x);
	//设置展示图片的长宽,用于模板读取的比例设置
	
		//交换长宽的值
		int temp = m_width;
		m_width = m_height;
		m_height = temp;
	
	if(!m_xmlpath.isEmpty()){
	if (!getPatternInfoFromXML(m_xmlpath)) {
		if (!m_processingThread->getmodelAndRealSclar()) {
			if (setLanguageItem == "zh")
			{
				AppendText("XML模板图与当前相机展示的图片比例不一致,无法匹配" + m_xmlpath, Gray);
			}
			else if (setLanguageItem == "en")
			{
				AppendText("The scale of the XML template image is inconsistent with the image displayed by the current camera and cannot be matched"+ m_xmlpath, Gray);
			}
			else if (setLanguageItem == "es")
			{
				AppendText("La imagen de la plantilla XML no coincide con la escala de la imagen mostrada por la Cámara actual y no puede coincidir" + m_xmlpath, Gray);
			}
			return;
		}
	}
	}
}

void industrialVision::actionPasswordAction()
{
	//打开密码设置界面 
	passwordSetItem.setcurrentRole(CURRENT_ROLE);
	passwordSetItem.show();
}

//切换用户
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

void industrialVision::setLanguageInfo(QString language)
{
	if (language=="zh")
	{
		languageSet_zh();
	}else if (language == "en")
	{
		languageSet_en();
	}else if (language == "es")
	{
		languageSet_es();
	}
}

void industrialVision::reinitialize() {
	//设置日志
	if (setLanguageItem == "zh")
	{
		AppendText("切换用户按钮点击", Green);
	}
	else if (setLanguageItem == "en")
	{
		AppendText("Switch user button click", Green);
	}
	else if (setLanguageItem == "es")
	{
		AppendText("Cambiar el botón del usuario y hacer clic", Green);
	}
	setWindowTitle("V-Gp System V1.0");
	//重新加载设置界
	//重新设置标题
}

void industrialVision::resetParameters()
{
	centerPoint.setX(0);
	centerPoint.setY(0);
	areaNodeREAL_size.setX(0);
	areaNodeREAL_size.setY(0);
	areaNodeREAL_size.setWidth(0);
	areaNodeREAL_size.setHeight(0);
	//重置rect
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



//中文语言设置
void industrialVision::languageSet_zh()
{
	//设置当前语言环境
	QString settingPath = QCoreApplication::applicationDirPath() + "/setting.ini";
	QSettings* settings = new QSettings(settingPath, QSettings::IniFormat);
	settings->beginGroup("Idus");
	settings->setValue("language","zh");
	settings->endGroup();
	setLanguageItem = "zh";
	delete settings;

	ui.pushButton_continuousOperation->setText("启动");
	ui.pushButton_editVision->setText("视觉模板");
	ui.pushButton_manualOperation->setText("锁定状态");
	ui.pushButton_connect->setText("通讯");
	ui.pushButton_stopOperation->setText("停止");
	ui.label_9->setText("状态");
	ui.textEdit_2->setText("日志记录");
	ui.label_8->setText("相机参数");
	ui.get_camera_Parameter_button->setText("获取参数");
	ui.set_camera_Parameter_button->setText("设置参数");
	ui.label_10->setText("相机型号:");
	ui.label_5->setText("图片宽度:");
	ui.label_6->setText("图片高度:");
	ui.label_4->setText("像素格式:");
	ui.label->setText("曝光时间:");
	ui.label_2->setText("相机增益:");
	ui.label_3->setText("相机帧率:");
	ui.label_7->setText("得分设置:");
	action_SetModelFile->setText("工程导入");
	action_RestoreWindow->setText("实时显示");
	action_SetAttributes->setText("图标设置");
	action_password->setText("密码设置");
	action_setLogoPath->setText("日志设置");
	SettingMenus->setTitle("设置");
	action_userSwitch->setText("用户切换");
	action_helpInfo->setText("帮助");
	LanguageMenus->setTitle("语言");
	ui.run_time_label->setText("运行时长");
	ui.lineEdit->setText("运行次数");

	//各个界面设置
	//1.myglwidget
	emit send_system_language("zh");
	//2.登录界面
	emit send_login_language("zh");
	//3.密码设置
	passwordSetItem.setPasswordLanguage("zh");
	//4.日志设置
	logoPathItem.setlogPathItem_language("zh");
	//5.模板创造界面
	createModelItem.setcreateModelItem_language("zh");
	//6.创建服务端界面
	connectValual.setconnectValual_language("zh");
}

void industrialVision::languageSet_en()
{
	QString settingPath = QCoreApplication::applicationDirPath() + "/setting.ini";
	QSettings* settings = new QSettings(settingPath, QSettings::IniFormat);
	settings->beginGroup("Idus");
	settings->setValue("language", "en");
	setLanguageItem = "en";
	settings->endGroup();
	delete settings;

	//设置当前界面
	ui.pushButton_continuousOperation->setText("Start");
	ui.pushButton_editVision->setText("Templates");
	ui.pushButton_manualOperation->setText("Locked state");
	ui.pushButton_connect->setText("Communication");
	ui.pushButton_stopOperation->setText("Stop");
	ui.label_9->setText("State");
	ui.textEdit_2->setText("Logging");
	ui.label_8->setText("Camera parameters");
	ui.get_camera_Parameter_button->setText("Get parameters");
	ui.set_camera_Parameter_button->setText("Set parameters");
	ui.label_10->setText("Camera model:");
	ui.label_5->setText("Image width:");
	ui.label_6->setText("Image height:");
	ui.label_4->setText("Pixel format:");
	ui.label->setText("Exposure time:");
	ui.label_2->setText("Camera gain:");
	ui.label_3->setText("Frame rate:");
	ui.label_7->setText("Score setting:");

	action_SetModelFile->setText("Import");
	action_RestoreWindow->setText("Display");
	action_SetAttributes->setText("Icon");
	action_password->setText("Password");
	action_setLogoPath->setText("Log");
	SettingMenus->setTitle("Setting");
	action_userSwitch->setText("Switching");
	action_helpInfo->setText("Help");
	LanguageMenus->setTitle("Language");
	ui.run_time_label->setText("Run time");
	ui.lineEdit->setText("Runs");

	//1.myglwidget
	emit send_system_language("en");
	//2.登录界面
	emit send_login_language("en");
	//3.密码设置
	passwordSetItem.setPasswordLanguage("en");
	//4.日志设置
	logoPathItem.setlogPathItem_language("en");
	//5.模板创造界面
	createModelItem.setcreateModelItem_language("en");
	//6.创建服务端界面
	connectValual.setconnectValual_language("en");
}

void industrialVision::languageSet_es()
{
	QString settingPath = QCoreApplication::applicationDirPath() + "/setting.ini";
	QSettings* settings = new QSettings(settingPath, QSettings::IniFormat);
	settings->beginGroup("Idus");
	settings->setValue("language", "es");
	setLanguageItem = "es";
	settings->endGroup();
	delete settings;

	//设置当前界面
	ui.pushButton_continuousOperation->setText("Inicio");
	ui.pushButton_editVision->setText("Plantilla");
	ui.pushButton_manualOperation->setText("Bloqueo");
	ui.pushButton_connect->setText("Expresión");
	ui.pushButton_stopOperation->setText("Parar");
	ui.label_9->setText("Estado");
	ui.textEdit_2->setText("Inicio de sesión");
	ui.label_8->setText("Parámetros de la Cámara");
	ui.get_camera_Parameter_button->setText("Obtener parámetros");
	ui.set_camera_Parameter_button->setText("Establecer parámetros");
	ui.label_10->setText("Modelo de cámara:");
	ui.label_5->setText("Ancho de la imagen:");
	ui.label_6->setText("Altura de la imagen:");
	ui.label_4->setText("Formato de píxeles:");
	ui.label->setText("Tiempo de exposición:");
	ui.label_2->setText("Ganancia de la Cámara:");
	ui.label_3->setText("Tasa de fotogramas:");
	ui.label_7->setText("Puntuación:");

	action_SetModelFile->setText("Importación");
	action_RestoreWindow->setText("Exhibición");
	action_SetAttributes->setText("ídolo");
	action_password->setText("Palabras clave");
	action_setLogoPath->setText("Diario");
	SettingMenus->setTitle("Antecedentes");
	action_userSwitch->setText("Cambiar");
	action_helpInfo->setText("Ayudar");
	LanguageMenus->setTitle("Idiomas");
	ui.run_time_label->setText("Número de veces");
	ui.lineEdit->setText("Correr");


	//1.myglwidget
	emit send_system_language("es");
	//2.登录界面
	emit send_login_language("es");
	//3.密码设置
	passwordSetItem.setPasswordLanguage("es");
	//4.日志设置
	logoPathItem.setlogPathItem_language("es");
	//5.模板创造界面
	createModelItem.setcreateModelItem_language("es");
	//6.创建服务端界面
	connectValual.setconnectValual_language("es");

}