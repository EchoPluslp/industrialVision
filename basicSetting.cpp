#include "basicSetting.h"

basicSetting::basicSetting(QWidget* parent) :QWidget(parent),
	ui(new Ui::FormSetting)
{
	ui->setupUi(this);
	connect(ui->radioButton, &QRadioButton::clicked, this,
		&basicSetting::onRadioButtonClicked);

	QString settingPath = QCoreApplication::applicationDirPath() + "/setting.ini";
	QSettings* settings = new QSettings(settingPath, QSettings::IniFormat);
	settings->beginGroup("Idus");
	//读取默认值
	QString logDirectory = settings->value("Y_Flag").toString();
	if (logDirectory == "1")
	{
		ui->radioButton->setChecked(true);
	}
	else {
		ui->radioButton->setChecked(false);

	}
}

basicSetting::~basicSetting()
{
}

void basicSetting::onRadioButtonClicked() {
	//获取当前点击状态
	bool flag = ui->radioButton->isChecked();
	//保存当前点击状态到ini文件中

	QString settingPath = QCoreApplication::applicationDirPath() + "/setting.ini";
	QSettings* settings = new QSettings(settingPath, QSettings::IniFormat);
	settings->beginGroup("Idus");
	if (flag)
	{
		settings->setValue("Y_Flag", "1");
	}
	else {
		settings->setValue("Y_Flag", "0");
	}
}