#include "connectServer.h"

connectServer::connectServer(QMainWindow*parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	//�����,���������߳�
	//itemInfo = new connectItemInfo(this);
	//itemInfo->moveToThread(subThread);
	//QObject::connect(subThread, &QThread::started, itemInfo, &connectItemInfo::Thread_deal);
	
	//��ɫ������Ͻ�ͼ��
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



//��������¼�
bool connectServer::create_server() {

	//���ip��ַ�Ƿ���ȷ
	QString ip = ui.lineEdit->text();
	if(!NetParameterAck::isIPv4(ip))
	{
		QMessageBox::warning(nullptr, tr("Path"),
			tr("ip���߶˿����ó�������."));
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
	QMessageBox::warning(nullptr, tr("ͨѶ"),
		tr("ip���߶˿����ó�������."));
	return false;
}

