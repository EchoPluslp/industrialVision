
#include <QMainWindow>
#include <QFileDialog>
#include <QtWidgets>

#include "bee_rect.h"
class FileOrder {
public:
	 FileOrder() {};
	
	~FileOrder()
	{
		//delete fileList_sourceRect;
		//fileList_sourceRect = nullptr;
	}

public:
	//��ѡ����l,��������
	bee_rect *fileList_sourceRect;
	//��ѡ����2,��������
	bee_rect *fileList_mattchRect;
	//ͼ��
	QImage ImageItem;

	//�ļ�ִ��˳��
	QString fileOrderList;
};
