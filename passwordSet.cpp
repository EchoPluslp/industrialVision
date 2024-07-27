#include "passwordSet.h"
passwordSet::passwordSet(QWidget* parent) :
	QWidget(parent),
	ui(new Ui::password)
{
	ui->setupUi(this);
	//读取本地设置的日志文件夹地址
	QString settingPath = QCoreApplication::applicationDirPath() + "/setting.ini";
	QSettings* settings = new QSettings(settingPath, QSettings::IniFormat);
	settings->beginGroup("Idus");
	//密码
	QString logDirectory = settings->value("password").toString();
	//管理员密码
	QString passwordAdmin = settings->value("passwordAdmin", "2222").toString();
	//操作员密码
	QString passwordUser = settings->value("passwordUser", "1111").toString();
	ui->lineEdit_admin->setText(passwordAdmin);
	ui->lineEdit_user->setText(passwordUser);


	//白色填充左上角图标
	setWindowIcon(QIcon("icon.ico"));

	setWindowTitle("密码设置");
	setWindowFlags(Qt::WindowCloseButtonHint);

}
passwordSet::~passwordSet() {

}

void passwordSet::readSetPassword()
{

}

void passwordSet::passwordAdminSetclick() {
	QString password = ui->lineEdit_admin->text();
	//设置密码
	if(isAlphaNumeric(password)){
	QString settingPath = QCoreApplication::applicationDirPath() + "/setting.ini";
	QSettings* settings = new QSettings(settingPath, QSettings::IniFormat);
	settings->beginGroup("Idus");
	settings->setValue("passwordAdmin", password);
	//创建成功
	QMessageBox::information(nullptr, "设置管理员密码", "管理员密码创建成功");
	this->close();
	}
	else {
		QMessageBox::warning(nullptr, "设置管理员密码", "管理员创建失败,只能包含数字和字母");

	}
}
void passwordSet::passwordUserSetclick() {
	QString password = ui->lineEdit_user->text();
	//设置密码
	if (isAlphaNumeric(password)) {
		QString settingPath = QCoreApplication::applicationDirPath() + "/setting.ini";
		QSettings* settings = new QSettings(settingPath, QSettings::IniFormat);
		settings->beginGroup("Idus");
		settings->setValue("passwordUser", password);
		//创建成功
		QMessageBox::information(nullptr, "设置操作员密码", "操作员密码创建成功");
		this->close();
	}
	else {
		QMessageBox::warning(nullptr, "设置操作员密码", "操作员创建失败,只能包含数字和字母");

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
	if (stringRole == "操作员")
	{
		ui->pushButton_setadmin->setHidden(true);
		ui->lineEdit_admin->setHidden(true);
	}
	else {
		ui->pushButton_setadmin->setHidden(false);
		ui->lineEdit_admin->setHidden(false);
	}
}
