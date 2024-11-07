
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

//初始化
void MyModbus::initModbus()
{
    myClient = new QModbusTcpClient();
    //connect(myClient,SIGNAL(stateChanged()),this,SLOT(slot_stateChanged()));
    connect(myClient, &QModbusClient::stateChanged, this, &MyModbus::slot_stateChanged);
}





//连接到modbus设备
void MyModbus::connectToModbus(QString ip, int port)
{
    if (!myClient)
    {
        return;
    }

    //判断当前连接状态是否为断开状态
    if (myClient->state() != QModbusDevice::ConnectedState)
    {
        //配置ModbusTcp的连接参数IP+Port
        myClient->setConnectionParameter(QModbusDevice::NetworkAddressParameter, ip);
        myClient->setConnectionParameter(QModbusDevice::NetworkPortParameter, port);
        myClient->connectDevice();
    }
    //else
    //{
    //    myClient->disconnectDevice();
    //}
}

//读取modbus设备各寄存器数据
//typeNum:1_线圈 2_离散输入 3_保持 4_输入
bool MyModbus::readModbusData(int typeNum, int startAdd, quint16 numbers)
{
    if (myClient->state() != QModbusDevice::ConnectedState)
    {
        return false;
    }

    //确定寄存器类型
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
        //  LOGDEBUG<<"读取寄存器类型错误";
        LOGDEBUG("读取寄存器类型错误\n");
        //   qDebug() << "Read register type error";
        return false;
    }
    // LOGDEBUG<<"readModbusData typeNum:"<<typeNum;
    LOGDEBUG("readModbusData typeNum:" << typeNum);
    //多读
    if (auto* reply = myClient->sendReadRequest(ReadUnit, 1))
    {
        if (!reply->isFinished())
        {
            if ((typeNum == 1) || (typeNum == 2))
            {
                QObject::connect(reply, &QModbusReply::finished, this, &MyModbus::slot_readReadyCoils);   //读取线圈
            }
            if ((typeNum == 3) || (typeNum == 4))
            {
                QObject::connect(reply, &QModbusReply::finished, this, &MyModbus::slot_readReadyRegisters);   //读取寄存器
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
        // LOGDEBUG<<"读取错误:" + myClient->errorString();
        QString errorMsg = "读取错误:" + myClient->errorString();
        LOGDEBUG(errorMsg);



        //  qDebug() << "读取错误";
        return false;
    }
}

//对modbus设备各寄存器写入数据
//typeNum:1_线圈 2_保持 (这两类寄存器可读可写,其余的只读)
bool MyModbus::writeModbusData(int typeNum, int startAdd, int writeNum)
{
    if (myClient->state() != QModbusDevice::ConnectedState)
    {
        return false;
    }

    //确定寄存器类型
    QModbusDataUnit writeUnit;
    if (typeNum == 1)
    {
        writeUnit = QModbusDataUnit(QModbusDataUnit::Coils, startAdd, 1);   //写入一个数据
        writeUnit.setValue(0, writeNum);

        //单写
        //bool ok;
        //quint16 hexData = writeData.toInt(&ok,16);   //转16进制
    }
    else if (typeNum == 2)
    {
        writeUnit = QModbusDataUnit(QModbusDataUnit::HoldingRegisters, startAdd, 2);   //写入两个数据
        quint16 uData16[2] = { 0 };
        uData16[0] = writeNum & 0xffff;
        uData16[1] = (writeNum >> 16) & 0xffff;
        writeUnit.setValue(0, uData16[0]);
        writeUnit.setValue(1, uData16[1]);
        //LOGDEBUG<<"uData16[0]:"<<uData16[0]<<"   uData16[1]:"<<uData16[1]<<"   writeNum:"<<writeNum;
    }
    else
    {
        // LOGDEBUG<<"写入寄存器类型错误";
        LOGDEBUG("写入寄存器类型错误\n");
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
                        /*LOGDEBUG<<"写入返回错误:"<<reply->error();

                        reply->deleteLater();*/
                        LOGDEBUG(QString("写入返回错误: %1").arg(reply->error()));
                        reply->deleteLater();
                        //  qDebug() << "写入返回错误";
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

        //LOGDEBUG<<"写入错误:" + myClient->errorString();
        LOGDEBUG("写入错误:" + myClient->errorString());
        //qDebug() << "Write error";
        return false;
    }
    return true;
}

//监听TCP连接的状态,若状态发生改变,发出对应的信号
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

//接收到读取线圈/离散输入寄存器请求后执行的槽函数
void MyModbus::slot_readReadyCoils()
{
    QVector<quint16> vAllData;
    QModbusReply* reply = qobject_cast<QModbusReply*>(sender());
    if (!reply)
    {
        // LOGDEBUG<<"读取线圈/离散输入寄存器错误";
        LOGDEBUG("读取线圈/离散输入寄存器错误\n");
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
        // LOGDEBUG<<"线圈/离散输入寄存器回复错误:"<<reply->error();
        QString errorMessage = "线圈/离散输入寄存器回复错误: " + reply->errorString();
        LOGDEBUG(errorMessage);

    }
    reply->deleteLater();
}

//接收到读取保持/输入寄存器请求后执行的槽函数
void MyModbus::slot_readReadyRegisters()
{
    QModbusReply* reply = qobject_cast<QModbusReply*>(sender());
    if (!reply)
    {
        // LOGDEBUG<<"读取保持/输入寄存器错误";
        LOGDEBUG("读取保持/输入寄存器错误\n");
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
            // LOGDEBUG<<"保持寄存器返回数据错误,个数:"<<nSize;
             // 使用 QString 的构造和 + 操作符来组合字符串  
            QString logMessage = QString("保持寄存器返回数据错误,个数:%1").arg(nSize);
            LOGDEBUG(logMessage.toUtf8().constData());
        }
    }
    else
    {
        // LOGDEBUG<<"保持/输入寄存器回复错误:"<<reply->error();
        QString errorMessage1 = "保持/输入寄存器回复错误:: " + reply->errorString();
        LOGDEBUG(errorMessage1);
    }
    reply->deleteLater();
}

