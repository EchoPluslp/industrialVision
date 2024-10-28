#include "server1.h"
#include "ui_server1.h"
#include <QtNetwork> //在pro文件里面添加QT+=network
#include <QMessageBox>
#include <QStatusBar> 
#include<QTimer>
#include<QDebug>
//#pragma execution_character_set("utf-8")

Server::Server(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Server)
    , timer(new QTimer(this))// 定时器成为Server的子对象  
    , currentAddress(0) // 初始化当前地址（可选
{
    ui->setupUi(this);
    setWindowIcon(QIcon("images/mainIcon.png"));
    this->initWidget();
    tcpServer=new QTcpServer(this);
    QString ip = GetLocalIP();
    if(ip.isEmpty())
    {
        QMessageBox::about(this,tr("提示"),tr("没有获取到本机的IP"));
    }
    else
    {
        ui->lineEdit_Local_IP->setText(ip);
    }
    ui->pushButton_StartToListen->setStyleSheet(red);
    ui->listWidget_IP->setContextMenuPolicy(Qt::CustomContextMenu);//右键点击，弹出菜单
    ui->label_hostName->setText(tr("HostName:")+QHostInfo::localHostName());
    ui->label_hostName->adjustSize();
    ui->lineEdit_Local_Port->setText(tr("6666"));
   // ui->plainTextEdit_Send_Msg->setPlainText("I am Server您好");
    ui->pushButton_StartToListen->setEnabled(true);
    ui->pushButton_Send->setEnabled(false);
    iplistMenu=new QMenu(ui->listWidget_IP);//菜单绑定到列表控件，by txwtech
   // QAction *pnew0 = new QAction(QString::fromLocal8Bit("Delete"),ui->listWidget_IP);
    QAction *pnew0 = new QAction("删除",ui->listWidget_IP);
    iplistMenu->addAction(pnew0);
   // connect( ui->actiondel, SIGNAL(triggered()), this, SLOT(slot_delmenu()));
    connect( pnew0, SIGNAL(triggered()), this, SLOT(slot_delmenu()));
    connect(tcpServer,SIGNAL(newConnection()),this,SLOT(slot_newConnectionClient()));

    //状态栏
    /*m_status = new QLabel;
    m_status->setPixmap(QPixmap("./disconnect1.png").scaled(20,20));
    ui->statusbar->addWidget(new QLabel("连接状态："));
    ui->statusbar->addWidget(m_status);*/
    
    // 创建TCP连接状态的QLabel和QPixmap  
    QLabel* tcpStatusLabel = new QLabel("TCP连接状态：");
    m_tcpStatusPixmap = new QLabel;
    m_tcpStatusPixmap->setPixmap(QPixmap("./disconnect1.png").scaled(20, 20));

    // 创建Modbus连接状态的QLabel和QPixmap  
    QLabel* modbusStatusLabel = new QLabel("Modbus连接状态：");
    m_modbusStatusPixmap = new QLabel;
    m_modbusStatusPixmap->setPixmap(QPixmap("./disconnect1.png").scaled(20, 20));

    // 将TCP连接状态的控件添加到状态栏  
    ui->statusbar->addWidget(tcpStatusLabel);
    ui->statusbar->addWidget(m_tcpStatusPixmap);
    //ui->statusbar->addStretch();
    // 将Modbus连接状态的控件添加到状态栏  
    ui->statusbar->addWidget(modbusStatusLabel);
    ui->statusbar->addWidget(m_modbusStatusPixmap);



    // 连接定时器的timeout信号到读取Modbus数据的槽函数  
    connect(timer, &QTimer::timeout, this, &Server::readModbusDataPeriodically);


}
void Server::onTextReady1(const QString& text)
{
    qDebug() << "text:" << text;
    ui->plainTextEdit_Send_Msg->setPlainText(text); // 将接收到的文本设置到plainTextEdit上  
    foreach(QListWidgetItem * item, ui->listWidget_IP->selectedItems())
    {
        QString clientIP = item->text().split(":")[0];
        int clientPort = item->text().split(":")[1].toInt();
        for (int i = 0; i < tcpClients.length(); i++)
        {
            if (tcpClients[i]->peerAddress().toString() == clientIP && tcpClients[i]->peerPort() == clientPort)
            {
                tcpClients[i]->write(ui->plainTextEdit_Send_Msg->toPlainText().toLocal8Bit());////fromLocal8Bit显示中文，解决乱码
                ui->plainTextEdit_Recv_Msg->appendPlainText(GetCurrentTimeStr() + " 发送：" + ui->plainTextEdit_Send_Msg->toPlainText());
            }
        }
     //
        // 清空 plainTextEdit_Send_Msg 中的内容  
        ui->plainTextEdit_Send_Msg->clear();
    }
}




