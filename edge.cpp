#include "edge.h"
#include "common.h"


CEdgePoint::CEdgePoint()
{
	m_pdCenter = Point2d(0, 0);
	m_dAngle = 0;
	m_dK = 0;
	m_dB = 0;
	m_dLength = 0;
	m_dHeight = 0;
	m_dSigma = 0;
	m_nThreshold = 0;
}


void CEdgePoint::getWarpImageMat()
{
	if (m_mInputMat.empty())
	{
		return;
	}
	if (m_mInputMat.channels() > 1)
	{
		cvtColor(m_mInputMat, m_mInputMat, COLOR_BGR2GRAY);
	}

	//Get ROI mat.
	RotatedRect rMaskRegion(m_pdCenter, Size2f(getPoint2PointLength(m_pdStart, m_pdEnd) + 10, m_dLength + 10), m_dAngle);
	Point2f rRegionPoints[4];
	rMaskRegion.points(rRegionPoints);
	Mat mask = Mat::zeros(m_mInputMat.size(), CV_8UC1);
	Point ppt[] = { rRegionPoints[0], rRegionPoints[1], rRegionPoints[2], rRegionPoints[3] };
	const Point* pts[] = { ppt };
	int npt[] = { 4 };
	fillPoly(mask, pts, npt, 1, Scalar::all(255), 8);
	Mat RoiMat;
	bitwise_and(m_mInputMat, m_mInputMat, RoiMat, mask);

	Mat RotateMat = getRotationMatrix2D(m_pdCenter, -m_dAngle, 1);

	cv::warpAffine(RoiMat, RoiMat, RotateMat, m_mInputMat.size(), WARP_INVERSE_MAP | INTER_CUBIC);
	Mat newCenter = RotateMat * (Mat_<double>(3, 1) << m_pdCenter.x, m_pdCenter.y, 1);
	double x = newCenter.at<double>(0, 0);
	double y = newCenter.at<double>(1, 0);

	Mat M = (Mat_<double>(2, 3) << 1, 0, x - m_dLength * 0.5, 0, 1, y - m_dHeight * 0.5);
	cv::warpAffine(RoiMat, m_mInputMat, M, Size2d(m_dLength, m_dHeight), WARP_INVERSE_MAP | INTER_CUBIC);


}

void CEdgePoint::imagePreGussianBlur()
{
	if (m_mInputMat.empty())
	{
		return;
	}
	if (m_mInputMat.channels() > 1)
	{
		cvtColor(m_mInputMat, m_mInputMat, COLOR_BGR2GRAY);
	}
	GaussianBlur(m_mInputMat, m_mInputMat, Size(1, 3), m_dSigma);
}

void CEdgePoint::getImageGrad()
{
	if (m_mInputMat.empty())
	{
		return;
	}
	if (m_mInputMat.channels() > 1)
	{
		cvtColor(m_mInputMat, m_mInputMat, COLOR_BGR2GRAY);
	}
	reduce(m_mInputMat, m_mInputMat, 0, REDUCE_AVG, CV_64FC1);
	Sobel(m_mInputMat, m_mInputMat, CV_64FC1, 1, 0, 1);

	m_mInputMat = m_dSigma * sqrt(2 * PI) * m_mInputMat;
}

