#include "passwordSet.h"
passwordSet::passwordSet(QWidget* parent) :
	QWidget(parent),
	ui(new Ui::password)
{
	ui->setupUi(this);
	//��ȡ�������õ���־�ļ��е�ַ
	QString settingPath = QCoreApplication::applicationDirPath() + "/setting.ini";
	QSettings* settings = new QSettings(settingPath, QSettings::IniFormat);
	settings->beginGroup("Idus");
	//����
	QString logDirectory = settings->value("password").toString();
	ui->lineEdit->setText(logDirectory);

	//��ɫ������Ͻ�ͼ��
	QPixmap pixmap(100, 100);
	pixmap.fill(Qt::white);
	setWindowIcon(QIcon(pixmap));
	setWindowTitle("��������");
	setWindowFlags(Qt::WindowCloseButtonHint);

}
passwordSet::~passwordSet() {

}

void passwordSet::readSetPassword()
{

	//��ȡ�������õ���־�ļ��е�ַ
	QString settingPath = QCoreApplication::applicationDirPath() + "/setting.ini";
	QSettings* settings = new QSettings(settingPath, QSettings::IniFormat);
	settings->beginGroup("Idus");
	//�ع�
	QString logDirectory = settings->value("password").toString();
	ui->lineEdit->setText(logDirectory); 
}

void passwordSet::passwordSetclick() {
	QString password = ui->lineEdit->text();
	//��������
	if(isAlphaNumeric(password)){
	QString settingPath = QCoreApplication::applicationDirPath() + "/setting.ini";
	QSettings* settings = new QSettings(settingPath, QSettings::IniFormat);
	settings->beginGroup("Idus");
	settings->setValue("password", password);
	//�����ɹ�
	QMessageBox::information(nullptr, "��������", "���봴���ɹ�");
	this->close();
	}
	else {
		QMessageBox::warning(nullptr, "��������", "����ʧ��,ֻ�ܰ������ֺ���ĸ");

	}
}
bool passwordSet::isAlphaNumeric(const QString& qstr) {
	std::string stdStr = qstr.toStdString();
	const char* charPtr = stdStr.c_str();

	for (int i = 0; charPtr[i] != '\0'; i++) {
		if (!std::isalpha(charPtr[i]) && !std::isdigit(charPtr[i])) {
			return false;
		}
	}
	return true;
}