//“删除”菜单，断开指定Client
void Server:: slot_delmenu()
{
    if(  lwdgitem != NULL )
    {
        if( !tcpClients.isEmpty() ){
            QString clientIP = lwdgitem->text().split(":")[0];
            int clientPort = lwdgitem->text().split(":")[1].toInt();
            for(int i=0; i<tcpClients.length(); i++)
            {
                if(tcpClients[i]->peerAddress().toString()==clientIP && tcpClients[i]->peerPort()==clientPort)
                {
                    tcpClients[i]->disconnectFromHost();
                    break;
                }
            }
        }
    }
}

void Server::slot_newConnectionClient()
{
    while(tcpServer->hasPendingConnections())
    {
        QTcpSocket *client=tcpServer->nextPendingConnection();
        m_tcpStatusPixmap->setPixmap(QPixmap("./connect1.png").scaled(20, 20));//连接状态
        tcpClients.append(client);
        ui->listWidget_IP->addItem(tr("%1:%2").arg(client->peerAddress().toString()).arg(client->peerPort()));
       // ui->plainTextEdit_Recv_Msg->appendPlainText(GetCurrentTimeStr()+tr(" %1:%2").arg(client->peerAddress().toString()).arg(client->peerPort()));
        QString peerInfo = tr("%1:%2 connect").arg(client->peerAddress().toString()).arg(client->peerPort());
        ui->plainTextEdit_Recv_Msg->appendPlainText(GetCurrentTimeStr() + " " + peerInfo);
        connect(client,SIGNAL(readyRead()),this,SLOT(slot_readData()));
        connect(client,SIGNAL(disconnected()),this,SLOT(slot_disconnectedClient()));

    }
}
QString Server::GetCurrentTimeStr()
{
    QDateTime current_time=QDateTime::currentDateTime();
   // QString str_time=current_time.toString("yyyy-MM-dd hh:mm:ss.zzz ddd");//ddd--周五，zzz毫秒
    QString str_time=current_time.toString("yyyy-MM-dd hh:mm:ss");//ddd--周五，zzz毫秒
    return str_time;
}
void Server::slot_readData()
{
    QTcpSocket *obj=(QTcpSocket*)sender();
    QByteArray buf=obj->readAll();
    if(buf.isEmpty())
        return;
    QString ipc;
    ipc=tr("[%1:%2]").arg(obj->peerAddress().toString()).arg(obj->peerPort());
    //ui->textEdit_Recv_Msg->append(ipc);
  //  ui->textEdit_Recv_Msg->append(QString::fromLocal8Bit(buf));
   // toUtf8
    QDateTime current_time=QDateTime::currentDateTime();
   // QString str_time=current_time.toString("yyyy-MM-dd hh:mm:ss.zzz ddd");//ddd--周五，zzz毫秒
    QString str_time=current_time.toString("yyyy-MM-dd hh:mm:ss");//ddd--周五，zzz毫秒
    ui->plainTextEdit_Recv_Msg->appendPlainText(str_time+" "+ipc);
    ui->plainTextEdit_Recv_Msg->appendPlainText(str_time+" "+QString::fromLocal8Bit(buf)); //fromLocal8Bit显示中文，解决乱码



}
void Server::slot_disconnectedClient()
{
  if(!tcpClients.isEmpty())
  {
      QTcpSocket *obj=(QTcpSocket*) sender();

      QListWidgetItem *item=ui->listWidget_IP->findItems(
                  tr("%1:%2")\
                  .arg( obj->peerAddress().toString())\
                  .arg( obj->peerPort()),Qt::MatchContains|Qt::MatchEndsWith
                  ).at(0);
      ui->listWidget_IP->removeItemWidget(item);
      delete item;
      obj->close();
      tcpClients.removeOne(obj);
      QString int_to_String_port=tr(":%1").arg(obj->peerPort());
      ui->plainTextEdit_Recv_Msg->appendPlainText(GetCurrentTimeStr()+" "+obj->peerAddress().toString()+int_to_String_port+" 已断开");
      m_status->setPixmap(QPixmap("./disconnect1.png").scaled(20, 20));

  }
}


