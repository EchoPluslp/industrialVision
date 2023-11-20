#pragma once
#ifndef LOGOPATH_H_
#define LOGOPATH_H_

#include "ui_logoPath.h"
#include "QTextCodec.h"
#pragma execution_character_set("utf-8")
#include <QSettings>
#include <QFileDialog>
#include <QUrl>
#include <QMessageBox>
class logoPath : public QWidget
{
	Q_OBJECT

public:
	logoPath(QWidget* parent = Q_NULLPTR);
	//已选操作和属性设置 
	//~logoPath();
private:
	Ui::logoPath* ui;

public slots:
	void logoSet_click();
	void namePath_click();
	void setParam_click();
	void cancelParam_click();

};
#endif