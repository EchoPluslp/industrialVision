#pragma once
#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
#include <QThread>
#include <QImage>
#include <QDebug>
#include <mutex>
#include <condition_variable>
#include "globalvar.h"
#include "SharedData.h"

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
	bool getSwitchFlag();
	void setRotateIndex(int rotateIndex);
	int getRotateIndex();
	bool getStartRunFlag();
	void run();
	 const 	std::vector<int> degrees = { 0,90,180,270 };
	 cv::Mat CameraThread::rotateImage(cv::Mat& src_img, int degree);
	 QSize realPictureSize;
	 QSize getrealPictureSize();
	 bool startFlag;
	 bool startRunFlag = false;

signals:
	void signal_messImage(QImage myImage, int index);

private:
	int m_cameraIndex;
	int rotateIndexValue = 0;
	CMvCamera* cameraPtr = NULL;
};

#endif // MYTHREAD_H
