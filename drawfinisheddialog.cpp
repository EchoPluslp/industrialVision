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
	QListWidget* listWidget = ui.listWidget; // �滻����� QListWidget ����

	if (currentShape)
	{
		//���ο�
		for (int i = 0; i < 3; i++) {
			QListWidgetItem* item = listWidget->item(i); // ��ȡ��i��Ԫ��
			if (item) {
				item->setHidden(false); // ��ʾԪ��
			}
			if (i == 2)
			{
				item->setHidden(true); // �������һ��Ԫ��
			}
		}
	}
	else {
		//�����
		for (int i = 0; i < 3; i++) {
			QListWidgetItem* item = listWidget->item(i); // ��ȡ��i��Ԫ��
			if (item) {
				item->setHidden(true); // ��ʾԪ��
			}
			if (i == 2)
			{
				item->setHidden(false); // �������һ��Ԫ��
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
		QMessageBox::warning(this, tr("Warn"), tr("δѡ������"));
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