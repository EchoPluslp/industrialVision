#include"logoPath.h"


logoPath::logoPath(QWidget* parent) :
	QWidget(parent),
	ui(new Ui::logoPath)
{
	ui->setupUi(this);
	//读取本地设置的日志文件夹地址
	QString settingPath = QCoreApplication::applicationDirPath() + "/setting.ini";
	QSettings* settings = new QSettings(settingPath, QSettings::IniFormat);
	settings->beginGroup("Idus");
	//logo参数和日志参数
	QString lineEdit_logoPath = settings->value("logopath", "Image/login/logo.png").toString();
	QString lineEdit_namePath = settings->value("textpath", "Image/login/text.png").toString();

	ui->lineEdit_logoPath->setText(lineEdit_logoPath);
	ui->lineEdit_namePath->setText(lineEdit_namePath);


	//白色填充左上角图标

	setWindowIcon(QIcon("Image/login/icon.ico"));
	setWindowTitle("logo设置");
	setWindowFlags(Qt::WindowCloseButtonHint);

}

void logoPath::logoSet_click()
{
	// 打开文件对话框以选择图像文件
	QString filePath = QFileDialog::getOpenFileName(this, tr("选择图像"), QDir::homePath(), tr("图像文件 (*.png *.jpg *.bmp)"));

	// 检查是否选择了文件
	if (!filePath.isEmpty()) {
		ui->lineEdit_logoPath->setText(filePath); 
	}
	
}

void logoPath::setlogPathItem_language(QString language)
{
	if (language == "zh")
	{
		ui->pushButton->setText("图标设置");
		ui->pushButton_2->setText("软件名称设置");
		ui->pushButton_3->setText("保存");
		ui->pushButton_4->setText("取消");
	}else if (language == "en")
	{
		ui->pushButton->setText("Icon");
		ui->pushButton_2->setText("SoftwareName");
		ui->pushButton_3->setText("Save");
		ui->pushButton_4->setText("Cancel");
	}else if (language =="es")
	{
		ui->pushButton->setText("Iconos");
		ui->pushButton_2->setText("Nombre");
		ui->pushButton_3->setText("Guardar");
		ui->pushButton_4->setText("Cancelación");
	}
}

void logoPath::namePath_click()
{
	// 打开文件对话框以选择图像文件
	QString filePath = QFileDialog::getOpenFileName(this, tr("选择图像"), QDir::homePath(), tr("图像文件 (*.png *.jpg *.bmp)"));

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
	QMessageBox::warning(this, "保存信息", "保存成功,将在下一次重启时生效");
	close();
}

void logoPath::cancelParam_click()
{
	close();
}
