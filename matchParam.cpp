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
	int result = str.toInt(&ok);  // ���Խ�QStringת��Ϊ����
	if (!ok) {
		return 0;  // ���ת��ʧ�ܣ�����0
	}
	return result;  // ת���ɹ�������ת���������
}

void matchParam::showEvent(QShowEvent* event)
{
	//��ȡini�ļ��еĲ���
		//  //��ȡ�ϴιر�ʱ��״̬
	QString settingPath = QCoreApplication::applicationDirPath() + "/setting.ini";
	QSettings* settings = new QSettings(settingPath, QSettings::IniFormat);
	settings->beginGroup("Idus");
	QString file_toleranceAngle = settings->value("toleranceAngle", "0").toString();
	QString file_rotationDirection = settings->value("rotationDirection", "0").toString();
	//�ж�Qstring �Ƿ�������
	
	setRotationParam(convert_to_number(file_toleranceAngle),
		convert_to_number(file_rotationDirection));
	
	//uiչʾ��ǰ��ֵ
	ui->lineEdit_param->setText(QString::number(current_dToleranceAngle));
	ui->comboBox_rotation->setCurrentIndex(current_rotationDirection);
}

void matchParam::cancelRotateParam()
{
	this->setHidden(true);
}

void matchParam::setRotateParam() {
	//��ȡ��ǰֵ
		//uiչʾ��ǰ��ֵ
	int itemParam = ui->lineEdit_param->text().toInt();
	int itemAngleRotate = ui->comboBox_rotation->currentIndex();

	if (itemParam < 0 || itemParam > 180) {
		ui->lineEdit_param->setText("0");  // �������0��180��Χ�ڣ�����Ϊ0
		itemParam = 0;
	}

	//���������Ϣ�洢����ǰλ��
	QString settingPath = QCoreApplication::applicationDirPath() + "/setting.ini";
	QSettings* settings = new QSettings(settingPath, QSettings::IniFormat);
	settings->beginGroup("Idus");
	settings->setValue("toleranceAngle", itemParam);
	settings->setValue("rotationDirection", itemAngleRotate);
	settings->endGroup();
	delete settings;
	QMessageBox::warning(this, "������Ϣ", "����ɹ�");
	close();
}

matchParam::matchParam(QWidget* parent) :
	QWidget(parent),
	ui(new Ui::matchparam_param)
{
	ui->setupUi(this);

	QIntValidator* validator = new QIntValidator(0,180,ui->lineEdit_param);
	// ����֤�����ø��ı���
	ui->lineEdit_param->setValidator(validator);

}
