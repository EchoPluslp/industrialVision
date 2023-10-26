#include "tablewidget.h"

TableWidget::TableWidget(QWidget* parent)
{
    mainwindow = parent;
    setShowGrid(true);
    setAlternatingRowColors(false);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    horizontalHeader()->setVisible(false);
    verticalHeader()->setVisible(false);
    horizontalHeader()->sectionResizeMode(QHeaderView::Stretch);
    verticalHeader()->sectionResizeMode(QHeaderView::Stretch);
    horizontalHeader()->setStretchLastSection(true);
    setFocusPolicy(Qt::NoFocus);
}

void TableWidget::update_item()
{
    ParamsItem p;
    p = get_params();
    //ListWidgetItem* item;
    //item = (ListWidgetItem*)window->used_List_Widget->currentItem();
    //item->update_params(p);
    //window->update_image();
}

GaryPatternItemWidget::GaryPatternItemWidget(QWidget* parent)
    :TableWidget(parent)
{

}
//GaryPatternItemWidget::GaryPatternItemWidget()
//{
//
//}
void GaryPatternItemWidget::create_UI() {
    kind_comBox = new QComboBox;
    QStringList kind_items = { "平方差匹配","标准平方差匹配","相关匹配","标准相关匹配","相关系数匹配","标准相关系数匹配" };
    kind_comBox->addItems(kind_items);
    kind_comBox->setObjectName("kind");

    ksize_spinBox = new QSpinBox;
    ksize_spinBox->setObjectName("grade");
    ksize_spinBox->setMinimum(1);
    ksize_spinBox->setSingleStep(2);


    this->setColumnCount(2);
    this->setRowCount(2);
    QTableWidgetItem* title_kind;
    title_kind = new QTableWidgetItem("匹配方法");
    this->setItem(0, 0, title_kind);
    this->setCellWidget(0, 1, kind_comBox);

    QTableWidgetItem* title_ksize;
    title_ksize = new QTableWidgetItem("得分");
    this->setItem(1, 0, title_ksize);
    this->setCellWidget(1, 1, ksize_spinBox);

    connect(kind_comBox, SIGNAL(currentIndexChanged(int)), this, SLOT(update_item()));
    connect(ksize_spinBox, SIGNAL(valueChanged(int)), this, SLOT(update_item()));
}

void GaryPatternItemWidget::update_params(ParamsItem p)
{
    int x = p.getKindValue();
    int xx = p.getKsizeValue();
    kind_comBox->setCurrentIndex(p.getKindValue());
    ksize_spinBox->setValue(p.getKsizeValue());
}

ParamsItem GaryPatternItemWidget::get_params()
{
    ParamsItem p;
    int x = kind_comBox->currentIndex();
    p.setKindValue(kind_comBox->currentIndex());
    p.setKsizeValue(ksize_spinBox->value());
    return p;
}
