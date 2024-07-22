#include "login.h"
#include <QtWidgets/QApplication>
#include <QCoreApplication>
bool checkOnly();


int main(int argc, char *argv[]){
	cv::Point center(1263, 1413);
	cv::Point right(1446, 1402);
	cv::Point left(1250, 1181);

	double cal = cv::norm(center-right);
	double calsf = cv::norm(center - left);

	// ����1 (pt1 -> pt2)
	cv::Point2f vector1 = center - right;
	// ����2 (pt3 -> pt4)
	cv::Point2f vector2 = center - left;

	// ���������ĵ����ģ
	double dotProduct = vector1.x * vector2.x + vector1.y * vector2.y;
	double magnitude1 = std::sqrt(vector1.x * vector1.x + vector1.y * vector1.y);
	double magnitude2 = std::sqrt(vector2.x * vector2.x + vector2.y * vector2.y);

	// ����нǣ����ȣ�
	double angleRad = std::acos(dotProduct / (magnitude1 * magnitude2));

	// ������ת��Ϊ����
	double angleDeg = angleRad * 180.0 / CV_PI;

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