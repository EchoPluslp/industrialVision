#include "Server.h"
s_SingleTargetMatch finall_Total_Result; 
 
Server::Server() 
{
	server = new QTcpServer(this);

	connect(server, &QTcpServer::newConnection, this, &Server::onNewConnection);
	connect(this, &Server::triggerPattern, &TransmitSignals::GetInstance(), &TransmitSignals::create_once_pattern, Qt::UniqueConnection);
	connect(this, &Server::logoString, &TransmitSignals::GetInstance(), &TransmitSignals::sendToIndustrString,Qt::QueuedConnection);


	//  //��ȡ�ϴιر�ʱ��״̬
	QString settingPath = QCoreApplication::applicationDirPath() + "/setting.ini";
	QSettings* settings = new QSettings(settingPath, QSettings::IniFormat);
	settings->beginGroup("Idus");
	//��ʱ����ʱ��
	QString timevalueQString = settings->value("timevalue","40000").toString();
	timestart = timevalueQString.toInt();
	if (server->listen(QHostAddress::LocalHost, 1000)) {
		emit logoString("���������������ȴ��ͻ�������...", "GREEN");
	}
	else {
			emit logoString("�޷�����������...", "GREEN");
	}
	
	//// Create and start the PLC worker thread
	QThread* thread = new QThread;
	PlcWorker* worker = new PlcWorker(client);
	worker->moveToThread(thread);

	connect(thread, &QThread::started, worker, [worker]() { worker->startPolling(1000); }); // Poll every 1 second

	connect(thread, &QThread::finished, worker, &QObject::deleteLater);

	connect(worker, &PlcWorker::takeMattchPhoto, this,&Server::onReadyRead); 

	connect(worker, &PlcWorker::Relayer, this, &Server::RelayerServer); 

	thread->start();

}



void Server::RelayerServer()
{
	GlobalUniqueFace = 0;
}




void Server::onNewConnection()
{
	QTcpSocket* clientSocket = server->nextPendingConnection();
	QString x = QString::number(clientSocket->peerPort());
	QString logString =  "�ͻ��� " + clientSocket->peerAddress().toString() + ":" + x + " ���ӳɹ�";
	emit logoString(logString, "GREEN");

	clientQueue.clear();
	isBusy = false;

	// ���ͻ���������ӵ��������
	clientQueue.enqueue(clientSocket);

	// ������������У���ʼ��������
	if (!isBusy) {
		processNextRequest();
	}
}

void Server::onReadyRead()
{
	QTcpSocket* clientSocket = clientQueue.at(0);

	if (!clientSocket) {
		QString logStringFromClient = "onReadyRead: ������";
		return;
	}

	QString logStringFromClient = "��⵽������PLC��ֵ�䶯 ";// + message;    
	emit logoString(logStringFromClient, "GREEN");
	if (FaceStringListItemInfo.isEmpty())
	{
		return;
	}
	if (checkCurrFaceIsMatch(FaceStringListItemInfo.at(GlobalUniqueFace)))
	{
		//ִ��һ��ƥ��
		sendMessager = recvMsg("");
	}
	if (finall_Total_Result.ptCenter.x ==8888 && finall_Total_Result.ptCenter.y == 8888)
	{
		QString logStringFromClient = "�ӵ�һ�濪ʼ�����¿�ʼƥ�� ";
		GlobalUniqueFace = 0;
		return;
	}

	QString logStringToClient = "���ͻ��˷�������:" + sendMessager;
	emit logoString(logStringToClient, "GREEN");

	clientSocket->write("E3_DISSELENT ALLLAYER\r\n");
	QString logStringFromClient_Layer = "E3_SELECTLAYER";
	logStringFromClient_Layer.append(" ");
	logStringFromClient_Layer.append(FaceStringListItemInfo.at(GlobalUniqueFace++));
	logStringFromClient_Layer.append("\r\n");
	clientSocket->write(logStringFromClient_Layer.toUtf8());

	clientSocket->write("E3_SelectEnt ALL\r\n");
	if (!sendMessager.isEmpty())
	{
		clientSocket->write(sendMessager.toUtf8());
	}
	else {
			//�����һ��û�������Ӿ��㣬��Ĭ������
		clientSocket->write("E3_StartMark Select\r\n");
	}

	// ���ʹ�������Ϣ�ؿͻ���
	// ����������󣬼���������һ������
	if ((GlobalUniqueFace >= FaceStringListItemInfo.size()))
	{
		//��ͷ��ʼ
		GlobalUniqueFace = 0;
	}
	QString logStringToFace = "��ǰδִ�е�����:" + FaceStringListItemInfo.at(GlobalUniqueFace);
	logStringToFace += "!";
	emit logoString(logStringToFace, "GREEN");

	processNextRequest();
}

Server::~Server()
{
	server->close();
}

