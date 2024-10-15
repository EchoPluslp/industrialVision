#include "layerSetting.h"
layerSetting::layerSetting(QWidget* parent) :
	QWidget(parent),
	ui(new Ui::layerSetting)
{
	ui->setupUi(this);
	ui->pushButton->setStyleSheet("background-color: red");
	ui->pushButton_3->setStyleSheet("background-color: red");
	ui->pushButton_2->setStyleSheet("background-color: red");

}

layerSetting::~layerSetting()
{
	
}

//��ȡ��ǰ��currentIndex
void layerSetting::showEvent(QShowEvent* event)
{
	if (saveFolderPath.isEmpty())
	{
		return;
	}
	QString laySetting = saveFolderPath + "/layerSetting.ini";
	QSettings* settings = new QSettings(laySetting, QSettings::IniFormat);
	//ͼ��˳��
	QString layerOrder = settings->value("LayerOrder","").toString();
	ui->layerOrder->setText(layerOrder);
	
	//�Ӿ�ʶ���
	QString visionPoint = settings->value("VisionPoint","").toString();
	ui->visionPoint->setText(visionPoint);

	if (FaceStringListItemInfo.size() > GlobalUniqueFace)
	{
		//��ǰ����
		ui->uniqueFace->setText(FaceStringListItemInfo.at(GlobalUniqueFace));
		//��ȡ��ǰ·���µ�setting�ļ�������û����ǰ���õ�ͼ��
	}
}

//���浱ǰ�����Ӿ���
void layerSetting::visionPointSetting()
{
	QString visionPointList = ui->visionPoint->toPlainText();
	//���浱ǰ�б�
	QString laySetting = saveFolderPath + "/layerSetting.ini";
	QSettings* settings = new QSettings(laySetting, QSettings::IniFormat);
	settings->setValue("VisionPoint", visionPointList);

	VisionPointListItemInfo = AnalyzeLayerList(visionPointList);
	QMessageBox::warning(nullptr, "����ɹ�", "�����Ӿ����յ�ɹ���");
	ui->pushButton_3->setStyleSheet("background-color: white");
}

//ͼ��˳��
void layerSetting::layerOrderSetting()
{
	QString layerOrderList = ui->layerOrder->toPlainText();
	if (layerOrderList.isEmpty())
	{
		QMessageBox::warning(nullptr, "����ʧ��", "����ͼ��˳��ʧ�ܣ�");
		return;
	}
	//���浱ǰͼ��˳��
	QString laySetting = saveFolderPath + "/layerSetting.ini";
	QSettings* settings = new QSettings(laySetting, QSettings::IniFormat);
	settings->setValue("layerOrder", layerOrderList);

	FaceStringListItemInfo = AnalyzeLayerList(layerOrderList);
	QMessageBox::warning(nullptr, "����ɹ�", "����ͼ��˳��ɹ���");

	ui->uniqueFace->setText(FaceStringListItemInfo.at(GlobalUniqueFace));
	//�������
	ui->pushButton->setStyleSheet("background-color: white");
}

//��ǰ��������
void layerSetting::globalUniqueFaceSetting()
{
	QString string = ui->uniqueFace->toPlainText();
	// ���� QList ������Ƿ������ string ��ȵ��ַ���
	for (int i = 0; i < FaceStringListItemInfo.size(); ++i) {
		if (FaceStringListItemInfo[i] == string) {
			GlobalUniqueFace = i;
			ui->pushButton_2->setStyleSheet("background-color: white");
			QMessageBox::warning(nullptr, "����ɹ�", "���õ�ǰ�����ɹ���");
			return; 
		}
	}
	QMessageBox::warning(nullptr, "��������ʧ��", "���õ�����û���ڵ�ǰ��ͼ��˳���У�");
}


QList<QString>  layerSetting::AnalyzeLayerList(QString layerItem)
{
	QStringList list = layerItem.split("-"); // ʹ��Ӣ�Ķ��ŷָ��ַ���
	// ȥ��ÿ���ַ���ǰ��Ŀո�
	for (int i = 0; i < list.size(); ++i) {
		list[i] = list[i].trimmed();
	}
	// �� QStringList �е�Ԫ����ӵ� QList<QString>
	QList<QString> qList;
	for (const QString& item : list) {
		qList.append(item);
	}
	return qList;
}

