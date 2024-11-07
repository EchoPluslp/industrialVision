#include "serverplc.h"
#include "ui_serverplc.h"
#include <QtNetwork> //��pro�ļ��������QT+=network
#include <QMessageBox>
#include <QStatusBar> 
#include<QTimer>
#include<QDebug>
//#pragma execution_character_set("utf-8")

Serverplc::Serverplc(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::Serverplc)
    , timer(new QTimer(this))// ��ʱ����ΪServer���Ӷ���  
    , currentAddress(0) // ��ʼ����ǰ��ַ����ѡ
{
    ui->setupUi(this);
    setWindowIcon(QIcon("images/mainIcon.png"));
    this->initWidget();
    tcpServer = new QTcpServer(this);
    QString ip = GetLocalIP();
    if (ip.isEmpty())
    {
        QMessageBox::about(this, tr("��ʾ"), tr("û�л�ȡ��������IP"));
    }
    else
    {
        ui->lineEdit_Local_IP->setText(ip);
    }
    ui->pushButton_StartToListen->setStyleSheet(red);
    ui->listWidget_IP->setContextMenuPolicy(Qt::CustomContextMenu);//�Ҽ�����������˵�
    ui->label_hostName->setText(tr("HostName:") + QHostInfo::localHostName());
    ui->label_hostName->adjustSize();
    ui->lineEdit_Local_Port->setText(tr("6666"));
    // ui->plainTextEdit_Send_Msg->setPlainText("I am Server����");
    ui->pushButton_StartToListen->setEnabled(true);
    ui->pushButton_Send->setEnabled(false);
    iplistMenu = new QMenu(ui->listWidget_IP);//�˵��󶨵��б�ؼ���by txwtech
   // QAction *pnew0 = new QAction(QString::fromLocal8Bit("Delete"),ui->listWidget_IP);
    QAction* pnew0 = new QAction("ɾ��", ui->listWidget_IP);
    iplistMenu->addAction(pnew0);
    // connect( ui->actiondel, SIGNAL(triggered()), this, SLOT(slot_delmenu()));
    connect(pnew0, SIGNAL(triggered()), this, SLOT(slot_delmenu()));
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(slot_newConnectionClient()));

    //״̬��
    /*m_status = new QLabel;
    m_status->setPixmap(QPixmap("./disconnect1.png").scaled(20,20));
    ui->statusbar->addWidget(new QLabel("����״̬��"));
    ui->statusbar->addWidget(m_status);*/

    // ����TCP����״̬��QLabel��QPixmap  
    QLabel* tcpStatusLabel = new QLabel("TCP����״̬��");
    m_tcpStatusPixmap = new QLabel;
    m_tcpStatusPixmap->setPixmap(QPixmap("./disconnect1.png").scaled(20, 20));

    // ����Modbus����״̬��QLabel��QPixmap  
    QLabel* modbusStatusLabel = new QLabel("Modbus����״̬��");
    m_modbusStatusPixmap = new QLabel;
    m_modbusStatusPixmap->setPixmap(QPixmap("./disconnect1.png").scaled(20, 20));

    // ��TCP����״̬�Ŀؼ���ӵ�״̬��  
    ui->statusbar->addWidget(tcpStatusLabel);
    ui->statusbar->addWidget(m_tcpStatusPixmap);
    //ui->statusbar->addStretch();
    // ��Modbus����״̬�Ŀؼ���ӵ�״̬��  
    ui->statusbar->addWidget(modbusStatusLabel);
    ui->statusbar->addWidget(m_modbusStatusPixmap);



    // ���Ӷ�ʱ����timeout�źŵ���ȡModbus���ݵĲۺ���  
    connect(timer, &QTimer::timeout, this, &Serverplc::readModbusDataPeriodically);


}
void Serverplc::onTextReady1(const QString& text)
{
    qDebug() << "text:" << text;
    ui->plainTextEdit_Send_Msg->setPlainText(text); // �����յ����ı����õ�plainTextEdit��  
    foreach(QListWidgetItem * item, ui->listWidget_IP->selectedItems())
    {
        QString clientIP = item->text().split(":")[0];
        int clientPort = item->text().split(":")[1].toInt();
        for (int i = 0; i < tcpClients.length(); i++)
        {
            if (tcpClients[i]->peerAddress().toString() == clientIP && tcpClients[i]->peerPort() == clientPort)
            {
                tcpClients[i]->write(ui->plainTextEdit_Send_Msg->toPlainText().toLocal8Bit());////fromLocal8Bit��ʾ���ģ��������
                ui->plainTextEdit_Recv_Msg->appendPlainText(GetCurrentTimeStr() + " ���ͣ�" + ui->plainTextEdit_Send_Msg->toPlainText());
            }
        }
        //
           // ��� plainTextEdit_Send_Msg �е�����  
        ui->plainTextEdit_Send_Msg->clear();
    }
}