QString Server::GetLocalIP()
{
    QList<QHostAddress> list=QNetworkInterface::allAddresses();
    foreach(QHostAddress address,list)
    {
        if(address.protocol()==QAbstractSocket::IPv4Protocol)
        {
            return address.toString();
        }
    }
    return "";
}

Server::~Server()
{
    delete ui;
}


void Server::on_pushButton_StartToListen_clicked()
{
    if(ui->pushButton_StartToListen->text()=="停止")
    {
        if(!tcpClients.isEmpty())
            for(int i=0;i<tcpClients.length();i++)
            {
                tcpClients[i]->disconnectFromHost();
            }
        tcpClients.clear();
        tcpServer->close();
        ui->listWidget_IP->clear();
        ui->pushButton_StartToListen->setText("监听");
         ui->pushButton_StartToListen->setStyleSheet(red);
       ui->pushButton_Send->setEnabled(false);

    }
    else
    {
        if(ui->lineEdit_Local_Port->text().toInt()==0)
        {
            QMessageBox::about(this,tr("提示"),tr("请输入端口号"));
        }
        else if(tcpServer->listen(QHostAddress::AnyIPv4,ui->lineEdit_Local_Port->text().toInt()))
        {
            ui->pushButton_StartToListen->setText("停止");
            ui->pushButton_Send->setEnabled(true);

            ui->pushButton_StartToListen->setStyleSheet(green);
        }
    }

}


void Server::on_listWidget_IP_customContextMenuRequested(const QPoint &pos)
{
    lwdgitem = ui->listWidget_IP->itemAt(pos);
    if(lwdgitem!=NULL)
    {
        iplistMenu->exec(QCursor::pos());
    }

}

void Server::on_pushButton_Send_clicked()
{
    if(ui->listWidget_IP->selectedItems().length()>0)
    {
        foreach(QListWidgetItem* item, ui->listWidget_IP->selectedItems())
        {
            QString clientIP=item->text().split(":")[0];
            int clientPort=item->text().split(":")[1].toInt();
            for(int i=0;i<tcpClients.length();i++)
            {
                if(tcpClients[i]->peerAddress().toString()==clientIP &&tcpClients[i]->peerPort()==clientPort)
                {
                     tcpClients[i]->write(ui->plainTextEdit_Send_Msg->toPlainText().toLocal8Bit());////fromLocal8Bit显示中文，解决乱码
                     ui->plainTextEdit_Recv_Msg->appendPlainText(GetCurrentTimeStr()+" 发送："+ui->plainTextEdit_Send_Msg->toPlainText());
                }
            }

        }
    }
    else
    {
        QMessageBox::information(this,"提示","IP列表未选择");
    }
}


void Server::initWidget()
{
    //初始化MyModbus对象
    m_myModsbus = new MyModbus();
    connect(m_myModsbus, SIGNAL(signal_stateChanged(bool)), this, SLOT(slot_stateChanged(bool)));
    connect(m_myModsbus, SIGNAL(signal_readCoils(QVector<quint16>)), this, SLOT(slot_readCoils(QVector<quint16>)));
    connect(m_myModsbus, SIGNAL(signal_readRegisters(int)), this, SLOT(slot_readRegisters(int)));

}

