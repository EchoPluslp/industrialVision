#pragma once
#include <QObject>
#include <QModbusTcpClient>
#include <QModbusDataUnit>
#include <QTimer>
#include <QFile>
#include <QTextStream>

class PlcWorker : public QObject
{
	Q_OBJECT

public:
	explicit PlcWorker(QModbusTcpClient* client, QObject* parent = nullptr)
		: QObject(parent), client(client)
	{
		timer = new QTimer(this);
		connect(timer, &QTimer::timeout, this, &PlcWorker::pollPlcAddress);
	}

	void startPolling(int interval)
	{
		timer->start(interval);
	}

	int i = 0;
public slots:
	void pollPlcAddress()
	{
		QFile file("C:/tmp/temp.txt");
		if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
			// ����ļ��޷��򿪣�����ѡ���׳�һ���쳣������һ��������Ϣ�����߷���һ�����ַ���
			return ; // ����򵥵ط���һ�����ַ���
		}

		QTextStream in(&file);
		QString content = in.readAll(); // ��ȡ�ļ���ȫ������
		file.close(); // ��ʽ�ر��ļ�����Ȼ����ʱҲ���Զ��رգ�

		if (content=="1")
		{
			QFile Wfile("C:/tmp/temp.txt");
			if (!Wfile.open(QIODevice::WriteOnly | QIODevice::Text)) {
				// ����ļ��޷��򿪣�����ѡ���׳�һ���쳣������һ��������Ϣ�����߷���һ�����ַ���
				return; // ����򵥵ط���һ�����ַ���
			}

			QTextStream out(&Wfile);
			out << "0"; // д���µ����ݣ��⽫�����ļ��е����о�����
			emit takeMattchPhoto();

		}else if (content == "2")
		{
			emit Relayer();
		}

	}
signals:
	void takeMattchPhoto();
	void Relayer();
private:
	QModbusTcpClient* client;
	QTimer* timer;
};