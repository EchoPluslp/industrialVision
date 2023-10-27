#include "camerathread.h"

CameraThread::CameraThread(QObject* parent)
	: QThread{ parent }
{
	this->initThread();
}

CameraThread::~CameraThread()
{
	if (cameraPtr == NULL)
	{
		delete cameraPtr;
	}
}

void CameraThread::initThread()
{
	startFlag = false;
	m_cameraIndex = 0;
}

void CameraThread::setCameraPtr(CMvCamera* camera)
{
	cameraPtr = camera;
}

void CameraThread::setCameraIndex(int index)
{
	m_cameraIndex = index;
}

void CameraThread::setSwitchFlag(bool switchFlag)
{
	startFlag = switchFlag;
}

bool CameraThread::getSwitchFlag()
{
	return startFlag;
}

void CameraThread::setRotateIndex(int rotateIndex)
{
	rotateIndexValue = rotateIndex;
//	startRunFlag = true;
}
int CameraThread::getRotateIndex()
{
	return rotateIndexValue;
}
QSize CameraThread::getrealPictureSize()
{
	msleep(30);   //�ʵ�����,����cpu������

	return realPictureSize;
}
bool CameraThread::getStartRunFlag()
{
	return startRunFlag;
}


void CameraThread::run()
{
	if (cameraPtr == NULL)
	{
		return;
	}
	while (startFlag)
	{
		//��ͼƬ��ӵ������߳�
		cv::Mat imagePtr;
		cameraPtr->CommandExecute("TriggerSoftware");
		cameraPtr->ReadBuffer(imagePtr);
		//��ӵ�����
		if(imagePtr.empty())
			continue;
		std::lock_guard<std::mutex> lock(mtx);

		//��ת
		cv::Mat srcCopy = rotateImage(imagePtr, degrees[rotateIndexValue % degrees.size()]);
		dimensions.width = srcCopy.cols;
		dimensions.height = srcCopy.rows;
		m_imageVector_1.push_back(srcCopy);



		msleep(30);   //�ʵ�����,����cpu������
	}
}

/**
 * degree ֻ���� 90�㡢180�㡢270��
 * �ýǶ�Ϊ˳ʱ�뷽���������ʱ����ת��������� 90�� �� 270�� ��ʵ�ֽ�������
 * @param src_img
 * @param degree
 * @return
 */
cv::Mat CameraThread::rotateImage(cv::Mat& src_img, int degree) {
	if (degree == 90) {
		cv::Mat srcCopy = cv::Mat(src_img.rows, src_img.cols, src_img.depth());
		cv::transpose(src_img, srcCopy);
		cv::flip(srcCopy, srcCopy, 1);
		return srcCopy;
	}
	else if (degree == 180) {
		cv::Mat srcCopy = cv::Mat(src_img.rows, src_img.cols, src_img.depth());
		cv::flip(src_img, srcCopy, -1);
		return srcCopy;
	}
	else if (degree == 270) {
		cv::Mat srcCopy = cv::Mat(src_img.rows, src_img.cols, src_img.depth());
		cv::transpose(src_img, srcCopy);
		cv::flip(srcCopy, srcCopy, 0);
		return srcCopy;
	}
	else {
		return src_img;
	}
}


