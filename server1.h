#ifndef SERVER_H
#define SERVER_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTcpServer>
#include <QListWidget>
#include<QLabel>
//#include"printdetect.h"
#include <QFileDialog>
#include <QDateTime>

#include "mymodbus.h"
QT_BEGIN_NAMESPACE
namespace Ui { class Server;


             }
QT_END_NAMESPACE
class QTcpServer;

class Server : public QMainWindow
{
    Q_OBJECT

public:
    Server(QWidget *parent = nullptr);
    ~Server();
    void initWidget();
   
private:
    Ui::Server *ui;

    QTcpServer *tcpServer;
    QTcpSocket *client;
    QList<QTcpSocket *> tcpClients;
    QListWidgetItem *lwdgitem;
    QMenu *iplistMenu;
     QAction* actiondel = NULL;
      QString green="background-color:rgb(0,255,0)";//CSS方式
      QString red = "background-color:rgb(255,0,0)";//CSS方式
      QString yellow = "background-color:rgb(255,255,0)";//CSS方式
    //  printdetect* t;// 指向printdetect对象的指针 
      QLabel* m_status;
      QLabel* m_tcpStatusPixmap;
      QLabel* m_modbusStatusPixmap;
      MyModbus* m_myModsbus;   //MyModbus对象


      QTimer* timer; // 指向定时器的指针  
      bool PLCOK = false;
      int currentAddress; // 当前读取的Modbus地址（可选，根据需要添加）
      int mopen = 0;
signals:
    //void itemSelected();//有客户端发信号
    void takePhotoSignal(); // 定义拍照信号

public slots:
    // 一个假设的槽函数，用于检查是否有选中的项并发射信号  
 //   void checkForSelectedItems();
   void onTextReady1(const QString& text); // 用于接收文本并显示 
   void on_pb_start_clicked(); // 新的按钮槽函数，用于启动定期读取  
   void readModbusDataPeriodically(); // 定时读取Modbus数据的槽函数  
   void on_pb_stop_clicked();
   void BaoSucc();
   void writequality(QString quality);
   void onOpen();//接受相机打开信号
private slots:
    QString GetLocalIP();
    void on_pushButton_StartToListen_clicked();
    //void on_listView_IP_List_customContextMenuRequested(const QPoint &pos);
    void on_listWidget_IP_customContextMenuRequested(const QPoint &pos);
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