//��ɾ�����˵����Ͽ�ָ��Client
void Serverplc::slot_delmenu()
{
    if (lwdgitem != NULL)
    {
        if (!tcpClients.isEmpty()) {
            QString clientIP = lwdgitem->text().split(":")[0];
            int clientPort = lwdgitem->text().split(":")[1].toInt();
            for (int i = 0; i < tcpClients.length(); i++)
            {
                if (tcpClients[i]->peerAddress().toString() == clientIP && tcpClients[i]->peerPort() == clientPort)
                {
                    tcpClients[i]->disconnectFromHost();
                    break;
                }
            }
        }
    }
}

void Serverplc::slot_newConnectionClient()
{
    while (tcpServer->hasPendingConnections())
    {
        QTcpSocket* client = tcpServer->nextPendingConnection();
        m_tcpStatusPixmap->setPixmap(QPixmap("./connect1.png").scaled(20, 20));//����״̬
        tcpClients.append(client);
        ui->listWidget_IP->addItem(tr("%1:%2").arg(client->peerAddress().toString()).arg(client->peerPort()));
        // ui->plainTextEdit_Recv_Msg->appendPlainText(GetCurrentTimeStr()+tr(" %1:%2").arg(client->peerAddress().toString()).arg(client->peerPort()));
        QString peerInfo = tr("%1:%2 connect").arg(client->peerAddress().toString()).arg(client->peerPort());
        ui->plainTextEdit_Recv_Msg->appendPlainText(GetCurrentTimeStr() + " " + peerInfo);
        connect(client, SIGNAL(readyRead()), this, SLOT(slot_readData()));
        connect(client, SIGNAL(disconnected()), this, SLOT(slot_disconnectedClient()));

    }
}
QString Serverplc::GetCurrentTimeStr()
{
    QDateTime current_time = QDateTime::currentDateTime();
    // QString str_time=current_time.toString("yyyy-MM-dd hh:mm:ss.zzz ddd");//ddd--���壬zzz����
    QString str_time = current_time.toString("yyyy-MM-dd hh:mm:ss");//ddd--���壬zzz����
    return str_time;
}
void Serverplc::slot_readData()
{
    QTcpSocket* obj = (QTcpSocket*)sender();
    QByteArray buf = obj->readAll();
    if (buf.isEmpty())
        return;
    QString ipc;
    ipc = tr("[%1:%2]").arg(obj->peerAddress().toString()).arg(obj->peerPort());
    //ui->textEdit_Recv_Msg->append(ipc);
  //  ui->textEdit_Recv_Msg->append(QString::fromLocal8Bit(buf));
   // toUtf8
    QDateTime current_time = QDateTime::currentDateTime();
    // QString str_time=current_time.toString("yyyy-MM-dd hh:mm:ss.zzz ddd");//ddd--���壬zzz����
    QString str_time = current_time.toString("yyyy-MM-dd hh:mm:ss");//ddd--���壬zzz����
    ui->plainTextEdit_Recv_Msg->appendPlainText(str_time + " " + ipc);
    ui->plainTextEdit_Recv_Msg->appendPlainText(str_time + " " + QString::fromLocal8Bit(buf)); //fromLocal8Bit��ʾ���ģ��������



}
void Serverplc::slot_disconnectedClient()
{
    if (!tcpClients.isEmpty())
    {
        QTcpSocket* obj = (QTcpSocket*)sender();

        QListWidgetItem* item = ui->listWidget_IP->findItems(
            tr("%1:%2")\
            .arg(obj->peerAddress().toString())\
            .arg(obj->peerPort()), Qt::MatchContains | Qt::MatchEndsWith
        ).at(0);
        ui->listWidget_IP->removeItemWidget(item);
        delete item;
        obj->close();
        tcpClients.removeOne(obj);
        QString int_to_String_port = tr(":%1").arg(obj->peerPort());
        ui->plainTextEdit_Recv_Msg->appendPlainText(GetCurrentTimeStr() + " " + obj->peerAddress().toString() + int_to_String_port + " �ѶϿ�");
        m_status->setPixmap(QPixmap("./disconnect1.png").scaled(20, 20));

    }
}


