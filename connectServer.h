#pragma once

#ifndef CONNECTSERVER_H_
#define CONNECTSERVER_H_
#pragma execution_character_set("utf-8")

#include"connectItemInfo.h"
#include <QDialog>
#include <QMainWindow>
#include "ui_connectServer.h"
#include <QDesktopWidget>
#include <QThread>
#include"Server.h"
#include <QMessageBox>
class connectServer : public QDialog
{
	Q_OBJECT

public:
	connectServer(QMainWindow *parent = Q_NULLPTR);
	Server  getConnectItemInfo();
	~connectServer();
	void setconnectValual_language(QString language);
public	slots:
	bool create_server();

signals:
	void send_ipAndPort(QString ip, QString port);
	void send_createServerCallBack(QString flag);
private:
	Ui::connectServer ui;
	connectItemInfo* itemInfo  = new connectItemInfo(this);
	QThread* subThread = new QThread(this);

};

#endif