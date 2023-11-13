#ifndef TRANSMITSIGNALS_H
#define TRANSMITSIGNALS_H
#include <QObject>
#include<qpoint.h>
#include <QString>
#include <QStringList>


class TransmitSignals : public QObject
{
    Q_OBJECT
public:
    static TransmitSignals& GetInstance();
private:
    TransmitSignals();
    ~TransmitSignals();

    TransmitSignals(const TransmitSignals&) = delete;
    TransmitSignals(const TransmitSignals&&) = delete;
    TransmitSignals& operator=(const TransmitSignals&) = delete;



signals:
    void create_once_pattern();
    void send_pattern_result(QPoint point);
	void sendToIndustrString(QString mess,QString color);

public slots:
};

/*
 * ���������Ч�Լ��
 */
class NetParameterAck
{
public:
    NetParameterAck();

    /*
     * static bool isIPv4(QString ip)
     *
     * ��̬���������IPv4��IP��ַ���ַ�����Ч��
     *
     * ���������ַ����͵��ʮ���Ƶ�IP��ַ�������Ч����true
     * ���򷵻�false��
     */
    static bool isIPv4(QString ip) {
        QStringList list = ip.split('.');
        if (list.size() != 4)
            return false;
        for (int i = 0; i < 4; i++) {
            QString temp = list.at(i);
            if (i == 0) {
                if (!(temp.toInt() > 0 &&
                    temp.toInt() < 256 &&
                    temp.at(0) != '0'))
                    return false;
            }
            else {
                if (!(temp.toInt() >= 0 &&
                    temp.toInt() < 256 &&
                    temp.size() > 0))
                    return false;
                else {
                    int num = temp.toInt();
                    int j = 0;
                    while (num > 0) {
                        j++;
                        num = num / 10;
                    }
                    if (!(temp.size() == j ||
                        (temp.size() == 1 &&
                            j == 0)))
                        return false;
                }
            }
        }
        return true;
    }

    /*
     * static bool isMask(QString mask)
     *
     * ��̬�������ж�IPv4�ĵ��ʮ���Ƶ��������Ч�ԣ�������
     * ת��Ϊ�����Ƶ��ַ������ж��Ƿ���01��ϡ�������򷵻�
     * false�����򷵻�true��
     */
    static bool isMask(QString mask) {
        QStringList list = mask.split('.');

        if (list.size() != 4)
            return false;

        QString str;
        for (int i = 0; i < 4; i++)
        {
            str += QString("%1").arg(list.at(i).toInt(), 8, 2, QLatin1Char('0'));
        }

        if (str.indexOf("01") > 0)
            return false;
        return true;
    }
};

#endif // NETPARAMETERACK_H
