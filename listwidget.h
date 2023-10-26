#pragma once
#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include "paramsitem.h"
#include <QImage>
#include <QListWidget>
#include <QContextMenuEvent>

class ListWidgetItem;

class UsedListWidget : public QListWidget
{
    Q_OBJECT
private:
    QWidget* mainwindow;
public:
    UsedListWidget(QWidget* parent);
    void contextMenuEvent(QContextMenuEvent* event);
private slots:
    void show_attr();
    void delete_item(ListWidgetItem* item);
};

class ListWidgetItem : public QListWidgetItem
{
private:
    ParamsItem params;
public:
    ListWidgetItem(const QString& text, QListWidget* parent = nullptr);
    virtual QImage item_process_img(QImage) = 0;
    virtual ParamsItem get_params() = 0;
    virtual int get_index() = 0;
    virtual void update_params(ParamsItem p) = 0;
    //virtual  QImage get_item_image(QImage srcImg, int width, int height);
};


class GaryPatternItem : public ListWidgetItem
{
private:
    ParamsItem params;
public:
    GaryPatternItem(const QString& text, QListWidget* parent = nullptr);
    int get_index() { return 1; }
    ParamsItem get_params() { return params; }
    void update_params(ParamsItem p) { params = p; }
    QImage item_process_img(QImage);
    QImage get_item_image(QImage srcImg,int width,int height);
};

#endif