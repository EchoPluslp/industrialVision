#include "drawfinisheddialog.h"

DrawFinishedDialog::DrawFinishedDialog(QWidget* parent) :
	QDialog(parent)
{
	ui.setupUi(this);
}

void DrawFinishedDialog::setList(const QList<QListWidgetItem*>& list) {
	//ui.listWidget->clear();
	//for (int i = 0; i < list.count(); i++) {
	//	if (list[i])
	//		ui.listWidget->addItem(list[i]->text());
	//}
}

void DrawFinishedDialog::showEvent(QShowEvent* e)
{
	QListWidget* listWidget = ui.listWidget; // 替换成你的 QListWidget 对象

	if (currentShape)
	{
		//矩形框
		for (int i = 0; i < 3; i++) {
			QListWidgetItem* item = listWidget->item(i); // 获取第i个元素
			if (item) {
				item->setHidden(false); // 显示元素
			}
			if (i == 2)
			{
				item->setHidden(true); // 隐藏最后一个元素
			}
		}
	}
	else {
		//输出点
		for (int i = 0; i < 3; i++) {
			QListWidgetItem* item = listWidget->item(i); // 获取第i个元素
			if (item) {
				item->setHidden(true); // 显示元素
			}
			if (i == 2)
			{
				item->setHidden(false); // 隐藏最后一个元素
			}
		}
	}
	ui.lineEdit->selectAll();
}

void DrawFinishedDialog::setShapeFlag(bool flag)
{
	currentShape = flag;
}


void DrawFinishedDialog::on_listWidget_itemClicked(QListWidgetItem* item)
{
	ui.lineEdit->setText(item->text());
}

void DrawFinishedDialog::on_buttonBox_accepted()
{
	auto text = ui.lineEdit->text();
	if (text.isNull() || text.isEmpty()) {
		QMessageBox::warning(this, tr("Warn"), tr("未选择属性"));
	}
	else {

		emit sendTypeName(text);
	}
	this->close();
}

void DrawFinishedDialog::on_buttonBox_rejected()
{
	this->close();

}