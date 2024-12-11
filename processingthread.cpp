#include "processingthread.h"
#define NOMINMAX

using namespace std;
using namespace cv;

bool compareScoreBig2Small(const s_MatchParameter& lhs, const s_MatchParameter& rhs) { return  lhs.dMatchScore > rhs.dMatchScore; }
bool comparePtWithAngle(const pair<cv::Point2f, double> lhs, const pair<Point2f, double> rhs) { return lhs.second < rhs.second; }



ProcessingThread::ProcessingThread(QObject* parent)
	: QThread{ parent }
{
	this->initThread();
	m_plineCaliperGUI = new CLineCaliperGUI();
	circleInstanceGui = new CCaliperCircleGUI();
}

ProcessingThread::~ProcessingThread()
{

}

void ProcessingThread::initThread()   //��ʼ��ͼ�����߳�
{
	startFlag = false;
	pattern_Flag = false;
	m_threadId = 0;
}

void ProcessingThread::setThreadId(int id)
{
	m_threadId = id;
}

void ProcessingThread::setSwitchFlag(bool switchFlag)
{
	startFlag = switchFlag;
}
void ProcessingThread::setmodelAndRealSclar(bool switchFlag)
{
	modelAndRealSclar = switchFlag;
}

bool ProcessingThread::getmodelAndRealSclar()
{
	return  modelAndRealSclar;
}
void ProcessingThread::run()
{
	while (startFlag)
	{
			if (m_imageVector_1.size() == 1)
			{
				Mat tempMap = m_imageVector_1.at(0);
				QPixmap newPixmap_1 = cvMatToPixmap(tempMap);

				//ִ����״ƥ��
				if (pattern_Flag && shape_match)
				{
					QTime timedebuge;//����һ��ʱ�Ӷ���
					timedebuge.start();//��ʼ��ʱ

					//����һ����
					if (shapeMatch_Patten.size()==1)
					{
						PatternInfo item = shapeMatch_Patten.at(0);

					//����ǰQpixmap ���͸�ƥ�����
					Mat src = tempMap(item.roi).clone();

					m_plineCaliperGUI->createLineKaChi(src, item.pt_begin_cv2,item.pt_end_cv2,item.height,
						item.width,1,item.nthresholdValuel,item.nSampleDirection,item.nMeasureNums);

					Point2d pdLineStart(0, 0), pdLineEnd(0, 0);
					double dAngle = 0;
					m_plineCaliperGUI->lineEdgePointSetsFit(pdLineStart, pdLineEnd, dAngle);
					pdLineStart.x = item.roi.tl().x + pdLineStart.x;
					pdLineStart.y = item.roi.tl().y + pdLineStart.y;
					pdLineEnd.x = item.roi.tl().x + pdLineEnd.x;
					pdLineEnd.y = item.roi.tl().y + pdLineEnd.y;

					//���㽻���
					vector<cv::Point2f> result = m_plineCaliperGUI->get_intersection(pdLineStart, pdLineEnd, item.pt_start_line, item.pt_end_line);
			              
					//��ȡ���ĵ�
					cv::Point center_Point = (result[0] + result[1]) / 2;
					//�������ĵ�
					lastResult.setX(center_Point.x- (m_width / 2));  //����Ϊx������
					lastResult.setY((m_height / 2) - center_Point.y);//����Ϊy������

					finall_Total_Result.ptCenter = cv::Point2d(lastResult.x(), lastResult.y());
					finall_Total_Result.dMatchedAngle = dAngle;
					finall_Total_Result.pattern_flag = true;
					finall_Total_Result.flag = true;

					pattern_Flag = false;
					int total_time = timedebuge.elapsed();
					resultPointF.setX(lastResult.x());
					resultPointF.setY(lastResult.y());


					//���͸�ǰ�� 
					emit signal_patternResult(resultPointF, total_time);                       
					//��temp����
					QPainter painter(&newPixmap_1);
					QPen pen;
					pen.setStyle(Qt::SolidLine);            //���廭�ʵķ��ֱ�ߡ����ߵ�
					pen.setWidth(2);                        //���廭�ʵĴ�С
					pen.setBrush(Qt::green);
					painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

					painter.setPen(pen);
					painter.drawLine(QPoint(result[0].x, result[0].y),
						QPoint(result[1].x, result[1].y));
					}
					//����������
					else if (shapeMatch_Patten.size() == 2)
					{
						vector<cv::Point2f> Two_Line_Result;
						//�ҵ�����ֱ��,����ý���ͽǶ�
						for (int i =0;i< shapeMatch_Patten.size();i++)
						{
							PatternInfo item = shapeMatch_Patten.at(i);

							//����ǰQpixmap ���͸�ƥ�����
							Mat src = tempMap(item.roi).clone();

							m_plineCaliperGUI->createLineKaChi(src, item.pt_begin_cv2, item.pt_end_cv2, item.height,
								item.width, 1, item.nthresholdValuel, item.nSampleDirection, item.nMeasureNums);

							Point2d pdLineStart(0, 0), pdLineEnd(0, 0);
							double dAngle = 0;
							m_plineCaliperGUI->lineEdgePointSetsFit(pdLineStart, pdLineEnd, dAngle);
							pdLineStart.x = item.roi.tl().x + pdLineStart.x;
							pdLineStart.y = item.roi.tl().y + pdLineStart.y;
							pdLineEnd.x = item.roi.tl().x + pdLineEnd.x;
							pdLineEnd.y = item.roi.tl().y + pdLineEnd.y;

							//����õ������߶ε�begin��end
							vector<cv::Point2f> result = m_plineCaliperGUI->get_intersection(pdLineStart, pdLineEnd, item.pt_start_line, item.pt_end_line);
							for (auto num : result) {
								Two_Line_Result.push_back(num);
							}
						}
						//�ж��Ƿ���������
						if (Two_Line_Result.size()!=4)
						{
							//�������,û����������
							return;
						}

						//	//����нǡ�        
						double angleDeg = m_plineCaliperGUI->findangle(Two_Line_Result.at(0), Two_Line_Result.at(1), Two_Line_Result.at(2),Two_Line_Result.at(3));
						//--------------------------���㽻��
						Line Line_1 = m_plineCaliperGUI->calculateLine(Two_Line_Result.at(0), Two_Line_Result.at(1));
						Line Line_2 = m_plineCaliperGUI->calculateLine(Two_Line_Result.at(2), Two_Line_Result.at(3));

						cv::Point2f Intersection(-1, -1);
						m_plineCaliperGUI->findIntersection(Two_Line_Result.at(0), Two_Line_Result.at(1), Two_Line_Result.at(2), Two_Line_Result.at(3), Intersection);
						
						//�������ĵ�
						lastResult.setX(Intersection.x - (m_width / 2));  //����Ϊx������
						lastResult.setY((m_height / 2) - Intersection.y);//����Ϊy������

						finall_Total_Result.ptCenter = cv::Point2d(lastResult.x(), lastResult.y());
						finall_Total_Result.dMatchedAngle = angleDeg;
						finall_Total_Result.pattern_flag = true;
						finall_Total_Result.flag = true;

						pattern_Flag = false;
						int total_time = timedebuge.elapsed();
						resultPointF.setX(lastResult.x());
						resultPointF.setY(lastResult.y());


						//���͸�ǰ�� 
						emit signal_patternResult(resultPointF, total_time);

						//��temp����
						QPainter painter(&newPixmap_1);
						QPen pen;
						pen.setStyle(Qt::SolidLine);            //���廭�ʵķ��ֱ�ߡ����ߵ�
						pen.setWidth(2);                        //���廭�ʵĴ�С
						pen.setBrush(Qt::green);
						painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
						painter.setPen(pen);
						painter.drawLine(QPoint(Two_Line_Result[0].x, Two_Line_Result[0].y),
							QPoint(Two_Line_Result[1].x, Two_Line_Result[1].y));
						painter.drawLine(QPoint(Two_Line_Result[2].x, Two_Line_Result[2].y),
							QPoint(Two_Line_Result[3].x, Two_Line_Result[3].y));
					}
					else if (shapeMatch_Patten.size() >= 3)
					{
						//��ȡ������
						PatternInfo item_mainLine = shapeMatch_Patten.at(0);
						//����ǰQpixmap ���͸�ƥ�����
						Mat src = tempMap(item_mainLine.roi).clone();
						m_plineCaliperGUI->createLineKaChi(src, item_mainLine.pt_begin_cv2, item_mainLine.pt_end_cv2, item_mainLine.height,
							item_mainLine.width, 1, item_mainLine.nthresholdValuel, item_mainLine.nSampleDirection, item_mainLine.nMeasureNums);
						Point2d pdLineStart_mainLine(0, 0), pdLineEnd_mainLine(0, 0);
						double dAngle = 0;
						m_plineCaliperGUI->lineEdgePointSetsFit(pdLineStart_mainLine, pdLineEnd_mainLine, dAngle);
						pdLineStart_mainLine.x = item_mainLine.roi.tl().x + pdLineStart_mainLine.x;
						pdLineStart_mainLine.y = item_mainLine.roi.tl().y + pdLineStart_mainLine.y;
						pdLineEnd_mainLine.x = item_mainLine.roi.tl().x + pdLineEnd_mainLine.x;
						pdLineEnd_mainLine.y = item_mainLine.roi.tl().y + pdLineEnd_mainLine.y;
	

						//�ҵ�ʣ�µ�����
						vector<cv::Point2f> Two_Line_Result;
						for (int i = 1; i < shapeMatch_Patten.size(); i++)
						{
							PatternInfo item = shapeMatch_Patten.at(i);

							//����ǰQpixmap ���͸�ƥ�����
							Mat src = tempMap(item.roi).clone();

							m_plineCaliperGUI->createLineKaChi(src, item.pt_begin_cv2, item.pt_end_cv2, item.height,
								item.width, 1, item.nthresholdValuel, item.nSampleDirection, item.nMeasureNums);

							Point2d pdLineStart(0, 0), pdLineEnd(0, 0);
							double dAngle = 0;
							m_plineCaliperGUI->lineEdgePointSetsFit(pdLineStart, pdLineEnd, dAngle);
							pdLineStart.x = item.roi.tl().x + pdLineStart.x;
							pdLineStart.y = item.roi.tl().y + pdLineStart.y;
							pdLineEnd.x = item.roi.tl().x + pdLineEnd.x;
							pdLineEnd.y = item.roi.tl().y + pdLineEnd.y;

							//����õ������߶ε�begin��end
							vector<cv::Point2f> result = m_plineCaliperGUI->get_intersection(pdLineStart, pdLineEnd, item.pt_start_line, item.pt_end_line);
							for (auto num : result) {
								Two_Line_Result.push_back(num);
							}
 						}
						if (Two_Line_Result.size()!=8)
						{
							return;
						}
						//���ߺ������߼����ɡ�
 						//����н�
						double angleDeg_1_realTime = m_plineCaliperGUI->findangle(Two_Line_Result.at(0), Two_Line_Result.at(1), Two_Line_Result.at(2), Two_Line_Result.at(3));
						double angleDeg_2_realTime = m_plineCaliperGUI->findangle(Two_Line_Result.at(4), Two_Line_Result.at(5), Two_Line_Result.at(6), Two_Line_Result.at(7));
						//���㽻��1		
						cv::Point2f Intersection_1_realTime(-1, -1);
						m_plineCaliperGUI->findIntersection(Two_Line_Result.at(0), Two_Line_Result.at(1), Two_Line_Result.at(2), Two_Line_Result.at(3), Intersection_1_realTime);
						//���㽻��2	
						cv::Point2f Intersection_2_realTime(-1, -1);
						m_plineCaliperGUI->findIntersection(Two_Line_Result.at(4), Two_Line_Result.at(5), Two_Line_Result.at(6), Two_Line_Result.at(7), Intersection_2_realTime);
						//������߾���
						double pointToLineDistance_1_realTime = m_plineCaliperGUI->pointToLineDistance(Intersection_1_realTime, pdLineStart_mainLine, pdLineEnd_mainLine);
						double pointToLineDistance_2_realTime = m_plineCaliperGUI->pointToLineDistance(Intersection_2_realTime, pdLineStart_mainLine, pdLineEnd_mainLine);
						//�жϱ�׼--------------------------------
						double percentageA = 0.2;    // 10%
						double lowerBoundA_1 = angleDeg_1_newp * (1 - percentageA);
						double upperBoundA_1 = angleDeg_1_newp * (1 + percentageA);
						resultPointF.setX(1);
						resultPointF.setY(1);
						finall_Total_Result.pattern_flag = true;

						if (angleDeg_1_realTime < lowerBoundA_1 || angleDeg_1_realTime > upperBoundA_1)
						{
							resultPointF.setX(100);
							resultPointF.setY(100);
							finall_Total_Result.pattern_flag = false;

						}

						double lowerBoundA_2 = angleDeg_2_newp * (1 - percentageA);
						double upperBoundA_2 = angleDeg_2_newp * (1 + percentageA);
						if (angleDeg_2_realTime < lowerBoundA_2 || angleDeg_2_realTime > upperBoundA_2)
						{
							resultPointF.setX(101);
							resultPointF.setY(101);
							finall_Total_Result.pattern_flag = false;

						} 
						// ����������֮��ľ���--------------------------------
						double percentagePP = 0.2; // 10%
						double distance_1 = cv::norm(Intersection_1_realTime - Intersection_1_newP);
						// �����µĽ��������������
						double maxDistance_1 = cv::norm(Intersection_1_newP) * percentagePP;
						cv::circle(tempMap, Intersection_1_newP, 10, cv::Scalar(0, 0, 0), cv::FILLED); // ��ɫ���ĵ�
						cv::circle(tempMap, Intersection_1_realTime, 10, cv::Scalar(0, 0, 0), cv::FILLED); // ��ɫ���ĵ�

						if (distance_1 > maxDistance_1)
						{
							resultPointF.setX(102);
							resultPointF.setY(102);
						}
						double distance_2 = cv::norm(Intersection_2_realTime - Intersection_2_newP);
						// �����µĽ��������������
						double maxDistance_2 = cv::norm(Intersection_2_newP) * percentagePP;
						if (distance_2 > maxDistance_2)
						{
							resultPointF.setX(103);
							resultPointF.setY(103);
							finall_Total_Result.pattern_flag = false;

						}
						// �������֮��ľ���--------------------------------
						double percentagePX = 0.2;    // 10%
						double lowerBoundPX_1 = pointToLineDistance_1_newP * (1 - percentagePX);
						double upperBoundPX_1 = pointToLineDistance_1_newP * (1 + percentagePX);
						if (pointToLineDistance_1_realTime < lowerBoundPX_1 || pointToLineDistance_1_realTime > upperBoundPX_1)
						{
							resultPointF.setX(104);
							resultPointF.setY(104);
							finall_Total_Result.pattern_flag = false;

						}

						double lowerBoundPX_2 = pointToLineDistance_2_newP * (1 - percentagePX);
						double upperBoundPX_2 = pointToLineDistance_2_newP * (1 + percentagePX);
						if (pointToLineDistance_2_realTime < lowerBoundPX_2 || pointToLineDistance_2_realTime > upperBoundPX_2)
						{
							resultPointF.setX(105);
							resultPointF.setY(105);
							finall_Total_Result.pattern_flag = false;
						}

						//��temp����
						QPainter painter(&newPixmap_1);
						QPen pen;
						pen.setStyle(Qt::SolidLine);            //���廭�ʵķ��ֱ�ߡ����ߵ�
						pen.setWidth(10);                        //���廭�ʵĴ�С
						pen.setBrush(Qt::green);
						painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
						painter.setPen(pen);
						painter.drawLine(QPoint(Two_Line_Result[0].x, Two_Line_Result[0].y),
							QPoint(Two_Line_Result[1].x, Two_Line_Result[1].y));
						painter.drawLine(QPoint(Two_Line_Result[2].x, Two_Line_Result[2].y),
							QPoint(Two_Line_Result[3].x, Two_Line_Result[3].y));
						painter.drawLine(QPoint(Two_Line_Result[4].x, Two_Line_Result[4].y),
							QPoint(Two_Line_Result[5].x, Two_Line_Result[5].y));	
						painter.drawLine(QPoint(Two_Line_Result[6].x, Two_Line_Result[6].y),
								QPoint(Two_Line_Result[7].x, Two_Line_Result[7].y));
						painter.drawLine(QPoint(Two_Line_Result[8].x, Two_Line_Result[8].y),
							QPoint(Two_Line_Result[9].x, Two_Line_Result[9].y));
						//���͸�ǰ�� 
						finall_Total_Result.ptCenter = cv::Point2d(resultPointF.x(), resultPointF.y());
						finall_Total_Result.dMatchedAngle = 0;
						finall_Total_Result.flag = true;

						emit signal_patternResult(resultPointF, 0);
					}
					if (shapeMatch_Patten_Circle.size() == 1) {
					//��Բ
					PatternInfo_circle item = shapeMatch_Patten_Circle.at(0);

					//����Բ�Ŀ���λ��
					circleInstanceGui->createCircleKaChi(tempMap, item.pdCenter, item.nRadius,
						item.dMeasureLength,item.dMeasureHeight,item.dSigma,item.nThreshold,
						item.nTranslation,item.nMesureNums,item.nSampleDirection);
					
					Point2d pdCenter(0, 0);
					double dRadius = 0;
					//fit�µ�Բ      
					circleInstanceGui->circleFitOpt(pdCenter, dRadius, 1);

					QPainter painter(&newPixmap_1);
					QPen pen;
					pen.setStyle(Qt::SolidLine);            //
					pen.setWidth(2);    
					painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
					//
					pen.setBrush(Qt::green);
					painter.setPen(pen);
					painter.drawEllipse(pdCenter.x - dRadius, pdCenter.y - dRadius,2*dRadius,2*dRadius);
					painter.drawPoint(pdCenter.x, pdCenter.y );
					cv::Point2d line_EndPoint;
					//�ж��Ƿ���ֱ��
					if (shapeMatch_Patten.size() == 1)
					{
						line_EndPoint.x = lastResult.x();
						line_EndPoint.y = lastResult.y();
						//�������ĵ�,ֱ�����ߺ�Բ��Բ�����ߵĽ���
						lastResult.setX(((line_EndPoint.x+ pdCenter.x)/2) - (m_width / 2));  //����Ϊx������
						lastResult.setY((m_height / 2) -((line_EndPoint.y+pdCenter.y)/2));//����Ϊy������

					}
					else {
						//�������ĵ�
						lastResult.setX(pdCenter.x - (m_width / 2));  //����Ϊx������
						lastResult.setY((m_height / 2) - pdCenter.y);//����Ϊy������

					}
					

					finall_Total_Result.ptCenter = cv::Point2d(lastResult.x(), lastResult.y());
					finall_Total_Result.dMatchedAngle = 0;
					finall_Total_Result.pattern_flag = true;
					finall_Total_Result.flag = true;

			

					pattern_Flag = false;
					int total_time = timedebuge.elapsed();
					resultPointF.setX(lastResult.x());
					resultPointF.setY(lastResult.y());


					//���͸�ǰ�� 
					emit signal_patternResult(resultPointF, total_time);

					}

				}
				//�ж�ģ��ƥ��
				else if (pattern_Flag && modelAndRealSclar)
				{
					QTime timedebuge;//����һ��ʱ�Ӷ���
					timedebuge.start();//��ʼ��ʱ
					if (areaMatRect.x+ areaMatRect.width>tempMap.cols )
					{
						areaMatRect.x = tempMap.cols - areaMatRect.width;
					}else if (areaMatRect.y + areaMatRect.height > tempMap.rows)
					{
						areaMatRect.y = tempMap.rows - areaMatRect.height;
					 
					}	
					cv::Point2f Point_1;
					ResultPoint Point_2;
					if (shape_type)
					{
						 Point_1 = MatchPicture(patternMat, tempMap(areaMatRect), false);

						 pattern_Flag = false;
						 resultPointF.setX(Point_1.x);
						 resultPointF.setY(Point_1.y);

						 int total_time = timedebuge.elapsed();

						 emit signal_patternResult(resultPointF, total_time);
					
					}
					else {
						Point_2 = slot_processMatchPictureWithSource(tempMap(areaMatRect));
						pattern_Flag = false;
						int total_time = timedebuge.elapsed();
						resultPointF.setX(Point_2.X);
						resultPointF.setY(Point_2.Y);

						emit signal_patternResult(resultPointF, total_time);

					}
					if (resultPointF.x() != -m_width && resultPointF.y() != m_height) {
						QPainter painter(&newPixmap_1);

						QPen pen;
						pen.setStyle(Qt::SolidLine);            //���廭�ʵķ��ֱ�ߡ����ߵ�
						pen.setWidth(10);                        //���廭�ʵĴ�С
						pen.setBrush(Qt::green);
						painter.setPen(pen);
						painter.drawEllipse(QPointF(drawCenterPoint.x, drawCenterPoint.y), 50, 50);
					}
				}
				//����ִ��ƥ��,���Ǳ������Ե����,Ҳ��˵��ͬ����ʱ,������ƥ��,ƥ�����
			/*	if (pattern_Flag && modelAndRealSclar == false) {
					patternNG();
				}*/

				pattern_Flag = false;
				//�ж��Ƿ���Ҫչʾ��Χͼ
				if (area_Flag)
				{
					QPen pen;
					pen.setStyle(Qt::SolidLine);            //���廭�ʵķ��ֱ�ߡ����ߵ�
					pen.setWidth(10);                        //���廭�ʵĴ�С
					pen.setBrush(Qt::red);
					QPainter painter(&newPixmap_1);
					painter.setPen(pen);
					painter.drawRect(areaMatRect.x, areaMatRect.y, areaMatRect.width, areaMatRect.height);
				}
				else if (shape_match) {
					QPen pen;
					pen.setStyle(Qt::SolidLine);            //���廭�ʵķ��ֱ�ߡ����ߵ�
					pen.setWidth(10);                        //���廭�ʵĴ�С
					pen.setBrush(Qt::red);
					QPainter painter(&newPixmap_1);
					painter.setPen(pen);
					for (int i = 0; i < shapeMatch_Patten.size();i++)
					{
						PatternInfo item = shapeMatch_Patten.at(i);
						painter.drawRect(item.roi.x, item.roi.y, item.roi.width, item.roi.height);
					}
					for (int i = 0; i < shapeMatch_Patten_Circle.size(); i++)
					{
						PatternInfo_circle item = shapeMatch_Patten_Circle.at(i);
						//painter.drawRect(item.roi.x(), item.roi.y(), item.roi.width(), item.roi.height());
						painter.drawEllipse(item.pdCenter.x - (item.nRadius+item.dMeasureLength/2),
											item.pdCenter.y - (item.nRadius + item.dMeasureLength / 2),
							2 * (item.nRadius + item.dMeasureLength / 2), 2 * (item.nRadius + item.dMeasureLength / 2));
						painter.drawEllipse(item.pdCenter.x - (item.nRadius - item.dMeasureLength / 2),
							item.pdCenter.y - (item.nRadius - item.dMeasureLength / 2),
							2 * (item.nRadius - item.dMeasureLength / 2), 2 * (item.nRadius - item.dMeasureLength / 2));
					}
					
				}


				//������õ�ͼ���ֵ�������
				emit signal_newPixmap(newPixmap_1, 0);
				m_width = newPixmap_1.width();
				m_height = newPixmap_1.height();
				//ʹ������������
				m_imageVector_1.clear();
			}
			//��ձ���
			if (m_imageVector_1.size() > 1) {
				m_imageVector_1.clear();
			}
		msleep(20);   //�˴�����ɼ���cpu������,ע�ⲻҪ��������̵߳Ļ���
	}
}



