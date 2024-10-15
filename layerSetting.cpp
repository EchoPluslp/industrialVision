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

//读取当前的currentIndex
void layerSetting::showEvent(QShowEvent* event)
{
	if (saveFolderPath.isEmpty())
	{
		return;
	}
	QString laySetting = saveFolderPath + "/layerSetting.ini";
	QSettings* settings = new QSettings(laySetting, QSettings::IniFormat);
	//图层顺序
	QString layerOrder = settings->value("LayerOrder","").toString();
	ui->layerOrder->setText(layerOrder);
	
	//视觉识别点
	QString visionPoint = settings->value("VisionPoint","").toString();
	ui->visionPoint->setText(visionPoint);

	if (FaceStringListItemInfo.size() > GlobalUniqueFace)
	{
		//当前面数
		ui->uniqueFace->setText(FaceStringListItemInfo.at(GlobalUniqueFace));
		//读取当前路径下的setting文件，看有没有以前设置的图层
	}
}

//保存当前拍照视觉点
void layerSetting::visionPointSetting()
{
	QString visionPointList = ui->visionPoint->toPlainText();
	//保存当前列表
	QString laySetting = saveFolderPath + "/layerSetting.ini";
	QSettings* settings = new QSettings(laySetting, QSettings::IniFormat);
	settings->setValue("VisionPoint", visionPointList);

	VisionPointListItemInfo = AnalyzeLayerList(visionPointList);
	QMessageBox::warning(nullptr, "保存成功", "保存视觉拍照点成功！");
	ui->pushButton_3->setStyleSheet("background-color: white");
}

//图层顺序
void layerSetting::layerOrderSetting()
{
	QString layerOrderList = ui->layerOrder->toPlainText();
	if (layerOrderList.isEmpty())
	{
		QMessageBox::warning(nullptr, "保存失败", "保存图层顺序失败！");
		return;
	}
	//保存当前图层顺序
	QString laySetting = saveFolderPath + "/layerSetting.ini";
	QSettings* settings = new QSettings(laySetting, QSettings::IniFormat);
	settings->setValue("layerOrder", layerOrderList);

	FaceStringListItemInfo = AnalyzeLayerList(layerOrderList);
	QMessageBox::warning(nullptr, "保存成功", "保存图层顺序成功！");

	ui->uniqueFace->setText(FaceStringListItemInfo.at(GlobalUniqueFace));
	//背景清空
	ui->pushButton->setStyleSheet("background-color: white");
}

//当前面数设置
void layerSetting::globalUniqueFaceSetting()
{
	QString string = ui->uniqueFace->toPlainText();
	// 遍历 QList 并检查是否存在与 string 相等的字符串
	for (int i = 0; i < FaceStringListItemInfo.size(); ++i) {
		if (FaceStringListItemInfo[i] == string) {
			GlobalUniqueFace = i;
			ui->pushButton_2->setStyleSheet("background-color: white");
			QMessageBox::warning(nullptr, "保存成功", "设置当前面数成功！");
			return; 
		}
	}
	QMessageBox::warning(nullptr, "设置面数失败", "设置的面数没有在当前的图层顺序中！");
}


QList<QString>  layerSetting::AnalyzeLayerList(QString layerItem)
{
	QStringList list = layerItem.split("-"); // 使用英文逗号分割字符串
	// 去除每个字符串前后的空格
	for (int i = 0; i < list.size(); ++i) {
		list[i] = list[i].trimmed();
	}
	// 将 QStringList 中的元素添加到 QList<QString>
	QList<QString> qList;
	for (const QString& item : list) {
		qList.append(item);
	}
	return qList;
}

