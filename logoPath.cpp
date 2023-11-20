#include"logoPath.h"


logoPath::logoPath(QWidget* parent) :
	QWidget(parent),
	ui(new Ui::logoPath)
{
	ui->setupUi(this);
	//��ȡ�������õ���־�ļ��е�ַ
	QString settingPath = QCoreApplication::applicationDirPath() + "/setting.ini";
	QSettings* settings = new QSettings(settingPath, QSettings::IniFormat);
	settings->beginGroup("Idus");
	//logo��������־����
	QString lineEdit_logoPath = settings->value("logopath", "Image/login/logo.png").toString();
	QString lineEdit_namePath = settings->value("textpath", "Image/login/text.png").toString();

	ui->lineEdit_logoPath->setText(lineEdit_logoPath);
	ui->lineEdit_namePath->setText(lineEdit_namePath);


	//��ɫ������Ͻ�ͼ��
	QPixmap pixmap(100, 100);
	pixmap.fill(Qt::white);
	setWindowIcon(QIcon(pixmap));
	setWindowTitle("logo����");
	setWindowFlags(Qt::WindowCloseButtonHint);

}

void logoPath::logoSet_click()
{
	// ���ļ��Ի�����ѡ��ͼ���ļ�
	QString filePath = QFileDialog::getOpenFileName(this, tr("ѡ��ͼ��"), QDir::homePath(), tr("ͼ���ļ� (*.png *.jpg *.bmp)"));

	// ����Ƿ�ѡ�����ļ�
	if (!filePath.isEmpty()) {
		ui->lineEdit_logoPath->setText(filePath); 
	}
	
}

void logoPath::namePath_click()
{
	// ���ļ��Ի�����ѡ��ͼ���ļ�
	QString filePath = QFileDialog::getOpenFileName(this, tr("ѡ��ͼ��"), QDir::homePath(), tr("ͼ���ļ� (*.png *.jpg *.bmp)"));

	if (!filePath.isEmpty()) {
		ui->lineEdit_namePath->setText(filePath);
	}
}

void logoPath::setParam_click()
{
	QString namePath = ui->lineEdit_namePath->text();
	QString logoPath = ui->lineEdit_logoPath->text();
	if (namePath.isEmpty())
	{
		namePath = "Image/login/text.png";
	}
	if (logoPath.isEmpty())
	{
		logoPath = "Image/login/logo.png";
	}
	QString settingPath = QCoreApplication::applicationDirPath() + "/setting.ini";
	QSettings* settings = new QSettings(settingPath, QSettings::IniFormat);
	settings->beginGroup("Idus");
	settings->setValue("logopath", logoPath);
	settings->setValue("namepath", namePath);
	QMessageBox::warning(this, "������Ϣ", "����ɹ�,������һ������ʱ��Ч");
	close();
}

void logoPath::cancelParam_click()
{
	close();
}
