#include "myCCaliperGUI.h"
#include "common.h"


CCaliperCircleGUI::CCaliperCircleGUI()
{
	m_pdCenter = Point2d(0, 0);
	m_dRadius = 0;
	m_dMeasureLength = 0;
	m_dMeasureHeight = 0;
	m_dSigma = 1;
	m_nThreshold = 30;
	m_edgeFindWay = 1;
	m_nMeasureNums = 0;
	m_eAdjustWay = CAdjustWay::None;
	m_pdSampleLineEnd = Point2d(0, 0);
	m_nCircleSize = 0;
	m_samplingDir = 0;
}

bool CCaliperCircleGUI::judgePointInCircleFlag(Point2d pdCenter, Point2d pdPoint)
{
	return getPoint2PointLength(pdCenter, pdPoint) <= m_nCircleSize;
}


void CCaliperCircleGUI::createCircleKaChi(
	Mat& InputMat,
	Point2d pdCenter,
	double dRadius,
	double dMeasureLength,
	double dMeasureHeight,
	double dSigma,
	int nThreshold,
	int nTranslation,
	int nMesureNums,
	int nSampleDirection)
{
	if (InputMat.empty())
	{
		return;
	}
	InputMat.copyTo(m_mInputMat);
	m_pdCenter = pdCenter;
	m_dRadius = dRadius;
	m_dMeasureLength = dMeasureLength;
	m_dMeasureHeight = dMeasureHeight;
	m_dSigma = dSigma;
	m_nThreshold = nThreshold;
	m_edgeFindWay = nTranslation;
	m_nMeasureNums = nMesureNums;
	m_nCircleSize = InputMat.cols / 150;
	m_samplingDir = nSampleDirection;

	//不画圆
	//circle(InputMat, pdCenter, dRadius, Cyan);
	//circle(InputMat, m_pdCenter, m_nCircleSize, RED, 1, 16);

	// 设置m_equPointSetsVec参数
	getCircleFromEquPointSets(pdCenter, dRadius, m_nMeasureNums, m_equPointSetsVec);

	RotatedRect rRect;
	double dAngle = 0;
	m_angleVec.clear();
	Point2d pdStart, pdEnd;
	for (int i = 0; i < m_equPointSetsVec.size(); i++)
	{
		//设置投影方向
		if (m_samplingDir == 1)
		{
			dAngle = getAnglePoint2Point(pdCenter, m_equPointSetsVec[i]);
		}
		else if (m_samplingDir == 0)
		{
			dAngle = getAnglePoint2Point(m_equPointSetsVec[i], pdCenter);
		}
		//获取卡尺最外面的点  ->pdend
		getLineLastPoint(m_equPointSetsVec[i], dAngle, m_dMeasureLength, pdStart, pdEnd);
		//绘制箭头方向
		//drawMyArrow(InputMat, pdStart, pdEnd, m_nCircleSize * 5, Cyan);

		m_angleVec.push_back(dAngle);

		//rRect = RotatedRect(m_equPointSetsVec[i], Size2f(dMeasureLength, dMeasureHeight), dAngle);
		//drawMyRect(InputMat, rRect, BLUE, 1);
	}

	dAngle = m_samplingDir == 1 ? 0 : 180;
	int nIndex = 0;
	getLineLastPoint(m_equPointSetsVec[nIndex], dAngle, dMeasureLength, pdStart, m_pdSampleLineEnd);
	m_pdSampleLineCenter = m_equPointSetsVec[nIndex];
	//drawMyArrow(InputMat, pdStart, m_pdSampleLineEnd, m_nCircleSize * 5, Cyan);
	//circle(InputMat, m_pdSampleLineEnd, m_nCircleSize, RED, 1, 16);
}

