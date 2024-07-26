#pragma once
#include <QObject>
#include <QModbusTcpClient>
#include <QModbusDataUnit>
#include <QTimer>

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

public slots:
	void pollPlcAddress()
	{
		if (client->state() == QModbusDevice::ConnectedState)
		{
			QModbusDataUnit readUnit(QModbusDataUnit::HoldingRegisters, 500, 2); 
			if (auto* reply = client->sendReadRequest(readUnit, 1)) 
			{
				if (!reply->isFinished())
				{
					connect(reply, &QModbusReply::finished, this, [this, reply]()
						{
							if (reply->error() == QModbusDevice::NoError)
							{
								int tim = reply->result().value(0);
								int tim3 = reply->result().value(1);
								if (tim == 1)
								{
									emit takeMattchPhoto();
								}
								// Process the response
								//qDebug() << "PLC Value:" << reply->result().value(0);
							}
							else
							{
							//	qDebug() << "Read response error:" << reply->error();
							}
							reply->deleteLater();
						});
				}
				else
				{
					reply->deleteLater();
				}
			}
		}
		else
		{
			client->connectDevice();
			//qDebug() << "PLC not connected!";
		}
	}
signals:
	void takeMattchPhoto();
private:
	QModbusTcpClient* client;
	QTimer* timer;
};