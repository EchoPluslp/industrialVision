
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
    /*ch:控件对应变量 | en:Control corresponding variable*/
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
    QString                  m_xmlpath;
    QComboBox*               m_ctrlDeviceCombo;                // ch:枚举到的设备 | en:Enumerated device
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
    double                  m_dScore = 0.8;//得分阈值
    double                  m_iMaxPos =   2;//最多数量
    double                  m_dMaxOverlap  = 0;
    bool                       rotate_image = false;
    bool                    shizixian = true;
    bool       smallwindow_flag = true; //小窗口标志
    int                   OK_count = 0;
    int                   total_count = 0;
    bool                  open_mv = false;//相机打开标志

    bool flag_90 = false;//90度标识符
    //菜单栏按钮
    QAction *action_RestoreWindow;

    //界面计时器
	QTimer* timer  = new QTimer;  //初始化定时器
     //定时器 每秒更新时间
	QTime* TimeRecord = new QTime(0, 0, 0); ;  //记录时间
	bool isStart = false;        //记录是否已经开始计时
	QPoint patternRectCenterPoint;
    bool m_h_Value_Swap_Flag = true;
private:
	Ui::industrialVision ui;
    logoSet logoset;
    /*ch:最开始时的窗口初始化 | en:Window initialization*/
    bool DisplayWindowInitial();
    void setButtonClickLimits(bool flag);
    void ShowErrorMsg(QString csMessage, int nErrorNum);

    int GetExposureTime();               // ch:设置曝光时间 | en:Set Exposure Time
    int SetExposureTime();
    int GetGain();                       // ch:设置增益 | en:Set Gain
    int SetGain();
    int GetFrameRate();                  // ch:设置帧率 | en:Set Frame Rate
    int SetFrameRate();
    int GetPixelFormat();                // ch:获取像素格式 | en:Get Pixel Format
    int GetWidth();                      //获取图片宽度
	int GetHeight();                      //获取图片高度

    int CloseDevice();                   // ch:关闭设备 | en:Close Device

private:
    bool                    m_bOpenDevice = false;                        // ch:是否打开设备 | en:Whether to open device
    bool                    m_bStartGrabbing;                     // ch:是否开始抓图 | en:Whether to start grabbing
    int                     m_nTriggerMode;                       // ch:触发模式 | en:Trigger Mode
    int                     m_nTriggerSource;                     // ch:触发源 | en:Trigger Source
    s_TemplData      m_TemplData;

    CMvCamera* m_pcMyCamera;               // ch:CMyCamera封装了常用接口 | en:CMyCamera packed commonly used interface
    HWND                    m_hwndDisplay;                        // ch:显示句柄 | en:Display Handle
    MV_CC_DEVICE_INFO_LIST  m_stDevList;

    CRITICAL_SECTION        m_hSaveImageMux;
    
    unsigned char* m_pSaveImageBuf = NULL;
    unsigned int            m_nSaveImageBufSize;
    MV_FRAME_OUT_INFO_EX    m_stImageInfo;

    void* m_hGrabThread;              // ch:取流线程句柄 | en:Grab thread handle
    bool                    m_bThreadState;
    bool  AcquiesceScale = false;
    //模板匹配需要的参数
    QRectF areaNodeREAL_size;
    QRectF patternAreaREAL_size;
    QPointF lastResult;
    //从本地读取的模板图
    QImage patternSrcQImage;
    QImage myImageToModel;
    void resetParameters();
    //框选出来的范围
public:
     //Mat转QImage
     QImage Mat2QImage(const cv::Mat InputMat);

     bool industrialVision::visualTemplate_Control = false;
     passwordSet passwordSetItem;

     bool industrialVision::getPatternInfoFromXML(QString path);
     //false 没有更新 true 已经更新
     bool  shishiPiPei_FLAG = false;
	 bool  small_window_flag = false;

     bool openCamcal();
     void AppendText(const QString& text,QString flag);
     void restoreWindow();
     bool compareAspectRatio(QSize smallPicture);
	 CameraThread* m_cameraThread;           //相机线程对象
	 ProcessingThread* m_processingThread;   //处理线程对象
	 QPixmap MatToPixmap(const cv::Mat cvMat);
	 QPoint getCenterPointFromCircle(QList<QPoint> listCircle);
	 MyGLWidget glWidget = new MyGLWidget(this);
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
	 /*ch:参数设置获取 | en:Parameters Get and Set*/
	 void OnBnClickedSetParameterButton();       // ch:设置参数 | en:Exit from upper right corner
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
};

#endif