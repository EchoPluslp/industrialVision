#include "basicSetting.h"

basicSetting::basicSetting(QWidget* parent) :QWidget(parent),
	ui(new Ui::FormSetting)
{
	ui->setupUi(this);
	connect(ui->radioButton, &QRadioButton::clicked, this,
		&basicSetting::onRadioButtonClicked);
	connect(ui->radioButton_zh, &QRadioButton::clicked, this,
		&basicSetting::onRadioButton_ZH_Clicked);
	connect(ui->radioButton_en, &QRadioButton::clicked, this,
		&basicSetting::onRadioButton_EN_Clicked);

	QString settingPath = QCoreApplication::applicationDirPath() + "/setting.ini";
	QSettings* settings = new QSettings(settingPath, QSettings::IniFormat);
	settings->beginGroup("Idus");
	//��ȡĬ��ֵ
	QString logDirectory = settings->value("Y_Flag").toString();
	if (logDirectory == "1")
	{
		ui->radioButton->setChecked(true);
	}
	else {
		ui->radioButton->setChecked(false);

	}
	//ϵͳ��������
	QString systenLanguage = settings->value("System_language").toString();
	if (systenLanguage =="zh")
	{
		ui->radioButton_zh->setChecked(true);
		ui->radioButton_en->setChecked(false);
		setWindowTitle("ͨ������");
	}
	else if (systenLanguage=="en")
	{
		ui->radioButton_en->setChecked(true);
		ui->radioButton_zh->setChecked(false);
		setWindowTitle("Settings");

	}
}

basicSetting::~basicSetting()
{
}

void basicSetting::onRadioButton_ZH_Clicked()
{
	//����󣬱��浱ǰ������Ϣ
	QString settingPath = QCoreApplication::applicationDirPath() + "/setting.ini";
	QSettings* settings = new QSettings(settingPath, QSettings::IniFormat);
	settings->beginGroup("Idus");
		settings->setValue("system_language", "zh");
	
}

void basicSetting::onRadioButton_EN_Clicked()
{
	//����󣬱��浱ǰ������Ϣ
	QString settingPath = QCoreApplication::applicationDirPath() + "/setting.ini";
	QSettings* settings = new QSettings(settingPath, QSettings::IniFormat);
	settings->beginGroup("Idus");
	settings->setValue("system_language", "en");
}

void basicSetting::onRadioButtonClicked() {
	//��ȡ��ǰ���״̬
	bool flag = ui->radioButton->isChecked();
	//���浱ǰ���״̬��ini�ļ���

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