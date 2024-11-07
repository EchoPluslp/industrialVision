#pragma once
#ifndef SERVERPLC_H
#define SERVERPLC_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTcpServer>
#include <QListWidget>
#include<QLabel>
#include <QFileDialog>
#include <QDateTime>
#include "mymodbus.h"

QT_BEGIN_NAMESPACE
namespace Ui {
	class Serverplc;


}
QT_END_NAMESPACE
class QTcpServer;

class Serverplc : public QMainWindow
{
	Q_OBJECT

public:
	Serverplc(QWidget* parent = nullptr);
	~Serverplc();
	void initWidget();

private:
	Ui::Serverplc* ui;

	QTcpServer* tcpServer;
	QTcpSocket* client;
	QList<QTcpSocket*> tcpClients;
	QListWidgetItem* lwdgitem;
	QMenu* iplistMenu;
	QAction* actiondel = NULL;
	QString green = "background-color:rgb(0,255,0)";//CSS��ʽ
	QString red = "background-color:rgb(255,0,0)";//CSS��ʽ
	QString yellow = "background-color:rgb(255,255,0)";//CSS��ʽ
  //  printdetect* t;// ָ��printdetect�����ָ�� 
	QLabel* m_status;
	QLabel* m_tcpStatusPixmap;
	QLabel* m_modbusStatusPixmap;
	MyModbus* m_myModsbus;   //MyModbus����


	QTimer* timer; // ָ��ʱ����ָ��  
	bool PLCOK = false;
	int currentAddress; // ��ǰ��ȡ��Modbus��ַ����ѡ��������Ҫ��ӣ�
	int mopen = 0;
signals:
	//void itemSelected();//�пͻ��˷��ź�
	void takePhotoSignal(); // ���������ź�

public slots:
	// һ������Ĳۺ��������ڼ���Ƿ���ѡ�е�������ź�  
 //   void checkForSelectedItems();
	void onTextReady1(const QString& text); // ���ڽ����ı�����ʾ 
	void on_pb_start_clicked(); // �µİ�ť�ۺ����������������ڶ�ȡ  
	void readModbusDataPeriodically(); // ��ʱ��ȡModbus���ݵĲۺ���  
	void on_pb_stop_clicked();
	void BaoSucc();
	void writequality(QString quality);
	void onOpen();//����������ź�
private slots:
	QString GetLocalIP();
	void on_pushButton_StartToListen_clicked();
	//void on_listView_IP_List_customContextMenuRequested(const QPoint &pos);
	void on_listWidget_IP_customContextMenuRequested(const QPoint& pos);
	void slot_newConnectionClient();
	void slot_readData();
	void slot_disconnectedClient();

	void on_pushButton_Send_clicked();
	void slot_delmenu();
	QString  GetCurrentTimeStr();
	void slot_stateChanged(bool flag);
	void slot_readCoils(QVector<quint16> vAllData);
	void slot_readRegisters(int resultNum);
	void on_pb_connect_clicked();
	void on_pb_readM_clicked();
	void on_pb_writeM_clicked();
	void on_pb_readD_clicked();
	void on_pb_writeD_clicked();
};
#endif // SERVER_H
