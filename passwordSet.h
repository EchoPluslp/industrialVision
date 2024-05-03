#pragma once
#ifndef passwordSet_H_
#define passwordSet_H_
#include "ui_passwordSet.h"
#include <QMessageBox>
#include "QTextCodec.h"
#pragma execution_character_set("utf-8")
#include <QSettings>
#include <cctype>

class passwordSet : public QWidget
{
	Q_OBJECT

public:
	passwordSet(QWidget* parent = Q_NULLPTR);
	//已选操作和属性设置 
	~passwordSet();
	bool passwordSet::isAlphaNumeric(const QString& str);
	QString stringRole;
	void setcurrentRole(QString stringRole);
	void setPasswordLanguage(QString language);
private:
	Ui::password* ui;

public slots:
	void passwordAdminSetclick();
	void passwordUserSetclick();

	void readSetPassword();
};
#endif