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
protected:
	void contextMenuEvent(QContextMenuEvent* event) override {
		// 禁用右键菜单，什么都不做
		Q_UNUSED(event);
	}
signals:
};

#endif // TOOLBAR_H