QString Serverplc::GetLocalIP()
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach(QHostAddress address, list)
    {
        if (address.protocol() == QAbstractSocket::IPv4Protocol)
        {
            return address.toString();
        }
    }
    return "";
}

Serverplc::~Serverplc()
{
    delete ui;
}


void Serverplc::on_pushButton_StartToListen_clicked()
{
    if (ui->pushButton_StartToListen->text() == "ֹͣ")
    {
        if (!tcpClients.isEmpty())
            for (int i = 0; i < tcpClients.length(); i++)
            {
                tcpClients[i]->disconnectFromHost();
            }
        tcpClients.clear();
        tcpServer->close();
        ui->listWidget_IP->clear();
        ui->pushButton_StartToListen->setText("����");
        ui->pushButton_StartToListen->setStyleSheet(red);
        ui->pushButton_Send->setEnabled(false);

    }
    else
    {
        if (ui->lineEdit_Local_Port->text().toInt() == 0)
        {
            QMessageBox::about(this, tr("��ʾ"), tr("������˿ں�"));
        }
        else if (tcpServer->listen(QHostAddress::AnyIPv4, ui->lineEdit_Local_Port->text().toInt()))
        {
            ui->pushButton_StartToListen->setText("ֹͣ");
            ui->pushButton_Send->setEnabled(true);

            ui->pushButton_StartToListen->setStyleSheet(green);
        }
    }

}


void Serverplc::on_listWidget_IP_customContextMenuRequested(const QPoint& pos)
{
    lwdgitem = ui->listWidget_IP->itemAt(pos);
    if (lwdgitem != NULL)
    {
        iplistMenu->exec(QCursor::pos());
    }

}

void Serverplc::on_pushButton_Send_clicked()
{
    if (ui->listWidget_IP->selectedItems().length() > 0)
    {
        foreach(QListWidgetItem * item, ui->listWidget_IP->selectedItems())
        {
            QString clientIP = item->text().split(":")[0];
            int clientPort = item->text().split(":")[1].toInt();
            for (int i = 0; i < tcpClients.length(); i++)
            {
                if (tcpClients[i]->peerAddress().toString() == clientIP && tcpClients[i]->peerPort() == clientPort)
                {
                    tcpClients[i]->write(ui->plainTextEdit_Send_Msg->toPlainText().toLocal8Bit());////fromLocal8Bit��ʾ���ģ��������
                    ui->plainTextEdit_Recv_Msg->appendPlainText(GetCurrentTimeStr() + " ���ͣ�" + ui->plainTextEdit_Send_Msg->toPlainText());
                }
            }

        }
    }
    else
    {
        QMessageBox::information(this, "��ʾ", "IP�б�δѡ��");
    }
}


void Serverplc::initWidget()
{
    //��ʼ��MyModbus����
    m_myModsbus = new MyModbus();
    connect(m_myModsbus, SIGNAL(signal_stateChanged(bool)), this, SLOT(slot_stateChanged(bool)));
    connect(m_myModsbus, SIGNAL(signal_readCoils(QVector<quint16>)), this, SLOT(slot_readCoils(QVector<quint16>)));
    connect(m_myModsbus, SIGNAL(signal_readRegisters(int)), this, SLOT(slot_readRegisters(int)));

}

