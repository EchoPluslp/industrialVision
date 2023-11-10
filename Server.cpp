#include "Server.h"
s_SingleTargetMatch finall_Total_Result; 
 
Server::Server() 
{
	server = new QTcpServer(this);

	connect(server, &QTcpServer::newConnection, this, &Server::onNewConnection);
	connect(this, &Server::triggerPattern, &TransmitSignals::GetInstance(), &TransmitSignals::create_once_pattern, Qt::UniqueConnection);
	connect(this, &Server::logoString, &TransmitSignals::GetInstance(), &TransmitSignals::sendToIndustrString,Qt::QueuedConnection);

	//  //读取上次关闭时的状态
	QString settingPath = QCoreApplication::applicationDirPath() + "/setting.ini";
	QSettings* settings = new QSettings(settingPath, QSettings::IniFormat);
	settings->beginGroup("Idus");
	//定时设置时间
	QString timevalueQString = settings->value("timevalue","1000").toString();
	timestart = timevalueQString.toInt();
	if (server->listen(QHostAddress::LocalHost, 60000)) {
		emit logoString("服务器已启动，等待客户端连接...", "GREEN");
	}
	else {
			emit logoString("无法启动服务器...", "GREEN");
	}
}

void Server::onNewConnection()
{
	QTcpSocket* clientSocket = server->nextPendingConnection();
	QString x = QString::number(clientSocket->peerPort());
	QString logString =  "客户端 " + clientSocket->peerAddress().toString() + ":" + x + " 连接成功";
	emit logoString(logString, "GREEN");

	clientQueue.clear();
	isBusy = false;

	// 将客户端连接添加到请求队列
	clientQueue.enqueue(clientSocket);

	// 如果服务器空闲，开始处理请求
	if (!isBusy) {
		processNextRequest();
	}
}
void Server::onReadyRead()
{
	QTcpSocket* clientSocket = qobject_cast<QTcpSocket*>(sender());

	if (!clientSocket) {
		QString logStringFromClient = "服务端接受到消息异常: 请重启";
		return;
	}

	QByteArray data = clientSocket->readAll();
	QString message = QString(data);

	QString logStringFromClient =  "接收到来自客户端的消息: " + message;
	emit logoString(logStringFromClient, "GREEN");


	// 在这里可以对客户端消息进行处理，例如将消息转为大写
	QString sendMessager = recvMsg(message);

	// 发送处理后的消息回客户端
	clientSocket->write(sendMessager.toUtf8());
	// 处理完请求后，继续处理下一个请求
	processNextRequest();

	QString logStringToClient = "给客户端发送数据:" + sendMessager;

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
		QString logStringToClient = "接受receiveMessage函数异常:";
		emit logoString(logStringToClient, "RED");
		return false;
	}
	emit triggerPattern(); 	
	// 创建一个定时器
	QTimer timer;
	timer.setSingleShot(true); // 设置为单次触发

	// 连接定时器的timeout信号到一个槽函数，该槽函数在定时器超时时触发
	QObject::connect(&timer, &QTimer::timeout, [&]() {
		// 在定时器超时时执行中断处理
		QString logStringToClient = "定时器触发，中断当前处理";
		emit logoString(logStringToClient, "GRAY");

		finall_Total_Result.flag = true; // 修改flag的值
		finall_Total_Result.pattern_flag = false;
		});

	timer.start(timestart); // 启动定时器，设置超时时间为1秒

	while (!finall_Total_Result.flag) {
		// 在这里等待，直到定时器触发或flag变为true
		QCoreApplication::processEvents(); // 允许Qt事件处理
	}
	//定时器停止
	timer.stop();
	timer.deleteLater();
	//重置flag值
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
		//保留1个小数点
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
		// 队列为空，没有等待的请求
		isBusy = false;
		return;
	}

	// 从队列中取出下一个客户端连接
	QTcpSocket* clientSocket = clientQueue.dequeue();

	// 处理客户端请求
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
//		//保留1个小数点
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