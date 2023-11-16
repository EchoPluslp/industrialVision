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
	QTcpSocket* clientSocket = qobject_cast<QTcpSocket*>(sender());

	if (!clientSocket) {
		QString logStringFromClient = "����˽��ܵ���Ϣ�쳣: ������";
		return;
	}

	QByteArray data = clientSocket->readAll();
	QString message = QString(data);

	QString logStringFromClient =  "���յ����Կͻ��˵���Ϣ: " + message;
	emit logoString(logStringFromClient, "GREEN");


	// ��������ԶԿͻ�����Ϣ���д������罫��ϢתΪ��д
	QString sendMessager = recvMsg(message);

	// ���ʹ�������Ϣ�ؿͻ���
	clientSocket->write(sendMessager.toUtf8());
	// ����������󣬼���������һ������
	processNextRequest();

	QString logStringToClient = "���ͻ��˷�������:" + sendMessager;

	emit logoString(logStringToClient, "GREEN");

}


Server::~Server()
{
	server->close();
}

QString Server::recvMsg(QString receiveMessage)
{
	QString send_buf = "T;1;100;1;1;1,";
	if (receiveMessage <= 0)
	{
		QString logStringToClient = "����receiveMessage�����쳣:";
		emit logoString(logStringToClient, "RED");
		return false;
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
	if (finall_Total_Result.pattern_flag){
		char s[10];
		char xx[10];
		sprintf(s, "%.1f", finall_Total_Result.ptCenter.x);
		send_buf.append(s);
		send_buf.append(",");

		sprintf(xx, "%.1f", finall_Total_Result.ptCenter.y);
		send_buf.append(xx);
		send_buf.append(",");

		std::ostringstream out;
		//����1��С����
		out << std::fixed << std::setprecision(1) << finall_Total_Result.dMatchedAngle;
		std::string angle_String = out.str();

		char* p = new char[strlen(angle_String.c_str()) + 1];
		strcpy(p, angle_String.c_str());
		send_buf.append(p);
		send_buf.append("#;");
		delete[] p;
	}
	else {
		QString errSend  = "T;1;100;0;1;0,999,999,999#;";
		return errSend;
	}
	return send_buf;
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
