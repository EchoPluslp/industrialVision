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

// ��mainwindow���ļ������ͨ��
// ��������ʱ���ļ�����
// �����ļ����漰��ȡ�Ĵ�����

class FileController : public QObject
{
	Q_OBJECT

		//�ļ���-�ļ�·��(�����ļ���)��ӳ��
		QMap<QString, QString> m_filePath;
	//�ļ���-ͼƬ��ӳ��
	QMap<QString, QImage> m_image;
	QList<QPair<QString, QImage> > m_imageList;

	//��ʶ��ǰ�ļ�
	QString m_currImageName;
	cv::Point MatchPicture(cv::Mat templateImage, cv::Mat srcImage, bool threSholdFlag);
	cv::Mat CalculateNcc(cv::Mat src, cv::Mat temp, cv::Mat& result);




public:
	FileController(QObject* parent = nullptr);
	~FileController();

	cv::Mat QImage2Mat(QImage image);

public slots:
	//���һ���ļ�
	bool addFile(QString& fileName, const QString& filePath, const QImage& image);
	//�ر�һ���ļ�
	bool removeFile(QString& fileName);

	//�򿪵���/����ļ������Զ�����(ͼƬ��.xml)�ļ�
	void openFile(LabelController* labelController);

	//���ļ��У����Զ�����(�ļ�����.xml)�ļ�
	void openDir(LabelController* labelController);

	//���浥��ͼƬ��Label��Ϣ
	void saveFile(LabelController* labelController);

	//���浱ǰ����ͼƬ��Label��Ϣ
	void saveAsFile(LabelController* labelController);

	//�ֶ�����Label��Ϣ
	void importFromFile(LabelController* labelController);

	//��������ͼƬ
	void getImageFromCamera(QImage image);

	//��������ͼƬ
	void getImageFromCamera(QImage image,QString modePath);

	//����ͼƬ������ͼƬ
	void getImageFromCameraFromImport(QString imageName);

	//��root���fileName
	QString FileController::getImageNameFromXML(QDomElement root);
	//����·�����ļ�
	void openFileByPath(const QString& path);

	//ִ��ƥ�����
	void onExecPattern(LabelController* labelController);

	//�ļ�����
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
};

#endif // FILECONTROLLER_H
