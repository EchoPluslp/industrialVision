#ifndef GLOBALVAR_H
#define GLOBALVAR_H

#include <QVector>
#include "MvCamera.h"
#include <QImage>
extern QVector<cv::Mat> m_imageVector_1;   //帧保存容器
extern QVector<cv::Mat> m_imageVector_2;

//20240801当前面数
extern int GlobalUniqueFace;

//图层名称列表f
extern QList<QString> FaceStringListItemInfo;
//图层拍照点
extern QList<QString> VisionPointListItemInfo;


#endif // GLOBALVAR_H
