#ifndef LOGIN_H_

#define LOGIN_H_

#include "ui_login.h"
#include "industrialVision.h"
#include "QTextCodec.h"
#pragma execution_character_set("utf-8")


class login : public QWidget
{
	Q_OBJECT

public:
	login(QWidget* parent = Q_NULLPTR);
	void setPassword(QString passwordAdmin, QString passwordUser);
	//已选操作和属性设置 
	~login();
private:
	Ui::login* ui;
	industrialVision w;
	QString passwordAdmin;
	QString passwordUser;
	QString system_language;
	QStringList  usersWithRoleUser = { "操作员", "operator", "operador" };
	QStringList  usersWithRoleAdmin = { "管理员", "administrator", "administrador" };
public slots:
	void login_in();
	void slot_switchLog();

};
#endif
