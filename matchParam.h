#pragma once
#include "ui_mattchParam.h"
#include <QSettings>
#include <QMessageBox>
#include "QTextCodec.h"
#pragma execution_character_set("utf-8")

class matchParam : public QWidget
{
	Q_OBJECT

public:
	matchParam(QWidget* parent = Q_NULLPTR);
	//����system�е�ȫ�ֱ���
	void setRotationParam(int toleranceAngle, int rotationDrirection);

	//��ȡsystem�е�ȫ�ֱ���
	void getRotationParam();

	//��ѡ�������������� 
	~matchParam();

	int convert_to_number(const QString& str);
protected:
	void showEvent(QShowEvent* event) override;
private:
	Ui::matchparam_param* ui;
	//ȫ�ֵı���,�Ƕ���Ϣ
	int current_dToleranceAngle = 0;

	//ȫ�ֱ�������ת����
	int current_rotationDirection = 0;
public slots:
	void setRotateParam();
	void cancelRotateParam();
};



