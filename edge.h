#pragma once
#include<opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;



enum class CLineFineDir
{
	All = 0,
	Negative,
	Poisitive
};

enum class CEdgeSearchDir
{
	All = 0,
	Negative,
	Poisitive
};

enum class CEdgeFindWay
{
	All = 0,
	Fisrt,
	Last,
	Strongest,
	weakest,
};

struct CEdgePointRes
{
	CEdgePointRes(Point2d edgePoint, double gradient) :
		m_pdEdgePoint(edgePoint), m_dGradient(gradient)
	{

	}
	Point2d m_pdEdgePoint;
	double m_dGradient;
};

class CEdgePoint
{
public:
	CEdgePoint();

private:
	Mat		m_mInputMat;
	Mat		m_mProfieMat;
	Point2d m_pdCenter;
	double	m_dSigma;
	int		m_nThreshold;
	double	m_dAngle;//deg
	double	m_dLength;
	double	m_dHeight;
	double	m_dK;
	double	m_dB;
	vector<Point2d>m_vpCandidate;//x: xedg; y: grandient
	vector<CEdgePointRes>m_vEdgesResult;
	Point2d m_pdStart;
	Point2d m_pdEnd;
	void	getWarpImageMat();
	void	imagePreGussianBlur();
	void	getImageGrad();
	void	getEdgePointSets(int threshold,
		CEdgeSearchDir traslation = (CEdgeSearchDir)0,
		CEdgeFindWay selection = (CEdgeFindWay)0);

public:
	vector<CEdgePointRes> getEdgePoint(
		Mat inputMat,
		Point2d pdCenter,
		double dMeasureLength,
		double dMeasureHeight,
		double dMeasureAngle,
		double sigma,
		int threshold,
		CEdgeSearchDir traslation = (CEdgeSearchDir)0,
		CEdgeFindWay selection = (CEdgeFindWay)0);


};


//ÏßµÄÆ¥Åä
class CLineEdgePoint
{
public:
	CLineEdgePoint();

private:
	Mat		m_mInputMat;
	Mat		m_mProfieMat;
	Point2d m_pdCenter;
	double	m_dSigma;
	int		m_nThreshold;
	double	m_dAngle;
	double	m_dLength;
	double	m_dHeight;
	double	m_dK;
	double	m_dB;

private:

	vector<Point2d>m_candiPointSetsVec;
	vector<CEdgePointRes>m_lineEdgePointSetsVec;
	Point2d m_pdStart;
	Point2d m_pdEnd;
	void	getLineRoiImage();
	void	imageGaussBlur();
	void	getLineImageGrad();
	void	getLineEdgePoint(int threshold,
		CLineFineDir dir = (CLineFineDir)0,
		CEdgeFindWay way = (CEdgeFindWay)0);

	double getLineK(Point2d p1, Point2d p2);
	void getLineLastPoint(Point2d pdCenter, double dAngle,
		double dLength, Point2d& pdStart, Point2d& pdEnd);

public:
	vector<CEdgePointRes> getLineEdgePoints(Mat inputMat,
		Point2d pdCenter,
		double dMeasureLength,
		double dMeasureHeight,
		double dMeasureAngle,
		double sigma,
		int threshold,
		CLineFineDir dir = (CLineFineDir)0,
		CEdgeFindWay way = (CEdgeFindWay)0);


};

