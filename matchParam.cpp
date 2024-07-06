#include "matchParam.h"

void matchParam::setRotationParam(int toleranceAngle, int rotationDrirection)
{
	current_dToleranceAngle = toleranceAngle;
	current_rotationDirection = rotationDrirection;
}

void matchParam::getRotationParam()
{
}

matchParam::~matchParam()
{
}

int matchParam::convert_to_number(const QString& str)
{
	bool ok;
	int result = str.toInt(&ok);  // 尝试将QString转换为整数
	if (!ok) {
		return 0;  // 如果转换失败，返回0
	}
	return result;  // 转换成功，返回转换后的整数
}

void matchParam::showEvent(QShowEvent* event)
{
	//读取ini文件中的参数
		//  //读取上次关闭时的状态
	QString settingPath = QCoreApplication::applicationDirPath() + "/setting.ini";
	QSettings* settings = new QSettings(settingPath, QSettings::IniFormat);
	settings->beginGroup("Idus");
	QString file_toleranceAngle = settings->value("toleranceAngle", "0").toString();
	QString file_rotationDirection = settings->value("rotationDirection", "0").toString();
	//判断Qstring 是否是数字
	
	setRotationParam(convert_to_number(file_toleranceAngle),
		convert_to_number(file_rotationDirection));
	
	//ui展示当前的值
	ui->lineEdit_param->setText(QString::number(current_dToleranceAngle));
	ui->comboBox_rotation->setCurrentIndex(current_rotationDirection);
}

void matchParam::cancelRotateParam()
{
	this->setHidden(true);
}

void matchParam::setRotateParam() {
	//获取当前值
		//ui展示当前的值
	int itemParam = ui->lineEdit_param->text().toInt();
	int itemAngleRotate = ui->comboBox_rotation->currentIndex();

	if (itemParam < 0 || itemParam > 180) {
		ui->lineEdit_param->setText("0");  // 如果不在0到180范围内，设置为0
		itemParam = 0;
	}

	//将保存的信息存储到当前位置
	QString settingPath = QCoreApplication::applicationDirPath() + "/setting.ini";
	QSettings* settings = new QSettings(settingPath, QSettings::IniFormat);
	settings->beginGroup("Idus");
	settings->setValue("toleranceAngle", itemParam);
	settings->setValue("rotationDirection", itemAngleRotate);
	settings->endGroup();
	delete settings;
	QMessageBox::warning(this, "保存信息", "保存成功");
	close();
}

matchParam::matchParam(QWidget* parent) :
	QWidget(parent),
	ui(new Ui::matchparam_param)
{
	ui->setupUi(this);

	QIntValidator* validator = new QIntValidator(0,180,ui->lineEdit_param);
	// 将验证器设置给文本框
	ui->lineEdit_param->setValidator(validator);

}