void Server::slot_stateChanged(bool flag)
{
    if (flag)
    {
        ui->lb_state->setText("连接成功");
        PLCOK = true;
        ui->te_show->appendPlainText(QTime::currentTime().toString("hh:mm:ss: ") + "连接成功");
        m_modbusStatusPixmap->setPixmap(QPixmap("./connect1.png").scaled(20, 20));
        //QMessageBox::warning(this, "警告", "连接成功!");

    }
    else
    {
        ui->lb_state->setText("连接断开");
        PLCOK = false;
        ui->te_show->appendPlainText(QTime::currentTime().toString("hh:mm:ss: ") + "连接断开");
        QMessageBox::information(nullptr, "警告", "连接断开!");
        m_modbusStatusPixmap->setPixmap(QPixmap("./disconnect1.png").scaled(20, 20));
        // timer->stop();
    }
}

void Server::onOpen() {
    mopen = 1;
}

//写入0
void Server::BaoSucc() {
    if (PLCOK)
    {
        //int startAdd = ui->le_addressM->text().toInt();
        int startAdd = 1000;
        int writeNum = 0;
        // LOGDEBUG << "startAdd:" << startAdd << "   writeNum:" << writeNum;
        LOGDEBUG("startAdd:" << startAdd << "   writeNum:" << writeNum);
        //进行写入寄存器数据的处理
        if (!m_myModsbus->writeModbusData(2, startAdd, writeNum))
        {
            // QMessageBox::warning(this, "警告", "保存寄存器数据写入失败!");
            QMessageBox::information(nullptr, "警告", "写入失败!");
        }
    }  
}

void Server::writequality(QString quality) {

    /*int startAdd = ui->le_addressM->text().toInt();*/
    int startAdd = 1010;
    int writeNum =0 ;
    if (quality == "OK")
    {
        writeNum = 1;
    }
    else
    {
        writeNum = 2;
    }
    // LOGDEBUG << "startAdd:" << startAdd << "   writeNum:" << writeNum;
    LOGDEBUG("startAdd:" << startAdd << "   writeNum:" << writeNum);
    //进行写入寄存器数据的处理
    if (!m_myModsbus->writeModbusData(2, startAdd, writeNum))
    {
        // QMessageBox::warning(this, "警告", "保存寄存器数据写入失败!");
        QMessageBox::information(nullptr, "警告", "写入失败!");
    }
}

void Server::slot_readCoils(QVector<quint16> vAllData)
{
    // LOGDEBUG << "readCoils size:" << vAllData.size();
    QString logMessage = QString("readCoils size: %1").arg(vAllData.size());
    LOGDEBUG(logMessage.toUtf8().constData()); // 如果 LOGDEBUG 需要 const char*
    for (int i = 0; i < vAllData.size(); i++)
    {
        // LOGDEBUG << "i:" << vAllData[i];
        QString logMessage1 = QString("i: %1, value: %2").arg(i).arg(vAllData[i]);
        LOGDEBUG(logMessage1.toUtf8().constData()); // 如果 LOGDEBUG 需要 const char* 

        ui->te_show->appendPlainText(QTime::currentTime().toString("hh:mm:ss: ") + "线圈返回数据:" + QString::number(vAllData[i]));
        ui->le_dataM->setText(QString::number(vAllData[0]));

        //如果返回数据是1，就发送信号拍照，保证相机是开着的，进入了死循环
        if (vAllData[i] == 1 && mopen) {
            emit takePhotoSignal(); // 发送信号  
        }
    }
}



void Server::slot_readRegisters(int resultNum)
{
    //LOGDEBUG << "resultNum:" << resultNum;

    QString logMessage2 = QString("resultNum:%1").arg(resultNum);
    LOGDEBUG(logMessage2.toUtf8().constData());

    ui->te_show->appendPlainText(QTime::currentTime().toString("hh:mm:ss: ") + "保存返回数据:" + QString::number(resultNum));
    ui->le_dataD->setText(QString::number(resultNum));
    //如果返回数据是1，就发送信号拍照，保证相机是开着的，进入了死循环
    if (resultNum == 1 && mopen) {
        emit takePhotoSignal(); // 发送信号  
    }
}

