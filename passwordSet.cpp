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
	//����Ա����
	QString passwordAdmin = settings->value("passwordAdmin", "2222").toString();
	//����Ա����
	QString passwordUser = settings->value("passwordUser", "1111").toString();
	ui->lineEdit_admin->setText(passwordAdmin);
	ui->lineEdit_user->setText(passwordUser);


	//��ɫ������Ͻ�ͼ��
	setWindowIcon(QIcon("icon.ico"));

	setWindowTitle("��������");
	setWindowFlags(Qt::WindowCloseButtonHint);

}
passwordSet::~passwordSet() {

}

void passwordSet::readSetPassword()
{

}

void passwordSet::passwordAdminSetclick() {
	QString password = ui->lineEdit_admin->text();
	//��������
	if(isAlphaNumeric(password)){
	QString settingPath = QCoreApplication::applicationDirPath() + "/setting.ini";
	QSettings* settings = new QSettings(settingPath, QSettings::IniFormat);
	settings->beginGroup("Idus");
	settings->setValue("passwordAdmin", password);
	//�����ɹ�
	QMessageBox::information(nullptr, "���ù���Ա����", "����Ա���봴���ɹ�");
	this->close();
	}
	else {
		QMessageBox::warning(nullptr, "���ù���Ա����", "����Ա����ʧ��,ֻ�ܰ������ֺ���ĸ");

	}
}
void passwordSet::passwordUserSetclick() {
	QString password = ui->lineEdit_user->text();
	//��������
	if (isAlphaNumeric(password)) {
		QString settingPath = QCoreApplication::applicationDirPath() + "/setting.ini";
		QSettings* settings = new QSettings(settingPath, QSettings::IniFormat);
		settings->beginGroup("Idus");
		settings->setValue("passwordUser", password);
		//�����ɹ�
		QMessageBox::information(nullptr, "���ò���Ա����", "����Ա���봴���ɹ�");
		this->close();
	}
	else {
		QMessageBox::warning(nullptr, "���ò���Ա����", "����Ա����ʧ��,ֻ�ܰ������ֺ���ĸ");

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

void passwordSet::setcurrentRole(QString stringRole)
{
	if (stringRole == "����Ա")
	{
		ui->pushButton_setadmin->setHidden(true);
		ui->lineEdit_admin->setHidden(true);
	}
	else {
		ui->pushButton_setadmin->setHidden(false);
		ui->lineEdit_admin->setHidden(false);
	}
}
