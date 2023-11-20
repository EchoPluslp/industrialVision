#ifndef FILECONTROLLER_H
#define FILECONTROLLER_H
#include <QDir>
#include <QtDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QImage>
#include <QList>
#include <QImageReader>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QDomDocument>
#include "labelcontroller.h"
#include "MvCamera.h"
#include <QTextStream>
#include <QFile>
#include<QTimer>
#include<QApplication>
#include<QTextCodec>
#pragma execution_character_set("utf-8")

// 在mainwindow与文件组件间通信
// 负责运行时的文件储存
// 负责文件保存及读取的处理工作

class FileController : public QObject
{
	Q_OBJECT

		//文件名-文件路径(包含文件名)的映射
		QMap<QString, QString> m_filePath;
	//文件名-图片的映射
	QMap<QString, QImage> m_image;
	QList<QPair<QString, QImage> > m_imageList;

	//标识当前文件
	QString m_currImageName;
	cv::Point MatchPicture(cv::Mat templateImage, cv::Mat srcImage, bool threSholdFlag);
	cv::Mat CalculateNcc(cv::Mat src, cv::Mat temp, cv::Mat& result);
	QImage FileController::Mat2QImage(const cv::Mat cvImage);
	
	//搜索范围图
	cv::Mat srcImgMat;
	cv::Point srcImgResultPoint;
	
		QRect areaChooseREAL_Size;
	QRect patternAreaREAL_size;

	QString importFilepath;
public:
	FileController(QObject* parent = nullptr);
	~FileController();

	cv::Mat QImage2Mat(QImage image);
	QString FileController::returnImportFilepath();
public slots:
	//添加一个文件
	bool addFile(QString& fileName, const QString& filePath, const QImage& image);
	//关闭一个文件
	bool removeFile(QString& fileName);

	//打开单个/多个文件，并自动导入(图片名.xml)文件
	void openFile(LabelController* labelController);

	//打开文件夹，并自动导入(文件夹名.xml)文件
	void openDir(LabelController* labelController);

	//保存单张图片的Label信息
	void saveFile(LabelController* labelController);

	//另存为
	void saveAsFile(LabelController* labelController);

	//手动导入Label信息
	void importFromFile(LabelController* labelController);

	//从相机获得图片
	void getImageFromCamera(QImage image);

	//从相机获得图片
	void getImageFromCamera(QImage image,QString modePath);

	//根据图片名加载图片
	bool getImageFromCameraFromImport(QString imageName);

	//从root获得fileName
	QString FileController::getImageNameFromXML(QDomElement root);
	//根据路径打开文件
	void openFileByPath(const QString& path);

	//执行匹配代码
	void onExecPattern(LabelController* labelController);

	//文件搜索
	QStringList findFiles(const QString& startDir, QStringList filters);


	const QImage getImage(const QString& fileName) const;
	QList<QImage> getImages() const;
	int getIndexOfImage(QString imageName) const;
	QString getFilePath(const QString& fileName) const;
	QString getCurrImageName() const;
	QString getCurrImagePath() const;

	bool hasCurrImage();
	void setCurrImageName(const QString& name);

signals:
	void fileAdded(const QString&);
	void fileRemoved(const QString&);
	void updateFiles();
	void currImageChanged(const QString&);
	void modelFilePATH( QString xmlPath);
	void sendImageToPattern(QImage patternImage, QImage sourceImage);
public slots:
	void slot_receiveDrawPoint(QPoint resultPoint,int totalModelTime);
};

#endif // FILECONTROLLER_H
