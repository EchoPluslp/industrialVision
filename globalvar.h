#ifndef GLOBALVAR_H
#define GLOBALVAR_H

#include <QVector>
#include "MvCamera.h"
#include <QImage>
extern QVector<cv::Mat> m_imageVector_1;   //֡��������
extern QVector<cv::Mat> m_imageVector_2;

//20240801��ǰ����
extern int GlobalUniqueFace;

//ͼ�������б�f
extern QList<QString> FaceStringListItemInfo;
//ͼ�����յ�
extern QList<QString> VisionPointListItemInfo;


#endif // GLOBALVAR_H