//Mat->QImage
QImage ProcessingThread::cvMatToImage(const cv::Mat cvMat)
{
	if (cvMat.channels() > 1)
	{
		return QImage((const unsigned char*)(cvMat.data), cvMat.cols, cvMat.rows, QImage::Format_RGB888);
	}
	else
	{
		return QImage((const unsigned char*)(cvMat.data), cvMat.cols, cvMat.rows, QImage::Format_Indexed8);
	}
}

//Mat->QPixmap
QPixmap ProcessingThread::cvMatToPixmap(const cv::Mat cvMat)
{
	QImage myImage = QImage((const unsigned char*)(cvMat.data), cvMat.cols, cvMat.rows, QImage::Format_Indexed8);   //�Ҷ�ͼ

	//QSize��ʾͼ���ڽ����ϵ���ʾ�ߴ�
	return QPixmap::fromImage(myImage).scaled(QSize(myImage.width(), myImage.height()), Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

cv::Mat ProcessingThread::ImageToMat(QImage& image) //QImageתMat
{

	Mat mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());

	return mat;
}


int ProcessingThread::GetTopLayer(cv::Mat* matTempl, int iMinDstLength)
{
	int iTopLayer = 0;
	int iMinReduceArea = iMinDstLength * iMinDstLength;
	int iArea = matTempl->cols * matTempl->rows;
	while (iArea > iMinReduceArea)
	{
		iArea /= 4;
		iTopLayer++;
	}
	return iTopLayer;
}


void ProcessingThread::slot_recievePatternImage(QString pattern_Path,QRectF pattern_Rect,QRectF areaRect, QPoint centerPoint, QPoint patternRectCenterPoint )
{
	areaMatRect.x = areaRect.x();
	areaMatRect.y = areaRect.y();
	areaMatRect.width = areaRect.width();
	areaMatRect.height = areaRect.height();

	QString appleModel = QApplication::applicationDirPath() + "/model/";
	QString dirpath = appleModel + pattern_Path;

	std::string readImage = dirpath.toLocal8Bit();
	 Mat ReadImagestd =  imread(readImage, CV_8UC1);
	 
	 if (ReadImagestd.empty())
	 {
		 emit signal_modelPictureReadFlag();
		 //ģ��ͼ��ȡ����!!!
		 return;
	 }
	 patternMat = ReadImagestd(Rect(pattern_Rect.x(), pattern_Rect.y(), pattern_Rect.width(), pattern_Rect.height())).clone();
	///////////////////����ģ��ͼ

	m_TemplData.clear();

	int iTopLayer = GetTopLayer(&patternMat, (int)sqrt((double)256));
	cv::buildPyramid(patternMat, m_TemplData.vecPyramid, iTopLayer);
	s_TemplData* templData = &m_TemplData;
	templData->iBorderColor = mean(patternMat).val[0] < 128 ? 255 : 0;
	int iSize = templData->vecPyramid.size();
	templData->resize(iSize);
	for (int i = 0; i < iSize; i++)
	{
		double invArea = 1. / ((double)templData->vecPyramid[i].rows * templData->vecPyramid[i].cols);
		cv::Scalar templMean, templSdv;
		double templNorm = 0, templSum2 = 0;

		meanStdDev(templData->vecPyramid[i], templMean, templSdv);
		templNorm = templSdv[0] * templSdv[0] + templSdv[1] * templSdv[1] + templSdv[2] * templSdv[2] + templSdv[3] * templSdv[3];

		if (templNorm < DBL_EPSILON)
		{
			templData->vecResultEqual1[i] = true;
		}
		templSum2 = templNorm + templMean[0] * templMean[0] + templMean[1] * templMean[1] + templMean[2] * templMean[2] + templMean[3] * templMean[3];


		templSum2 /= invArea;
		templNorm = std::sqrt(templNorm);
		templNorm /= std::sqrt(invArea); // care of accuracy here


		templData->vecInvArea[i] = invArea;
		templData->vecTemplMean[i] = templMean;
		templData->vecTemplNorm[i] = templNorm;
	}
	
	//�������������
	centerPointInProcess.setX(centerPoint.x());
	centerPointInProcess.setY(centerPoint.y()); 

	patternRectCenterPointInProcess.setX(patternRectCenterPoint.x());
	patternRectCenterPointInProcess.setY(patternRectCenterPoint.y());
	
	 initialDistance = calculateInitialDistance(patternRectCenterPoint, centerPoint);    // Initial distance between A and B
	 initialDirection = calculateInitialDirection(patternRectCenterPoint, centerPoint);  // Initial direction angle in degrees
	
	 templData->bIsPatternLearned = true;
}

//ģ��ͼ,ԭͼ
cv::Point2f ProcessingThread::MatchPicture(cv::Mat m_matDst, cv::Mat m_matSrc,bool modelflag)
{
	if (m_matSrc.empty() || m_matDst.empty())
	{patternNG();return resultPoint;}
    if ((m_matDst.cols < m_matSrc.cols && m_matDst.rows > m_matSrc.rows) || (m_matDst.cols > m_matSrc.cols && m_matDst.rows < m_matSrc.rows))
	{patternNG();return resultPoint;}
    if (m_matDst.size().area() > m_matSrc.size().area())
	{patternNG();return resultPoint;}
    if (!modelflag &&!m_TemplData.bIsPatternLearned)
	{patternNG();return resultPoint;}

    //
    int iTopLayer = GetTopLayer(&m_matDst, (int)sqrt((double)256));
    //����������
    std::vector<cv::Mat> vecMatSrcPyr;
    cv::buildPyramid(m_matSrc, vecMatSrcPyr, iTopLayer);
	s_TemplData* pTemplData;

	if (modelflag)
	{
		pTemplData = &m_TemplData_model;
	}
	else {
		pTemplData = &m_TemplData;
	}
    int cols = pTemplData->vecPyramid[iTopLayer].cols;
    int rows = pTemplData->vecPyramid[iTopLayer].rows;
    double x = 2.0 / max(pTemplData->vecPyramid[iTopLayer].cols, pTemplData->vecPyramid[iTopLayer].rows);
    //��һ�׶δ�����ҳ����µĽǶ���roi
    double dAngleStep = atan(2.0 / max(pTemplData->vecPyramid[iTopLayer].cols, pTemplData->vecPyramid[iTopLayer].rows)) * R2D;

    std::vector<double> vecAngles;
    if (m_dToleranceAngle < VISION_TOLERANCE)
        vecAngles.push_back(0.0);
    else
    {
        for (double dAngle = 0; dAngle < m_dToleranceAngle + dAngleStep; dAngle += dAngleStep)
            vecAngles.push_back(dAngle);
        for (double dAngle = -dAngleStep; dAngle > -m_dToleranceAngle - dAngleStep; dAngle -= dAngleStep)
            vecAngles.push_back(dAngle);
    }
    int iTopSrcW = vecMatSrcPyr[iTopLayer].cols, iTopSrcH = vecMatSrcPyr[iTopLayer].rows;
    cv::Point2f ptCenter((iTopSrcW - 1) / 2.0f, (iTopSrcH - 1) / 2.0f);
    int iSize = (int)vecAngles.size();
    //vector<s_MatchParameter> vecMatchParameter (iSize * (m_iMaxPos + MATCH_CANDIDATE_NUM));
    std::vector<s_MatchParameter> vecMatchParameter;
    //Caculate lowest score at every layer
    std::vector<double> vecLayerScore(iTopLayer + 1, m_dScore);
    for (int iLayer = 1; iLayer <= iTopLayer; iLayer++)
        vecLayerScore[iLayer] = vecLayerScore[iLayer - 1] * 0.9;
    cv::Size sizePat = pTemplData->vecPyramid[iTopLayer].size();
    bool bCalMaxByBlock = (vecMatSrcPyr[iTopLayer].size().area() / sizePat.area() > 500) && m_iMaxPos > 10;

    for (int i = 0; i < iSize; i++)
    {
        cv::Mat matRotatedSrc, matR = getRotationMatrix2D(ptCenter, vecAngles[i], 1);
        cv::Mat matResult;
        cv::Point ptMaxLoc;
        double dValue, dMaxVal;
        double dRotate = clock();
        cv::Size sizeBest = GetBestRotationSize(vecMatSrcPyr[iTopLayer].size(), pTemplData->vecPyramid[iTopLayer].size(), vecAngles[i]);

        float fTranslationX = (sizeBest.width - 1) / 2.0f - ptCenter.x;
        float fTranslationY = (sizeBest.height - 1) / 2.0f - ptCenter.y;
        matR.at<double>(0, 2) += fTranslationX;
        matR.at<double>(1, 2) += fTranslationY;
        warpAffine(vecMatSrcPyr[iTopLayer], matRotatedSrc, matR, sizeBest, INTER_LINEAR, BORDER_CONSTANT, Scalar(pTemplData->iBorderColor));

        MatchTemplate(matRotatedSrc, pTemplData, matResult, iTopLayer, false);

        if (bCalMaxByBlock)
        {
            s_BlockMax blockMax(matResult, pTemplData->vecPyramid[iTopLayer].size());
            blockMax.GetMaxValueLoc(dMaxVal, ptMaxLoc);
            if (dMaxVal < vecLayerScore[iTopLayer])
                continue;
            vecMatchParameter.push_back(s_MatchParameter(Point2f(ptMaxLoc.x - fTranslationX, ptMaxLoc.y - fTranslationY), dMaxVal, vecAngles[i]));
            for (int j = 0; j < m_iMaxPos + MATCH_CANDIDATE_NUM - 1; j++)
            {
                ptMaxLoc = GetNextMaxLoc(matResult, ptMaxLoc, pTemplData->vecPyramid[iTopLayer].size(), dValue, m_dMaxOverlap, blockMax);
                if (dMaxVal < vecLayerScore[iTopLayer])
                    continue;
                vecMatchParameter.push_back(s_MatchParameter(Point2f(ptMaxLoc.x - fTranslationX, ptMaxLoc.y - fTranslationY), dValue, vecAngles[i]));
            }
        }
        else
        {
            minMaxLoc(matResult, 0, &dMaxVal, 0, &ptMaxLoc);
            if (dMaxVal < vecLayerScore[iTopLayer])
                continue;
            vecMatchParameter.push_back(s_MatchParameter(Point2f(ptMaxLoc.x - fTranslationX, ptMaxLoc.y - fTranslationY), dMaxVal, vecAngles[i]));
            for (int j = 0; j < m_iMaxPos + MATCH_CANDIDATE_NUM - 1; j++)
            {
                ptMaxLoc = GetNextMaxLoc(matResult, ptMaxLoc, pTemplData->vecPyramid[iTopLayer].size(), dValue, m_dMaxOverlap);
                if (dMaxVal < vecLayerScore[iTopLayer])
                    continue;
                vecMatchParameter.push_back(s_MatchParameter(Point2f(ptMaxLoc.x - fTranslationX, ptMaxLoc.y - fTranslationY), dValue, vecAngles[i]));
            }
        }
    }
    sort(vecMatchParameter.begin(), vecMatchParameter.end(), compareScoreBig2Small);

    int iMatchSize = (int)vecMatchParameter.size();
    int iDstW = pTemplData->vecPyramid[iTopLayer].cols, iDstH = pTemplData->vecPyramid[iTopLayer].rows;

    //��һ�׶�debug�Y��
    //�Ƿ���������
    bool bSubPixelEstimation = true;
    int iStopLayer = 0;
    //int iSearchSize = min (m_iMaxPos + MATCH_CANDIDATE_NUM, (int)vecMatchParameter.size ());//���ܲ���Ҫ�ь���ȫ�� ̫���M�r�g
    vector<s_MatchParameter> vecAllResult;
    for (int i = 0; i < (int)vecMatchParameter.size(); i++)
        //for (int i = 0; i < iSearchSize; i++)
    {
        double dRAngle = -vecMatchParameter[i].dMatchAngle * D2R;
        Point2f ptLT = ptRotatePt2f(vecMatchParameter[i].pt, ptCenter, dRAngle);

        double dAngleStep = atan(2.0 / max(iDstW, iDstH)) * R2D;//min�Ğ�max
        vecMatchParameter[i].dAngleStart = vecMatchParameter[i].dMatchAngle - dAngleStep;
        vecMatchParameter[i].dAngleEnd = vecMatchParameter[i].dMatchAngle + dAngleStep;

        if (iTopLayer <= iStopLayer)
        {
            vecMatchParameter[i].pt = Point2d(ptLT * ((iTopLayer == 0) ? 1 : 2));
            vecAllResult.push_back(vecMatchParameter[i]);
        }
        else
        {
            for (int iLayer = iTopLayer - 1; iLayer >= iStopLayer; iLayer--)
            {
                //��Ѱ�Ƕ�
                dAngleStep = atan(2.0 / max(pTemplData->vecPyramid[iLayer].cols, pTemplData->vecPyramid[iLayer].rows)) * R2D;//min�Ğ�max
                vector<double> vecAngles;
                //double dAngleS = vecMatchParameter[i].dAngleStart, dAngleE = vecMatchParameter[i].dAngleEnd;
                double dMatchedAngle = vecMatchParameter[i].dMatchAngle;
                if (m_dToleranceAngle < VISION_TOLERANCE)
                    vecAngles.push_back(0.0);
                else
                    for (int i = -1; i <= 1; i++)
                        vecAngles.push_back(dMatchedAngle + dAngleStep * i);
                Point2f ptSrcCenter((vecMatSrcPyr[iLayer].cols - 1) / 2.0f, (vecMatSrcPyr[iLayer].rows - 1) / 2.0f);
                iSize = (int)vecAngles.size();
                vector<s_MatchParameter> vecNewMatchParameter(iSize);
                int iMaxScoreIndex = 0;
                double dBigValue = -1;
                for (int j = 0; j < iSize; j++)
                {
                    Mat matResult, matRotatedSrc;
                    double dMaxValue = 0;
                    Point ptMaxLoc;
                    GetRotatedROI(vecMatSrcPyr[iLayer], pTemplData->vecPyramid[iLayer].size(), ptLT * 2, vecAngles[j], matRotatedSrc);

                    MatchTemplate(matRotatedSrc, pTemplData, matResult, iLayer, true);
                    //matchTemplate (matRotatedSrc, pTemplData->vecPyramid[iLayer], matResult, CV_TM_CCOEFF_NORMED);
                    minMaxLoc(matResult, 0, &dMaxValue, 0, &ptMaxLoc);
                    vecNewMatchParameter[j] = s_MatchParameter(ptMaxLoc, dMaxValue, vecAngles[j]);

                    if (vecNewMatchParameter[j].dMatchScore > dBigValue)
                    {
                        iMaxScoreIndex = j;
                        dBigValue = vecNewMatchParameter[j].dMatchScore;
                    }
                    //�����ع�Ӌ
                    if (ptMaxLoc.x == 0 || ptMaxLoc.y == 0 || ptMaxLoc.x == matResult.cols - 1 || ptMaxLoc.y == matResult.rows - 1)
                        vecNewMatchParameter[j].bPosOnBorder = true;
                    if (!vecNewMatchParameter[j].bPosOnBorder)
                    {
                        for (int y = -1; y <= 1; y++)
                            for (int x = -1; x <= 1; x++)
                                vecNewMatchParameter[j].vecResult[x + 1][y + 1] = matResult.at<float>(ptMaxLoc + Point(x, y));
                    }
                    //�����ع���
                }
                if (vecNewMatchParameter[iMaxScoreIndex].dMatchScore < vecLayerScore[iLayer])
                    break;

                //�����ع���
                if (bSubPixelEstimation
                    && iLayer == 0
                    && (!vecNewMatchParameter[iMaxScoreIndex].bPosOnBorder)
                    && iMaxScoreIndex != 0
                    && iMaxScoreIndex != 2)
                {
                     double dNewX = 0, dNewY = 0, dNewAngle = 0;
                     SubPixEsimation(&vecNewMatchParameter, &dNewX, &dNewY, &dNewAngle, dAngleStep, iMaxScoreIndex);
                     vecNewMatchParameter[iMaxScoreIndex].pt = Point2d(dNewX, dNewY);
                     vecNewMatchParameter[iMaxScoreIndex].dMatchAngle = dNewAngle;
                }
                //�����ع�Ӌ
                double dNewMatchAngle = vecNewMatchParameter[iMaxScoreIndex].dMatchAngle;

                //������ϵ�ص���תʱ(GetRotatedROI)��(0, 0)
                Point2f ptPaddingLT = ptRotatePt2f(ptLT * 2, ptSrcCenter, dNewMatchAngle * D2R) - Point2f(3, 3);
                Point2f pt(vecNewMatchParameter[iMaxScoreIndex].pt.x + ptPaddingLT.x, vecNewMatchParameter[iMaxScoreIndex].pt.y + ptPaddingLT.y);
                //����ת
                pt = ptRotatePt2f(pt, ptSrcCenter, -dNewMatchAngle * D2R);

                if (iLayer == iStopLayer)
                {
                    vecNewMatchParameter[iMaxScoreIndex].pt = pt * (iStopLayer == 0 ? 1 : 2);
                    vecAllResult.push_back(vecNewMatchParameter[iMaxScoreIndex]);
                }
                else
                {
                    //����MatchAngle ptLT
                    vecMatchParameter[i].dMatchAngle = dNewMatchAngle;
                    vecMatchParameter[i].dAngleStart = vecMatchParameter[i].dMatchAngle - dAngleStep / 2;
                    vecMatchParameter[i].dAngleEnd = vecMatchParameter[i].dMatchAngle + dAngleStep / 2;
                    ptLT = pt;
                }
            }
        }
    }
    FilterWithScore(&vecAllResult, m_dScore);


    //����˵��ص�
    iDstW = pTemplData->vecPyramid[iStopLayer].cols, iDstH = pTemplData->vecPyramid[iStopLayer].rows;

    for (int i = 0; i < (int)vecAllResult.size(); i++)
    {
        Point2f ptLT, ptRT, ptRB, ptLB;
        double dRAngle = -vecAllResult[i].dMatchAngle * D2R;
        ptLT = vecAllResult[i].pt;
        ptRT = Point2f(ptLT.x + iDstW * (float)cos(dRAngle), ptLT.y - iDstW * (float)sin(dRAngle));
        ptLB = Point2f(ptLT.x + iDstH * (float)sin(dRAngle), ptLT.y + iDstH * (float)cos(dRAngle));
        ptRB = Point2f(ptRT.x + iDstH * (float)sin(dRAngle), ptRT.y + iDstH * (float)cos(dRAngle));
        //��¼��ת����
        Point2f ptRectCenter = Point2f((ptLT.x + ptRT.x + ptLB.x + ptRB.x) / 4.0f, (ptLT.y + ptRT.y + ptLB.y + ptRB.y) / 4.0f);
        vecAllResult[i].rectR = RotatedRect(ptRectCenter, pTemplData->vecPyramid[iStopLayer].size(), (float)vecAllResult[i].dMatchAngle);
    }
    FilterWithRotatedRect(&vecAllResult, CV_TM_CCOEFF_NORMED, m_dMaxOverlap);
    //����˵��ص����
    //���ݷ�������
    sort(vecAllResult.begin(), vecAllResult.end(), compareScoreBig2Small);

    iMatchSize = (int)vecAllResult.size();
    if (vecAllResult.size() == 0)
    {
        //ûƥ����
		patternNG();

        return resultPoint;
    }

    int iW = pTemplData->vecPyramid[0].cols, iH = pTemplData->vecPyramid[0].rows;
    for (int i = 0; i < iMatchSize; i++)
    {
        double dRAngle = -vecAllResult[i].dMatchAngle * D2R;

        sstm.ptLT = vecAllResult[i].pt;

        sstm.ptRT = Point2d(sstm.ptLT.x + iW * cos(dRAngle), sstm.ptLT.y - iW * sin(dRAngle));
        sstm.ptLB = Point2d(sstm.ptLT.x + iH * sin(dRAngle), sstm.ptLT.y + iH * cos(dRAngle));
        sstm.ptRB = Point2d(sstm.ptRT.x + iH * sin(dRAngle), sstm.ptRT.y + iH * cos(dRAngle));
        sstm.ptCenter = Point2d((sstm.ptLT.x + sstm.ptRT.x + sstm.ptRB.x + sstm.ptLB.x) / 4, (sstm.ptLT.y + sstm.ptRT.y + sstm.ptRB.y + sstm.ptLB.y) / 4);
        sstm.dMatchedAngle = -vecAllResult[i].dMatchAngle;
        sstm.dMatchScore = vecAllResult[i].dMatchScore;
        if (sstm.dMatchedAngle < -180)
            sstm.dMatchedAngle += 360;
        if (sstm.dMatchedAngle > 180)
            sstm.dMatchedAngle -= 360;

        //Test Subpixel
        //���MATCH ROI
        if (i + 1 == m_iMaxPos)
            break;
        if (sstm.ptCenter.x > 0 && sstm.ptCenter.y > 0) {
			if (modelflag)
			{
				return cv::Point2d(sstm.ptCenter.x, sstm.ptCenter.y);
			}
            lastResult.setX(areaMatRect.x + sstm.ptCenter.x);
            lastResult.setY(areaMatRect.y + sstm.ptCenter.y);
            resultPoint.x = areaMatRect.x + sstm.ptCenter.x;
			resultPoint.y = areaMatRect.y + sstm.ptCenter.y;


			//�������
			if ((!(centerPointInProcess.x() == 0 && centerPointInProcess.y() == 0)))
			{     
				
				QPoint centerPointx = calculateOffsetB(patternRectCenterPointInProcess, centerPointInProcess,initialDistance,initialDirection,QPoint(lastResult.x(), lastResult.y()));
				lastResult.setX(centerPointx.x());
				lastResult.setY(centerPointx.y());
			}
			//��ͼ�����ĵ�
			drawCenterPoint.x = lastResult.x();
			drawCenterPoint.y = lastResult.y();

            lastResult.setX(lastResult.x()- (m_width / 2));  //����Ϊx������
            lastResult.setY((m_height / 2) - lastResult.y());//����Ϊy������
			
		  
            finall_Total_Result.ptCenter = cv::Point2d(lastResult.x(), lastResult.y());
            finall_Total_Result.dMatchedAngle = sstm.dMatchedAngle;
            finall_Total_Result.pattern_flag = true;
            finall_Total_Result.flag = true;
        }
        return finall_Total_Result.ptCenter;
    }
}

void ProcessingThread::patternNG()
{
	//ûƥ����
		finall_Total_Result.ptCenter = cv::Point2d(-m_width, -m_height);
	finall_Total_Result.dMatchedAngle = sstm.dMatchedAngle;
	finall_Total_Result.pattern_flag = false;
	finall_Total_Result.flag = true;

	resultPoint.x = -m_width;
	resultPoint.y = -m_height;
}

Size ProcessingThread::GetBestRotationSize(Size sizeSrc, Size sizeDst, double dRAngle)
{

	double dRAngle_radian = dRAngle * D2R;
	Point ptLT(0, 0), ptLB(0, sizeSrc.height - 1), ptRB(sizeSrc.width - 1, sizeSrc.height - 1), ptRT(sizeSrc.width - 1, 0);
	Point2f ptCenter((sizeSrc.width - 1) / 2.0f, (sizeSrc.height - 1) / 2.0f);
	Point2f ptLT_R = ptRotatePt2f(Point2f(ptLT), ptCenter, dRAngle_radian);
	Point2f ptLB_R = ptRotatePt2f(Point2f(ptLB), ptCenter, dRAngle_radian);
	Point2f ptRB_R = ptRotatePt2f(Point2f(ptRB), ptCenter, dRAngle_radian);
	Point2f ptRT_R = ptRotatePt2f(Point2f(ptRT), ptCenter, dRAngle_radian);

	float fTopY = max(max(ptLT_R.y, ptLB_R.y), max(ptRB_R.y, ptRT_R.y));
	float fBottomY = min(min(ptLT_R.y, ptLB_R.y), min(ptRB_R.y, ptRT_R.y));
	float fRightX = max(max(ptLT_R.x, ptLB_R.x), max(ptRB_R.x, ptRT_R.x));
	float fLeftX = min(min(ptLT_R.x, ptLB_R.x), min(ptRB_R.x, ptRT_R.x));

	if (dRAngle > 360)
		dRAngle -= 360;
	else if (dRAngle < 0)
		dRAngle += 360;

	if (fabs(fabs(dRAngle) - 90) < VISION_TOLERANCE || fabs(fabs(dRAngle) - 270) < VISION_TOLERANCE)
	{
		return Size(sizeSrc.height, sizeSrc.width);
	}
	else if (fabs(dRAngle) < VISION_TOLERANCE || fabs(fabs(dRAngle) - 180) < VISION_TOLERANCE)
	{
		return sizeSrc;
	}

	double dAngle = dRAngle;

	if (dAngle > 0 && dAngle < 90)
	{
		;
	}
	else if (dAngle > 90 && dAngle < 180)
	{
		dAngle -= 90;
	}
	else if (dAngle > 180 && dAngle < 270)
	{
		dAngle -= 180;
	}
	else if (dAngle > 270 && dAngle < 360)
	{
		dAngle -= 270;
	}
	else//Debug
	{
		//AfxMessageBox(L"Unkown");
	}

	float fH1 = sizeDst.width * sin(dAngle * D2R) * cos(dAngle * D2R);
	float fH2 = sizeDst.height * sin(dAngle * D2R) * cos(dAngle * D2R);

	int iHalfHeight = (int)ceil(fTopY - ptCenter.y - fH1);
	int iHalfWidth = (int)ceil(fRightX - ptCenter.x - fH2);

	Size sizeRet(iHalfWidth * 2, iHalfHeight * 2);

	BOOL bWrongSize = (sizeDst.width < sizeRet.width&& sizeDst.height > sizeRet.height)
		|| (sizeDst.width > sizeRet.width && sizeDst.height < sizeRet.height
			|| sizeDst.area() > sizeRet.area());
	if (bWrongSize)
		sizeRet = Size(int(fRightX - fLeftX + 0.5), int(fTopY - fBottomY + 0.5));

	return sizeRet;
}


Point2f ProcessingThread::ptRotatePt2f(Point2f ptInput, Point2f ptOrg, double dAngle) {
	double dWidth = ptOrg.x * 2;
	double dHeight = ptOrg.y * 2;
	double dY1 = dHeight - ptInput.y, dY2 = dHeight - ptOrg.y;

	double dX = (ptInput.x - ptOrg.x) * cos(dAngle) - (dY1 - ptOrg.y) * sin(dAngle) + ptOrg.x;
	double dY = (ptInput.x - ptOrg.x) * sin(dAngle) + (dY1 - ptOrg.y) * cos(dAngle) + dY2;

	dY = -dY + dHeight;
	return Point2f((float)dX, (float)dY);
}

void ProcessingThread::MatchTemplate(cv::Mat& matSrc, s_TemplData* pTemplData, cv::Mat& matResult, int iLayer, bool bUseSIMD)
{
	cv::matchTemplate(matSrc, pTemplData->vecPyramid[iLayer], matResult, cv::TM_CCORR);
	CCOEFF_Denominator(matSrc, pTemplData, matResult, iLayer);
}

void ProcessingThread::CCOEFF_Denominator(cv::Mat& matSrc, s_TemplData* pTemplData, cv::Mat& matResult, int iLayer)
{
	if (pTemplData->vecResultEqual1[iLayer])
	{
		matResult = Scalar::all(1);
		return;
	}
	double* q0 = 0, * q1 = 0, * q2 = 0, * q3 = 0;

	Mat sum, sqsum;
	integral(matSrc, sum, sqsum, CV_64F);

	double d2 = clock();

	q0 = (double*)sqsum.data;
	q1 = q0 + pTemplData->vecPyramid[iLayer].cols;
	q2 = (double*)(sqsum.data + pTemplData->vecPyramid[iLayer].rows * sqsum.step);
	q3 = q2 + pTemplData->vecPyramid[iLayer].cols;

	double* p0 = (double*)sum.data;
	double* p1 = p0 + pTemplData->vecPyramid[iLayer].cols;
	double* p2 = (double*)(sum.data + pTemplData->vecPyramid[iLayer].rows * sum.step);
	double* p3 = p2 + pTemplData->vecPyramid[iLayer].cols;

	int sumstep = sum.data ? (int)(sum.step / sizeof(double)) : 0;
	int sqstep = sqsum.data ? (int)(sqsum.step / sizeof(double)) : 0;

	//
	double dTemplMean0 = pTemplData->vecTemplMean[iLayer][0];
	double dTemplNorm = pTemplData->vecTemplNorm[iLayer];
	double dInvArea = pTemplData->vecInvArea[iLayer];
	//

	int i, j;
	for (i = 0; i < matResult.rows; i++)
	{
		float* rrow = matResult.ptr<float>(i);
		int idx = i * sumstep;
		int idx2 = i * sqstep;

		for (j = 0; j < matResult.cols; j += 1, idx += 1, idx2 += 1)
		{
			double num = rrow[j], t;
			double wndMean2 = 0, wndSum2 = 0;

			t = p0[idx] - p1[idx] - p2[idx] + p3[idx];
			wndMean2 += t * t;
			num -= t * dTemplMean0;
			wndMean2 *= dInvArea;


			t = q0[idx2] - q1[idx2] - q2[idx2] + q3[idx2];
			wndSum2 += t;


			//t = std::sqrt (MAX (wndSum2 - wndMean2, 0)) * dTemplNorm;

			double diff2 = MAX(wndSum2 - wndMean2, 0);
			if (diff2 <= min(0.5, 10 * FLT_EPSILON * wndSum2))
				t = 0; // avoid rounding errors
			else
				t = std::sqrt(diff2) * dTemplNorm;

			if (fabs(num) < t)
				num /= t;
			else if (fabs(num) < t * 1.125)
				num = num > 0 ? 1 : -1;
			else
				num = 0;

			rrow[j] = (float)num;
		}
	}
}

Point ProcessingThread::GetNextMaxLoc(Mat& matResult, Point ptMaxLoc, Size sizeTemplate, double& dMaxValue, double dMaxOverlap, s_BlockMax& blockMax)
{
	//�ȶԵ������������¿����ص�����
	int iStartX = int(ptMaxLoc.x - sizeTemplate.width * (1 - dMaxOverlap));
	int iStartY = int(ptMaxLoc.y - sizeTemplate.height * (1 - dMaxOverlap));
	cv::Rect rectIgnore(iStartX, iStartY, int(2 * sizeTemplate.width * (1 - dMaxOverlap))
		, int(2 * sizeTemplate.height * (1 - dMaxOverlap)));
	//Ϳ��
	rectangle(matResult, rectIgnore, Scalar(-1), CV_FILLED);
	blockMax.UpdateMax(rectIgnore);
	Point ptReturn;
	blockMax.GetMaxValueLoc(dMaxValue, ptReturn);
	return ptReturn;
}


Point ProcessingThread::GetNextMaxLoc(Mat& matResult, Point ptMaxLoc, Size sizeTemplate, double& dMaxValue, double dMaxOverlap)
{
	//�Ȍ����ą^����ȫ���دB : +-һ���Ӱ匒��
	//int iStartX = ptMaxLoc.x - iTemplateW;
	//int iStartY = ptMaxLoc.y - iTemplateH;
	//int iEndX = ptMaxLoc.x + iTemplateW;

	//int iEndY = ptMaxLoc.y + iTemplateH;
	////�T��
	//rectangle (matResult, Rect (iStartX, iStartY, 2 * iTemplateW * (1-dMaxOverlap * 2), 2 * iTemplateH * (1-dMaxOverlap * 2)), Scalar (dMinValue), CV_FILLED);
	////�õ���һ�����ֵ
	//Point ptNewMaxLoc;
	//minMaxLoc (matResult, 0, &dMaxValue, 0, &ptNewMaxLoc);
	//return ptNewMaxLoc;

	//�ص��������ص���Χ
	int iStartX = ptMaxLoc.x - sizeTemplate.width * (1 - dMaxOverlap);
	int iStartY = ptMaxLoc.y - sizeTemplate.height * (1 - dMaxOverlap);
	//Ϳ��
	rectangle(matResult, cv::Rect(iStartX, iStartY, 2 * sizeTemplate.width * (1 - dMaxOverlap), 2 * sizeTemplate.height * (1 - dMaxOverlap)), Scalar(-1), CV_FILLED);
	//�õ���һ�����ֵ
	Point ptNewMaxLoc;
	minMaxLoc(matResult, 0, &dMaxValue, 0, &ptNewMaxLoc);
	return ptNewMaxLoc;
}

void ProcessingThread::GetRotatedROI(Mat& matSrc, Size size, Point2f ptLT, double dAngle, Mat& matROI)
{
	double dAngle_radian = dAngle * D2R;
	Point2f ptC((matSrc.cols - 1) / 2.0f, (matSrc.rows - 1) / 2.0f);
	Point2f ptLT_rotate = ptRotatePt2f(ptLT, ptC, dAngle_radian);
	Size sizePadding(size.width + 6, size.height + 6);

	Mat rMat = getRotationMatrix2D(ptC, dAngle, 1);
	rMat.at<double>(0, 2) -= ptLT_rotate.x - 3;
	rMat.at<double>(1, 2) -= ptLT_rotate.y - 3;
	//ƽ����ת����(0, 2) (1, 2)�ĲΪ��ת���ԭ��ƫ�ƣ�-= ptLT_rotate.x - 3 ������ת��ķ���-X�����Ƅ�ptLT_rotate.x - 3
	//Debug

	//Debug
	warpAffine(matSrc, matROI, rMat, sizePadding);
}

bool ProcessingThread::SubPixEsimation(std::vector<s_MatchParameter>* vec, double* dNewX, double* dNewY, double* dNewAngle, double dAngleStep, int iMaxScoreIndex)
{
	//Az=S, (A.T)Az=(A.T)s, z = ((A.T)A).inv (A.T)s

	Mat matA(27, 10, CV_64F);
	Mat matZ(10, 1, CV_64F);
	Mat matS(27, 1, CV_64F);

	double dX_maxScore = (*vec)[iMaxScoreIndex].pt.x;
	double dY_maxScore = (*vec)[iMaxScoreIndex].pt.y;
	double dTheata_maxScore = (*vec)[iMaxScoreIndex].dMatchAngle;
	int iRow = 0;
	/*for (int x = -1; x <= 1; x++)
	{
		for (int y = -1; y <= 1; y++)
		{
			for (int theta = 0; theta <= 2; theta++)
			{*/
	for (int theta = 0; theta <= 2; theta++)
	{
		for (int y = -1; y <= 1; y++)
		{
			for (int x = -1; x <= 1; x++)
			{
				//xx yy tt xy xt yt x y t 1
				//0  1  2  3  4  5  6 7 8 9
				double dX = dX_maxScore + x;
				double dY = dY_maxScore + y;
				//double dT = (*vec)[theta].dMatchAngle + (theta - 1) * dAngleStep;
				double dT = (dTheata_maxScore + (theta - 1) * dAngleStep) * D2R;
				matA.at<double>(iRow, 0) = dX * dX;
				matA.at<double>(iRow, 1) = dY * dY;
				matA.at<double>(iRow, 2) = dT * dT;
				matA.at<double>(iRow, 3) = dX * dY;
				matA.at<double>(iRow, 4) = dX * dT;
				matA.at<double>(iRow, 5) = dY * dT;
				matA.at<double>(iRow, 6) = dX;
				matA.at<double>(iRow, 7) = dY;
				matA.at<double>(iRow, 8) = dT;
				matA.at<double>(iRow, 9) = 1.0;
				matS.at<double>(iRow, 0) = (*vec)[iMaxScoreIndex + (theta - 1)].vecResult[x + 1][y + 1];
				iRow++;
#ifdef _DEBUG
				/*string str = format ("%.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f", dValueA[0], dValueA[1], dValueA[2], dValueA[3], dValueA[4], dValueA[5], dValueA[6], dValueA[7], dValueA[8], dValueA[9]);
				fileA <<  str << endl;
				str = format ("%.6f", dValueS[iRow]);
				fileS << str << endl;*/
#endif
			}
		}
	}
	//���Z��ꇣ��õ�k0~k9
	//[ x* ] = [ 2k0 k3 k4 ]-1 [ -k6 ]
	//| y* | = | k3 2k1 k5 |   | -k7 |
	//[ t* ] = [ k4 k5 2k2 ]   [ -k8 ]

	//solve (matA, matS, matZ, DECOMP_SVD);
	matZ = (matA.t() * matA).inv() * matA.t() * matS;
	Mat matZ_t;
	transpose(matZ, matZ_t);
	double* dZ = matZ_t.ptr<double>(0);
	Mat matK1 = (Mat_<double>(3, 3) <<
		(2 * dZ[0]), dZ[3], dZ[4],
		dZ[3], (2 * dZ[1]), dZ[5],
		dZ[4], dZ[5], (2 * dZ[2]));
	Mat matK2 = (Mat_<double>(3, 1) << -dZ[6], -dZ[7], -dZ[8]);
	Mat matDelta = matK1.inv() * matK2;

	*dNewX = matDelta.at<double>(0, 0);
	*dNewY = matDelta.at<double>(1, 0);
	*dNewAngle = matDelta.at<double>(2, 0) * R2D;
	return TRUE;
}

void ProcessingThread::SortPtWithCenter(std::vector<cv::Point2f>& vecSort)
{
	int iSize = (int)vecSort.size();
	Point2f ptCenter;
	for (int i = 0; i < iSize; i++)
		ptCenter += vecSort[i];
	ptCenter /= iSize;

	Point2f vecX(1, 0);

	vector<pair<Point2f, double>> vecPtAngle(iSize);
	for (int i = 0; i < iSize; i++)
	{
		vecPtAngle[i].first = vecSort[i];//pt
		Point2f vec1(vecSort[i].x - ptCenter.x, vecSort[i].y - ptCenter.y);
		float fNormVec1 = vec1.x * vec1.x + vec1.y * vec1.y;
		float fDot = vec1.x;

		if (vec1.y < 0)//���������ĵ��Ϸ�
		{
			vecPtAngle[i].second = acos(fDot / fNormVec1) * R2D;
		}
		else if (vec1.y > 0)//�·�
		{
			vecPtAngle[i].second = 360 - acos(fDot / fNormVec1) * R2D;
		}
		else//������������ͬY
		{
			if (vec1.x - ptCenter.x > 0)
				vecPtAngle[i].second = 0;
			else
				vecPtAngle[i].second = 180;
		}

	}
	std::sort(vecPtAngle.begin(), vecPtAngle.end(), comparePtWithAngle);
	for (int i = 0; i < iSize; i++)
		vecSort[i] = vecPtAngle[i].first;
}

void ProcessingThread::FilterWithScore(std::vector<s_MatchParameter>* vec, double dScore)
{
	sort(vec->begin(), vec->end(), compareScoreBig2Small);
	int iSize = vec->size(), iIndexDelete = iSize + 1;
	for (int i = 0; i < iSize; i++)
	{
		if ((*vec)[i].dMatchScore < dScore)
		{
			iIndexDelete = i;
			break;
		}
	}
	if (iIndexDelete == iSize + 1)//�]���κ�Ԫ��С��dScore
		return;
	vec->erase(vec->begin() + iIndexDelete, vec->end());
	return;
}

void ProcessingThread::FilterWithRotatedRect(vector<s_MatchParameter>* vec, int iMethod, double dMaxOverLap)
{
	int iMatchSize = (int)vec->size();
	RotatedRect rect1, rect2;
	for (int i = 0; i < iMatchSize - 1; i++)
	{
		if (vec->at(i).bDelete)
			continue;
		for (int j = i + 1; j < iMatchSize; j++)
		{
			if (vec->at(j).bDelete)
				continue;
			rect1 = vec->at(i).rectR;
			rect2 = vec->at(j).rectR;
			vector<Point2f> vecInterSec;
			int iInterSecType = rotatedRectangleIntersection(rect1, rect2, vecInterSec);
			if (iInterSecType == INTERSECT_NONE)//�޽���
				continue;
			else if (iInterSecType == INTERSECT_FULL) //һ�����ΰ�������һ��
			{
				int iDeleteIndex;
				if (iMethod == CV_TM_SQDIFF)
					iDeleteIndex = (vec->at(i).dMatchScore <= vec->at(j).dMatchScore) ? j : i;
				else
					iDeleteIndex = (vec->at(i).dMatchScore >= vec->at(j).dMatchScore) ? j : i;
				vec->at(iDeleteIndex).bDelete = TRUE;
			}
			else//���� > 0
			{
				if (vecInterSec.size() < 3)//һ��������������
					continue;
				else
				{
					int iDeleteIndex;
					//��������ص��ı���
					SortPtWithCenter(vecInterSec);
					double dArea = contourArea(vecInterSec);
					double dRatio = dArea / rect1.size.area();
					//�����������ص�������ѡ�����ߵ�
					if (dRatio > dMaxOverLap)
					{
						if (iMethod == CV_TM_SQDIFF)
							iDeleteIndex = (vec->at(i).dMatchScore <= vec->at(j).dMatchScore) ? j : i;
						else
							iDeleteIndex = (vec->at(i).dMatchScore >= vec->at(j).dMatchScore) ? j : i;
						vec->at(iDeleteIndex).bDelete = TRUE;
					}
				}
			}
		}
	}
	vector<s_MatchParameter>::iterator it;
	for (it = vec->begin(); it != vec->end();)
	{
		if ((*it).bDelete)
			it = vec->erase(it);
		else
			++it;
	}
}


void ProcessingThread::slot_processThread_Pattren()
{
	pattern_Flag = true;
}

void ProcessingThread::set_Grade(QString grade)
{
	// ��QStringת��Ϊ����
	int m_dScoreint = grade.toInt();

	// ���ֵ�Ƿ��� [0, 100] ��Χ��
	if (m_dScoreint >= 0 && m_dScoreint <= 100)
	{
		// ����ת��Ϊ������λС����ʮ������
		double m_dScoreDecimal = static_cast<double>(m_dScoreint) / 100.0;

		// ʹ�� 'f' ��ʽ�淶��ʮ��������ʽ��Ϊ������λС����QString
		QString formattedGrade = QString::number(m_dScoreDecimal, 'f', 2);

		m_dScore = formattedGrade.toDouble();
	}
	else
	{

		m_dScore = 0.8;
	}
}

//ģ��ƥ����洰������
void ProcessingThread::slot_processMatchPicture(QImage patternImage, QImage sourceImage)
{

	patternImage = patternImage.convertToFormat(QImage::Format_RGB888);
	sourceImage = sourceImage.convertToFormat(QImage::Format_RGB888);

	cv::Mat patternImageMat = Mat(patternImage.height(),
		patternImage.width(),
		CV_8UC(3),
		patternImage.bits(),
		patternImage.bytesPerLine());
//ImageToMat(patternImage);
		cv::cvtColor(patternImageMat, patternImageMat, COLOR_BGR2GRAY);

	cv::Mat sourceImageMat = Mat(sourceImage.height(),
		sourceImage.width(),
		CV_8UC(3),
		sourceImage.bits(),
		sourceImage.bytesPerLine());
		//ImageToMat(sourceImage);
	cv::cvtColor(sourceImageMat, sourceImageMat, COLOR_BGR2GRAY);

	m_TemplData_model.clear();
	int iTopLayer = GetTopLayer(&patternImageMat, (int)sqrt((double)256));
	cv::buildPyramid(patternImageMat, m_TemplData_model.vecPyramid, iTopLayer);
	s_TemplData* templData = &m_TemplData_model;
	templData->iBorderColor = mean(patternImageMat).val[0] < 128 ? 255 : 0;
	int iSize = templData->vecPyramid.size();
	templData->resize(iSize);
	for (int i = 0; i < iSize; i++)
	{
		double invArea = 1. / ((double)templData->vecPyramid[i].rows * templData->vecPyramid[i].cols);
		cv::Scalar templMean, templSdv;
		double templNorm = 0, templSum2 = 0;

		meanStdDev(templData->vecPyramid[i], templMean, templSdv);
		templNorm = templSdv[0] * templSdv[0] + templSdv[1] * templSdv[1] + templSdv[2] * templSdv[2] + templSdv[3] * templSdv[3];

		if (templNorm < DBL_EPSILON)
		{
			templData->vecResultEqual1[i] = true;
		}
		templSum2 = templNorm + templMean[0] * templMean[0] + templMean[1] * templMean[1] + templMean[2] * templMean[2] + templMean[3] * templMean[3];


		templSum2 /= invArea;
		templNorm = std::sqrt(templNorm);
		templNorm /= std::sqrt(invArea); // care of accuracy here


		templData->vecInvArea[i] = invArea;
		templData->vecTemplMean[i] = templMean;
		templData->vecTemplNorm[i] = templNorm;
	}
	//�������������

	templData->bIsPatternLearned = true;

	QTime timedebuge;//����һ��ʱ�Ӷ���
	timedebuge.start();//��ʼ��ʱ

	cv::Point2d resultPoint = MatchPicture(patternImageMat, sourceImageMat, true);

	int total_time = timedebuge.elapsed();
	
	emit QPointSendtoFileControl(QPoint(resultPoint.x, resultPoint.y), total_time);
}


//��תͼ��
Mat ImageRotate(Mat image, double angle)
{
	Mat newImg;
	Point2f pt = Point2f((float)image.cols / 2, (float)image.rows / 2);
	Mat M = getRotationMatrix2D(pt, angle, 1.0);
	warpAffine(image, newImg, M, image.size());
	return newImg;
}

ResultPoint ProcessingThread::slot_processMatchPictureWithSource( cv::Mat sourceImage)
{            
	QTime timedebuge;//����һ��ʱ�Ӷ���
	timedebuge.start();//��ʼ��ʱ

	cv::Mat patternImageMat = patternMatEllipse;
	cv::Mat sourceImageMat = sourceImage;
	cv::Mat maskImageMat = patternMatEllipseMask;
	int patternWidth = patternImageMat.cols;
	int patternHeight = patternImageMat.rows;

	double step = 2;
	double start = 0;
	double range = 10;

	//����ͼƬƥ������Ҫ�Ĳ���
	int resultCols = sourceImageMat.cols - patternImageMat.cols + 1;
	int resultRows = sourceImageMat.rows - patternImageMat.rows + 1;
	Mat result = Mat(resultCols, resultRows, CV_8U);
	Mat src, model, mask, maskTemplate;
	sourceImageMat.copyTo(src);
	patternImageMat.copyTo(model);
	maskImageMat.copyTo(mask);
	//��ģ��ͼ��ʹ����ͼ��ֱ����ͼ��������²���  3��
	for (int i = 0; i < 3; i++)
	{
		pyrDown(src, src, Size(src.cols / 2, src.rows / 2));
		pyrDown(model, model, Size(model.cols / 2, model.rows / 2));
		pyrDown(mask, mask, Size(mask.cols / 2, mask.rows / 2));
		// ��ֵ���������ֶ�ֵͼ����
		cv::threshold(mask, mask, 128, 255, cv::THRESH_BINARY);
	}
	mask.copyTo(maskTemplate);


	TemplateMatchModes matchMode = TM_CCOEFF_NORMED;

	//��û����ת������½��е�һ��ƥ��
	double minVal, maxVal;
	Point minLoc, maxLoc;
	matchTemplate(src, model, result, matchMode, mask);
	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);
	Point location = maxLoc;
	double temp = maxVal;
	if (isinf(temp))
	{
		temp = 0;
		//��һ�ξ�û�ҵ�
		patternNG();
		//return ResultPoint(-m_width, -m_height,0,0);
	}
	double angle = 0;
	Mat newImg;
	Mat maskImg;

	do
	{
		for (int i = 0; i <= (int)range / step; i++)
		{
			newImg = ImageRotate(model, start + step * i);
			maskImg = ImageRotate(maskTemplate, start + step * i);

			matchTemplate(src, newImg, result, matchMode, maskImg);
			double minval, maxval;
			Point minloc, maxloc;
			minMaxLoc(result, &minval, &maxval, &minloc, &maxloc);
			if (isinf(maxval))
			{
				maxval = 0;
			}
			if (maxval > temp)
			{
				location = maxloc;
				temp = maxval;
				angle = start + step * i;
			}
		}
		range = step * 2; 
		start = angle - step;
		step = step / 10;
	} while (step > 5);                  
	if (temp > m_dScore)
	{
		ResultPoint itemn(areaMatRect.x+location.x * pow(2, 3) + patternWidth / 2 , areaMatRect.y+location.y * pow(2, 3)+ patternHeight / 2, -angle, temp);

		drawCenterPoint.x = itemn.X;
		drawCenterPoint.y = itemn.Y;
		lastResult.setX(itemn.X);
		lastResult.setY(itemn.Y);
		//�������
		if ((!(centerPointInProcess.x() == 0 && centerPointInProcess.y() == 0)))
		{

			QPoint centerPointx = calculateOffsetB(patternRectCenterPointInProcess, centerPointInProcess, initialDistance, initialDirection, QPoint(drawCenterPoint.x, drawCenterPoint.y));
			lastResult.setX(centerPointx.x());
			lastResult.setY(centerPointx.y());
			drawCenterPoint.x = centerPointx.x();
			drawCenterPoint.y = centerPointx.y();
		}

		lastResult.setX(lastResult.x() - (m_width / 2));  //����Ϊx������
		lastResult.setY((m_height / 2) - lastResult.y());//����Ϊy������

		
		finall_Total_Result.ptCenter = cv::Point2d(lastResult.x(), lastResult.y());
		finall_Total_Result.dMatchedAngle = itemn.T;
		finall_Total_Result.pattern_flag = true;
		finall_Total_Result.flag = true;
		int total_time = timedebuge.elapsed();
		//emit QPointSendtoFileControl(QPoint(itemn.X + patternWidth / 2, itemn.Y + patternHeight / 2), total_time);
		return itemn;
	}
	else {
		//δƥ��ɹ�
		patternNG();
		return ResultPoint(-m_width, -m_height,0,0);

	}

}