QString Server::recvMsg(QString receiveMessage)
{



		//emit logoString("ִ��", "GREEN");

	QString send_buf = "E3_StartMark Select X=";
	//if (receiveMessage <= 0)
	//{
	//	QString logStringToClient = "����receiveMessage�����쳣:";
	//	emit logoString(logStringToClient, "RED");
	//	return false;
	//}
	emit triggerPattern(); 	
	// ����һ����ʱ��
	QTimer timer;
	timer.setSingleShot(true); // ����Ϊ���δ���

	// ���Ӷ�ʱ����timeout�źŵ�һ���ۺ������òۺ����ڶ�ʱ����ʱʱ����
	QObject::connect(&timer, &QTimer::timeout, [&]() {
		// �ڶ�ʱ����ʱʱִ���жϴ���
		QString logStringToClient = "��ʱ���������жϵ�ǰ����";
		emit logoString(logStringToClient, "GRAY");

		finall_Total_Result.flag = true; // �޸�flag��ֵ
		finall_Total_Result.pattern_flag = false;
		});

	timer.start(timestart); // ������ʱ�������ó�ʱʱ��Ϊ4��v      

	while (!finall_Total_Result.flag) {
		// ������ȴ���ֱ����ʱ��������flag��Ϊtrue
		QCoreApplication::processEvents(); // ����Qt�¼�����
	} 
	//��ʱ��ֹͣ
	timer.stop();
	timer.deleteLater();

	//���            
	finall_Total_Result.flag = false;
	if (finall_Total_Result.pattern_flag) {
		char xx[10];
		char xxx[10];

		float ptCentX = finall_Total_Result.ptCenter.x / 2;
		float decimalPartX = ptCentX - static_cast<int>(ptCentX);
		//if (abs(decimalPartX) < 0.2) {
		//	// ���С������
		//	finall_Total_Result.ptCenter.x = static_cast<int>(ptCentX);
		//}
		//else {
		//	finall_Total_Result.ptCenter.x = ptCentX;
		//}

		sprintf(xxx, "%.1f", ptCentX);
		send_buf.append(xxx);

		send_buf.append(" Y=");
		float ptCentY = finall_Total_Result.ptCenter.y / 2;
		float decimalPartY = ptCentY - static_cast<int>(ptCentY);
		//if (abs(decimalPartY) < 0.2) {
		//	// ���С������
		//	finall_Total_Result.ptCenter.y = static_cast<int>(ptCentY);
		//}
		//else {
		//	finall_Total_Result.ptCenter.y = ptCentY;
		//}
		sprintf(xx, "%.1f", ptCentY);

		send_buf.append(xx);
		send_buf.append(" ");
		send_buf.append("\r\n");
	}
	else {
		QString errSend = "E3_StartMark Select\r\n";
		return errSend;
	}
	return send_buf;
}

QJsonObject Server::recvMsgByJson(QString receiveMessage, QString cmdID)
{
	// ����һ���յ� JSON ����
	QJsonObject jsonObject;

	if (receiveMessage <= 0)
	{
		QString logStringToClient = "����receiveMessage�����쳣:";
		emit logoString(logStringToClient, "RED");
		return jsonObject;
	}
	emit triggerPattern();
	// ����һ����ʱ��
	QTimer timer;
	timer.setSingleShot(true); // ����Ϊ���δ���

	// ���Ӷ�ʱ����timeout�źŵ�һ���ۺ������òۺ����ڶ�ʱ����ʱʱ����
	QObject::connect(&timer, &QTimer::timeout, [&]() {
		// �ڶ�ʱ����ʱʱִ���жϴ���
		QString logStringToClient = "��ʱ���������жϵ�ǰ����";
		emit logoString(logStringToClient, "GRAY");

		finall_Total_Result.flag = true; // �޸�flag��ֵ
		finall_Total_Result.pattern_flag = false;
		});

	timer.start(timestart); // ������ʱ�������ó�ʱʱ��Ϊ1��

	while (!finall_Total_Result.flag) {
		// ������ȴ���ֱ����ʱ��������flag��Ϊtrue
		QCoreApplication::processEvents(); // ����Qt�¼�����
	}
	//��ʱ��ֹͣ
	timer.stop();
	timer.deleteLater();
	//����flagֵ
	finall_Total_Result.flag = false;

		jsonObject["CmdId"] = cmdID;	    
		jsonObject["ErrCode"] = "0";
		jsonObject["ErrDesc"] = "";
		// ���� "Datas" ����
		QJsonArray datasArray;
		QJsonObject dataObject;

		//ƥ���Ƿ�ɹ�
		if (finall_Total_Result.pattern_flag) {

			dataObject["isOK"] = "OK";
		}
		else {
			dataObject["isOK"] = "NG";
		}

		dataObject["x"] = QString::number(finall_Total_Result.ptCenter.x, 'f', 1);
		dataObject["y"] = QString::number(finall_Total_Result.ptCenter.y, 'f', 1);
		dataObject["a"] = QString::number(finall_Total_Result.dMatchedAngle, 'f', 1);
		dataObject["MatchRatio"] = "1.0";
		// ���Ӷ�����ӵ� "Datas" ����
		datasArray.append(dataObject);

		//��ӵ�json������
		jsonObject["Datas"] = datasArray;

		jsonObject["ReslutImage"] = "";

		// �� JSON ����ת��Ϊ JSON �ĵ�
		QJsonDocument jsonDocument(jsonObject);

		// �� JSON �ĵ�ת��Ϊ�ַ���
		QString jsonString = jsonDocument.toJson(QJsonDocument::Compact);

	
	return jsonObject;
}


void Server::processNextRequest()
{
	if (clientQueue.isEmpty()) {
		// ����Ϊ�գ�û�еȴ�������
		isBusy = false;
		return;
	}

	// �Ӷ�����ȡ����һ���ͻ�������
	//QTcpSocket* clientSocket = clientQueue.dequeue();

	//// ����ͻ�������
	//isBusy = true;
	////onReadyRead();
	//connect(clientSocket, &QTcpSocket::readyRead, this, &Server::onReadyRead);
}

bool Server::isJsonString(const QString& str) {
	QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());
	return !doc.isNull() && !doc.isEmpty() && doc.isObject();
}
bool Server::checkCurrFaceIsMatch(QString currentFace)
{
	return VisionPointListItemInfo.contains(currentFace);
}