void Serverplc::slot_stateChanged(bool flag)
{
    if (flag)
    {
        ui->lb_state->setText("���ӳɹ�");
        PLCOK = true;
        ui->te_show->appendPlainText(QTime::currentTime().toString("hh:mm:ss: ") + "���ӳɹ�");
        m_modbusStatusPixmap->setPixmap(QPixmap("./connect1.png").scaled(20, 20));
        //QMessageBox::warning(this, "����", "���ӳɹ�!");

    }
    else
    {
        ui->lb_state->setText("���ӶϿ�");
        PLCOK = false;
        ui->te_show->appendPlainText(QTime::currentTime().toString("hh:mm:ss: ") + "���ӶϿ�");
        QMessageBox::information(nullptr, "����", "���ӶϿ�!");
        m_modbusStatusPixmap->setPixmap(QPixmap("./disconnect1.png").scaled(20, 20));
        // timer->stop();
    }
}

void Serverplc::onOpen() {
    mopen = 1;
}

//д��0
void Serverplc::BaoSucc() {
    if (PLCOK)
    {
        //int startAdd = ui->le_addressM->text().toInt();
        int startAdd = 1000;
        int writeNum = 0;
        // LOGDEBUG << "startAdd:" << startAdd << "   writeNum:" << writeNum;
        LOGDEBUG("startAdd:" << startAdd << "   writeNum:" << writeNum);
        //����д��Ĵ������ݵĴ���
        if (!m_myModsbus->writeModbusData(2, startAdd, writeNum))
        {
            // QMessageBox::warning(this, "����", "����Ĵ�������д��ʧ��!");
            QMessageBox::information(nullptr, "����", "д��ʧ��!");
        }
    }
}

void Serverplc::writequality(QString quality) {

    /*int startAdd = ui->le_addressM->text().toInt();*/
    int startAdd = 1010;
    int writeNum = 0;
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
    //����д��Ĵ������ݵĴ���
    if (!m_myModsbus->writeModbusData(2, startAdd, writeNum))
    {
        // QMessageBox::warning(this, "����", "����Ĵ�������д��ʧ��!");
        QMessageBox::information(nullptr, "����", "д��ʧ��!");
    }
}

void Serverplc::slot_readCoils(QVector<quint16> vAllData)
{
    // LOGDEBUG << "readCoils size:" << vAllData.size();
    QString logMessage = QString("readCoils size: %1").arg(vAllData.size());
    LOGDEBUG(logMessage.toUtf8().constData()); // ��� LOGDEBUG ��Ҫ const char*
    for (int i = 0; i < vAllData.size(); i++)
    {
        // LOGDEBUG << "i:" << vAllData[i];
        QString logMessage1 = QString("i: %1, value: %2").arg(i).arg(vAllData[i]);
        LOGDEBUG(logMessage1.toUtf8().constData()); // ��� LOGDEBUG ��Ҫ const char* 

        ui->te_show->appendPlainText(QTime::currentTime().toString("hh:mm:ss: ") + "��Ȧ��������:" + QString::number(vAllData[i]));
        ui->le_dataM->setText(QString::number(vAllData[0]));

        //�������������1���ͷ����ź����գ���֤����ǿ��ŵģ���������ѭ��
        if (vAllData[i] == 1 && mopen) {
            emit takePhotoSignal(); // �����ź�  
        }
    }
}



void Serverplc::slot_readRegisters(int resultNum)
{
    //LOGDEBUG << "resultNum:" << resultNum;

    QString logMessage2 = QString("resultNum:%1").arg(resultNum);
    LOGDEBUG(logMessage2.toUtf8().constData());

    ui->te_show->appendPlainText(QTime::currentTime().toString("hh:mm:ss: ") + "���淵������:" + QString::number(resultNum));
    ui->le_dataD->setText(QString::number(resultNum));
    //�������������1���ͷ����ź����գ���֤����ǿ��ŵģ���������ѭ��
    if (resultNum == 1 && mopen) {
        emit takePhotoSignal(); // �����ź�  
    }
}

void Serverplc::on_pb_connect_clicked()
{
    QString ip = ui->le_ip->text();
    int port = ui->le_port->text().toInt();
    // LOGDEBUG << "ip:" << ip << "   port:" << port;
    LOGDEBUG(QString("IP: %1, Port: %2").arg(ip).arg(port));


    ui->te_show->appendPlainText(QTime::currentTime().toString("hh:mm:ss: ") + "ip:" + ip + " port:" + QString::number(port));
    qDebug() << "333" << ip;
    //���ӵ�modbus�豸
    m_myModsbus->connectToModbus(ip, port);
}