ResultPoint ProcessingThread::slot_processMatchPictureWithMask(QImage patternImage, QImage sourceImage,QImage maskImage)
{
	QTime timedebuge;//����һ��ʱ�Ӷ���
	timedebuge.start();//��ʼ��ʱ

	cv::Mat patternImageMat = ImageToMat(patternImage);
	cv::Mat sourceImageMat = ImageToMat(sourceImage);
	cv::Mat maskImageMat = ImageToMat(maskImage);
	int patternWidth = patternImageMat.cols;
	int patternHeight = patternImageMat.rows;

	double step = 1;
	double start = 0;
	double range = 20;

	//����ͼƬƥ������Ҫ�Ĳ���
	int resultCols = sourceImageMat.cols - patternImageMat.cols + 1;
	int resultRows = sourceImageMat.rows - patternImageMat.rows + 1;
	Mat result = Mat(resultCols, resultRows, CV_8U);
	Mat src, model,mask,maskTemplate;
	sourceImageMat.copyTo(src);
	patternImageMat.copyTo(model);
	maskImageMat.copyTo(mask);
	//��ģ��ͼ��ʹ����ͼ��ֱ����ͼ��������²���  3��
	for (int i = 0; i <  3; i++)
	{
		pyrDown(src, src, Size(src.cols / 2, src.rows / 2));
		pyrDown(model, model, Size(model.cols / 2, model.rows / 2));
		pyrDown(mask, mask, Size(mask.cols / 2, mask.rows / 2));
	 // ��ֵ���������ֶ�ֵͼ����
		cv::threshold(mask, mask, 128, 255, cv::THRESH_BINARY);
	}
	mask.copyTo(maskTemplate);

	TemplateMatchModes matchMode = TM_CCOEFF_NORMED;

	//��û����ת������½��е�һ��ƥ��
	double minVal, maxVal;
	Point minLoc, maxLoc;
	matchTemplate(src, model, result, matchMode,mask);
	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);
	Point location = maxLoc;
	double temp = maxVal;
	double angle = 0;
	Mat newImg;
	Mat maskImageWith;

	do
	{
		for (int i = 0; i <= (int)range / step; i++) 
		{
			newImg = ImageRotate(model, start + step * i);
			maskImageWith = ImageRotate(maskTemplate, start + step * i);
			cv::threshold(maskImageWith, maskImageWith, 128, 255, cv::THRESH_BINARY);

			matchTemplate(src, newImg, result, matchMode, maskImageWith);
			double minval, maxval;
			Point minloc, maxloc;
			minMaxLoc(result, &minval, &maxval,                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               &minloc, &maxloc);
			if (maxval > temp)
			{
				location = maxloc;
				temp = maxval;
				angle = start + step * i;
			}
		}
		range = step * 2;
		start = angle - step;
		step = step / 10;
	} while (step > 2);                                                                                                                                                                                                                                                                                                                                                          
	if (temp > m_dScore)
	{
		 ResultPoint itemn(location.x * pow(2, 3), location.y * pow(2, 3), angle, temp);
		 int total_time = timedebuge.elapsed();
		 emit QPointSendtoFileControl(QPoint(itemn.X + patternWidth/2 , itemn.Y + patternHeight / 2), total_time);
		 return itemn;
	}
}


