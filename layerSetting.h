#pragma once
#include "ui_layerSetting.h"
#include <QSettings>
#include <QMessageBox>
#include "QTextCodec.h"
#include <QDialog>
#include <QRegularExpression>
#include "globalvar.h"
#pragma execution_character_set("utf-8")

class layerSetting : public QWidget
{
	Q_OBJECT

public:
	layerSetting(QWidget* parent = Q_NULLPTR);

	int  sortNameSize = 0;
	//保存路径
	QString saveFolderPath;

	//已选操作和属性设置 
	~layerSetting();

	//分析出当前图层列表
	QList<QString>  AnalyzeLayerList(QString layerItem);
protected:
	void showEvent(QShowEvent* event) override;
private:
	Ui::layerSetting* ui;
public slots:
	void visionPointSetting();
	void layerOrderSetting();
	void globalUniqueFaceSetting();
signals:
	void setCurrentFaceToProcess(int value);


};

