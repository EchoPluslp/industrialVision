#ifndef TABLEWIDGET_H
#define TABLEWIDGET_H
#pragma once

#include <QTableWidget>
#include <QHeaderView>
#include <QComboBox>
#include <QSpinBox>
#include "paramsitem.h"
class TableWidget : public QTableWidget
{
    Q_OBJECT
public:
    QWidget* mainwindow;
    TableWidget(QWidget* parent);
    virtual void update_params(ParamsItem p) = 0;
    virtual ParamsItem get_params() = 0;
public slots:
    void update_item();
};

class GaryPatternItemWidget : public TableWidget
{
    Q_OBJECT
private:
    QComboBox* kind_comBox;
    QSpinBox* ksize_spinBox;
    QSpinBox* ksigma_spinBox;
    void GaryPatternItemWidget::create_UI();
public:
    GaryPatternItemWidget(QWidget* parent);
    GaryPatternItemWidget();

    void update_params(ParamsItem p);
    ParamsItem get_params();
};


#endif