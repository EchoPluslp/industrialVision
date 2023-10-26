#pragma once
#ifndef LOGOSET_H_
#define LOGOSET_H_

#include "ui_logoSet.h"
#include "QTextCodec.h"
#pragma execution_character_set("utf-8")
#include <QSettings>
#include <QFileDialog>
#include <QUrl>
#include <QProcess>
#include <QDesktopServices>
class logoSet : public QWidget
{
	Q_OBJECT

public:
	logoSet(QWidget* parent = Q_NULLPTR);
	//已选操作和属性设置 
	~logoSet();
private:
	Ui::logoSet* ui;

public slots:
	void logSet_click();
	void readLog_click();
};
#endif