void ProcessingThread::slot_setSourceArea(bool flag)
{
	area_Flag = flag;
}

void ProcessingThread::slot_recievePatternImageWithMask(QString pattern_Path, QRectF pattern_Rect, QRectF areaRect, QPoint centerPoint, QPoint patternRectCenterPoint)
{
	areaMatRect.x = areaRect.x();
	areaMatRect.y = areaRect.y();
	areaMatRect.width = areaRect.width();
	areaMatRect.height = areaRect.height();


	String pattern_STD_Path = pattern_Path.toLocal8Bit().constData();
	size_t pos = pattern_STD_Path.find("="); // �ҵ��Ⱥŵ�λ��
	if (pos != string::npos) { // ����ҵ��˵Ⱥ�
		pattern_STD_Path.erase(pos, 1); // ɾ���Ⱥ��ַ�
	}

	//ԭͼ
	Mat ReadImagestd = imread(pattern_STD_Path, CV_8UC1);

	if (ReadImagestd.empty())
	{
		emit signal_modelPictureReadFlag();
		//ģ��ͼ��ȡ����!!!
		return;
	}

	Mat patternMatEllipse1 = ReadImagestd(Rect(pattern_Rect.x(), pattern_Rect.y(), pattern_Rect.width(), pattern_Rect.height())).clone();
	patternMatEllipse = patternMatEllipse1;
	// ������Բ����
	cv::Point center(pattern_Rect.x() + pattern_Rect.width() / 2, pattern_Rect.y()
		+ pattern_Rect.height() / 2);
	int width = pattern_Rect.width();
	int length = pattern_Rect.height();

	// ������Բ������
	cv::Mat mask = cv::Mat::zeros(ReadImagestd.size(), CV_8UC1);
	cv::ellipse(mask, center, cv::Size(width / 2, length / 2), 0, 0, 360, 255, -1);
	cv::Rect patternAreaRealSize(pattern_Rect.x(), pattern_Rect.y(), pattern_Rect.width(), pattern_Rect.height());

	patternMatEllipseMask = mask(patternAreaRealSize);	

	//�������������
	centerPointInProcess.setX(centerPoint.x());
	centerPointInProcess.setY(centerPoint.y());

	patternRectCenterPointInProcess.setX(patternRectCenterPoint.x());
	patternRectCenterPointInProcess.setY(patternRectCenterPoint.y());

	initialDistance = calculateInitialDistance(patternRectCenterPoint, centerPoint);    // Initial distance between A and B
	initialDirection = calculateInitialDirection(patternRectCenterPoint, centerPoint);  // Initial direction angle in degrees

}

