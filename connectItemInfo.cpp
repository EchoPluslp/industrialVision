#include "connectItemInfo.h"

connectItemInfo::connectItemInfo(QObject* parent)
{
    //�����ź������� �м䴫�ݺ���

    connect(&TransmitSignals::GetInstance(), &TransmitSignals::send_pattern_result, this, &connectItemInfo::receive_pattern_result, Qt::UniqueConnection);

}
void connectItemInfo::Thread_deal()
{

    //����  ����Ĭ��ֵ
    if (server_ip.isEmpty()) {
        server_ip = "127.0.0.1";
    }
    if (server_port.isEmpty())
    {
        server_port = "60000";
    }

	char* ch = server_ip.toLatin1().data();
    int x = server_port.toInt();
        return;

}

void connectItemInfo::receive_pattern_result(QPoint point)
{
    //operator_FLAG = flag;
    pointF = point;
}

void connectItemInfo::reveive_ipAndPort(QString ip, QString port)
{
    server_ip = ip;
    server_port = port;
}
 