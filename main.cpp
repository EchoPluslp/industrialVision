#include "login.h"
#include <QtWidgets/QApplication>
#include <QCoreApplication>
bool checkOnly();

int main(int argc, char *argv[]){
	QApplication a(argc, argv);
	 
	//  //读取上次关闭时的状态
	QString settingPath = QCoreApplication::applicationDirPath() + "/setting.ini";
	QSettings* settings = new QSettings(settingPath, QSettings::IniFormat);
	settings->beginGroup("Idus");
	//曝光
	QString password = settings->value("password").toString();
	if (password.isNull() || password.size() == 0)
	{
		industrialVision w;

		//没有密码时,直接进去
		w.showNormal();
		return a.exec();
	}
	login x;
	x.setPassword(password);
	x.show();
	return a.exec();
}
 
bool checkOnly()
{
	//  创建互斥量  
	HANDLE m_hMutex = CreateMutex(NULL, FALSE, L"fortest_abc123");
	//  检查错误代码  
	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		//  如果已有互斥量存在则释放句柄并复位互斥量  
		CloseHandle(m_hMutex);
		m_hMutex = NULL;
		//  程序退出  
		return  false;
	}
	else
		return true;
}