void ProcessingThread::slot_recievePatternImageWithPolygonMask(QString pattern_Path, QPolygonF pattern_Rect, QRectF areaRect, QPoint centerPoint, QPoint patternRectCenterPoint)
{
	areaMatRect.x = areaRect.x();
	areaMatRect.y = areaRect.y();
	areaMatRect.width = areaRect.width();
	areaMatRect.height = areaRect.height();


	QString appleModel = QApplication::applicationDirPath() + "/model/";
	QString dirpath = appleModel + pattern_Path;

	std::string readImage = dirpath.toLocal8Bit();
	
	//ԭͼ
	Mat ReadImagestd = imread(readImage, CV_8UC1);
	if (ReadImagestd.empty())
	{
		emit signal_modelPictureReadFlag();
		//ģ��ͼ��ȡ����!!!
		return;
	}

	//���ģ��ͼ
		// ��QPolygonF�еĵ�����ת��Ϊvector<cv::Point>
	std::vector<cv::Point> points;
	for (const QPointF& point : pattern_Rect) {
		points.emplace_back(static_cast<int>(point.x()), static_cast<int>(point.y()));
	}
	points.pop_back();
	// ����һ����ͼ����ͬ��С�ĺ�ɫ����
	cv::Mat mask = cv::Mat::zeros(ReadImagestd.size(), ReadImagestd.type());


	// ��������ת��Ϊvector<vector<cv::Point>>��ʽ�Է���cv::fillPoly��Ҫ��
	std::vector<std::vector<cv::Point>> pts = { points };
	// �������������������
	cv::fillPoly(mask, pts, cv::Scalar(255, 255, 255));

	// ʹ��������ȡ���������
	cv::Mat result;
	cv::bitwise_and(ReadImagestd, mask, result);
	// ���Ұ�������ε���С���� �߽��
	cv::Rect boundingRect = cv::boundingRect(points);

	// �ü�ͼ��ȥ���Աߵĺ�ɫ����
	cv::Mat croppedResult = result(boundingRect).clone();


	// ѡ���ʵ�����ֵ������ʹ�� 128 ��Ϊ���ӣ����Ը���ʵ���������
	int threshold_value = 1;

	// Ӧ�ö�ֵ������
	//ģ��ͼ,maskͼ
	cv::threshold(croppedResult, mask, threshold_value, 255, cv::THRESH_BINARY);

	//maskͼ
	patternMatEllipseMask = mask;
	//ģ��ͼ
	patternMatEllipse = croppedResult;



	//�������������
	centerPointInProcess.setX(centerPoint.x());
	centerPointInProcess.setY(centerPoint.y());

	patternRectCenterPointInProcess.setX(patternRectCenterPoint.x());
	patternRectCenterPointInProcess.setY(patternRectCenterPoint.y());

	initialDistance = calculateInitialDistance(patternRectCenterPoint, centerPoint);    // Initial distance between A and B
	initialDirection = calculateInitialDirection(patternRectCenterPoint, centerPoint);  // Initial direction angle in degrees


}

