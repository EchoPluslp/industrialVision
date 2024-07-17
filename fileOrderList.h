
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
	//已选操作l,搜索区域
	bee_rect *fileList_sourceRect;
	//已选操作2,特征区域
	bee_rect *fileList_mattchRect;
	//图像
	QImage ImageItem;

	//文件执行顺序
	QString fileOrderList;
};
