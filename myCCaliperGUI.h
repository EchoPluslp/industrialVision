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

	void circleFitOpt(Point2d& pdCenter, double& dRadius, int nFitMethod = 1);

	
	void edgePointSetsShow(Mat& InputMat, double dSize, Scalar color);
	void getEdgeSetsInfo(vector<Point2d>& edgePointSetsVec, vector<double>& edgePointsGrad);

};





//线匹配

// 定义直线结构体
struct Line {
	double m; // 斜率
	double b; // 截距
};

class CLineCaliperGUI
{
protected:
	enum class CAdjustWay
	{
		None = 0,
		AdjustCenter,
		AdjustStart,
		AdjustEnd,
		AdjustMeasureLength,
		AdjustMeasureHeight,
		AdjustMeasureNums,
	};
	cv::Mat		m_mInputMat_line;
	Point2d m_pdStart;
	Point2d m_pdEnd;
	Point2d m_pdCenter;
	double	m_dAngle;
	double	m_dLength;

	Point2d m_pdSampleLineStart;
	Point2d m_pdSampleLineEnd;


	vector<Point2d>m_LineSamplingPointSetsVec;
	vector<Point2d>m_lineEdgeEquPointSetsVec;

	vector<Point2d>m_vpdEdgePoints;
	vector<Point2d>m_errortLineEdgePointSets;
	vector<double>m_vdEdgeGradient;

	double	m_dMeasureAngle;
	double	m_dMeasureLength;
	double	m_dMeasureHeight;
	double	m_dSigma;
	int		m_nThreshold;
	int		m_nTranslation;
	int		m_nMeasureNums;
private:
	int				m_nCircleSize;
	bool			IsPointInCircle(Point2d pdCenter, Point2d pdPoint);
	CAdjustWay		m_eAdjustWay;
	CLineEdgePoint	lineEdgePointCalc;
	void RansacLineFiler(
		const vector<Point2d>& points,
		vector<Point2d>& vpdExceptPoints,
		double sigma = 1);
public:
	CLineCaliperGUI();
	void createLineKaChi(Mat& InputMat,
		Point2d pdStart,
		Point2d pdEnd,
		double dMeasureLength,
		double dMeasureHeight,
		double dSigma,
		int nThreshold,
		int nTranslation,
		int nMesureNums);

	void adjustLineKaChi(Mat& InputMat,
		Point2d pdPoint,
		int nMeasureLength,
		int nMeasureHeight,
		double dSigma,
		int nThreshold,
		int nTranslation,
		int nMeasureNums,
		int nFlag = 0);

	void drawMyArrow(Mat& inputMat, Point2d p1, Point2d p2, int dSize, Scalar color, int nThickness = 1);

	void drawMyCross(Mat& inputMat, Point2d p, double dAngle, double dSize, Scalar color, int nThickness = 1);

	void drawMyRect(Mat& inputMat, RotatedRect rRect, Scalar color, int nThickness = 1);


	double getPoint2PointLength(Point2d p1, Point2d p2);

	Point2d GetPPCenter(Point2d p1, Point2d p2);

	double getLineK(Point2d p1, Point2d p2);
	void getLineEquPointSets(Point2d pdStart, Point2d pdEnd, int nEquinoxNums, vector<Point2d>& vpdEquinoxPoints);
	void getLineLastPoint(Point2d pdCenter, double dAngle, double dLength, Point2d& pdStart, Point2d& pdEnd);

	Point2d getLineIntersections(Point2d p1, Point2d p2, Point2d p11, Point2d p22);


	double getAnglePoint2Point(Point2d p1, Point2d p2);

	void lineEdgePointSetsFit(Point2d& pdStart, Point2d& pdEnd, double& dAngle);
	void lineEdgePointSetsShow(Mat& InputMat, double dSize, Scalar color);
	void getLineEdgePointSetsInfo(vector<Point2d>& vpdEdgePoints,
		vector<double>& vpdEdgePointsGradient);

	vector<Point2d> getEdgePoints();
	vector<Point2d> getExcepetEdgePoints();
	vector<cv::Point2f> get_intersection(cv::Point2f pt1, cv::Point2f pt2, cv::Point2f begin, cv::Point2f end);

	//找到夹角
	double findangle(cv::Point p_1, cv::Point p_2, cv::Point p_3, cv::Point p_4);

	//找到交点
	void findIntersection(cv::Point p_1, cv::Point p_2, cv::Point p_3, cv::Point p_4, cv::Point2f& intersection);
	
	Line calculateLine(const cv::Point& p1, const cv::Point& p2);

};

