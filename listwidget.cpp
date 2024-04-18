#include "listwidget.h"
#include "algorithmLibrary.h"
#include <QMenu>

//已选择列表
UsedListWidget::UsedListWidget(QWidget* parent)
{
    mainwindow = parent;
    setDragEnabled(true);
    setFocusPolicy(Qt::NoFocus);
    setAcceptDrops(true);
    setFlow(QListView::TopToBottom);
    setDefaultDropAction(Qt::MoveAction);
    setDragDropMode(QAbstractItemView::InternalMove);
    setMinimumWidth(300);
    connect(this, &QListWidget::itemClicked, this, &UsedListWidget::show_attr);
}

void UsedListWidget::contextMenuEvent(QContextMenuEvent*)
{
    ListWidgetItem* item;
    item = (ListWidgetItem*)this->itemAt(this->mapFromGlobal(QCursor::pos()));
    if (item == NULL) { return; }
    QMenu* menu = new QMenu(this);
    QAction* delete_action = new QAction("删除", this);
    connect(delete_action, &QAction::triggered, this, [item, this] { delete_item(item); });
    menu->addAction(delete_action);
    menu->move(cursor().pos());
    menu->show();
}

void UsedListWidget::delete_item(ListWidgetItem* item)
{
    //this->takeItem(this->row(item));
    //MainWindow* window;
    //window = (MainWindow*)mainwindow;
    //window->update_image();
    //window->close_attr();
}

void UsedListWidget::show_attr()
{ 
    //    item = (MyItem *)this->itemAt(this->mapFromGlobal(QCursor::pos()));//另一种方法
    ListWidgetItem* item = (ListWidgetItem*)this->currentItem();
    int index = item->get_index();
    ParamsItem p;                                                   
    p = item->get_params();
    //TableWidget* table_widget = (TableWidget*)window->stacked_Widget->currentWidget();
    //table_widget->update_params(p);
    //window->dock_attr->setVisible(true);
}

//
//SmoothItem::SmoothItem(const QString& text, QListWidget* parent)
//    : MyItem(text, parent)
//{
//    this->setIcon(QIcon(":/images/blur.png"));
//    params.setKindValue(0);
//    params.setKsizeValue(1);
//    params.setKsigmaValue(1);
//}
//
//QImage SmoothItem::item_process_img(QImage img)
//{
//    if (params.getKindValue() == 0) {
//        img = Tools::GaussianFilter(img, params.getKsizeValue(), params.getKsigmaValue());
//    }
//    else if (params.getKindValue() == 1) {
//        img = Tools::AverageFilter(img, params.getKsizeValue());
//    }
//    else {
//        img = Tools::MeidanFilter(img, params.getKsizeValue());
//    }
//    return img;
//}
//
//SharpenItem::SharpenItem(const QString& text, QListWidget* parent)
//    :MyItem(text, parent)
//{
//    this->setIcon(QIcon(":/images/sharpen.png"));
//    params.setKindValue(0);
//}
//
//QImage SharpenItem::item_process_img(QImage img)
//{
//    if (params.getKindValue() == 0) {
//        img = Tools::SobelFilter(img, true);
//    }
//    else if (params.getKindValue() == 1) {
//        img = Tools::SobelFilter(img, false);
//    }
//    else {
//        img = Tools::LaplaceFilter(img);
//    }
//    return img;
//}

GaryPatternItem::GaryPatternItem(const QString& text, QListWidget* parent)
            :ListWidgetItem(text, parent)
{
    this->params.setKindValue(0);
    this->params.setKsizeValue(75);
}

QImage GaryPatternItem::item_process_img(QImage image)
{
    QImage img;
    if (params.getKindValue() == 0) {
        img = Tools::PowerLevelTransformation(img, image, params.getWwValue());
    }
    else {
        img = Tools::PowerLevelTransformation(img, image,params.getWwValue());
    }
    return img;
}

//GaryPatternItem灰度匹配获取图
QImage GaryPatternItem::get_item_image(QImage srcImg, int width, int height)
{

    return QImage();
}

ListWidgetItem::ListWidgetItem(const QString& text, QListWidget* parent) 
    :QListWidgetItem(text, parent)
{

}
