#pragma once
#include<WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")                           
#include<iostream>
#include<list>
#include"QPointF"
#include<qobject.h>
#include "transmitsignals.h"
#include <WS2tcpip.h>
#include <assert.h>
#include"qthread.h"
#include <QtNetwork>
#include <QTcpServer>
#include <QTcpSocket>
#include "PlcWorker.h"

#include <QModbusTcpClient>
#include <QModbusDataUnit>  //Êý¾ÝÈÝÆ÷
#include <QModbusReply> 



#include <QCoreApplication>
#include<opencv2\opencv.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#pragma execution_character_set("utf-8")

struct s_SingleTargetMatch
{
	cv::Point2d ptLT, ptRT, ptRB, ptLB, ptCenter;
	double dMatchedAngle;
	double dMatchScore;
	bool flag = false;
	int width;
	int height;
	bool pattern_flag = false;
	int currentIndex;
};
class CustomData {
public:
	QString isOK;
	double x;
	double y;
	double a;
	double matchRatio;
};




class Server  :public QObject
{
	Q_OBJECT
public:
	Server();
	 //Server(QObject* parent = nullptr);

	~Server();
	//void close();
	//bool init(const char* ip, unsigned short port);
//	bool start();
	//bool recvMsg(fd_set& fdRead, SOCKET clntSock);
	QString Server::recvMsg(QString receiveMessage);
	QJsonObject Server::recvMsgByJson(QString receiveMessage, QString cmdID);

	
	QPointF currPoint;
	
	double dMatchAngle = 0;
	//void setMatchAngle(double angle);
	bool operator_TCPCanSend = false;
//	void setTCPCanSendStatus(bool flag);
	bool isJsonString(const QString& str);
private:
	//bool coreFunc();
	SOCKET servSock;
	QTcpServer* server;

	QModbusTcpClient* client;

	bool isBusy = false;
	std::list<SOCKET> clientList;
	QQueue<QTcpSocket*> clientQueue;
	void processNextRequest();
	int timestart = 4000;
signals:
	void triggerPattern();
	void logoString(QString message, QString color);
public slots:
	void onNewConnection();
	void onReadyRead();
};