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
	QPixmap pixmap(100, 100);
	pixmap.fill(Qt::white);
	setWindowIcon(QIcon(pixmap));

  connect(this, &connectServer::send_ipAndPort, itemInfo, &connectItemInfo::reveive_ipAndPort);
  connect(subThread, &QThread::started, itemInfo, &connectItemInfo::Thread_deal);

	//this->setWindowFlags( Qt::WindowTitleHint);
}

Server connectServer::getConnectItemInfo()
{
	return itemInfo->serverItem;
}

connectServer::~connectServer()
{
	itemInfo->serverItem.close();
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