void CCaliperCircleGUI::AdjustCaliper(
	//输入图片
	Mat& InputMat,
	//鼠标点
	Point2d pdPoint,
	//半径
	double dRadius,
	double dMeasureLength,
	double dMeasureHeight,
	double dSigma,
	int nThreshold,
	int searchDir,
	int nMeasureNums,
	int nSampleDirecion,
	int nFlag)
{
	if (nFlag == 1)
	{
		m_eAdjustWay = CAdjustWay::None;
		createCircleKaChi(InputMat, m_pdCenter, m_dRadius, m_dMeasureLength, m_dMeasureHeight, m_dSigma,
			m_nThreshold, m_edgeFindWay, m_nMeasureNums, m_samplingDir);
		return;
	}

	std::cout << m_pdCenter << " " << pdPoint << endl;

	if (judgePointInCircleFlag(m_pdCenter, pdPoint))
	{
		m_eAdjustWay = CAdjustWay::AdjustCenter;
	}
	else if (judgePointInCircleFlag(m_pdSampleLineEnd, pdPoint))
	{
		m_eAdjustWay = CAdjustWay::AdjustMeasureLength;
	}

	if (m_eAdjustWay == CAdjustWay::AdjustCenter)
	{
		createCircleKaChi(InputMat, pdPoint, m_dRadius, m_dMeasureLength, m_dMeasureHeight, m_dSigma,
			m_nThreshold, m_edgeFindWay, m_nMeasureNums, m_samplingDir);
	}
	else if (m_eAdjustWay == CAdjustWay::AdjustMeasureLength)
	{
		double dMeasureLength = getPoint2PointLength(pdPoint, m_pdSampleLineCenter);
		if (dMeasureLength >= m_dRadius - 2 * (double)m_nCircleSize || dMeasureLength < 1)
		{
			dMeasureLength = m_dMeasureLength;
		}
		else
		{
			dMeasureLength *= 2;
		}
		createCircleKaChi(InputMat, m_pdCenter, m_dRadius, dMeasureLength, m_dMeasureHeight, m_dSigma,
			m_nThreshold, m_edgeFindWay, m_nMeasureNums, m_samplingDir);
	}
	else if (pdPoint.x == -1 && pdPoint.y == -1)
	{
		createCircleKaChi(InputMat, m_pdCenter, m_dRadius, m_dMeasureLength, m_dMeasureHeight, m_dSigma,
			m_nThreshold, m_edgeFindWay, nMeasureNums, m_samplingDir);
	}
	else if (pdPoint.x == -2 && pdPoint.y == -2)
	{
		createCircleKaChi(InputMat, m_pdCenter, m_dRadius, dMeasureLength, m_dMeasureHeight, m_dSigma,
			m_nThreshold, m_edgeFindWay, m_nMeasureNums, m_samplingDir);
	}
	else if (pdPoint.x == -3 && pdPoint.y == -3)
	{
		createCircleKaChi(InputMat, m_pdCenter, m_dRadius, m_dMeasureLength, dMeasureHeight, m_dSigma,
			m_nThreshold, m_edgeFindWay, m_nMeasureNums, m_samplingDir);
	}
	else if (pdPoint.x == -4 && pdPoint.y == -4)
	{
		createCircleKaChi(InputMat, m_pdCenter, dRadius, m_dMeasureLength, m_dMeasureHeight, m_dSigma,
			m_nThreshold, m_edgeFindWay, m_nMeasureNums, m_samplingDir);
	}
	else if (pdPoint.x == -5 && pdPoint.y == -5)
	{
		createCircleKaChi(InputMat, m_pdCenter, m_dRadius, m_dMeasureLength, m_dMeasureHeight, m_dSigma,
			m_nThreshold, searchDir, m_nMeasureNums, m_samplingDir);
	}
	else if (pdPoint.x == -6 && pdPoint.y == -6)
	{
		createCircleKaChi(InputMat, m_pdCenter, m_dRadius, m_dMeasureLength, m_dMeasureHeight, m_dSigma,
			m_nThreshold, m_edgeFindWay, m_nMeasureNums, nSampleDirecion);
	}
	else if (pdPoint.x == -7 && pdPoint.y == -7)
	{
		createCircleKaChi(InputMat, m_pdCenter, m_dRadius, m_dMeasureLength, m_dMeasureHeight, dSigma,
			m_nThreshold, m_edgeFindWay, m_nMeasureNums, m_samplingDir);
	}
	else if (pdPoint.x == -8 && pdPoint.y == -8)
	{
		createCircleKaChi(InputMat, m_pdCenter, m_dRadius, m_dMeasureLength, m_dMeasureHeight, m_dSigma,
			nThreshold, m_edgeFindWay, m_nMeasureNums, m_samplingDir);
	}
	else
	{
		createCircleKaChi(InputMat, m_pdCenter, m_dRadius, m_dMeasureLength, m_dMeasureHeight, m_dSigma,
			m_nThreshold, m_edgeFindWay, m_nMeasureNums, m_samplingDir);
	}
}

