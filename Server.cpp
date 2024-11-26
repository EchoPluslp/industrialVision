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
	//QTcpSocket* clientSocket = qobject_cast<QTcpSocket*>(sender());

	//if (!clientSocket) {
	//	QString logStringFromClient = "服务端接受到消息异常: 请重启";
	//	return;
	//}

	//QByteArray data = clientSocket->readAll();
	QString message = QString();

	QString logStringFromClient =  "接收到PLC触发拍照: " + message;
	emit logoString(logStringFromClient, "GREEN");
	recvMsg("");
	// 发送处理后的消息回客户端
// 处理完请求后，继续处理下一个请求
	//processNextRequest();
	return;

	//QString sendMessager;
	//if (isJsonString(message)) {
	//	//取得ID
	//	QJsonDocument jsonDocument = QJsonDocument::fromJson(message.toUtf8());
	//	// 获取JSON对象
	//	QJsonObject jsonObject = jsonDocument.object();

	//	QJsonValue nameValue = jsonObject.value("CmdId");
	//	QString cmdID = nameValue.toString();

	//	//是json
	//	QJsonObject sendMessager = recvMsgByJson(message, cmdID);
	//	QJsonDocument document;

	//	document.setObject(sendMessager);

	//	QByteArray abyte = document.toJson(QJsonDocument::Compact);

	//	//clientSocket->write(abyte);
	//	QString logStringToClient = "给客户端发送数据:" + abyte;

	//	emit logoString(logStringToClient, "GREEN");
	//}
	//else {
	//	//判断接受的数据格式 不是json

	//	//在这里可以对客户端消息进行处理
	//	 sendMessager = recvMsg(message);
	//	// clientSocket->write(sendMessager.toUtf8());
	//	 QString logStringToClient = "给客户端发送数据:" + sendMessager;

	//	 emit logoString(logStringToClient, "GREEN");
	//}

	//// 发送处理后的消息回客户端
	//// 处理完请求后，继续处理下一个请求
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
	//	QString logStringToClient = "接受receiveMessage函数异常:";
	//	emit logoString(logStringToClient, "RED");
	//	return false;
	//}
	finall_Total_Result.flag = false;
	emit triggerPattern(); 	
	// 创建一个定时器
	//QTimer timer;
	//timer.setSingleShot(true); // 设置为单次触发

	//// 连接定时器的timeout信号到一个槽函数，该槽函数在定时器超时时触发
	//QObject::connect(&timer, &QTimer::timeout, [&]() {
	//	// 在定时器超时时执行中断处理
	//	QString logStringToClient = "定时器触发，中断当前处理";
	//	emit logoString(logStringToClient, "GRAY");

	//	finall_Total_Result.flag = true; // 修改flag的值
	//	finall_Total_Result.pattern_flag = false;
	//	});

	//timer.start(1000); // 启动定时器，设置超时时间为1秒

	while (!finall_Total_Result.flag) {
		// 在这里等待，直到定时器触发或flag变为true
		QCoreApplication::processEvents(); // 允许Qt事件处理
	}
	////定时器停止
	//timer.stop();
	//timer.deleteLater();
	//重置flag值 
	finall_Total_Result.flag = false;
	if (finall_Total_Result.pattern_flag) {

		QFile Wfile("C:/tmp/temp2.txt");
		if (!Wfile.open(QIODevice::WriteOnly | QIODevice::Text)) {
			// 如果文件无法打开，可以选择抛出一个异常，返回一个错误消息，或者返回一个空字符串
			return ""; // 这里简单地返回一个空字符串
		}
		QTextStream out(&Wfile);

		if (finall_Total_Result.ptCenter.x == 1 && finall_Total_Result.ptCenter.y == 1)
		{
			out << "1";  //没问题
		}
		else {
			out << "2"; //有问题

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
		////保留1个小数点
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
	// 创建一个空的 JSON 对象
	QJsonObject jsonObject;

	if (receiveMessage <= 0)
	{
		QString logStringToClient = "接受receiveMessage函数异常:";
		emit logoString(logStringToClient, "RED");
		return jsonObject;
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

		jsonObject["CmdId"] = cmdID;	    
		jsonObject["ErrCode"] = "0";
		jsonObject["ErrDesc"] = "";
		// 设置 "Datas" 数组
		QJsonArray datasArray;
		QJsonObject dataObject;

		//匹配是否成功
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
		// 将子对象添加到 "Datas" 数组
		datasArray.append(dataObject);

		//添加到json对象中
		jsonObject["Datas"] = datasArray;

		jsonObject["ReslutImage"] = "";

		// 将 JSON 对象转换为 JSON 文档
		QJsonDocument jsonDocument(jsonObject);

		// 将 JSON 文档转换为字符串
		QString jsonString = jsonDocument.toJson(QJsonDocument::Compact);

	
	return jsonObject;
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

bool Server::isJsonString(const QString& str) {
	QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());
	return !doc.isNull() && !doc.isEmpty() && doc.isObject();
}
