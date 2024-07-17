#include"logoSet.h"


logoSet::logoSet(QWidget* parent) :
	QWidget(parent),
	ui(new Ui::logoSet)
{
	ui->setupUi(this);
	//��ȡ�������õ���־�ļ��е�ַ
	QString settingPath = QCoreApplication::applicationDirPath() + "/setting.ini";
	QSettings* settings = new QSettings(settingPath, QSettings::IniFormat);
	settings->beginGroup("Idus");
	//�ع�
	QString logDirectory = settings->value("logDirectory").toString();
	ui->lineEdit->setText(logDirectory);

	//��ɫ������Ͻ�ͼ��
	setWindowIcon(QIcon("Image/login/icon.ico"));

	setWindowTitle("��־����");
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
	//�ļ���
	    QString  srcDirPath = QFileDialog::getExistingDirectory(this, "ѡ���ļ���","/");
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