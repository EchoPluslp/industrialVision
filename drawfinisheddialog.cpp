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
	QListWidget* listWidget = ui.listWidget;
	
		switch (currentShape) {
		case Shape::searchAreaRect: {
			for (int i = 0; i < 3; i++) {
				QListWidgetItem* item = listWidget->item(i); // ��ȡ��i��Ԫ��
				if (i==0) {
					item->setHidden(false); // ��ʾԪ��
					item->setSelected(true);
					ui.lineEdit->setText(item->text());
					continue;;
				}
					item->setHidden(true); // �������1,2��Ԫ��		
			}
			break;
			}
		case Shape::featureMatchingRect: {
			//���ο�
			for (int i = 0; i < 3; i++) {
				QListWidgetItem* item = listWidget->item(i); // ��ȡ��i��Ԫ��
				if (i==1) {
					item->setHidden(false); // ��ʾԪ��
					item->setSelected(true);
					ui.lineEdit->setText(item->text());
					continue;
				}
					item->setHidden(true); // �������һ��Ԫ��				
			}
			break;
			}
		case Shape::CirclePen: {
			//���ο�
			for (int i = 0; i < 3; i++) {
				QListWidgetItem* item = listWidget->item(i); // ��ȡ��i��Ԫ��
				if (i == 2) {
					item->setHidden(false); // ��ʾԪ��
					item->setSelected(true);
					ui.lineEdit->setText(item->text());
					continue;
				}
				item->setHidden(true); // 				
			}
			break;
		}
		}
	ui.lineEdit->selectAll();
}

void DrawFinishedDialog::setShapeFlag(Shape::Figure flag)
{
	currentShape = flag;
}
void DrawFinishedDialog::emitcurrentShapeType(Shape::Figure flag) {
	//emit sendTypeName(flag);
	if (flag == Shape::searchAreaRect)
	{
		QString string("1.��������");
		emit sendTypeName(string);

	}
	else if (flag == Shape::featureMatchingRect ||
		flag == Shape::Polygon ||
		flag == Shape::Ellipse)
	{
		QString string("2.��������");
		emit sendTypeName(string);
	}
	else if (flag == Shape::CirclePen)
	{
		QString string("3.�����");
		emit sendTypeName(string);
	}

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