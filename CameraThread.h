#pragma once
#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
#include <QThread>
#include <QImage>
#include <QDebug>
#include "globalvar.h"

class CameraThread : public QThread
{
	Q_OBJECT

public:
	explicit CameraThread(QObject* parent = nullptr);
	~CameraThread();

	void initThread();
	void setCameraPtr(CMvCamera* camera);
	void setCameraIndex(int index);
	void setSwitchFlag(bool switchFlag);
	void setRotateIndex(int rotateIndex);
	int getRotateIndex();
	void run();
	 const 	std::vector<int> degrees = { 0,90,180,270 };
	 cv::Mat CameraThread::rotateImage(cv::Mat& src_img, int degree);
	 QSize realPictureSize;
	 QSize getrealPictureSize();
signals:
	void signal_messImage(QImage myImage, int index);

private:
	bool startFlag;
	int m_cameraIndex;
	int rotateIndexValue = 0;
	CMvCamera* cameraPtr = NULL;
};

#endif // MYTHREAD_H
