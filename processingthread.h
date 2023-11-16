#ifndef PROCESSINGTHREAD_H
#define PROCESSINGTHREAD_H

#include "Server.h"
#include <QObject>
#include <QThread>
#include <QImage>
#include <QPixmap>
#include <QDateTime>
#include <QMutex>
#include "globalvar.h"
#include "Mstruct.h"
#include <string>
#include <vector>
#include<iostream>
#include <stdio.h>
#include <QReadWriteLock>
#include<QPainter>
#define M_PI       3.14159265358979323846   // pi

extern s_SingleTargetMatch finall_Total_Result;

class ProcessingThread : public QThread
{
	Q_OBJECT

public:
	explicit ProcessingThread(QObject* parent = nullptr);
	~ProcessingThread();

	void initThread();
	void setThreadId(int id);
	void setSwitchFlag(bool switchFlag);
	void setmodelAndRealSclar(bool switchFlag);
	bool getmodelAndRealSclar();

	void run();

	QImage cvMatToImage(const cv::Mat cvMat);
	QPixmap cvMatToPixmap(const cv::Mat cvMat);
	QImage tempqimageFromCamare;
	cv::Mat ImageToMat(QImage& image); //QImage转Mat

	//模板匹配相关函数::
	cv::Point2f ProcessingThread::MatchPicture(cv::Mat m_matDst, cv::Mat m_matSrc, bool modelflag);
	int GetTopLayer(cv::Mat* matTempl, int iMinDstLength);
	cv::Size GetBestRotationSize(cv::Size sizeSrc, cv::Size sizeDst, double dRAngle);
	cv::Point2f ptRotatePt2f(cv::Point2f ptInput, cv::Point2f ptOrg, double dAngle);
	void MatchTemplate(cv::Mat& matSrc, s_TemplData* pTemplData, cv::Mat& matResult, int iLayer, bool bUseSIMD);
	void CCOEFF_Denominator(cv::Mat& matSrc, s_TemplData* pTemplData, cv::Mat& matResult, int iLayer);
	cv::Point GetNextMaxLoc(cv::Mat& matResult, cv::Point ptMaxLoc, cv::Size sizeTemplate, double& dMaxValue, double dMaxOverlap, s_BlockMax& blockMax);
	cv::Point GetNextMaxLoc(cv::Mat& matResult, cv::Point ptMaxLoc, cv::Size sizeTemplate, double& dMaxValue, double dMaxOverlap);
	void GetRotatedROI(cv::Mat& matSrc, cv::Size size, cv::Point2f ptLT, double dAngle, cv::Mat& matROI);
	  bool SubPixEsimation(std::vector<s_MatchParameter>* vec, double* dNewX, double* dNewY, double* dNewAngle, double dAngleStep, int iMaxScoreIndex);
	void SortPtWithCenter(std::vector<cv::Point2f>& vecSort);

	void FilterWithScore(std::vector<s_MatchParameter>* vec, double dScore);
	void FilterWithRotatedRect(std::vector<s_MatchParameter>* vec, int iMethod =5, double dMaxOverLap = 0);
	void patternNG();
	double calculateInitialDistance(QPoint A, QPoint B);
	double calculateInitialDirection(QPoint A, QPoint B);
	QPoint calculateOffsetB(QPoint A, QPoint B, double initialDistance, double initialDirection, QPoint A_offset);

signals:
	void signal_newPixmap(QPixmap newPixmap, int id);
	void signal_patternResult(QPointF qpointf,int dateTime);
	void signal_modelPictureReadFlag();
	void QPointSendtoFileControl(QPoint resulePoint,int modelTotalTime);
public slots:
	void slot_recievePatternImage(QString pattern_Path, QRectF pattern_Rect, QRectF areaRect,QPoint centerPoint, QPoint patternRectCenterPoint);
	void slot_processThread_Pattren();
	void set_Grade(QString grade);
	void slot_processMatchPicture(QImage patternImage,QImage sourceImage);
	void slot_setSourceArea(bool flag);
private:
	cv::Point2d drawCenterPoint;
	bool startFlag = false;
	//判断模板图和实时图是否比例一致
	bool modelAndRealSclar = false;

	int m_threadId;
	QImage patternQImage;
	cv::Mat patternMat;
	cv::Rect areaMatRect;
	QPoint centerPointInProcess;
	QPointF resultPointF;
	QPoint patternRectCenterPointInProcess;
	bool pattern_Flag;
	bool area_Flag = false;

	//匹配结果
	cv::Point2f resultPoint;
	//矫正后的结果
	QPointF lastResult;
	double initialDistance;
	double initialDirection;
	//模板匹配相关参数
		//初始化时的参数
	s_TemplData      m_TemplData;
	s_TemplData      m_TemplData_model;

	double                      m_dToleranceAngle = 5;
	double                  m_dScore = 0.8;//得分阈值
	double                  m_iMaxPos = 2;//最多数量
	double                  m_dMaxOverlap = 0;
	int                     m_width;
	int                     m_height;
	 s_SingleTargetMatch sstm;
};
#endif // PROCESSINGTHREAD_H
