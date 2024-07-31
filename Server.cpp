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
	QString timevalueQString = settings->value("timevalue","40000").toString();
	timestart = timevalueQString.toInt();
	if (server->listen(QHostAddress::LocalHost, 1000)) {
		emit logoString("服务器已启动，等待客户端连接...", "GREEN");
	}
	else {
			emit logoString("无法启动服务器...", "GREEN");
	}

	client = new QModbusTcpClient(this);
	const QUrl url = QUrl::fromUserInput("192.168.0.80:5001"); //;//获取IP和端口号

	client->setConnectionParameter(QModbusDevice::NetworkAddressParameter, url.host());
	client->setConnectionParameter(QModbusDevice::NetworkPortParameter, url.port());
	client->setTimeout(2000);
	client->setNumberOfRetries(3);

	bool ti = client->connectDevice();

	
	// Create and start the PLC worker thread
	QThread* thread = new QThread;
	PlcWorker* worker = new PlcWorker(client);
	worker->moveToThread(thread);

	connect(thread, &QThread::started, worker, [worker]() { worker->startPolling(1000); }); // Poll every 1 second

	connect(thread, &QThread::finished, worker, &QObject::deleteLater);

	connect(worker, &PlcWorker::takeMattchPhoto, this,&Server::onReadyRead); // Poll every 1 second


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
	QTcpSocket* clientSocket = clientQueue.at(0);

	if (!clientSocket) {
		QString logStringFromClient = "onReadyRead: 请重启";
		return;
	}

	//QByteArray data = clientSocket->readAll();
	//QString message = QString(data);

	QString logStringFromClient = "检测到到来自PLC的值变动 ";// + message;
	emit logoString(logStringFromClient, "GREEN");
	
		//判断接受的数据格式 不是json

		//在这里可以对客户端消息进行处理，触发匹配
	QString sendMessager = recvMsg("");
	if (finall_Total_Result.ptCenter.x ==8888 && finall_Total_Result.ptCenter.y == 8888)
	{
		QString logStringFromClient = "从第一面开始，重新开始匹配 ";
		return;
	}
	clientSocket->write(sendMessager.toUtf8());
	QString logStringToClient = "给客户端发送数据:" + sendMessager;
		emit logoString(logStringToClient, "GREEN");

		QString logStringToFace = "当前面数:" + QString::number(finall_Total_Result.currentIndex);
		logStringToFace += "!";
		emit logoString(logStringToFace, "GREEN");


	// 发送处理后的消息回客户端
	// 处理完请求后，继续处理下一个请求
	processNextRequest();
}


Server::~Server()
{
	server->close();
}

QString Server::recvMsg(QString receiveMessage)
{


	if (client->state() == QModbusDevice::ConnectedState)
	{
		emit logoString("连接到plc", "GREEN");

		//return;
	}


	QString send_buf = "E3_StartMark X=";
	//if (receiveMessage <= 0)
	//{
	//	QString logStringToClient = "接受receiveMessage函数异常:";
	//	emit logoString(logStringToClient, "RED");
	//	return false;
	//}
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

	timer.start(timestart); // 启动定时器，设置超时时间为4秒

	while (!finall_Total_Result.flag) {
		// 在这里等待，直到定时器触发或flag变为true
		QCoreApplication::processEvents(); // 允许Qt事件处理
	}
	//定时器停止
	timer.stop();
	timer.deleteLater();

	//填充
	finall_Total_Result.flag = false;
	if (finall_Total_Result.pattern_flag) {
		char xx[10];
		char xxx[10];

		float ptCentX = finall_Total_Result.ptCenter.x / 2;
		sprintf(xxx, "%.3f", ptCentX);
		send_buf.append(xxx);
		send_buf.append(" Y=");
		sprintf(xx, "%.3f", finall_Total_Result.ptCenter.y / 2);

		send_buf.append(xx);
		send_buf.append(" ");
		send_buf.append("\r\n");
	}
	else {
		QString errSend = "E3_StartMark";
		send_buf.append("\r\n");
		return errSend;
	}
	return send_buf;
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
	//QTcpSocket* clientSocket = clientQueue.dequeue();

	//// 处理客户端请求
	//isBusy = true;
	////onReadyRead();
	//connect(clientSocket, &QTcpSocket::readyRead, this, &Server::onReadyRead);
}

bool Server::isJsonString(const QString& str) {
	QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());
	return !doc.isNull() && !doc.isEmpty() && doc.isObject();
}
