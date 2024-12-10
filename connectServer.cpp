#include "connectServer.h"

connectServer::connectServer(QMainWindow*parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	//点击后,创建监听线程
	//itemInfo = new connectItemInfo(this);
	//itemInfo->moveToThread(subThread);
	//QObject::connect(subThread, &QThread::started, itemInfo, &connectItemInfo::Thread_deal);
	
	//白色填充左上角图标
	setWindowIcon(QIcon("icon.ico"));


  connect(this, &connectServer::send_ipAndPort, itemInfo, &connectItemInfo::reveive_ipAndPort);
  connect(subThread, &QThread::started, itemInfo, &connectItemInfo::Thread_deal);
  
  //读取语言
  QString settingPath = QCoreApplication::applicationDirPath() + "/setting.ini";
  QSettings* settings = new QSettings(settingPath, QSettings::IniFormat);
  settings->beginGroup("Idus");
  system_language = settings->value("system_language").toString();
  if (system_language == "en")
  {
	  ui.label_2->setText("Port：");
	  ui.pushButton_create_server->setText("Create server");
	  setWindowTitle("CONNECT");
  }
	//this->setWindowFlags( Qt::WindowTitleHint);
}

//Server connectServer::getConnectItemInfo()
//{
//	return itemInfo->serverItem;
//}

connectServer::~connectServer()
{
	//itemInfo->serverItem.close();
	subThread->requestInterruption();
	subThread->quit();
	subThread->wait();
	delete itemInfo;
	subThread->deleteLater();
	itemInfo = nullptr;
}



//创建点击事件
bool connectServer::create_server() {

	//检查ip地址是否正确
	QString ip = ui.lineEdit->text();
	if(!NetParameterAck::isIPv4(ip))
	{
		QMessageBox::warning(nullptr, tr("Path"),
			tr("ip或者端口设置出现问题."));
		return false;
	}
	QString port = ui.lineEdit_2->text();

	itemInfo->moveToThread(subThread);

	emit send_ipAndPort(ip,port);
	subThread->start();
	if (subThread->isRunning())
	{
		send_createServerCallBack("TRUE");
		this->close();
		return true;
	}
	QMessageBox::warning(nullptr, tr("通讯"),
		tr("ip或者端口设置出现问题."));
	return false;
}

