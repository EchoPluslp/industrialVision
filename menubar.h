#ifndef MENUBAR_H
#define MENUBAR_H

#include <QMenuBar>
#include <QMenu>
#include "myactions.h"
#pragma execution_character_set("utf-8")


class MenuBar : public QMenuBar
{
	Q_OBJECT
public:
	explicit MenuBar(QWidget* parent = nullptr);
signals:
};

#endif // MENUBAR_H