void CCaliperCircleGUI::circleFitOpt(Point2d& pdCenter, double& dRadius, int nFitMethod)
{
	//截图圆心到 卡尺最外面的点
	Mat RoiMat = Mat::zeros(m_mInputMat.size(), m_mInputMat.type());
	Mat mask = Mat::zeros(m_mInputMat.size(), CV_8U);
	//circle(mask, m_pdCenter, m_dRadius + m_dMeasureLength * 0.5 + 10, Scalar(255), -1);
	m_mInputMat.copyTo(RoiMat, mask);
	m_edgePointSetsVec.clear();
	m_edgeGradVec.clear();
	vector<Point>temp;

	for (int i = 0; i < m_equPointSetsVec.size(); i++)
	{
		vector<CEdgePointRes>edges = extract1DEdge.getEdgePoint(RoiMat, m_equPointSetsVec[i], m_dMeasureLength, m_dMeasureHeight, m_angleVec[i]
			, m_dSigma, m_nThreshold, m_edgeFindWay == 1 ? CEdgeSearchDir::Poisitive : CEdgeSearchDir::Negative, CEdgeFindWay::Strongest);
		for (int i = 0; i < edges.size(); i++)
		{
			m_edgePointSetsVec.push_back(edges[i].m_pdEdgePoint);
			m_edgeGradVec.push_back(edges[i].m_dGradient);
			temp.push_back(edges[i].m_pdEdgePoint);
		}
	}
	if (m_edgePointSetsVec.size() == 0)
	{
		return;
	}
	circlePointSetsRansacFilter(m_edgePointSetsVec, m_expectEdgePointsetsVec);
	for (Point2d point : m_expectEdgePointsetsVec)
	{
		for (int i = 0; i < m_edgePointSetsVec.size(); i++)
		{
			if (point == m_edgePointSetsVec[i])
			{
				m_edgePointSetsVec.erase(m_edgePointSetsVec.begin() + i);
				break;
			}
		}
	}
	if (nFitMethod == 0)
	{
		leastSquareCircleFit(m_edgePointSetsVec, pdCenter, dRadius);
	}
	else if (nFitMethod == 1)
	{
		hypeCircleFit(m_edgePointSetsVec, pdCenter, dRadius);
	}

}

void CCaliperCircleGUI::edgePointSetsShow(Mat& InputMat, double dSize, Scalar color)
{
	if (InputMat.empty())
	{
		return;
	}
	for (auto edge : m_edgePointSetsVec)
	{
		drawMyCross(InputMat, edge, 90, dSize, color, 1);
	}
	for (auto edge : m_expectEdgePointsetsVec)
	{
		drawMyCross(InputMat, edge, 90, dSize,	RED, 1);
	}
}

void CCaliperCircleGUI::getEdgeSetsInfo(vector<Point2d>& edgePointSetsVec, vector<double>& edgePointsGrad)
{
	edgePointSetsVec = m_edgePointSetsVec;
	edgePointsGrad = m_edgeGradVec;
}

void CCaliperCircleGUI::hypeCircleFit(vector<Point2d> pdPoints, Point2d& pdCenter, double& dRadius)
{
	int nSize = pdPoints.size();
	if (nSize < 3)
	{
		return;
	}
	vector<double>vdX;
	vector<double>vdY;
	double dMeanX = 0, dMeanY = 0;
	for (Point2d p : pdPoints)
	{
		vdX.push_back(p.x);
		vdY.push_back(p.y);
		dMeanX += p.x;
		dMeanY += p.y;
	}
	dMeanX /= (nSize * 1.);
	dMeanY /= (nSize * 1.);

	double Xi = 0, Yi = 0, Zi = 0;
	double Mz = 0, Mxy = 0, Mxx = 0, Myy = 0, Mxz = 0, Myz = 0, Mzz = 0, Cov_xy = 0, Var_z = 0;
	double A0 = 0, A1 = 0, A2 = 0, A22 = 0;
	double Dy = 0, xnew = 0, x = 0, ynew = 0, y = 0;
	double DET = 0, Xcenter = 0, Ycenter = 0;
	for (int i = 0; i < nSize; i++)
	{
		Xi = vdX[i] - dMeanX;
		Yi = vdY[i] - dMeanY;
		Zi = Xi * Xi + Yi * Yi;

		Mxy += Xi * Yi;
		Mxx += Xi * Xi;
		Myy += Yi * Yi;
		Mxz += Xi * Zi;
		Myz += Yi * Zi;
		Mzz += Zi * Zi;
	}
	Mxx /= (nSize * 1.);
	Myy /= (nSize * 1.);
	Mxy /= (nSize * 1.);
	Mxz /= (nSize * 1.);
	Myz /= (nSize * 1.);
	Mzz /= (nSize * 1.);

	Mz = Mxx + Myy;
	Cov_xy = Mxx * Myy - Mxy * Mxy;
	Var_z = Mzz - Mz * Mz;

	A2 = 4.0 * Cov_xy - 3.0 * Mz * Mz - Mzz;
	A1 = Var_z * Mz + 4.0 * Cov_xy * Mz - Mxz * Mxz - Myz * Myz;
	A0 = Mxz * (Mxz * Myy - Myz * Mxy) + Myz * (Myz * Mxx - Mxz * Mxy) - Var_z * Cov_xy;
	A22 = A2 + A2;

	x = 0., y = A0;
	for (int i = 0; i < 99; i++)
	{
		Dy = A1 + x * (A22 + 16. * x * x);
		xnew = x - y / Dy;
		if ((xnew == x) || (!isfinite(xnew)))
		{
			break;
		}
		ynew = A0 + xnew * (A1 + xnew * (A2 + 4.0 * xnew * xnew));
		if (abs(ynew) >= abs(y))
		{
			break;
		}
		x = xnew;  y = ynew;
	}

	DET = x * x - x * Mz + Cov_xy;
	Xcenter = (Mxz * (Myy - x) - Myz * Mxy) / DET / 2.0;
	Ycenter = (Myz * (Mxx - x) - Mxz * Mxy) / DET / 2.0;
	dRadius = sqrt(Xcenter * Xcenter + Ycenter * Ycenter + Mz - x - x);
	pdCenter = Point2d(Xcenter + dMeanX, Ycenter + dMeanY);

}

