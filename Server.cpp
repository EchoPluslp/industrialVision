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
	QString timevalueQString = settings->value("timevalue","1000").toString();
	timestart = timevalueQString.toInt();
	if (server->listen(QHostAddress::LocalHost, 60000)) {
		emit logoString("���������������ȴ��ͻ�������...", "GREEN");
	}
	else {
			emit logoString("�޷�����������...", "GREEN");
	}

	//// Create and start the PLC worker thread
	QThread* thread = new QThread;
	PlcWorker* worker = new PlcWorker();
	worker->moveToThread(thread);

	connect(thread, &QThread::started, worker, [worker]() { worker->startPolling(1000); }); // Poll every 1 second

	connect(thread, &QThread::finished, worker, &QObject::deleteLater);

	connect(worker, &PlcWorker::takeMattchPhoto, this, &Server::onReadyRead);

//	connect(worker, &PlcWorker::Relayer, this, &Server::RelayerServer);

	thread->start();


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
	//QTcpSocket* clientSocket = qobject_cast<QTcpSocket*>(sender());

	//if (!clientSocket) {
	//	QString logStringFromClient = "����˽��ܵ���Ϣ�쳣: ������";
	//	return;
	//}

	//QByteArray data = clientSocket->readAll();
	QString message = QString();

	QString logStringFromClient =  "���յ�PLC��������: " + message;
	emit logoString(logStringFromClient, "GREEN");
	recvMsg("");
	// ���ʹ�������Ϣ�ؿͻ���
// ����������󣬼���������һ������
	//processNextRequest();
	return;

	//QString sendMessager;
	//if (isJsonString(message)) {
	//	//ȡ��ID
	//	QJsonDocument jsonDocument = QJsonDocument::fromJson(message.toUtf8());
	//	// ��ȡJSON����
	//	QJsonObject jsonObject = jsonDocument.object();

	//	QJsonValue nameValue = jsonObject.value("CmdId");
	//	QString cmdID = nameValue.toString();

	//	//��json
	//	QJsonObject sendMessager = recvMsgByJson(message, cmdID);
	//	QJsonDocument document;

	//	document.setObject(sendMessager);

	//	QByteArray abyte = document.toJson(QJsonDocument::Compact);

	//	//clientSocket->write(abyte);
	//	QString logStringToClient = "���ͻ��˷�������:" + abyte;

	//	emit logoString(logStringToClient, "GREEN");
	//}
	//else {
	//	//�жϽ��ܵ����ݸ�ʽ ����json

	//	//��������ԶԿͻ�����Ϣ���д���
	//	 sendMessager = recvMsg(message);
	//	// clientSocket->write(sendMessager.toUtf8());
	//	 QString logStringToClient = "���ͻ��˷�������:" + sendMessager;

	//	 emit logoString(logStringToClient, "GREEN");
	//}

	//// ���ʹ�������Ϣ�ؿͻ���
	//// ����������󣬼���������һ������
	//processNextRequest();

	

}


Server::~Server()
{
	server->close();
}

QString Server::recvMsg(QString receiveMessage)
{
	//QString send_buf = "T;1;100;1;1;1,";
	//if (receiveMessage <= 0)
	//{
	//	QString logStringToClient = "����receiveMessage�����쳣:";
	//	emit logoString(logStringToClient, "RED");
	//	return false;
	//}
	finall_Total_Result.flag = false;
	emit triggerPattern(); 	
	// ����һ����ʱ��
	//QTimer timer;
	//timer.setSingleShot(true); // ����Ϊ���δ���

	//// ���Ӷ�ʱ����timeout�źŵ�һ���ۺ������òۺ����ڶ�ʱ����ʱʱ����
	//QObject::connect(&timer, &QTimer::timeout, [&]() {
	//	// �ڶ�ʱ����ʱʱִ���жϴ���
	//	QString logStringToClient = "��ʱ���������жϵ�ǰ����";
	//	emit logoString(logStringToClient, "GRAY");

	//	finall_Total_Result.flag = true; // �޸�flag��ֵ
	//	finall_Total_Result.pattern_flag = false;
	//	});

	//timer.start(1000); // ������ʱ�������ó�ʱʱ��Ϊ1��

	while (!finall_Total_Result.flag) {
		// ������ȴ���ֱ����ʱ��������flag��Ϊtrue
		QCoreApplication::processEvents(); // ����Qt�¼�����
	}
	////��ʱ��ֹͣ
	//timer.stop();
	//timer.deleteLater();
	//����flagֵ 
	finall_Total_Result.flag = false;
	if (finall_Total_Result.pattern_flag) {

		QFile Wfile("C:/tmp/temp2.txt");
		if (!Wfile.open(QIODevice::WriteOnly | QIODevice::Text)) {
			// ����ļ��޷��򿪣�����ѡ���׳�һ���쳣������һ��������Ϣ�����߷���һ�����ַ���
			return ""; // ����򵥵ط���һ�����ַ���
		}
		QTextStream out(&Wfile);

		if (finall_Total_Result.ptCenter.x == 1 && finall_Total_Result.ptCenter.y == 1)
		{
			out << "1";  //û����
		}
		else {
			out << "2"; //������

		}
		Wfile.close();
		//char s[10];
		//char xx[10];
		//sprintf(s, "%.1f", finall_Total_Result.ptCenter.x);
		//send_buf.append(s);
		//send_buf.append(",");

		//sprintf(xx, "%.1f", finall_Total_Result.ptCenter.y);
		//send_buf.append(xx);
		//send_buf.append(",");

		//std::ostringstream out;
		////����1��С����
		//out << std::fixed << std::setprecision(1) << finall_Total_Result.dMatchedAngle;
		//std::string angle_String = out.str();

		//char* p = new char[strlen(angle_String.c_str()) + 1];
		//strcpy(p, angle_String.c_str());
		//send_buf.append(p);
		//send_buf.append("#;");
		//delete[] p;
		return "";
	}
	else {
		QString errSend  = "T;1;100;0;1;0,999,999,999#;";
		return errSend;
	}
	return "";
	//return send_buf;
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
	QTcpSocket* clientSocket = clientQueue.dequeue();

	// ����ͻ�������
	isBusy = true;
	//onReadyRead();
	connect(clientSocket, &QTcpSocket::readyRead, this, &Server::onReadyRead);
}

bool Server::isJsonString(const QString& str) {
	QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());
	return !doc.isNull() && !doc.isEmpty() && doc.isObject();
}
