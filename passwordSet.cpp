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
	ui->lineEdit->setText(logDirectory);

	//白色填充左上角图标
	QPixmap pixmap(100, 100);
	pixmap.fill(Qt::white);
	setWindowIcon(QIcon(pixmap));
	setWindowTitle("密码设置");
	setWindowFlags(Qt::WindowCloseButtonHint);

}
passwordSet::~passwordSet() {

}

void passwordSet::readSetPassword()
{

	//读取本地设置的日志文件夹地址
	QString settingPath = QCoreApplication::applicationDirPath() + "/setting.ini";
	QSettings* settings = new QSettings(settingPath, QSettings::IniFormat);
	settings->beginGroup("Idus");
	//曝光
	QString logDirectory = settings->value("password").toString();
	ui->lineEdit->setText(logDirectory); 
}

void passwordSet::passwordSetclick() {
	QString password = ui->lineEdit->text();
	//设置密码
	if(isAlphaNumeric(password)){
	QString settingPath = QCoreApplication::applicationDirPath() + "/setting.ini";
	QSettings* settings = new QSettings(settingPath, QSettings::IniFormat);
	settings->beginGroup("Idus");
	settings->setValue("password", password);
	//创建成功
	QMessageBox::information(nullptr, "设置密码", "密码创建成功");
	this->close();
	}
	else {
		QMessageBox::warning(nullptr, "设置密码", "创建失败,只能包含数字和字母");

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