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
			QModbusDataUnit readUnit(QModbusDataUnit::HoldingRegisters, 1500, 1); 
			if (auto* reply = client->sendReadRequest(readUnit, 1)) 
			{
				if (!reply->isFinished())
				{
					connect(reply, &QModbusReply::finished, this, [this, reply]()
						{
							if (reply->error() == QModbusDevice::NoError)
							{
								int tim = reply->result().value(0);
								if (tim == 1)
								{
									emit takeMattchPhoto();
									QModbusDataUnit unit(QModbusDataUnit::HoldingRegisters, 1500, 1);
									unit.setValue(0, 0);
									client->sendWriteRequest(unit, 1);							
								}
								// Process the response
								//qDebug() << "PLC Value:" << reply->result().value(0);
							}
							else
							{
								reply->error();
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