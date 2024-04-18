#include"logoSet.h"


logoSet::logoSet(QWidget* parent) :
	QWidget(parent),
	ui(new Ui::logoSet)
{
	ui->setupUi(this);
	//读取本地设置的日志文件夹地址
	QString settingPath = QCoreApplication::applicationDirPath() + "/setting.ini";
	QSettings* settings = new QSettings(settingPath, QSettings::IniFormat);
	settings->beginGroup("Idus");
	//曝光
	QString logDirectory = settings->value("logDirectory").toString();
	ui->lineEdit->setText(logDirectory);

	//白色填充左上角图标
	QPixmap pixmap(100, 100);
	pixmap.fill(Qt::white);
	setWindowIcon(QIcon(pixmap));
	setWindowTitle("日志管理");
	setWindowFlags(Qt::WindowCloseButtonHint);

}
logoSet::~logoSet() {

}

void logoSet::readLog_click()
{
	QString logDir = ui->lineEdit->text();
	QString strFileName = QFileDialog::getOpenFileName(this, "Open File", logDir,
		tr("log(*.log)"), 0, QFileDialog::DontResolveSymlinks);
	QDesktopServices::openUrl(QUrl(strFileName));

	//this->close();

}

void logoSet::logSet_click() {
	//文件夹
	    QString  srcDirPath = QFileDialog::getExistingDirectory(this, "选择文件夹","/");
		 if (srcDirPath.isEmpty()){
		        return;
		     }
	     else{
		         srcDirPath += "/";
		     }
		ui->lineEdit->setText(srcDirPath);

		QString settingPath = QCoreApplication::applicationDirPath() + "/setting.ini";
		QSettings* settings = new QSettings(settingPath, QSettings::IniFormat);
		settings->beginGroup("Idus");
		settings->setValue("logDirectory", srcDirPath);
}