#ifndef SMALLWINDOW_H_
#define SMALLWINDOW_H_
#include "ui_smallMainWindow.h"
#pragma execution_character_set("utf-8")



class smallMainWindow : public QMainWindow
{
	Q_OBJECT
public:
	smallMainWindow(QWidget* parent = Q_NULLPTR);

	//已选操作和属性设置 
	~smallMainWindow();
private:
	Ui::smallMainWindow ui;

};

#endif