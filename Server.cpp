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



//Server::Server() {
//	servSock = INVALID_SOCKET;
//	
//	finall_Total_Result.flag = false;
//	connect(this, &Server::triggerPattern, &TransmitSignals::GetInstance(), &TransmitSignals::create_once_pattern, Qt::UniqueConnection);
//	clientList.clear();
//}
//Server::~Server()
//{
//	close();
//}
//Server::Server(const Server& other)
//{
//	servSock = other.servSock;
//}
//
//void Server::close()
//{
//	if (servSock != INVALID_SOCKET)
//	{
//		closesocket(servSock);
//		if (!clientList.empty()) {
//			for (auto client : clientList)
//				closesocket(client); 
//		}
//		WSACleanup();
//	}
//}
//bool Server::init(const char* ip, unsigned short port)
//{
//	WSADATA data{};
//	if (WSAStartup(MAKEWORD(2, 2), &data) == SOCKET_ERROR)
//	{
//		std::cout << "WSAStartup func error, error num is : " << WSAGetLastError() << std::endl;
//		return false;
//	}
//
//	servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//	if (servSock == INVALID_SOCKET)
//	{
//		std::cout << "socket func error, error num is : " << WSAGetLastError() << std::endl;
//		WSACleanup();
//		return false;
//	}
//
//	sockaddr_in servAddr{};
//	servAddr.sin_family = AF_INET;
//	servAddr.sin_port = htons(port);
//	inet_pton(AF_INET, ip, &servAddr.sin_addr.S_un.S_addr);
//	if (bind(servSock, (sockaddr*)&servAddr, sizeof(sockaddr)) == SOCKET_ERROR)
//	{
//		std::cout << "bind func error, error num is : " << WSAGetLastError() << std::endl;
//		closesocket(servSock);
//		servSock = INVALID_SOCKET;
//		WSACleanup();
//		return false;
//	}
//
//	if (listen(servSock, 128) == SOCKET_ERROR)
//	{
//		std::cout << "listen func error, error num is : " << WSAGetLastError() << std::endl;
//		closesocket(servSock);
//		servSock = INVALID_SOCKET;
//		WSACleanup();
//		return false;
//	}
//	return true;
//}
//bool Server::start()
//{
//	if (!coreFunc())
//	{
//		return false;
//	}
//}
//void Server::setMatchAngle(double angle)
//{
//	dMatchAngle = angle;
//}
//void Server::setTCPCanSendStatus(bool flag)
//{
//	operator_TCPCanSend = flag;
//}
//bool Server::coreFunc()
//{
//	assert(servSock != INVALID_SOCKET);
//	while (!QThread::currentThread()->isInterruptionRequested())
//	{
//		fd_set fdRead;
//		FD_ZERO(&fdRead);
//		FD_SET(servSock, &fdRead);
//		for (auto client : clientList)
//		{
//			FD_SET(client, &fdRead);
//		}
//		timeval tv{ 1, 0 };
//		int selectRes = select(0, &fdRead, nullptr, nullptr, &tv);
//		if (selectRes > 0)
//		{
//			for (unsigned i = 0; i < fdRead.fd_count; ++i)
//			{
//				if (fdRead.fd_array[i] == servSock)
//				{
//					SOCKET clntSock = accept(servSock, nullptr, nullptr);
//					if (clntSock == INVALID_SOCKET)
//					{
//						std::cout << "accept func error, error num is : " << WSAGetLastError() << std::endl;
//						break;
//					}
//					clientList.push_back(clntSock);
//				}
//				else
//				{
//					recvMsg(fdRead, fdRead.fd_array[i]);
//				}
//			}
//		}
//		else if (selectRes == 0)
//		{
//			std::cout << "do something else" << std::endl;
//			continue;
//		}
//		else
//		{
//			std::cout << "select func error, error num : " << WSAGetLastError() << std::endl;
//			return false;
//		}
//	}
//	return true;
//}
//bool Server::recvMsg(fd_set& fdRead, SOCKET clntSock)
//{
//	char send_buf[BUFSIZ] = { 0 };
//	char recv_buf[BUFSIZ] = { 0 };
//	int recvRes = recv(clntSock, recv_buf, 512, 0);
//	if (recvRes <= 0)
//	{
//		std::cout << "recv func error, error num is : " << WSAGetLastError() << std::endl;
//		return false;
//	}
//	if (recv_buf[0] == '\0') {
//		send(clntSock, recv_buf, BUFSIZ, 0);
//	}
//	emit triggerPattern();
//	while (!finall_Total_Result.flag) {
//
//	}
//	finall_Total_Result.flag = false;
//	if (finall_Total_Result.pattern_flag) {
//		strcpy(send_buf, recv_buf);
//		strcat_s(send_buf, "1;1;1,");
//
//		char s[10];
//		char xx[10];
//		if (fabsf(finall_Total_Result.ptCenter.x) < 2) {
//			finall_Total_Result.ptCenter.x = 0;
//		}
//		if (fabsf(finall_Total_Result.ptCenter.y) < 2) {
//			finall_Total_Result.ptCenter.y = 0;
//		}
//		sprintf(s, "%.1f", finall_Total_Result.ptCenter.x);
//		strcat_s(send_buf, s);
//		strcat_s(send_buf, ",");
//		sprintf(xx, "%.1f", finall_Total_Result.ptCenter.y);
//		strcat_s(send_buf, xx);
//
//		strcat_s(send_buf, ",");
//
//		std::ostringstream out;
//		//����1��С����
//		out << std::fixed << std::setprecision(1) << finall_Total_Result.dMatchedAngle;
//		std::string angle_String = out.str();
//
//		char* p = new char[strlen(angle_String.c_str()) + 1];
//		strcpy(p, angle_String.c_str());
//		strcat_s(send_buf, p);
//		strcat_s(send_buf, "#;");
//	}
//	else {
//		strcpy(send_buf, recv_buf);
//		strcat_s(send_buf, "0;1;0,999,999,999#");
//	}
//	if (send(clntSock, send_buf, BUFSIZ, 0) < 0)
//	{
//		std::cout << "send func error, error num is : " << WSAGetLastError() << std::endl;
//		return false;
//	}
//}