
#ifndef INDUSTRIALVISION_H_
#define INDUSTRIALVISION_H_

#pragma once
#pragma execution_character_set("utf-8")
#define SAFE_DELETE(p) { if (p) { delete (p);     (p) = nullptr; } }

#include <QtWidgets/QMainWindow>
#include "ui_industrialVision.h"
#include "MvCamera.h"
#include<QMessageBox>
#include<qstring.h>
#include <string>
#include <vector>
#include<iostream>
#include<opencv2\opencv.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include "connectServer.h"
#include "createModel.h"
#include "logoSet.h"
#include"smallMainWindow.h"
#include  <QtXml>
#include <QDateTime>
#include <QXmlStreamReader>
#include<vector>
#include <QSettings>
#include "camerathread.h"
#include "processingthread.h"
#include "passwordSet.h"
#include "SharedData.h"

using namespace cv;
using namespace std;


class industrialVision : public QMainWindow
{
	Q_OBJECT

public:
	industrialVision(QWidget *parent = Q_NULLPTR);
    ~industrialVision();
    int GetTopLayer(Mat* matTempl, int iMinDstLength);
    bool industrialVision::nativeEvent(const QByteArray& eventType, void* message, long* result);
    /*ch:�ؼ���Ӧ���� | en:Control corresponding variable*/
    const QString Green = "GREEN";
	const  QString Red = "GED";
	const  QString Gray = "GRAY";

	 connectServer connectValual;

private:
	static void __stdcall ImageCallback(unsigned char* pData, MV_FRAME_OUT_INFO_EX* pFrameInfo, void* pUser);
    BOOL                    m_bSoftWareTriggerCheck;
    double                  m_dExposureEdit;
    double                  m_dGainEdit;
    int                  m_dFrameRateEdit;
    char                    m_chPixelFormat[MV_MAX_SYMBOLIC_LEN];
    int                     m_width;
	int                     m_height;
    int                    const_m_width;
	int                    const_m_height;

    QString                  m_xmlpath;
    QComboBox*               m_ctrlDeviceCombo;                // ch:ö�ٵ����豸 | en:Enumerated device
    int                     m_nDeviceCombo;
   // QAction* action_SetModelFile;
    QAction *action_SetModelFile;
    int matchTime_total;
    QPointF resultPointF;
    createModel createModelItem;
    smallMainWindow smallMainWindow;
    cv::Rect srcRect;
	QRectF srcQRect;

    cv::Rect matRect;
    cv::Point2f resultPoint;
    cv::Mat rotaete_imgTemp;
	QPoint centerPoint;


    double                      m_dToleranceAngle =0;
    double                  m_dScore = 80;//�÷���ֵ
    double                  m_iMaxPos =   2;//�������
    double                  m_dMaxOverlap  = 0;
    bool                       rotate_image = false;
    bool                    shizixian = true;
    bool       smallwindow_flag = true; //С���ڱ�־
    int                   OK_count = 0;
    int                   total_count = 0;
    bool                  open_mv = false;//����򿪱�־

    bool flag_90 = false;//90�ȱ�ʶ��
    //�˵�����ť
    QAction *action_RestoreWindow;

    //�����ʱ��
	QTimer* timer  = new QTimer;  //��ʼ����ʱ��
     //��ʱ�� ÿ�����ʱ��
	QTime* TimeRecord = new QTime(0, 0, 0); ;  //��¼ʱ��
	bool isStart = false;        //��¼�Ƿ��Ѿ���ʼ��ʱ
	QPoint patternRectCenterPoint;
    bool m_h_Value_Swap_Flag = true;
private:
	Ui::industrialVision ui;
    logoSet logoset;
    /*ch:�ʼʱ�Ĵ��ڳ�ʼ�� | en:Window initialization*/
    bool DisplayWindowInitial();
    void setButtonClickLimits(bool flag);
    void ShowErrorMsg(QString csMessage, int nErrorNum);

    int GetExposureTime();               // ch:�����ع�ʱ�� | en:Set Exposure Time
    int SetExposureTime();
    int GetGain();                       // ch:�������� | en:Set Gain
    int SetGain();
    int GetFrameRate();                  // ch:����֡�� | en:Set Frame Rate
    int SetFrameRate();
    int GetPixelFormat();                // ch:��ȡ���ظ�ʽ | en:Get Pixel Format
    int GetWidth();                      //��ȡͼƬ���
	int GetHeight();                      //��ȡͼƬ�߶�

