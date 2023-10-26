#pragma once
#ifndef CONNECTITEMINFO_H_
#define CONNECTITEMINFO_H_
#include<qpoint.h>
#include<qobject.h>
#include "transmitsignals.h"
#include "Server.h"

class connectItemInfo : public QObject
{
    Q_OBJECT
public:
     QPointF currPoint;
     Server serverItem;

    explicit connectItemInfo(QObject* parent = nullptr);
    bool operator_FLAG = false;
    QPointF pointF;
    QString server_ip;
    QString server_port;
public slots:
    void Thread_deal();//线程处理函数
    void receive_pattern_result(QPoint point);
    void reveive_ipAndPort(QString ip, QString port);
signals:
    //void triggerPattern();
};
//QPoint connectItemInfo::currPoint = QPoint();

#endif