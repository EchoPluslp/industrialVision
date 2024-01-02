#include<opencv2/opencv.hpp>
#include<vector>
#include <QImage>
using namespace cv;
using namespace std;

#define PI 3.14159265358979323846
#define to_degree(x) x*180.0/PI
#define to_radian(x) x*PI/180.0
#define GREED   Scalar(0,255,0)
#define RED    Scalar(0,0,255)
#define BLUE    Scalar(255,0,0)
#define Cyan    Scalar(255,255,0)
#define Yellow  Scalar(0,255,255)  

static void drawMyArrow(Mat& inputMat,
	Point2d p1,
	Point2d p2,
	int dSize,
	Scalar color,
	int nThickness = 1)
{
	if (inputMat.empty())
	{
		return;
	}
	double dK = ((double)p2.y - (double)p1.y) / ((double)p2.x - (double)p1.x);
	double angle = atan(dK) * 180 / PI;
	line(inputMat, p1, p2, color, nThickness, LINE_AA);
	RotatedRect rotateRect(p2, Size(dSize, dSize * 0.5), angle);
	Point2f rectPoints[4];
	rotateRect.points(rectPoints);
	if ((angle >= 0 && p1.x <= p2.x) || (angle < 0 && p1.x <= p2.x))
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

static void drawMyCross(Mat& inputMat,
	Point2d p,
	double angle,
	double dSize,
	Scalar color,
	int nThickness = 1)
{
	if (inputMat.empty())
	{
		return;
	}
	RotatedRect rotateRect(p, Size2d(dSize, dSize), angle);
	Point2f rectPoints[4];
	rotateRect.points(rectPoints);
	line(inputMat, rectPoints[0], rectPoints[2], color, nThickness, LINE_AA);
	line(inputMat, rectPoints[1], rectPoints[3], color, nThickness, LINE_AA);
}

static void drawMyRect(Mat& inputMat, RotatedRect rRect,
	Scalar color, int nThickness = 1)
{
	Point2f pfRectPoints[4];
	rRect.points(pfRectPoints);
	for (int i = 0; i < 4; i++)
	{
		line(inputMat, pfRectPoints[i], pfRectPoints[(i + 1) % 4],
			color, nThickness, LINE_AA);
	}
}

static double getPoint2PointLength(Point2d p1, Point2d p2)
{
	return sqrt(pow((p1.x - p2.x), 2) + pow((p1.y - p2.y), 2));
}

static Point2d getPoint2PoinCenter(Point2d p1, Point2d p2)
{
	return Point2d((p1.x + p2.x) * 0.5, (p1.y + p2.y) * 0.5);
}

static double GetLineSlope(Point2d p1, Point2d p2)
{
	return (p1.y - p2.y) / (p1.x - p2.x);
}


static void getLineLastPoint(Point2d pdCenter, double dAngle,
	double dLength, Point2d& pdStart, Point2d& pdEnd)
{
	pdStart.x = pdCenter.x - cos(to_radian(dAngle)) * 0.5 * dLength;
	pdStart.y = pdCenter.y - sin(to_radian(dAngle)) * 0.5 * dLength;
	pdEnd = Point2d(pdCenter.x * 2 - pdStart.x, pdCenter.y * 2 - pdStart.y);
}


static double getAnglePoint2Point(Point2d p1, Point2d p2)
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

static double getPointLineDis(Point2d p1, double dK, double dB)
{
	return abs(dK * p1.x - p1.y + dB) / sqrt(1 + dK * dK);
}


static void getCircleFromEquPointSets(
	Point2d pdCenter,
	double dRadius,
	int nEquinoxNums,
	vector<Point2d>& vpdEquinoxPoints)
{
	if (nEquinoxNums == 0)
	{
		return;
	}
	double dEquinoxAngle = 360.00 / nEquinoxNums;
	Point2d p1, p2;
	double dAngle = 0;
	vpdEquinoxPoints.clear();

	for (int i = 0; i < nEquinoxNums; i++)
	{
		getLineLastPoint(pdCenter, dAngle,
			2 * dRadius, p1, p2);
		vpdEquinoxPoints.push_back(p2);
		dAngle += dEquinoxAngle;
	}

}

//将QImage转化为Mat
static cv::Mat QImageToCvMat(const QImage& inImage, bool inCloneImageData = true)
{
	switch (inImage.format())
	{
		// 8-bit, 4 channel
	case QImage::Format_ARGB32:
	case QImage::Format_ARGB32_Premultiplied:
	{
		cv::Mat  mat(inImage.height(), inImage.width(),
			CV_8UC4,
			const_cast<uchar*>(inImage.bits()),
			static_cast<size_t>(inImage.bytesPerLine())
		);

		return (inCloneImageData ? mat.clone() : mat);
	}

	//// 8-bit, 3 channel
	case QImage::Format_RGB32:
	case QImage::Format_RGB888:
	{
		if (!inCloneImageData)
		{
			//qWarning() << "CVS::QImageToCvMat() - Conversion requires cloning because we use a temporary QImage";
		}

		QImage   swapped = inImage;

		if (inImage.format() == QImage::Format_RGB32)
		{
			swapped = swapped.convertToFormat(QImage::Format_RGB888);
		}

		swapped = swapped.rgbSwapped();

		return cv::Mat(swapped.height(), swapped.width(),
			CV_8UC3,
			const_cast<uchar*>(swapped.bits()),
			static_cast<size_t>(swapped.bytesPerLine())
		).clone();
	}

	// 8-bit, 1 channel
	case QImage::Format_Indexed8:
	{
		cv::Mat  mat(inImage.height(), inImage.width(),
			CV_8UC1,
			const_cast<uchar*>(inImage.bits()),
			static_cast<size_t>(inImage.bytesPerLine())
		);
		       
		return (inCloneImageData ? mat.clone() : mat);
	}

	default:
		//qWarning() << "CVS::QImageToCvMat() - QImage format not handled in switch:" << inImage.format();
		break;
	}

	return cv::Mat();
}