void ProcessingThread::get_Info_From_industrial_pictureInfo(double angleDeg_1_newp, double angleDeg_2_newp, QPointF Intersection_1_newP
	, QPointF Intersection_2_newP, double pointToLineDistance_1_newP, double pointToLineDistance_2_newP) {
	this->angleDeg_1_newp = angleDeg_1_newp;
	this->angleDeg_2_newp = angleDeg_2_newp;
	this->Intersection_1_newP.x = Intersection_1_newP.x();
	this->Intersection_1_newP.y = Intersection_1_newP.y();
	this->Intersection_2_newP.x = Intersection_2_newP.x();
	this->Intersection_2_newP.y = Intersection_2_newP.y();
	this->pointToLineDistance_1_newP = pointToLineDistance_1_newP;
	this->pointToLineDistance_2_newP = pointToLineDistance_2_newP;

}
void ProcessingThread::get_Info_From_industrial(QPointF pt_begin_cv2, QPointF pt_end_cv2, qreal height, qreal width, qreal nthresholdValue, qreal nSampleDirection, qreal nMeasureNums, QRect roi,
	QPointF  pt_start_line, QPointF pt_end_line)
{
	PatternInfo shapeMatch(cv::Point(pt_begin_cv2.x(), pt_begin_cv2.y()),
		cv::Point(pt_end_cv2.x(), pt_end_cv2.y()), height, width, nthresholdValue,
		nSampleDirection, nMeasureNums, cv::Rect(roi.x(), roi.y(), roi.width(), roi.height()),
		cv::Point(pt_start_line.x(), pt_start_line.y()), cv::Point(pt_end_line.x(), pt_end_line.y()));
	shapeMatch_Patten.append(shapeMatch);
}
//��ȡԲƥ�����
void ProcessingThread::get_Info_From_industrial_circle(QPointF centerP, qreal nRadius, qreal dMeasureLength, qreal dMeasureHeight,
	qreal dSigma, qreal nThreshold, qreal nTranslation, qreal nMesureNums, qreal nCircleSize,
	qreal nSampleDirection,QRectF roi)
{
	PatternInfo_circle PatternInfo_item(cv::Point2d(centerP.x(), centerP.y()),  nRadius, dMeasureLength, dMeasureHeight,
		dSigma,  nThreshold,  nTranslation,  nMesureNums,  nCircleSize, nSampleDirection,roi);
	shapeMatch_Patten_Circle.append(PatternInfo_item);
}



