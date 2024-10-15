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
			// 如果文件无法打开，可以选择抛出一个异常，返回一个错误消息，或者返回一个空字符串
			return ; // 这里简单地返回一个空字符串
		}

		QTextStream in(&file);
		QString content = in.readAll(); // 读取文件的全部内容
		file.close(); // 显式关闭文件（虽然析构时也会自动关闭）

		if (content=="1")
		{
			QFile Wfile("C:/tmp/temp.txt");
			if (!Wfile.open(QIODevice::WriteOnly | QIODevice::Text)) {
				// 如果文件无法打开，可以选择抛出一个异常，返回一个错误消息，或者返回一个空字符串
				return; // 这里简单地返回一个空字符串
			}

			QTextStream out(&Wfile);
			out << "0"; // 写入新的内容，这将覆盖文件中的所有旧内容
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