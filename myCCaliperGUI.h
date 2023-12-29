#pragma once

#include<opencv2/opencv.hpp>

#include <iostream>
#include <vector>
#include <list>
#include "edge.h"


using namespace std;
using namespace cv;

class CCaliperCircleGUI
{
protected:
	enum class CAdjustWay
	{
		None = 0,
		AdjustCenter,
		AdjustRadius,
		AdjustMeasureLength,
		AdjustMeasureHeight,
		AdjustMeasureNums,
	};
	Mat		m_mInputMat;
	Point2d m_pdCenter;
	double	m_dRadius;
	double	m_dMeasureLength;
	double	m_dMeasureHeight;
	double	m_dSigma;
	int		m_nThreshold;
	int		m_edgeFindWay;
	int		m_nMeasureNums;
	int		m_samplingDir;


protected:
	vector<Point2d>m_equPointSetsVec;
	vector<double>m_angleVec;
	vector<Point2d>m_edgePointSetsVec;
	vector<Point2d>m_expectEdgePointsetsVec;
	vector<double>m_edgeGradVec;

	Point2d m_pdSampleLineEnd;
	Point2d m_pdSampleLineCenter;

	CAdjustWay m_eAdjustWay;
private:
	bool judgePointInCircleFlag(Point2d pdCenter, Point2d pdPoint);
	int m_nCircleSize = 0;

	CEdgePoint	extract1DEdge;
	void hypeCircleFit(
		vector<Point2d>pdPoints,
		Point2d& pdCenter,
		double& dRadius);

	void leastSquareCircleFit(
		vector<Point2d>pdPoints,
		Point2d& pdCenter,
		double& dRadius);

	void circlePointSetsRansacFilter(const vector<Point2d>& points,
		vector<Point2d>& errorPoints,
		double sigma = 10);
public:
	CCaliperCircleGUI();
	void createCircleKaChi(Mat& InputMat,
		Point2d pdCenter,
		double dRadius,
		double dMeasureLength,
		double dMeasureHeight,
		double dSigma,
		int nThreshold,
		int nTranslation,
		int nMesureNums,
		int nSampleDirection);

	void AdjustCaliper(Mat& InputMat,
		Point2d pdPoint,
		double dRadius,
		double dMeasureLength,
		double dMeasureHeight,
		double dSigma, int nThreshold,
		int nTranslation,
		int nMeasureNums,
		int nSampleDirection,
		int nFlag = 0);

	void circleFitOpt(Point2d& pdCenter, double& dRadius, int nFitMethod = 0);
	void edgePointSetsShow(Mat& InputMat, double dSize, Scalar color);
	void getEdgeSetsInfo(vector<Point2d>& edgePointSetsVec, vector<double>& edgePointsGrad);
};

