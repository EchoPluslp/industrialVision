#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <QStatusBar>
#include "ui_statusbar.h"
#pragma execution_character_set("utf-8")

class StatusBar : public QStatusBar
{
	Q_OBJECT

public:
	explicit StatusBar(QWidget* parent = nullptr);

public slots:
	void setScenePos(QPoint p);
private:
	Ui::StatusBar ui;
};

#endif // STATUSBAR_H
