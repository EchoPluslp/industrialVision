#include "login.h"
#include <QtWidgets/QApplication>
#include <QCoreApplication>
bool checkOnly();
int main(int argc, char *argv[]){
	 
	//cv::Point item1(1881,1428);
	//cv::Point item2(2177,1429);
	//double xe = cv::norm(item1 - item2);
	//double li = 16;//per/16mm
	//double perMM = xe / li;

#if (QT_VERSION > QT_VERSION_CHECK(5,6,0))
	QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
	if (!checkOnly())
	{
		QMessageBox::critical(NULL, "������Ϣ", "�Ӿ�����Ѿ�����,��رպ������´�");
		return NULL;
	}

	QApplication a(argc, argv);
	 
	//  //��ȡ�ϴιر�ʱ��״̬
	QString settingPath = QCoreApplication::applicationDirPath() + "/setting.ini";
	QSettings* settings = new QSettings(settingPath, QSettings::IniFormat);
	settings->beginGroup("Idus");
	//����Ա����
	QString passwordAdmin = settings->value("passwordAdmin","2222").toString();
	//����Ա����
	QString passwordUser = settings->value("passwordUser","1111").toString();

	
	//if (passwordUser.isNull() || passwordUser.size() == 0)
	//{
	//	industrialVision w;

	//	//û������ʱ,ֱ�ӽ�ȥ
	//	w.showNormal();
	//	return a.exec();
	//}
	login x;
	x.setPassword(passwordAdmin, passwordUser);
	x.show();
	return a.exec();
}
 
bool checkOnly()
{
	////  ����������  
	//HANDLE m_hMutex = CreateMutex(NULL, FALSE, L"fortest_abc123");
	////  ���������  
	//if (GetLastError() == ERROR_ALREADY_EXISTS) {
	//	//  ������л������������ͷž������λ������  
	//	CloseHandle(m_hMutex);
	//	m_hMutex = NULL;
	//	//  �����˳�  
	//	return  false;
	//}
	//else
		return true;
}
