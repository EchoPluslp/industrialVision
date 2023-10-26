#pragma once
#include<WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")                           
#include<iostream>
#include<list>
#include"QPointF"
#include<qobject.h>
#include "transmitsignals.h"
#include<opencv2\opencv.hpp>
#include <opencv2/imgproc/imgproc_c.h>

struct s_SingleTargetMatch
{
	cv::Point2d ptLT, ptRT, ptRB, ptLB, ptCenter;
	double dMatchedAngle;
	double dMatchScore;
	bool flag = false;
	int width;
	int height;
	bool pattern_flag = false;
};


class Server  :public QObject
{
	Q_OBJECT
public:
	Server();
	 //Server(QObject* parent = nullptr);
	Server(const Server& other);

	~Server();
	void close();
	bool init(const char* ip, unsigned short port);
	bool start();
	bool recvMsg(fd_set& fdRead, SOCKET clntSock);
	QPointF currPoint;
	
	double dMatchAngle = 0;
	void setMatchAngle(double angle);
	bool operator_TCPCanSend = false;
	void setTCPCanSendStatus(bool flag);
private:
	bool coreFunc();
	SOCKET servSock;
	std::list<SOCKET> clientList;
signals:
	void triggerPattern();
};