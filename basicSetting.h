#pragma once
#ifndef BASICSETTING_H_

#define BASICSETTING_H_

#include "ui_basicSetting.h"
#include "QTextCodec.h"
#include <QSettings>
#pragma execution_character_set("utf-8")


class basicSetting : public QWidget
{
	Q_OBJECT

public:
	basicSetting(QWidget* parent = Q_NULLPTR);
	//��ѡ�������������� 
	~basicSetting();
private:
	Ui::FormSetting* ui;



public slots:
	void onRadioButtonClicked();

};
#endif
