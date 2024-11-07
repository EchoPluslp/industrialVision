
//MyModbus::MyModbus(QObject *parent) : QObject(parent)
//{

//}
//#pragma execution_character_set("utf-8")
#pragma execution_character_set("utf-8")
#include "mymodbus.h"
#include <QMutex>
#include <QDateTime> 
#include <QApplication>
#include<qfile.h>
#include<qdir.h>
#include <QDebug>
#include <QMessageBox>
MyModbus::MyModbus(QObject* parent) : QObject(parent)
{
    this->initModbus();
}

MyModbus::~MyModbus()
{

}

//��ʼ��
void MyModbus::initModbus()
{
    myClient = new QModbusTcpClient();
    //connect(myClient,SIGNAL(stateChanged()),this,SLOT(slot_stateChanged()));
    connect(myClient, &QModbusClient::stateChanged, this, &MyModbus::slot_stateChanged);
}





//���ӵ�modbus�豸
void MyModbus::connectToModbus(QString ip, int port)
{
    if (!myClient)
    {
        return;
    }

    //�жϵ�ǰ����״̬�Ƿ�Ϊ�Ͽ�״̬
    if (myClient->state() != QModbusDevice::ConnectedState)
    {
        //����ModbusTcp�����Ӳ���IP+Port
        myClient->setConnectionParameter(QModbusDevice::NetworkAddressParameter, ip);
        myClient->setConnectionParameter(QModbusDevice::NetworkPortParameter, port);
        myClient->connectDevice();
    }
    //else
    //{
    //    myClient->disconnectDevice();
    //}
}

//��ȡmodbus�豸���Ĵ�������
//typeNum:1_��Ȧ 2_��ɢ���� 3_���� 4_����
bool MyModbus::readModbusData(int typeNum, int startAdd, quint16 numbers)
{
    if (myClient->state() != QModbusDevice::ConnectedState)
    {
        return false;
    }

    //ȷ���Ĵ�������
    QModbusDataUnit ReadUnit;
    if (typeNum == 1)
    {
        ReadUnit = QModbusDataUnit(QModbusDataUnit::Coils, startAdd, numbers);
    }
    else if (typeNum == 2)
    {
        ReadUnit = QModbusDataUnit(QModbusDataUnit::DiscreteInputs, startAdd, numbers);
    }
    else if (typeNum == 3)
    {
        ReadUnit = QModbusDataUnit(QModbusDataUnit::HoldingRegisters, startAdd, numbers);
    }
    else if (typeNum == 4)
    {
        ReadUnit = QModbusDataUnit(QModbusDataUnit::InputRegisters, startAdd, numbers);
    }
    else
    {
        //  LOGDEBUG<<"��ȡ�Ĵ������ʹ���";
        LOGDEBUG("��ȡ�Ĵ������ʹ���\n");
        //   qDebug() << "Read register type error";
        return false;
    }
    // LOGDEBUG<<"readModbusData typeNum:"<<typeNum;
    LOGDEBUG("readModbusData typeNum:" << typeNum);
    //���
    if (auto* reply = myClient->sendReadRequest(ReadUnit, 1))
    {
        if (!reply->isFinished())
        {
            if ((typeNum == 1) || (typeNum == 2))
            {
                QObject::connect(reply, &QModbusReply::finished, this, &MyModbus::slot_readReadyCoils);   //��ȡ��Ȧ
            }
            if ((typeNum == 3) || (typeNum == 4))
            {
                QObject::connect(reply, &QModbusReply::finished, this, &MyModbus::slot_readReadyRegisters);   //��ȡ�Ĵ���
            }
            //reply->deleteLater();
            return true;
        }
        else
        {
            reply->deleteLater();
            return false;
        }
    }
    else
    {
        // LOGDEBUG<<"��ȡ����:" + myClient->errorString();
        QString errorMsg = "��ȡ����:" + myClient->errorString();
        LOGDEBUG(errorMsg);



        //  qDebug() << "��ȡ����";
        return false;
    }
}

