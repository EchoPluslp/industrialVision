#ifndef DRAWFINISHEDDIALOG_H
#define DRAWFINISHEDDIALOG_H
#pragma execution_character_set("utf-8")

#include "ui_drawfinisheddialog.h"
#include <QMessageBox>
#include <QListWidgetItem>
#include <QList>
#include <QtDebug>
#include "shape.h"


// ����Labelʱ�ĶԻ�����������Label��Type
class DrawFinishedDialog : public QDialog
{
	Q_OBJECT
		// Ĭ�ϴ�����Type
		QListWidgetItem* defaultItem = nullptr;
public:
	explicit DrawFinishedDialog(QWidget* parent = nullptr);
	Shape::Figure currentShape;
	// �ڴ򿪶Ի���ʱͬ���Ի����Type�б�
	void setList(const QList<QListWidgetItem*>& list);

	void showEvent(QShowEvent* e);
	void setShapeFlag(Shape::Figure flag);
	void emitcurrentShapeType(Shape::Figure flag);
	QString language;

	
signals:
	// ����Ҫ������Type
	void sendTypeName(QString&);

private slots:
	void on_listWidget_itemClicked(QListWidgetItem* item);
	void on_buttonBox_accepted();
	void on_buttonBox_rejected();

private:
	Ui::DrawFinishedDialog ui;
};

#endif // DRAWFINISHEDDIALOG_H