void Serverplc::on_pb_readM_clicked()//��ȡ��ַ
{
    int startAdd = ui->le_addressM->text().toInt();
    // LOGDEBUG << "startAdd:" << startAdd;

    LOGDEBUG(QString("startAdd: %1").arg(startAdd));

    if (!m_myModsbus->readModbusData(1, startAdd, 1))
    {
        // QMessageBox::warning(this, "����", "��Ȧ�Ĵ������ݶ�ȡʧ�ܣ�");
        QMessageBox::information(nullptr, "����", "��ȡʧ��!");
    }


}
void Serverplc::on_pb_start_clicked()
{
    // ������ʱ����ÿ��50�������һ��readModbusDataPeriodically  
    timer->start(2000);

    // ע�⣺���ﲻ��Ҫ��on_pb_start_clicked()�е���on_pb_readM_clicked()��  
    // ��Ϊ��ʱ���ᶨ�ڵ���readModbusDataPeriodically()��  

    // �������Ҫ��ĳ���ض��ĵ�ַ��ʼ��ȡ�����������ַ��ͨ��UI���õģ�  
    // ��Ӧ���������ȡ����������洢��ĳ����Ա�����У�  
    // Ȼ����readModbusDataPeriodically()��ʹ������  
    //currentAddress = ui->le_addressM->text().toInt(); // ��������һ����ַ����� 
    currentAddress = ui->le_addressD->text().toInt();

}
void Serverplc::readModbusDataPeriodically()
{
    // ʹ��currentAddress��ȡModbus����  
    if (!m_myModsbus->readModbusData(3, currentAddress, 2))
    {
        QMessageBox::information(nullptr, "����", "��ȡʧ��!");
        timer->stop();
    }

    // ע�⣺�������Ҫ��ÿ�ζ�ȡ�������ַ��������������޸�currentAddress��ֵ��  
    // ���ǣ���ע�⣬������������ǳ��죨ÿ50����һ�Σ���  
    // �������Ҫȷ�����Modbus�豸�ܹ���������ٶȣ�  
    // �����������Ҫʵ��ĳ����ʽ���������ƻ������  
}

void Serverplc::on_pb_stop_clicked()
{
    timer->stop();
    // �������������һЩֹͣ����û��������������UI��  
}



void Serverplc::on_pb_writeM_clicked()
{
    int startAdd = ui->le_addressM->text().toInt();
    int writeNum = ui->le_dataM->text().toInt();
    // LOGDEBUG << "startAdd:" << startAdd << "   writeNum:" << writeNum;
    LOGDEBUG("startAdd:" << startAdd << "   writeNum:" << writeNum);


    //��д
    if (!m_myModsbus->writeModbusData(1, startAdd, writeNum))
    {
        // QMessageBox::warning(this, "����", "��Ȧ�Ĵ�������д��ʧ��!");
        QMessageBox::information(nullptr, "����", "д��ʧ��");
    }
}

void Serverplc::on_pb_readD_clicked()
{
    int startAdd = ui->le_addressD->text().toInt();
    // LOGDEBUG << "startAdd:" << startAdd;
    LOGDEBUG(QString("startAdd: %1").arg(startAdd));
    if (!m_myModsbus->readModbusData(3, startAdd, 2))
    {
        //  QMessageBox::warning(this, "����", "����Ĵ������ݶ�ȡʧ��!");
        QMessageBox::information(nullptr, "����", "��ȡʧ��");
    }
}

void Serverplc::on_pb_writeD_clicked()
{
    int startAdd = ui->le_addressD->text().toInt();
    int writeNum = ui->le_dataD->text().toInt();;
    // LOGDEBUG << "startAdd:" << startAdd << "   writeNum:" << writeNum;
    LOGDEBUG("startAdd:" << startAdd << "   writeNum:" << writeNum);
    //����д��Ĵ������ݵĴ���
    if (!m_myModsbus->writeModbusData(2, startAdd, writeNum))
    {
        // QMessageBox::warning(this, "����", "����Ĵ�������д��ʧ��!");
        QMessageBox::information(nullptr, "����", "д��ʧ��");
    }
}