//��modbus�豸���Ĵ���д������
//typeNum:1_��Ȧ 2_���� (������Ĵ����ɶ���д,�����ֻ��)
bool MyModbus::writeModbusData(int typeNum, int startAdd, int writeNum)
{
    if (myClient->state() != QModbusDevice::ConnectedState)
    {
        return false;
    }

    //ȷ���Ĵ�������
    QModbusDataUnit writeUnit;
    if (typeNum == 1)
    {
        writeUnit = QModbusDataUnit(QModbusDataUnit::Coils, startAdd, 1);   //д��һ������
        writeUnit.setValue(0, writeNum);

        //��д
        //bool ok;
        //quint16 hexData = writeData.toInt(&ok,16);   //ת16����
    }
    else if (typeNum == 2)
    {
        writeUnit = QModbusDataUnit(QModbusDataUnit::HoldingRegisters, startAdd, 2);   //д����������
        quint16 uData16[2] = { 0 };
        uData16[0] = writeNum & 0xffff;
        uData16[1] = (writeNum >> 16) & 0xffff;
        writeUnit.setValue(0, uData16[0]);
        writeUnit.setValue(1, uData16[1]);
        //LOGDEBUG<<"uData16[0]:"<<uData16[0]<<"   uData16[1]:"<<uData16[1]<<"   writeNum:"<<writeNum;
    }
    else
    {
        // LOGDEBUG<<"д��Ĵ������ʹ���";
        LOGDEBUG("д��Ĵ������ʹ���\n");
        // qDebug() << "Write register type error";
        return false;
    }
    //LOGDEBUG<<"writeModbusData typeNum:"<<typeNum<<"   writeNum:"<<writeNum;
    if (auto* reply = myClient->sendWriteRequest(writeUnit, 1))
    {
        if (!reply->isFinished())
        {
            connect(reply, &QModbusReply::finished, this, [reply]()
                {
                    if (reply->error() == QModbusDevice::NoError)
                    {
                        reply->deleteLater();
                        return true;
                    }
                    else
                    {
                        /*LOGDEBUG<<"д�뷵�ش���:"<<reply->error();

                        reply->deleteLater();*/
                        LOGDEBUG(QString("д�뷵�ش���: %1").arg(reply->error()));
                        reply->deleteLater();
                        //  qDebug() << "д�뷵�ش���";
                        return false;
                    }
                });
        }
        else
        {
            reply->deleteLater();
            return false;
        }
    }
    else
    {

        //LOGDEBUG<<"д�����:" + myClient->errorString();
        LOGDEBUG("д�����:" + myClient->errorString());
        //qDebug() << "Write error";
        return false;
    }
    return true;
}

//����TCP���ӵ�״̬,��״̬�����ı�,������Ӧ���ź�
void MyModbus::slot_stateChanged()
{
    // LOGDEBUG<<myClient->state();
    LOGDEBUG(myClient->state());

    cout << "111" << myClient->state() << QModbusDevice::ConnectedState;
    if (myClient->state() == QModbusDevice::ConnectedState)
    {
        emit signal_stateChanged(true);
        cout << "111" << myClient->state() << QModbusDevice::ConnectedState;
    }
    else if (myClient->state() == QModbusDevice::UnconnectedState)
    {
        emit signal_stateChanged(false);
        qDebug() << "222" << myClient->state() << QModbusDevice::ConnectedState;
    }
}

//���յ���ȡ��Ȧ/��ɢ����Ĵ��������ִ�еĲۺ���
void MyModbus::slot_readReadyCoils()
{
    QVector<quint16> vAllData;
    QModbusReply* reply = qobject_cast<QModbusReply*>(sender());
    if (!reply)
    {
        // LOGDEBUG<<"��ȡ��Ȧ/��ɢ����Ĵ�������";
        LOGDEBUG("��ȡ��Ȧ/��ɢ����Ĵ�������\n");
        return;
    }
    if (reply->error() == QModbusDevice::NoError)
    {
        const QModbusDataUnit unit = reply->result();
        vAllData = unit.values();
        emit signal_readCoils(vAllData);
    }
    else
    {
        // LOGDEBUG<<"��Ȧ/��ɢ����Ĵ����ظ�����:"<<reply->error();
        QString errorMessage = "��Ȧ/��ɢ����Ĵ����ظ�����: " + reply->errorString();
        LOGDEBUG(errorMessage);

    }
    reply->deleteLater();
}

//���յ���ȡ����/����Ĵ��������ִ�еĲۺ���
void MyModbus::slot_readReadyRegisters()
{
    QModbusReply* reply = qobject_cast<QModbusReply*>(sender());
    if (!reply)
    {
        // LOGDEBUG<<"��ȡ����/����Ĵ�������";
        LOGDEBUG("��ȡ����/����Ĵ�������\n");
        return;
    }
    if (reply->error() == QModbusDevice::NoError)
    {
        const QModbusDataUnit unit = reply->result();
        auto valueList = unit.values();
        int nSize = valueList.size();
        if (nSize == 2)
        {
            quint16 uData16[2] = { 0 };
            uData16[0] = valueList[0];
            uData16[1] = valueList[1];
            int resultNum = uData16[0] | (uData16[1] << 16);
            //LOGDEBUG<<"uData16[0]:"<<uData16[0]<<"   uData16[1]:"<<uData16[1]<<"   resultNum:"<<resultNum;
            emit signal_readRegisters(resultNum);
        }
        else
        {
            // LOGDEBUG<<"���ּĴ����������ݴ���,����:"<<nSize;
             // ʹ�� QString �Ĺ���� + ������������ַ���  
            QString logMessage = QString("���ּĴ����������ݴ���,����:%1").arg(nSize);
            LOGDEBUG(logMessage.toUtf8().constData());
        }
    }
    else
    {
        // LOGDEBUG<<"����/����Ĵ����ظ�����:"<<reply->error();
        QString errorMessage1 = "����/����Ĵ����ظ�����:: " + reply->errorString();
        LOGDEBUG(errorMessage1);
    }
    reply->deleteLater();
}