void CCaliperCircleGUI::leastSquareCircleFit(vector<Point2d> pdPoints, Point2d& pdCenter, double& dRadius)
{
	int nSize = pdPoints.size();
	if (nSize < 3)
	{
		return;
	}
	double X1 = 0.0;
	double Y1 = 0.0;
	double X2 = 0.0;
	double Y2 = 0.0;
	double X3 = 0.0;
	double Y3 = 0.0;
	double X1Y1 = 0.0;
	double X1Y2 = 0.0;
	double X2Y1 = 0.0;
	for (Point2d p : pdPoints)
	{
		X1 = X1 + p.x;
		Y1 = Y1 + p.y;
		X2 = X2 + p.x * p.x;
		Y2 = Y2 + p.y * p.y;
		X3 = X3 + p.x * p.x * p.x;
		Y3 = Y3 + p.y * p.y * p.y;
		X1Y1 = X1Y1 + p.x * p.y;
		X1Y2 = X1Y2 + p.x * p.y * p.y;
		X2Y1 = X2Y1 + p.x * p.x * p.y;
	}
	double C = 0.0;
	double D = 0.0;
	double E = 0.0;
	double G = 0.0;
	double H = 0.0;
	double a = 0.0;
	double b = 0.0;
	double c = 0.0;
	C = nSize * X2 - X1 * X1;
	D = nSize * X1Y1 - X1 * Y1;
	E = nSize * X3 + nSize * X1Y2 - (X2 + Y2) * X1;
	G = nSize * Y2 - Y1 * Y1;
	H = nSize * X2Y1 + nSize * Y3 - (X2 + Y2) * Y1;
	a = (H * D - E * G) / (C * G - D * D);
	b = (H * C - E * D) / (D * D - G * C);
	c = -(a * X1 + b * Y1 + X2 + Y2) / nSize;
	double A = 0.0;
	double B = 0.0;
	double R = 0.0;
	A = a / (-2);
	B = b / (-2);
	R = double(sqrt(a * a + b * b - 4 * c) / 2);
	pdCenter.x = A;
	pdCenter.y = B;
	dRadius = R;
}

void CCaliperCircleGUI::circlePointSetsRansacFilter(const vector<Point2d>& points, vector<Point2d>& errorPoints, double sigma)
{
	unsigned int n = points.size();

	if (n < 3)
	{
		return;
	}

	RNG random;
	double bestScore = -1.;
	vector<Point2d>vpdTemp;
	int iterations = log(1 - 0.99) / (log(1 - (1.00 / n)));

	for (int k = 0; k < iterations; k++)
	{
		int i1 = 0, i2 = 0, i3 = 0;
		Point2d p1(0, 0), p2(0, 0), p3(0, 0);
		while (true)
		{
			i1 = random(n);
			i2 = random(n);
			i3 = random(n);
			if ((i1 != i2 && i1 != i3 && i2 != i3))
			{
				if ((points[i1].y != points[i2].y) && (points[i1].y != points[i3].y))
				{
					break;
				}
			}
		}
		p1 = points[i1];
		p2 = points[i2];
		p3 = points[i3];

		Point2d pdP12 = getPoint2PoinCenter(p1, p2);
		double dK1 = -1 / GetLineSlope(p1, p2);
		double dB1 = pdP12.y - dK1 * pdP12.x;
		Point2d pdP13 = getPoint2PoinCenter(p1, p3);
		double dK2 = -1 / GetLineSlope(p1, p3);
		double dB2 = pdP13.y - dK2 * pdP13.x;
		Point2d pdCenter(0, 0);
		pdCenter.x = (dB2 - dB1) / (dK1 - dK2);
		pdCenter.y = dK1 * pdCenter.x + dB1;
		double dR = getPoint2PointLength(pdCenter, p1);
		double score = 0;
		vpdTemp.clear();
		for (int i = 0; i < n; i++)
		{
			double d = dR - getPoint2PointLength(points[i], pdCenter);
			if (fabs(d) < sigma)
			{
				score += 1;
			}
			else
			{
				vpdTemp.push_back(points[i]);
			}
		}
		if (score > bestScore)
		{
			bestScore = score;
			errorPoints = vpdTemp;
		}
	}
}