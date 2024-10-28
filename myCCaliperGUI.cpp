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
	circle(InputMat, pdCenter, dRadius, Cyan);
	circle(InputMat, m_pdCenter, m_nCircleSize, RED, 1, 16);

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
	circle(InputMat, m_pdSampleLineEnd, m_nCircleSize, RED, 1, 16);
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
		createCircleKaChi(InputMat, pdPoint, m_dRadius, m_dMeasureLength, m_dMeasureHeight, m_dSigma,
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
	int m_dRadiusInt = m_dRadius;
	circle(mask, m_pdCenter, m_dRadius + m_dMeasureLength * 0.5 + 10, Scalar(255), -1);
	//todo 圆形坐标不变
	Mat etsf = m_mInputMat;
	m_mInputMat.copyTo(RoiMat, mask);
	m_edgePointSetsVec.clear();
	m_edgeGradVec.clear();
	vector<Point>temp;
	for (int i = 0; i < m_equPointSetsVec.size(); i++)
	{ 
		//获取边缘点
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
		//圆拟合Taubin fit 方法

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


//线匹配====================================================

void CLineCaliperGUI::RansacLineFiler(const vector<Point2d>& points, vector<Point2d>& errPoints, double sigma)
{
	unsigned int n = points.size();

	if (n < 2)
	{
		return;
	}

	RNG random;
	double bestScore = -1.;
	vector<Point2d>tempVec;
	int iterations = log(1 - 0.99) / (log(1 - (1.00 / n))) * 10;

	for (int k = 0; k < iterations; k++)
	{
		int i1 = 0, i2 = 0;
		while (i1 == i2)
		{
			i1 = random(n);
			i2 = random(n);
		}
		const cv::Point2d& p1 = points[i1];
		const cv::Point2d& p2 = points[i2];
		Point2d vectorP21 = p2 - p1;
		vectorP21 *= 1. / norm(vectorP21);
		double score = 0;
		tempVec.clear();
		for (size_t i = 0; i < n; i++)
		{
			Point2d vectorPi1 = points[i] - p1;
			double d = vectorPi1.y * vectorP21.x - vectorPi1.x * vectorP21.y;
			if (fabs(d) < sigma)
			{
				score += 1;
			}
			else
			{
				tempVec.push_back(points[i]);
			}
		}
		if (score > bestScore)
		{
			bestScore = score;
			errPoints = tempVec;
		}
	}
}

CLineCaliperGUI::CLineCaliperGUI()
{
	m_pdStart = Point2d(0, 0);
	m_pdEnd = Point2d(0, 0);
	m_pdCenter = Point2d(0, 0);
	m_dAngle = 0;
	m_dLength = 0;
	m_dMeasureLength = 0;
	m_dSigma = 1;
	m_nThreshold = 30;
	m_nTranslation = 1;
	m_nMeasureNums = 1;
	m_LineSamplingPointSetsVec = { Point2d(0,0),Point2d(0,0) };
	m_pdSampleLineStart = { Point2d(-100,-100) };
	m_pdSampleLineEnd = { Point2d(-100,-100) };
	m_nCircleSize = 0;
	m_eAdjustWay = CAdjustWay::None;
}

bool CLineCaliperGUI::IsPointInCircle(Point2d pdCenter, Point2d pdPoint)
{
	return  getPoint2PointLength(pdCenter, pdPoint) <= m_nCircleSize;
}

void CLineCaliperGUI::drawMyCross(Mat& inputMat, Point2d p, double dAngle, double dSize, Scalar color, int nThickness)
{
	if (inputMat.empty())
	{
		return;
	}
	RotatedRect rotateRect(p, Size2d(dSize, dSize), dAngle);
	Point2f rectPoints[4];
	rotateRect.points(rectPoints);
	line(inputMat, rectPoints[0], rectPoints[2], color, nThickness, LINE_AA);
	line(inputMat, rectPoints[1], rectPoints[3], color, nThickness, LINE_AA);
}

void CLineCaliperGUI::drawMyArrow(Mat& inputMat, Point2d p1, Point2d p2, int dSize, Scalar color, int nThickness)
{
	if (inputMat.empty())
	{
		return;
	}
	double dK = ((double)p2.y - (double)p1.y) / ((double)p2.x - (double)p1.x);
	double dAngle = atan(dK) * 180 / PI;
	line(inputMat, p1, p2, color, nThickness, LINE_AA);
	RotatedRect rotateRect(p2, Size(dSize, dSize * 0.5), dAngle);
	Point2f rectPoints[4];
	rotateRect.points(rectPoints);
	if ((dAngle >= 0 && p1.x <= p2.x) || (dAngle < 0 && p1.x <= p2.x))
	{
		line(inputMat, p2, rectPoints[0], color, nThickness, LINE_AA);
		line(inputMat, p2, rectPoints[1], color, nThickness, LINE_AA);
	}
	else
	{
		line(inputMat, p2, rectPoints[2], color, nThickness, LINE_AA);
		line(inputMat, p2, rectPoints[3], color, nThickness, LINE_AA);
	}
}

void CLineCaliperGUI::createLineKaChi(Mat& InputMat,
	Point2d pdStart,
	Point2d pdEnd,
	double dMeasureLength,
	double dMeasureHeight,
	double dSigma,
	int nThreshold,
	int nTranslation,
	int nMesureNums)
{
	if (InputMat.empty())
	{
		return;
	}
	InputMat.copyTo(m_mInputMat_line);
	m_pdStart = pdStart;
	m_pdEnd = pdEnd;
	m_pdCenter = GetPPCenter(pdStart, pdEnd);
	m_dAngle = getAnglePoint2Point(pdStart, pdEnd);
	m_dLength = getPoint2PointLength(pdStart, pdEnd);
	m_dMeasureLength = dMeasureLength;
	m_dMeasureAngle = m_dAngle - 90;
	m_dMeasureHeight = dMeasureHeight;
	m_dSigma = dSigma;
	m_nThreshold = nThreshold;
	m_nTranslation = nTranslation;
	m_nMeasureNums = nMesureNums;
	m_nCircleSize = InputMat.cols / 150;

	drawMyArrow(InputMat, pdStart, pdEnd, (int)m_nCircleSize * 5, Cyan, 1);
	circle(InputMat, pdStart, m_nCircleSize, Yellow, 1, 16);
	circle(InputMat, pdEnd, m_nCircleSize, Yellow, 1, 16);
	circle(InputMat, m_pdCenter, m_nCircleSize, Yellow, 1, 16);


	getLineLastPoint(m_pdCenter, m_dAngle - 90, dMeasureLength + 100, m_pdSampleLineStart, m_pdSampleLineEnd);
	drawMyArrow(InputMat, m_pdSampleLineStart, m_pdSampleLineEnd, (int)m_nCircleSize * 5, Cyan, 1);
	circle(InputMat, m_pdSampleLineEnd, m_nCircleSize, Yellow, 1, 16);

	getLineEquPointSets(m_pdStart, m_pdEnd, nMesureNums + 1, m_lineEdgeEquPointSetsVec);
	RotatedRect rRect;
	Point2d p1, p2;
	for (size_t i = 0; i < m_lineEdgeEquPointSetsVec.size(); i++)
	{
		getLineLastPoint(m_lineEdgeEquPointSetsVec[i], m_dAngle - 90, dMeasureLength, p1, p2);
		drawMyArrow(InputMat, p1, p2, (int)m_nCircleSize * 5, Cyan, 1);
		rRect = RotatedRect(m_lineEdgeEquPointSetsVec[i], Size2f(dMeasureLength, dMeasureHeight), m_dAngle - 90);
		drawMyRect(InputMat, rRect, Scalar(255, 0, 0), 1);
	}
}

void CLineCaliperGUI::getLineEquPointSets(Point2d pdStart, Point2d pdEnd, int nEquinoxNums, vector<Point2d>& vpdEquinoxPoints)
{
	if (nEquinoxNums == 0)
	{
		return;
	}
	vpdEquinoxPoints.clear();
	double dXoffset = (pdEnd.x - pdStart.x) / nEquinoxNums;
	double dYoffset = (pdEnd.y - pdStart.y) / nEquinoxNums;
	for (int i = 0; i < nEquinoxNums - 1; i++)
	{
		double dx = dXoffset * ((double)i + 1);
		double dy = dYoffset * ((double)i + 1);
		vpdEquinoxPoints.push_back(Point2d(pdStart.x + dx, pdStart.y + dy));
	}
}

void CLineCaliperGUI::getLineLastPoint(Point2d pdCenter, double dAngle, double dLength, Point2d& pdStart, Point2d& pdEnd)
{
	pdStart.x = pdCenter.x - cos(to_radian(dAngle)) * 0.5 * dLength;
	pdStart.y = pdCenter.y - sin(to_radian(dAngle)) * 0.5 * dLength;
	pdEnd = Point2d(pdCenter.x * 2 - pdStart.x, pdCenter.y * 2 - pdStart.y);
}

Point2d CLineCaliperGUI::getLineIntersections(Point2d p1, Point2d p2, Point2d p11, Point2d p22)
{
	double dK1 = (p1.y - p2.y) / (p1.x - p2.x);
	double dK2 = (p11.y - p22.y) / (p11.x - p22.x);

	if (dK1 == dK2)//parallel
	{
		return Point2d(-1, -1);
	}
	Point2d pdIntersection(0, 0);
	if (isinf(dK1) || isnan(dK1))//paralle to the y-axis
	{
		pdIntersection.x = p1.x;
		double dB2 = p11.y - dK2 * p11.x;
		pdIntersection.y = dK2 * pdIntersection.x + dB2;
	}
	else if (isinf(dK2) || isnan(dK2))
	{
		pdIntersection.x = p11.x;
		double dB1 = p1.y - dK1 * p1.x;
		pdIntersection.y = dK1 * pdIntersection.x + dB1;
	}
	else
	{
		double dB1 = p1.y - dK1 * p1.x;
		double dB2 = p11.y - dK2 * p11.x;
		pdIntersection.x = (dB2 - dB1) / (dK1 - dK2);
		pdIntersection.y = dK1 * pdIntersection.x + dB1;
	}
	return pdIntersection;
}

double CLineCaliperGUI::getAnglePoint2Point(Point2d p1, Point2d p2)
{
	if (p1 == p2)
	{
		return -1;
	}
	Point2d vector = p2 - p1;
	if (vector.x == 0)
	{
		if (vector.y > 0)
		{
			return 90;
		}
		else
		{
			return -90;
		}
	}
	double angle = to_degree(acos(pow(vector.x, 2) / (vector.x * sqrt(pow(vector.x, 2) + pow(vector.y, 2)))));
	if (p1.y > p2.y)
	{
		angle = -angle;
	}


	return  angle;
}


double CLineCaliperGUI::getPoint2PointLength(Point2d p1, Point2d p2)
{
	return sqrt(pow((p1.x - p2.x), 2) + pow((p1.y - p2.y), 2));
}

Point2d CLineCaliperGUI::GetPPCenter(Point2d p1, Point2d p2)
{
	return Point2d((p1.x + p2.x) * 0.5, (p1.y + p2.y) * 0.5);
}

double CLineCaliperGUI::getLineK(Point2d p1, Point2d p2)
{
	return (p1.y - p2.y) / (p1.x - p2.x);
}

void CLineCaliperGUI::drawMyRect(Mat& inputMat, RotatedRect rRect, Scalar color, int nThickness)
{
	Point2f pfRectPoints[4];
	rRect.points(pfRectPoints);
	for (int i = 0; i < 4; i++)
	{
		line(inputMat, pfRectPoints[i], pfRectPoints[(i + 1) % 4], color, nThickness, LINE_AA);
	}
}

//无调用
void CLineCaliperGUI::adjustLineKaChi(Mat& InputMat, Point2d pdPoint, int nMeasureLength, int nMeasureHeight, double dSigma, int nThreshold, int nTranslation, int nMeasureNums, int nFlag)
{
	if (nFlag == 1)
	{
		m_eAdjustWay = CAdjustWay::None;
		createLineKaChi(InputMat, m_pdStart, m_pdEnd, m_dMeasureLength, m_dMeasureHeight, dSigma, nThreshold, nTranslation, m_nMeasureNums);
		return;
	}
	if (IsPointInCircle(m_pdStart, pdPoint))
	{
		m_eAdjustWay = CAdjustWay::AdjustStart;
	}
	else if (IsPointInCircle(m_pdEnd, pdPoint))
	{
		m_eAdjustWay = CAdjustWay::AdjustEnd;
	}
	else if (IsPointInCircle(m_pdCenter, pdPoint))
	{
		m_eAdjustWay = CAdjustWay::AdjustCenter;
	}
	else if (IsPointInCircle(m_pdSampleLineEnd, pdPoint))
	{
		m_eAdjustWay = CAdjustWay::AdjustMeasureLength;
	}
	else if (pdPoint.x == -1 && pdPoint.y == -1)
	{
		createLineKaChi(InputMat, m_pdStart, m_pdEnd, m_dMeasureLength, m_dMeasureHeight, dSigma, nThreshold, nTranslation, nMeasureNums);
	}
	else if (pdPoint.x == -2 && pdPoint.y == -2)
	{
		createLineKaChi(InputMat, m_pdStart, m_pdEnd, nMeasureLength, m_dMeasureHeight, dSigma, nThreshold, nTranslation, m_nMeasureNums);
	}
	else if (pdPoint.x == -3 && pdPoint.y == -3)
	{
		createLineKaChi(InputMat, m_pdStart, m_pdEnd, m_dMeasureLength, nMeasureHeight, dSigma, nThreshold, nTranslation, m_nMeasureNums);
	}
	else
	{
		createLineKaChi(InputMat, m_pdStart, m_pdEnd, m_dMeasureLength, m_dMeasureHeight, dSigma, nThreshold, nTranslation, m_nMeasureNums);
	}

	if (m_eAdjustWay == CAdjustWay::AdjustStart)
	{
		createLineKaChi(InputMat, pdPoint, m_pdEnd, m_dMeasureLength, m_dMeasureHeight, dSigma, nThreshold, nTranslation, m_nMeasureNums);
	}
	else if (m_eAdjustWay == CAdjustWay::AdjustEnd)
	{
		createLineKaChi(InputMat, m_pdStart, pdPoint, m_dMeasureLength, m_dMeasureHeight, dSigma, nThreshold, nTranslation, m_nMeasureNums);
	}
	else if (m_eAdjustWay == CAdjustWay::AdjustCenter)
	{
		getLineLastPoint(pdPoint, m_dAngle, m_dLength, m_pdStart, m_pdEnd);
		createLineKaChi(InputMat, m_pdStart, m_pdEnd, m_dMeasureLength, m_dMeasureHeight, dSigma, nThreshold, nTranslation, m_nMeasureNums);
	}
	else if (m_eAdjustWay == CAdjustWay::AdjustMeasureLength)
	{
		double dMeasureLength = getPoint2PointLength(m_pdCenter, pdPoint) * 2 - 100;

		if (dMeasureLength < 1)
		{
			dMeasureLength = m_dMeasureLength;
		}
		(InputMat, m_pdStart, m_pdEnd, dMeasureLength, m_dMeasureHeight, dSigma, nThreshold, nTranslation, m_nMeasureNums);
	}
}

void CLineCaliperGUI::lineEdgePointSetsFit(Point2d& pdStart, Point2d& pdEnd, double& dAngle)
{
	RotatedRect rMaskRegion(m_pdCenter, Size2f(getPoint2PointLength(m_pdStart, m_pdEnd) + 10, m_dMeasureLength + 10), m_dAngle);

	Point2f rRegionPoints[4];
	rMaskRegion.points(rRegionPoints);
	Mat mask = Mat::zeros(m_mInputMat_line.size(), CV_8UC1);
	Point ppt[] = { rRegionPoints[0], rRegionPoints[1], rRegionPoints[2], rRegionPoints[3] };
	const Point* pts[] = { ppt };
	int npt[] = { 4 };
	fillPoly(mask, pts, npt, 1, Scalar::all(255), 8);
	Mat RoiMat;
	bitwise_and(m_mInputMat_line, m_mInputMat_line, RoiMat, mask);

	m_vpdEdgePoints.clear();
	m_vdEdgeGradient.clear();
	m_errortLineEdgePointSets.clear();
	for (size_t i = 0; i < m_lineEdgeEquPointSetsVec.size(); i++)
	{
		vector<CEdgePointRes>edges = lineEdgePointCalc.getLineEdgePoints(RoiMat, m_lineEdgeEquPointSetsVec[i], m_dMeasureLength, m_dMeasureHeight, m_dAngle - 90
			, m_dSigma, m_nThreshold, m_nTranslation == 1 ? CLineFineDir::Poisitive : CLineFineDir::Negative, CEdgeFindWay::Strongest);
		for (size_t i = 0; i < edges.size(); i++)
		{
			m_vpdEdgePoints.push_back(edges[i].m_pdEdgePoint);
			m_vdEdgeGradient.push_back(edges[i].m_dGradient);
		}
	}
	Vec4f lines;
	int nSize = m_vpdEdgePoints.size() - 1;
	pdStart = Point2d(-1, -1);
	pdEnd = Point2d(-1, -1);
	if (nSize >= 0)
	{
		RansacLineFiler(m_vpdEdgePoints, m_errortLineEdgePointSets);
		for (Point2d point : m_errortLineEdgePointSets)
		{
			for (int i = 0; i < nSize + 1; i++)
			{
				if (point == m_vpdEdgePoints[i])
				{
					m_vpdEdgePoints.erase(m_vpdEdgePoints.begin() + i);
					break;
				}
			}
		}
		nSize = m_vpdEdgePoints.size() - 1;

		fitLine(m_vpdEdgePoints, lines, DIST_HUBER, 0, 0.01, 0.01);
		double dK = lines[1] / lines[0];


		Point2d pdfitLineP1 = Point2d(m_vpdEdgePoints[0].x, dK * (m_vpdEdgePoints[0].x - lines[2]) + lines[3]);
		Point2d pdfitLineP2 = Point2d(m_vpdEdgePoints[nSize].x, dK * (m_vpdEdgePoints[nSize].x - lines[2]) + lines[3]);
		Point2d pdP11(0, 0), pdP22(0, 0);

		getLineLastPoint(m_vpdEdgePoints[0], m_dAngle - 90, m_dMeasureLength, pdP11, pdP22);
		pdStart = getLineIntersections(pdfitLineP1, pdfitLineP2, pdP11, pdP22);

		getLineLastPoint(m_vpdEdgePoints[nSize], m_dAngle - 90, m_dMeasureLength, pdP11, pdP22);
		pdEnd = getLineIntersections(pdfitLineP1, pdfitLineP2, pdP11, pdP22);

		dAngle = getAnglePoint2Point(pdStart, pdEnd);
	}


}

void CLineCaliperGUI::lineEdgePointSetsShow(Mat& InputMat, double dSize, Scalar color)
{
	if (InputMat.empty())
	{
		return;
	}
	for (auto edge : m_vpdEdgePoints)
	{
		drawMyCross(InputMat, edge, 90, dSize, color, 1);
	}
	for (auto edge : m_errortLineEdgePointSets)
	{
		drawMyCross(InputMat, edge, 90, dSize, Scalar(0,0,255), 1);
	}
}

void CLineCaliperGUI::getLineEdgePointSetsInfo(vector<Point2d>& edgePointSets, vector<double>& edgePointSetGrad)
{
	edgePointSets = m_vpdEdgePoints;
	edgePointSetGrad = m_vdEdgeGradient;
}




vector<Point2d> CLineCaliperGUI::getEdgePoints()
{
	return m_vpdEdgePoints;
}

vector<Point2d> CLineCaliperGUI::getExcepetEdgePoints()
{
	return m_errortLineEdgePointSets;
}

double CLineCaliperGUI::findangle(cv::Point p_1, cv::Point p_2, cv::Point p_3, cv::Point p_4)
{
	
		// 计算两条线的向量
		cv::Point vec1 = p_1 - p_2;
		cv::Point vec2 = p_3 - p_4;

		// 计算向量的模
		double magnitude1 = cv::norm(vec1);
		double magnitude2 = cv::norm(vec2);

		// 计算向量的点积
		double dotProduct = vec1.x * vec2.x + vec1.y * vec2.y;

		// 计算余弦值
		double cosTheta = dotProduct / (magnitude1 * magnitude2);

		// 使用反余弦函数计算夹角（弧度）
		double angleRad = std::acos(std::max(-1.0, std::min(1.0, cosTheta)));

		// 将弧度转换为度数
		double angleDeg = angleRad * 180 / CV_PI;
		return angleDeg;

}

double CLineCaliperGUI::pointToLineDistance(const cv::Point2f& point, const cv::Point2d& lineStart, const cv::Point2d& lineEnd) {
	cv::Point2d lineVector = lineEnd - lineStart; // 线段的向量
	cv::Point2d pointVector = cv::Point2d(point.x, point.y) - lineStart; // 转换并计算

	double lineLength = cv::norm(lineVector); // 线段的长度
	if (lineLength == 0) return cv::norm(cv::Point2d(point.x, point.y) - lineStart); // 点到线段起点的距离

	double t = (lineVector.dot(pointVector)) / (lineLength * lineLength); // 投影比例
	if (t < 0) return cv::norm(cv::Point2d(point.x, point.y) - lineStart); // 点在起点左侧
	if (t > 1) return cv::norm(cv::Point2d(point.x, point.y) - lineEnd); // 点在终点右侧

	cv::Point2d projection = lineStart + t * lineVector; // 投影点
	return cv::norm(cv::Point2d(point.x, point.y) - projection); // 点到投影点的距离
}

vector<cv::Point2f> CLineCaliperGUI::get_intersection(cv::Point2f pt1, cv::Point2f pt2, cv::Point2f begin, cv::Point2f end)
{
	qreal k1;
	qreal b1;
	qreal k2;
	qreal b2;
	qreal k3;
	qreal b3;

	qreal x1;
	qreal y1;
	qreal x2;
	qreal y2;
	if ((pt1.x - pt2.x) != 0)
	{
		k1 = (pt1.y - pt2.y)/ (pt1.x - pt2.x);
		b1 = -k1 * pt1.x + pt1.y;
		k2 = -1 / k1;
		k3 = -1 / k1;
		b2 = -k2 * begin.x + begin.y;
		b3 = -k3 * end.x + end.y;

		x1 = (b2 - b1) / (k1 - k2);
		y1 = k1 * x1 + b1;
		x2 = (b3 - b1) / (k1 - k3);
		y2 = k1 * x2 + b1;
	}
	else if (pt1.y != pt2.y)
	{
		x1 = x2 = pt1.x; 
		y1 = begin.y;
		y2 = end.y;
	}
	else
	{
		x1 = x2 = y1 = y2 = 0;
	}

	vector<cv::Point2f> result;
	result.push_back(cv::Point(x1, y1));
	result.push_back(cv::Point(x2, y2));

	return result;
}

void CLineCaliperGUI::findIntersection(cv::Point p_1, cv::Point p_2, cv::Point p_3, cv::Point p_4, cv::Point2f& intersection)
{
	// 确保两条直线不平行
	//if (line1.m == line2.m) {
	//	qDebug() << "平行无交点.";
	//	return;
	//}

	// line1's cpmponent
	double X1 = p_2.x - p_1.x;//b1
	double Y1 = p_2.y - p_1.y;//a1
	// line2's cpmponent
	double X2 = p_4.x - p_3.x;//b2
	double Y2 = p_4.y - p_3.y;//a2
	// distance of 1,2
	double X21 = p_3.x - p_1.x;
	double Y21 = p_3.y - p_1.y;
	// determinant
	double D = Y1 * X2 - Y2 * X1;// a1b2-a2b1
	// 
	if (D == 0) return;
	// cross point
	intersection.x = (X1 * X2 * Y21 + Y1 * X2 * p_1.x - Y2 * X1 * p_3.x) / D;
	// on screen y is down increased ! 
	intersection.y = -(Y1 * Y2 * X21 + X1 * Y2 * p_1.y - X2 * Y1 * p_3.y) / D;

	return;
}

// 计算直线的斜率和截距
Line CLineCaliperGUI::calculateLine(const cv::Point& p1, const cv::Point& p2)
{
	Line line;
	double doux = p2.x - p1.x;
	if (doux == 0)
	{
		doux = 1;
	}
	line.m = (p2.y - p1.y) / (doux);
	line.b = p1.y - line.m * p1.x;
	return line;
}

