#pragma once
#pragma once
#ifndef MYMODBUS_H
#define MYMODBUS_H

#include <QtSerialBus/qmodbustcpclient.h>
#include <QtSerialBus/qmodbusdataunit.h>
//#include "qmodbustcpclient.h"

#include <QDebug>
#include<iostream>

#include <QApplication>
#include <QMutex>
//#pragma execution_character_set("utf-8")

using namespace std;

//#define LOGDEBUG qDebug()<<__FILE__<<__LINE__
#define LOGDEBUG(msg) qDebug() << __FILE__ << ":" << __LINE__ << msg

class MyModbus : public QObject
{
	Q_OBJECT
public:
	explicit MyModbus(QObject* parent = nullptr);
	~MyModbus();

	void initModbus();

	void connectToModbus(QString ip, int port);

	bool readModbusData(int typeNum, int startAdd, quint16 numbers);
	bool writeModbusData(int typeNum, int startAdd, int writeNum);
	// void outputMessage(QtMsgType type, const QMessageLogContext& context, const QString& msg);

signals:
	void signal_stateChanged(bool flag);
	void signal_readCoils(QVector<quint16> vAllData);
	void signal_readRegisters(int resultNum);

private slots:
	void slot_stateChanged();
	void slot_readReadyCoils();
	void slot_readReadyRegisters();

private:
	QModbusTcpClient* myClient;

};
#endif // MYMODBUS_H
