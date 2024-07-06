#pragma once
#include "ui_mattchParam.h"
#include <QSettings>
#include <QMessageBox>
class matchParam : public QWidget
{
	Q_OBJECT

public:
	matchParam(QWidget* parent = Q_NULLPTR);
	//设置system中的全局变量
	void setRotationParam(int toleranceAngle, int rotationDrirection);

	//获取system中的全局变量
	void getRotationParam();

	//已选操作和属性设置 
	~matchParam();

	int convert_to_number(const QString& str);
protected:
	void showEvent(QShowEvent* event) override;
private:
	Ui::matchparam_param* ui;
	//全局的变量,角度信息
	int current_dToleranceAngle;

	//全局变量，旋转方向
	int current_rotationDirection;
public slots:
	void setRotateParam();
	void cancelRotateParam();
};