void CEdgePoint::getEdgePointSets(int threshold, CEdgeSearchDir traslation, CEdgeFindWay selection)
{
	if (m_mInputMat.empty())
	{
		return;
	}
	if (m_mInputMat.channels() > 1)
	{
		cvtColor(m_mInputMat, m_mInputMat, COLOR_BGR2GRAY);
	}

	double* ptr = m_mInputMat.ptr<double>(0);
	m_vpCandidate.clear();
	m_vEdgesResult.clear();
	//The theshold condition is met
	for (int i = 0; i < m_mInputMat.cols; i++)
	{
		double dGradient = abs(ptr[i]);
		if (dGradient >= threshold)
		{
			m_vpCandidate.push_back(Point2d(i, ptr[i]));
		}
	}
	if (m_vpCandidate.size() == 0)
	{
		return;
	}

	if (traslation == CEdgeSearchDir::Poisitive)
	{
		for (vector<Point2d>::iterator iter = m_vpCandidate.begin(); iter != m_vpCandidate.end();)
		{
			if ((*iter).y <= 0)
			{
				iter = m_vpCandidate.erase(iter);
			}
			else
			{
				iter++;
			}
		}
	}
	else if (traslation == CEdgeSearchDir::Negative)
	{
		for (vector<Point2d>::iterator iter = m_vpCandidate.begin(); iter != m_vpCandidate.end();)
		{
			if ((*iter).y > 0)
			{
				iter = m_vpCandidate.erase(iter);
			}
			else
			{
				iter++;
			}
		}
	}
	if (m_vpCandidate.size() == 0)
	{
		return;
	}

	if (selection == CEdgeFindWay::Fisrt)
	{
		m_vpCandidate.erase(m_vpCandidate.begin() + 1, m_vpCandidate.end());
	}
	else if (selection == CEdgeFindWay::Last)
	{
		m_vpCandidate.erase(m_vpCandidate.begin(), m_vpCandidate.end() - 1);
	}
	else if (selection == CEdgeFindWay::Strongest)
	{
		Point2d pdMax(0, 0);
		double dGradientMax = 0;
		for (Point2d item : m_vpCandidate)
		{
			if (abs(item.y) >= dGradientMax)
			{
				pdMax = item;
				dGradientMax = abs(item.y);
			}
		}
		m_vpCandidate.clear();
		m_vpCandidate.push_back(pdMax);
	}
	else if (selection == CEdgeFindWay::weakest)
	{
		Point2d pdMin(0, 99999999);
		for (Point2d item : m_vpCandidate)
		{
			if (abs(item.y) <= pdMin.y)
			{
				pdMin.y = abs(item.y);
				pdMin.x = item.x;
			}
		}
		m_vpCandidate.clear();
		m_vpCandidate.push_back(pdMin);
	}

	double dEdgex = 0, dEdgey = 0;
	for (Point2d item : m_vpCandidate)
	{
		if (isinf(m_dK))
		{
			dEdgex = m_pdStart.x;
			dEdgey = m_pdStart.y + sin(to_radian(m_dAngle)) * item.x;
		}
		else
		{
			dEdgex = m_pdStart.x + item.x * cos(to_radian(m_dAngle));
			dEdgey = m_dK * dEdgex + m_dB;
		}
		m_vEdgesResult.push_back(CEdgePointRes(Point2d(dEdgex, dEdgey), item.y));
	}
}

vector<CEdgePointRes> CEdgePoint::getEdgePoint(Mat inputMat,
	Point2d pdCenter, double dMeasureLength,
	double dMeasureHeight, double dMeasureAngle,
	double sigma, int threshold,
	CEdgeSearchDir traslation, CEdgeFindWay selection)
{
	if (inputMat.empty())
	{
		return vector<CEdgePointRes>();
	}
	if (inputMat.channels() > 1)
	{
		cvtColor(inputMat, inputMat, COLOR_BGR2GRAY);
	}
	m_vEdgesResult.clear();
	inputMat.copyTo(m_mInputMat);
	m_pdCenter = pdCenter;
	m_dLength = dMeasureLength;
	m_dHeight = dMeasureHeight;
	m_dAngle = dMeasureAngle;
	m_dSigma = sigma;
	m_nThreshold = threshold * (m_dSigma * sqrt(2 * PI));
	getLineLastPoint(pdCenter, dMeasureAngle, dMeasureLength, m_pdStart, m_pdEnd);
	m_dK = GetLineSlope(m_pdStart, m_pdEnd);
	m_dB = m_pdStart.y - m_dK * m_pdStart.x;

	getWarpImageMat();
	imagePreGussianBlur();
	getImageGrad();
	getEdgePointSets(threshold, traslation, selection);

	return m_vEdgesResult;
}



