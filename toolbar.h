#ifndef TOOLBAR_H
#define TOOLBAR_H
#pragma execution_character_set("utf-8")

#include <QToolBar>
#include "myactions.h"
class ToolBar : public QToolBar
{
	Q_OBJECT
public:
	explicit ToolBar(QWidget* parent = nullptr);
signals:
};

#endif // TOOLBAR_H
