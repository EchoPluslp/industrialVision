#ifndef LABELSDOCKWIDGET_H
#define LABELSDOCKWIDGET_H
#pragma execution_character_set("utf-8")


#include <QWidget>
#include <QDockWidget>
#include <QListWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include "label.h"
#include "TableWidget.h"


// 显示标签列表，相应标签点击事件并发送给LabelController
class LabelsDockWidget : public QDockWidget
{
	Q_OBJECT

	QListWidget* listWidget;
	QPushButton* pushButton;
	QWidget* dockContent;
	QVBoxLayout* layout;
	QString toName(const QString& type, const int id);
public:
	explicit LabelsDockWidget(QWidget* parent = nullptr);
	QString currLabelName();
signals:
	void itemClicked(const QString&);
	void itemDoubleClicked(const QString&);

	void deleteCurrLabel(QString);

public slots:
	void labelCreated(const QString& type, const int id);
	void labelDeleted(const QString& type, const int id);
	void setLabelList(const QList<Label*> list);
	void labelSelected(const QString& labelName);
};

#endif // LABELSDOCKWIDGET_H
