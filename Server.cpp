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
	QString timevalueQString = settings->value("timevalue","5000").toString();
	 language = settings->value("language", "zh").toString();

	timestart = timevalueQString.toInt();
	if (server->listen(QHostAddress::LocalHost, 60000)) {
		if (language=="zh")
		{
			emit logoString("服务器已启动，等待客户端连接...", "GREEN");
		}else if (language == "en")
		{
			emit logoString("Server started, waiting for client connection", "GREEN");
		}else if (language == "es")
		{
			emit logoString("El servidor está activado, esperando la conexión del cliente...", "GREEN");
		}
	}
	else {
			if (language == "zh")
			{
				emit logoString("无法启动服务器...", "Red");
			}
			else if (language == "en")
			{
				emit logoString("Unable to start server...", "Red");
			}
			else if (language == "es")
			{
				emit logoString("No se puede iniciar el servidor...", "Red");
			}
	}
}

void Server::onNewConnection()
{
	QTcpSocket* clientSocket = server->nextPendingConnection();
	QString x = QString::number(clientSocket->peerPort());

	if (language == "zh")
	{
		QString logString = "客户端 " + clientSocket->peerAddress().toString() + ":" + x + " 连接成功";
		emit logoString(logString, "GREEN");
	}
	else if (language == "en")
	{
		QString logString = "client" + clientSocket->peerAddress().toString() + ":" + x + " Connection successful";
		emit logoString(logString, "GREEN");
	}
	else if (language == "es")
	{
		QString logString = "Cliente" + clientSocket->peerAddress().toString() + ":" + x + "Conexión exitosa";
		emit logoString(logString, "GREEN");
	}


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
		QString logStringFromClient_err = "";
		if (language == "zh")
		{
			logStringFromClient_err = logStringFromClient_err + "服务端接受到消息异常: 请重启程序";
		}
		else if (language == "en")
		{
			logStringFromClient_err = logStringFromClient_err + "The server received a message exception: Please restart the program";
		}
		else if (language == "es")
		{
			logStringFromClient_err = logStringFromClient_err + "El servidor recibió un mensaje anormal: reinicie el programa";
		}
		emit logoString(logStringFromClient_err, "Red");

		return;
	}

	QByteArray data = clientSocket->readAll();
	QString message = QString(data);

	QString logStringFromClient ="";
	if (language == "zh")
	{
		logStringFromClient = logStringFromClient + "接收到来自客户端的消息:" + message;
	}
	else if (language == "en")
	{
		logStringFromClient = logStringFromClient + "Received message from client:" + message;
	}
	else if (language == "es")
	{
		logStringFromClient = logStringFromClient + "Se recibieron mensajes del cliente:" + message;
	}

	emit logoString(logStringFromClient, "GREEN");
	QString sendMessager;
	if (isJsonString(message)) {
		//取得ID
		QJsonDocument jsonDocument = QJsonDocument::fromJson(message.toUtf8());
		// 获取JSON对象
		QJsonObject jsonObject = jsonDocument.object();

		QJsonValue nameValue = jsonObject.value("CmdId");
		QString cmdID = nameValue.toString();

		//是json
		QJsonObject sendMessager = recvMsgByJson(message, cmdID);
		QJsonDocument document;

		document.setObject(sendMessager);

		QByteArray abyte = document.toJson(QJsonDocument::Compact);

		clientSocket->write(abyte);
		

		QString logStringToClient = "";
		if (language == "zh")
		{
			logStringToClient = logStringToClient + "给客户端发送数据:" + abyte;
		}
		else if (language == "en")
		{
			logStringToClient = logStringToClient + "Send data to the client" + abyte;
		}
		else if (language == "es")
		{
			logStringToClient = logStringToClient + "Enviar datos al cliente" + abyte;
		}
		emit logoString(logStringToClient, "GREEN");
	}
	else {
		//判断接受的数据格式 不是json

		//在这里可以对客户端消息进行处理
		 sendMessager = recvMsg(message);
		 clientSocket->write(sendMessager.toUtf8());
		 QString logStringToClient = "";

		 if (language == "zh")
		 {
			 logStringToClient = logStringToClient + "给客户端发送数据:" + sendMessager;
		 }
		 else if (language == "en")
		 {
			 logStringToClient = logStringToClient + "Send data to the client" + sendMessager;
		 }
		 else if (language == "es")
		 {
			 logStringToClient = logStringToClient + "Enviar datos al cliente" + sendMessager;
		 }

		 emit logoString(logStringToClient, "GREEN");
	}

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
		QString logStringToClient = "";
		if (language == "zh")
		{
			logStringToClient = logStringToClient + "定时器触发，中断当前处理";
		}
		else if (language == "en")
		{
			logStringToClient = logStringToClient + "Timer triggered, interrupting current processing";
		}
		else if (language == "es")
		{
			logStringToClient = logStringToClient + "Activación del cronómetro para interrumpir el procesamiento actual";
		}

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
		QString logStringToClient ="";
		if (language == "zh")
		{
			logStringToClient = logStringToClient + "定时器触发，中断当前处理";
		}
		else if (language == "en")
		{
			logStringToClient = logStringToClient + "Timer triggered, interrupting current processing";
		}
		else if (language == "es")
		{
			logStringToClient = logStringToClient + "Activación del cronómetro para interrumpir el procesamiento actual";
		}

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
		dataObject["MatchRatio"] = QString::number(finall_Total_Result.dMatchScore, 'f', 2);
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