    int CloseDevice();                   // ch:�ر��豸 | en:Close Device

private:
    bool                    m_bOpenDevice = false;                        // ch:�Ƿ���豸 | en:Whether to open device
    bool                    m_bStartGrabbing;                     // ch:�Ƿ�ʼץͼ | en:Whether to start grabbing
    int                     m_nTriggerMode;                       // ch:����ģʽ | en:Trigger Mode
    int                     m_nTriggerSource;                     // ch:����Դ | en:Trigger Source
    s_TemplData      m_TemplData;

    CMvCamera* m_pcMyCamera;               // ch:CMyCamera��װ�˳��ýӿ� | en:CMyCamera packed commonly used interface
    HWND                    m_hwndDisplay;                        // ch:��ʾ��� | en:Display Handle
    MV_CC_DEVICE_INFO_LIST  m_stDevList;

    CRITICAL_SECTION        m_hSaveImageMux;
    
    unsigned char* m_pSaveImageBuf = NULL;
    unsigned int            m_nSaveImageBufSize;
    MV_FRAME_OUT_INFO_EX    m_stImageInfo;

    void* m_hGrabThread;              // ch:ȡ���߳̾�� | en:Grab thread handle
    bool                    m_bThreadState;
    bool  AcquiesceScale = false;
    //ģ��ƥ����Ҫ�Ĳ���
    QRectF areaNodeREAL_size;
    QRectF patternAreaREAL_size;
    QPointF lastResult;
    //�ӱ��ض�ȡ��ģ��ͼ
    QImage patternSrcQImage;
    QImage myImageToModel;
    void resetParameters();
public:
     //MatתQImage
     QImage Mat2QImage(const cv::Mat InputMat);

     bool industrialVision::visualTemplate_Control = false;
     passwordSet passwordSetItem;

     bool industrialVision::getPatternInfoFromXML(QString path);
     //false û�и��� true �Ѿ�����
     bool  shishiPiPei_FLAG = false;
	 bool  small_window_flag = false;
	 bool   defaultCamcare= true;

     bool openCamcal();
     void AppendText(const QString& text,QString flag);
     void restoreWindow();
     bool compareAspectRatio(QSize smallPicture);
	 CameraThread* m_cameraThread;           //����̶߳���
	 ProcessingThread* m_processingThread;   //�����̶߳���
	 QPixmap MatToPixmap(const cv::Mat cvMat);
	 QPoint getCenterPointFromCircle(QList<QPoint> listCircle);
	// MyGLWidget glWidget = new MyGLWidget(this);
     int rotateValue;

private slots:
	void click_continuousOperation();
    void click_stopOperation();
    void click_manualOperation();
    void click_editVision();
    void sendImgToVisualContinue(QString data);
    void click_createServer();
    void getImageOneFrame();
    void createOncePattern();
    void openShiShiPiPei();
	void openshizixian();
	void receive_ServerCreateInfo(QString flag);

     void setModelXMLFile();
     void rotatePicture();
     void addTextBrower(QString text,QString flag);
     void getXMLPATH(QString xmlPath);
	 /*ch:�������û�ȡ | en:Parameters Get and Set*/
	 void OnBnClickedSetParameterButton();       // ch:���ò��� | en:Exit from upper right corner
	 void Setinitializationparameters();
	 void SaveInitializationParameters();
     void smallwindow_button_click();
		 void slot_displayPixmap(QPixmap newPixmap, int index);
         void slot_get_patternResult(QPointF resultPoint,int matchTime);
         void logQuery(); 
         void helpInfo();
         void updateTime();
         void slot_modelPictureReadFlag();
         void getRotateValue(int x);
         void actionPasswordAction();
signals:
    void sendQimageToVisualTemplate(QImage data);
    void cameraTovisualTemplate(QImage img,QString path);
    void sendResultToServer(QPoint point);
    void SLOTAppendText(const QString& text,QString flag);
    void singal_sendPatternImage(QString patternImage_Path,QRectF patternRect,QRectF qrect,QPoint centerPoint,QPoint patternRectCenterPoint);
    void signal_processThread_pattren();
    void send_Grade(QString grade);
    void setdefultCamare(bool flag);
};

#endif