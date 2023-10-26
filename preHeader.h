#pragma once
#pragma execution_character_set("utf-8")

#ifndef PREHEADER_H
#define PREHEADER_H
#include <QtCore>
#include <QtWidgets>
#include <QDialog>
#include <QEvent>
enum DrawBaseShape
{
    selection,
    rotation,
    line,
    rectangle,  //����
    roundrect,
    ellipse,
    poly,
    arrowLine, //��ͷ�߶�
    text, //����
    penPath, //Ǧ��
    image, //ͼƬ
    triangle, //������
    None
};

#define SafeDelete(p) if (nullptr != p) { delete p; p = nullptr; }
#define USERCATALOGDIR  QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)
#endif // PREHEADER_H