double ProcessingThread::calculateInitialDistance(QPoint A, QPoint B)
{
	double distance = sqrt(pow(B.x() - A.x(), 2) + pow(B.y() - A.y(), 2));
	return distance;
}

double ProcessingThread::calculateInitialDirection(QPoint A, QPoint B)
{
	// Calculate the relative displacement vector
	double relativeDisplacement_x = B.x() - A.x();
	double relativeDisplacement_y = B.y() - A.y();

	// Calculate the initial direction in degrees
	double initialDirection = atan2(relativeDisplacement_y, relativeDisplacement_x) * 180.0 / M_PI;

	return initialDirection;
}

QPoint ProcessingThread::calculateOffsetB(QPoint A, QPoint B, double initialDistance, double initialDirection, QPoint A_offset)
{
		// Calculate the new coordinates of point A after the offset
		double Ax_new = A.x() + A_offset.x();
		double Ay_new = A.y() + A_offset.y();

		// Convert the initial direction from degrees to radians
		double initialDirectionRadians = initialDirection * M_PI / 180.0;

		// Calculate the initial offset vector
		double offset_x = initialDistance * cos(initialDirectionRadians);
		double offset_y = initialDistance * sin(initialDirectionRadians);

		// Calculate the offset coordinates of point B
		double Bx_offset = Ax_new + offset_x;
		double By_offset = Ay_new + offset_y;
		int finalB_x = Bx_offset - A.x();
		int finalB_y = By_offset - A.y();

		QPoint B_offset = { finalB_x,finalB_y };
		return B_offset;
}

void ProcessingThread::setShapeType(int value)
{
	shape_type = value;
}