void Server::on_pb_connect_clicked()
{
    QString ip = ui->le_ip->text();
    int port = ui->le_port->text().toInt();
    // LOGDEBUG << "ip:" << ip << "   port:" << port;
    LOGDEBUG(QString("IP: %1, Port: %2").arg(ip).arg(port));


    ui->te_show->appendPlainText(QTime::currentTime().toString("hh:mm:ss: ") + "ip:" + ip + " port:" + QString::number(port));
    qDebug() << "333" << ip;
    //连接到modbus设备
    m_myModsbus->connectToModbus(ip, port);
}

void Server::on_pb_readM_clicked()//读取地址
{
    int startAdd = ui->le_addressM->text().toInt();
    // LOGDEBUG << "startAdd:" << startAdd;

    LOGDEBUG(QString("startAdd: %1").arg(startAdd));

    if (!m_myModsbus->readModbusData(1, startAdd, 1))
    {
        // QMessageBox::warning(this, "警告", "线圈寄存器数据读取失败！");
        QMessageBox::information(nullptr, "警告", "读取失败!");
    }


}
void Server::on_pb_start_clicked()
{
    // 启动定时器，每隔50毫秒调用一次readModbusDataPeriodically  
    timer->start(2000);

    // 注意：这里不需要从on_pb_start_clicked()中调用on_pb_readM_clicked()，  
    // 因为定时器会定期调用readModbusDataPeriodically()。  

    // 如果你需要从某个特定的地址开始读取，并且这个地址是通过UI设置的，  
    // 你应该在这里获取它，并将其存储在某个成员变量中，  
    // 然后在readModbusDataPeriodically()中使用它。  
    //currentAddress = ui->le_addressM->text().toInt(); // 假设你有一个地址输入框 
    currentAddress = ui->le_addressD->text().toInt();
    
}
void Server::readModbusDataPeriodically()
{
    // 使用currentAddress读取Modbus数据  
    if (!m_myModsbus->readModbusData(3, currentAddress, 2))
    {
        QMessageBox::information(nullptr, "警告", "读取失败!");
        timer->stop();
    }

    // 注意：如果你需要在每次读取后递增地址，你可以在这里修改currentAddress的值。  
    // 但是，请注意，由于这个操作非常快（每50毫秒一次），  
    // 你可能需要确保你的Modbus设备能够跟上这个速度，  
    // 或者你可能需要实现某种形式的速率限制或错误处理。  
}

void Server::on_pb_stop_clicked()
{
    timer->stop();
    // 可以在这里添加一些停止后的用户反馈，比如更新UI等  
}



void Server::on_pb_writeM_clicked()
{
    int startAdd = ui->le_addressM->text().toInt();
    int writeNum = ui->le_dataM->text().toInt();
    // LOGDEBUG << "startAdd:" << startAdd << "   writeNum:" << writeNum;
    LOGDEBUG("startAdd:" << startAdd << "   writeNum:" << writeNum);


    //单写
    if (!m_myModsbus->writeModbusData(1, startAdd, writeNum))
    {
        // QMessageBox::warning(this, "警告", "线圈寄存器数据写入失败!");
        QMessageBox::information(nullptr, "警告", "写入失败");
    }
}

void Server::on_pb_readD_clicked()
{
    int startAdd = ui->le_addressD->text().toInt();
    // LOGDEBUG << "startAdd:" << startAdd;
    LOGDEBUG(QString("startAdd: %1").arg(startAdd));
    if (!m_myModsbus->readModbusData(3, startAdd, 2))
    {
        //  QMessageBox::warning(this, "警告", "保存寄存器数据读取失败!");
        QMessageBox::information(nullptr, "警告", "读取失败");
    }
}

void Server::on_pb_writeD_clicked()
{
    int startAdd = ui->le_addressD->text().toInt();
    int writeNum = ui->le_dataD->text().toInt();;
    // LOGDEBUG << "startAdd:" << startAdd << "   writeNum:" << writeNum;
    LOGDEBUG("startAdd:" << startAdd << "   writeNum:" << writeNum);
    //进行写入寄存器数据的处理
    if (!m_myModsbus->writeModbusData(2, startAdd, writeNum))
    {
        // QMessageBox::warning(this, "警告", "保存寄存器数据写入失败!");
        QMessageBox::information(nullptr, "警告", "写入失败");
    }
}


