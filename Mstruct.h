#pragma once
#pragma execution_character_set("utf-8")

#include<vector>
#include"vector"
#include<opencv2/opencv.hpp>
#define VISION_TOLERANCE 0.0000001
#define D2R (CV_PI / 180.0)
#define MATCH_CANDIDATE_NUM 5
#define R2D (180.0 / CV_PI)

#define TEXT_COLOR_RED(STRING)         "<font size=200 color=red>" STRING "</font>" 
#define TEXT_COLOR_BLUE(STRING)        "<font size=200 color=blue>" STRING "</font>" 
#define TEXT_COLOR_GREEN(STRING)        "<font size=200 color=green>" STRING "</font>" 


struct s_TemplData
{
	std::vector<cv::Mat> vecPyramid;
	std::vector<cv::Scalar> vecTemplMean;
	std::vector<double> vecTemplNorm;
	std::vector<double> vecInvArea;
	std::vector<bool> vecResultEqual1;
	bool bIsPatternLearned;
	int iBorderColor;
	void clear()
	{
		std::vector<cv::Mat>().swap(vecPyramid);
		std::vector<double>().swap(vecTemplNorm);
		std::vector<double>().swap(vecInvArea);
		std::vector<cv::Scalar>().swap(vecTemplMean);
		std::vector<bool>().swap(vecResultEqual1);
	}
	void resize(int iSize)
	{
		vecTemplMean.resize(iSize);
		vecTemplNorm.resize(iSize, 0);
		vecInvArea.resize(iSize, 1);
		vecResultEqual1.resize(iSize, false);
	}
	s_TemplData()
	{
		bIsPatternLearned = false;
	}
};
struct s_MatchParameter
{
	cv::Point2d pt;
	double dMatchScore;
	double dMatchAngle;
	//Mat matRotatedSrc;
	cv::Rect rectRoi;
	double dAngleStart;
	double dAngleEnd;
	cv::RotatedRect rectR;
	cv::Rect rectBounding;
	bool bDelete;

	double vecResult[3][3];//for subpixel
	int iMaxScoreIndex;//for subpixel
	bool bPosOnBorder;
	cv::Point2d ptSubPixel;
	double dNewAngle;

	s_MatchParameter(cv::Point2f ptMinMax, double dScore, double dAngle)//, Mat matRotatedSrc = Mat ())
	{
		pt = ptMinMax;
		dMatchScore = dScore;
		dMatchAngle = dAngle;

		bDelete = false;
		dNewAngle = 0.0;

		bPosOnBorder = false;
	}
	s_MatchParameter()
	{
		double dMatchScore = 0;
		double dMatchAngle = 0;
	}
	~s_MatchParameter()
	{

	}
};
struct s_BlockMax
{
	struct Block
	{
		cv::Rect rect;
		double dMax;
		cv::Point ptMaxLoc;
		Block()
		{}
		Block(cv::Rect rect_, double dMax_, cv::Point ptMaxLoc_)
		{
			rect = rect_;
			dMax = dMax_;
			ptMaxLoc = ptMaxLoc_;
		}
	};
	s_BlockMax()
	{}
	std::vector<Block> vecBlock;
	cv::Mat matSrc;
	s_BlockMax(cv::Mat matSrc_,cv::Size sizeTemplate)
	{
		matSrc = matSrc_;
		//將matSrc 拆成數個block，分別計算最大值
		int iBlockW = sizeTemplate.width * 2;
		int iBlockH = sizeTemplate.height * 2;

		int iCol = matSrc.cols / iBlockW;
		bool bHResidue = matSrc.cols % iBlockW != 0;

		int iRow = matSrc.rows / iBlockH;
		bool bVResidue = matSrc.rows % iBlockH != 0;

		if (iCol == 0 || iRow == 0)
		{
			vecBlock.clear();
			return;
		}

		vecBlock.resize(iCol * iRow);
		int iCount = 0;
		for (int y = 0; y < iRow; y++)
		{
			for (int x = 0; x < iCol; x++)
			{
				cv::Rect rectBlock(x * iBlockW, y * iBlockH, iBlockW, iBlockH);
				vecBlock[iCount].rect = rectBlock;
				minMaxLoc(matSrc(rectBlock), 0, &vecBlock[iCount].dMax, 0, &vecBlock[iCount].ptMaxLoc);
				vecBlock[iCount].ptMaxLoc += rectBlock.tl();
				iCount++;
			}
		}
		if (bHResidue && bVResidue)
		{
			cv::Rect rectRight(iCol * iBlockW, 0, matSrc.cols - iCol * iBlockW, matSrc.rows);
			Block blockRight;
			blockRight.rect = rectRight;
			minMaxLoc(matSrc(rectRight), 0, &blockRight.dMax, 0, &blockRight.ptMaxLoc);
			blockRight.ptMaxLoc += rectRight.tl();
			vecBlock.push_back(blockRight);

			cv::Rect rectBottom(0, iRow * iBlockH, iCol * iBlockW, matSrc.rows - iRow * iBlockH);
			Block blockBottom;
			blockBottom.rect = rectBottom;
			minMaxLoc(matSrc(rectBottom), 0, &blockBottom.dMax, 0, &blockBottom.ptMaxLoc);
			blockBottom.ptMaxLoc += rectBottom.tl();
			vecBlock.push_back(blockBottom);
		}
		else if (bHResidue)
		{
			cv::Rect rectRight(iCol * iBlockW, 0, matSrc.cols - iCol * iBlockW, matSrc.rows);
			Block blockRight;
			blockRight.rect = rectRight;
			minMaxLoc(matSrc(rectRight), 0, &blockRight.dMax, 0, &blockRight.ptMaxLoc);
			blockRight.ptMaxLoc += rectRight.tl();
			vecBlock.push_back(blockRight);
		}
		else
		{
			cv::Rect rectBottom(0, iRow * iBlockH, matSrc.cols, matSrc.rows - iRow * iBlockH);
			Block blockBottom;
			blockBottom.rect = rectBottom;
			minMaxLoc(matSrc(rectBottom), 0, &blockBottom.dMax, 0, &blockBottom.ptMaxLoc);
			blockBottom.ptMaxLoc += rectBottom.tl();
			vecBlock.push_back(blockBottom);
		}
	}
	void UpdateMax(cv::Rect rectIgnore)
	{
		if (vecBlock.size() == 0)
			return;
		//找出所有跟rectIgnore交集的block
		int iSize = vecBlock.size();
		for (int i = 0; i < iSize; i++)
		{
			cv::Rect rectIntersec = rectIgnore & vecBlock[i].rect;
			//無交集
			if (rectIntersec.width == 0 && rectIntersec.height == 0)
				continue;
			//有交集，更新極值和極值位置
			minMaxLoc(matSrc(vecBlock[i].rect), 0, &vecBlock[i].dMax, 0, &vecBlock[i].ptMaxLoc);
			vecBlock[i].ptMaxLoc += vecBlock[i].rect.tl();
		}
	}
	void GetMaxValueLoc(double& dMax, cv::Point& ptMaxLoc)
	{
		int iSize = vecBlock.size();
		if (iSize == 0)
		{
			minMaxLoc(matSrc, 0, &dMax, 0, &ptMaxLoc);
			return;
		}
		//從block中找最大值
		int iIndex = 0;
		dMax = vecBlock[0].dMax;
		for (int i = 1; i < iSize; i++)
		{
			if (vecBlock[i].dMax >= dMax)
			{
				iIndex = i;
				dMax = vecBlock[i].dMax;
			}
		}
		ptMaxLoc = vecBlock[iIndex].ptMaxLoc;
	}
};



