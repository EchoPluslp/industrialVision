#include "filecontroller.h"

cv::Point FileController::MatchPicture(cv::Mat templateImage, cv::Mat srcImage, bool threSholdFlag)
{

	int result_cols = srcImage.cols - templateImage.cols + 1;
	int result_rows = srcImage.rows - templateImage.rows + 1;


	if (result_cols < 0 || result_rows < 0)
	{
		//ShowErrorMsg(TEXT("模板图大小超过原图大小"), 0);
		return srcImage;
	}

	cv::Mat result;
	CalculateNcc(srcImage, templateImage, result);

	double minVal = -1;
	double maxVal;
	double threShold;
	cv::Point minLoc;
	cv::Point maxLoc;
	cv::Point matchLoc;
	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat());

	//取大值(视匹配方法而定)
	//    matchLoc = minLoc;
	matchLoc = maxLoc;



	/*if (threSholdFlag) {
		threShold = ScoreThreshold_1;
	}*/

	if (maxVal >= threShold) {
		//绘制最匹配的区域
		return matchLoc;
	}
	else {
		matchLoc.x = -1;
		matchLoc.y = -1;
		return matchLoc;
	}

}

cv::Mat FileController::CalculateNcc(cv::Mat src, cv::Mat temp, cv::Mat& result)
{
	int result_w = src.cols - temp.cols + 1;    //结果图像的尺寸
	int result_h = src.rows - temp.rows + 1;
	result.create(result_h, result_w, CV_32FC1);    //结果图像是单通道32位浮点型


	matchTemplate(src, temp, result, cv::TM_CCOEFF_NORMED); //模板匹配，参数1待检测的原图， 欲搜索的图像。它应该是单通道、8-比特或32-比特 浮点数图像，temp是匹配小图不能大于输入图像，参数3比较结果的映射图像   CV_TM_CCOEFF_NORMED  ：化相关系数匹配法，最好匹配为1；
	//normalize(result, result, 0, 1, NORM_MINMAX, -1);//归一化0到1
	return result;
}

FileController::FileController(QObject* parent) : QObject(parent), m_currImageName("") {
	m_image = QMap<QString, QImage>();
}


FileController::~FileController() {
}

bool FileController::addFile(QString& fileName, const QString& filePath, const QImage& image) {
	if (m_image.contains(fileName)) {
		qDebug() << "FileController::addItem::file exists";
		return false;
	}
	else {
		if (m_image.size() == 1)
		{
			QString m_imageName = getCurrImageName();
			removeFile(getCurrImageName());
		}

		m_image[fileName] = std::move(image);
		m_filePath[fileName] = std::move(filePath);
		m_imageList.append(QPair<QString, QImage>(fileName, image));
		emit fileAdded(fileName);
		return true;
	}
}


bool FileController::removeFile(QString& fileName) {

	if (!m_image.contains(fileName)) {
		qDebug() << "FileController::removeItem::file doesn't exist";
		return false;
	}
	else {
		m_image.remove(fileName);
		m_filePath.remove(fileName);
		for (int i = 0; i < m_imageList.count(); i++) {
			if (m_imageList[i].first == fileName) {
				m_imageList.removeAt(i);
				break;
			}
		}
		emit fileRemoved(fileName);
		if (m_image.count() == 0) {
			emit currImageChanged(QString::Null());
		}
		return true;
	}
}


void FileController::openFile(LabelController* labelController)
{
	QStringList pathes = QFileDialog::getOpenFileNames(nullptr,
		tr("Open File"),
		".",
		tr("Image Files(*.jpg *.png *.gif *.bmp *.jpeg)"));
	bool addedSucceeded = false;
	for (auto path : pathes) {
		if (!path.isEmpty()) {
			QImageReader reader(path);
			QString fileName = QFileInfo(path).fileName();
			reader.setDecideFormatFromContent(true);
			if (reader.canRead()) {
				QImage image;
				if (reader.read(&image)) {
					addFile(fileName, path, image);
					//auto import xml
					QStringList xmlFile = findFiles(QFileInfo(path).path(), QStringList() << fileName + ".xml");
					if (xmlFile.count() == 1) {
						QFile xml(xmlFile[0]);
						if (!xml.open(QFile::ReadOnly))
							return;
						QDomDocument doc;
						if (!doc.setContent(&xml))
						{
							xml.close();
							return;
						}
						xml.close();
						QDomElement root = doc.documentElement();
						labelController->createFromElement(root);
					}
					else if (xmlFile.count() > 1) {
						QMessageBox::warning(nullptr, tr("Warning"),
							"Too many xml files found!(" + fileName + ")");
					}
					addedSucceeded = true;
				}
				else {
					qDebug() << "open is failed...." << reader.errorString();
				}
			}
			else {
				qDebug() << "can not read...." << reader.errorString();
			}
		}
		else {
			QMessageBox::warning(nullptr, tr("Warning"),
				tr("You did not select any file."));
		}
	}
	if (addedSucceeded) {
		emit updateFiles();
	}
}

void FileController::openDir(LabelController* labelController)
{
	QString path = QFileDialog::getExistingDirectory(nullptr, tr("Open Dir"), ".");
	bool addedSucceeded = false;
	if (!path.isEmpty()) {
		QDir dir(path);
		auto stringFilter = QStringList() << "*.jpg" << "*.png" << "*.gif" << ".bmp" << ".jpeg";
		auto fileNameList = dir.entryList(stringFilter, QDir::Filter::Files, QDir::SortFlag::Name);
		QImageReader reader;
		for (QString fileName : fileNameList) {
			reader.setFileName(path + "/" + fileName);
			reader.setDecideFormatFromContent(true);

			if (reader.canRead()) {
				QImage image;
				if (reader.read(&image)) {
					addFile(fileName, path + "/" + fileName, image);
					addedSucceeded = true;
				}
				else {
					qDebug() << "open is failed...." << reader.errorString();
				}
			}
			else {
				qDebug() << "can not read...." << reader.errorString();
			}
		}
		//auto import xml
		QStringList xmlFile = findFiles(path, QStringList() << "*.xml");
		if (xmlFile.count() == 1) {
			QFile xml(xmlFile[0]);
			if (!xml.open(QFile::ReadOnly))
				return;
			QDomDocument doc;
			if (!doc.setContent(&xml))
			{
				xml.close();
				return;
			}
			xml.close();
			QDomElement root = doc.documentElement();
			labelController->createFromElement(root);
		}
	}
	if (addedSucceeded) {
		emit updateFiles();
	}
}

void FileController::saveFile(LabelController* labelController)
{
	if (m_currImageName == "")
		return;
	if (m_imageList.empty())
		return;
	if (!labelController->checkSaveLabelOn())
	{
		return;
	}
		//通过导入进来的,只改变画图的框,不能改变图片的属性
	if (!importFilepath.isNull()) {
		//已经存在，则覆盖
		QFile file(importFilepath);
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
			return;
		}
		QDomDocument doc;  
		QDomProcessingInstruction instruction;
		instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");

		doc.appendChild(instruction);

		doc.appendChild(labelController->getElementOfImage(doc, getCurrImageName()));

		QTextStream out_stream(&file);
		doc.save(out_stream, 4);
		file.close();
		QMessageBox::warning(0, "通知", "保存成功");
		emit modelFilePATH(importFilepath);
	}
	else {
		//未存在，则新建
		QString path = QFileDialog::getSaveFileName(nullptr,
			tr("Open File"),
			"",
			tr("XML Files(*.xml)"));

		QFile file(path);
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
			return;
		}
		QDomDocument doc;
		QDomProcessingInstruction instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
		doc.appendChild(instruction);
		doc.appendChild(labelController->getElementOfImage(doc, getCurrImageName()));

		QTextStream out_stream(&file);
		doc.save(out_stream, 4);

		QImage currentImage = getImage(getCurrImageName());
		currentImage = currentImage.convertToFormat(QImage::Format_Indexed8);
		cv::Mat MatSrcImage = QImage2Mat(currentImage);
		QTextCodec* codec = QTextCodec::codecForName("GB2312");
		QTextCodec::setCodecForLocale(codec);

		QString dirpath = QApplication::applicationDirPath() + "/model/";
		QDir dir(dirpath);
		if (!dir.exists())
		{
			//不存在则创建
			dir.mkdir(dirpath); //只创建一级子目录，即必须保证上级目录存在
		}

		QString fullpath = dirpath + getCurrImageName();

		std::string str = codec->fromUnicode(fullpath).data();

		cv::imwrite(str, MatSrcImage);
		emit modelFilePATH(path);
		file.close();
		QMessageBox::warning(0, "通知", "保存成功");

	}


}

void FileController::saveAsFile(LabelController* labelController)
{
	if (m_currImageName == "")
		return;
	if (m_imageList.empty())
		return;
	if (!labelController->checkSaveLabelOn())
	{
		return;
	}

	//另存为
	QString path = QFileDialog::getSaveFileName(nullptr,
		tr("Open File"),
		"",
		tr("XML Files(*.xml)"));

	QFile file(path);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		return;
	}
	QDomDocument doc;
	QDomProcessingInstruction instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
	doc.appendChild(instruction);
	doc.appendChild(labelController->getElementOfImage(doc, getCurrImageName()));

	QTextStream out_stream(&file);
	doc.save(out_stream, 4);

	QImage currentImage = getImage(getCurrImageName());
	currentImage = currentImage.convertToFormat(QImage::Format_Indexed8);
	cv::Mat MatSrcImage = QImage2Mat(currentImage);
	QTextCodec* codec = QTextCodec::codecForName("GB2312");
	QTextCodec::setCodecForLocale(codec);

	QString dirpath = QApplication::applicationDirPath() + "/model/";
	QDir dir(dirpath);
	if (!dir.exists())
	{
		//不存在则创建
		dir.mkdir(dirpath); //只创建一级子目录，即必须保证上级目录存在
	}

	QString fullpath = dirpath + getCurrImageName();

	std::string str = codec->fromUnicode(fullpath).data();

	cv::imwrite(str, MatSrcImage);
	emit modelFilePATH(path);
	file.close();	
	QMessageBox::warning(0, "通知", "另存为成功");

}


void FileController::importFromFile(LabelController* labelController)
{
	QString path = QFileDialog::getOpenFileName(nullptr, tr("Open File"), ".",
		tr("XML Files(*.xml)"));
	if (!path.isEmpty()) {
		QFile file(path);
		if (!file.open(QFile::ReadOnly))
			return;
		QDomDocument doc;
		if (!doc.setContent(&file))
		{
			file.close();
			return;
		}
		file.close();
		QDomElement root = doc.documentElement();
		//labelController->deleteAllLabel();

				//获得imageName
		QString icomString = getImageNameFromXML(root);
		if (icomString.isEmpty()) {
			QMessageBox::warning(nullptr, tr("视觉模板"),
				tr("视觉模板设置错误,请重新设置模板信息"));
			return;
		}

		if (!getImageFromCameraFromImport(icomString)) {
			return;
		}
		labelController->deleteAllLabel();

		labelController->createFromElement(root);

		importFilepath = path;

	}
	else {
		QMessageBox::warning(nullptr, tr("Path"),
			tr("未选择文件."));
	}
}

QString FileController::getImageNameFromXML(QDomElement elem) {
	elem = elem.cloneNode(true).toElement();
	qDebug() << elem.tagName();
	for (int i = 0; i < elem.childNodes().count(); i++) {
		auto typeElem = elem.childNodes().at(i).toElement();
		for (int j = 0; j < typeElem.childNodes().count(); j++) {
			auto labelElem = typeElem.childNodes().at(j).toElement();

			if (labelElem.tagName() == "Label") {

				Area* m_area_item = new Area;
				//给patternImageName赋值用于读取模板图
				return m_area_item->getFileName(labelElem.firstChildElement("Area")).trimmed();
			}
		}
	}
	return "";
}

void FileController::getImageFromCameraInFile(QImage image)
{

	bool addedSucceeded = false;
	QString fileName =  image.text("name");
	QString path =  QCoreApplication::applicationDirPath();
	path.append(fileName);
	addFile(fileName, path, image);
	addedSucceeded = true;
	//通过采集图片的时候,重置导入路径
	importFilepath.clear();
	if (addedSucceeded) {
		emit updateFiles( );
	}
}

void FileController::getImageFromCameraInFile(QImage image,QString modePath)
{
	bool addedSucceeded = false;
	QString fileName = modePath;
	QString path = QCoreApplication::applicationDirPath();
	addFile(fileName, path, image);
	addedSucceeded = true;
	if (addedSucceeded) {
		emit updateFiles();
	}
}

//模板设置界面导入功能设置
//name指定的图片文件名,不易修改,因为与label绑定
bool FileController::getImageFromCameraFromImport(QString imageName)
{
	bool addedSucceeded = false;

	//构建图片名字
	QStringList stringModelNameAndPath = 	imageName.split("=");
	 QString fileName = stringModelNameAndPath.at(1);
	QImage image(stringModelNameAndPath.at(0)+ fileName);
	int x = image.width();
	if (x==0)
	{
		QMessageBox::warning(nullptr, tr("图片异常"),
			"模板图片异常,导入失败");
		return addedSucceeded;
	}
	addFile(fileName, stringModelNameAndPath.at(0), image);
	addedSucceeded = true;
	if (addedSucceeded) {
		emit updateFiles();
	}
}


void FileController::openFileByPath(const QString& path)
{
	bool addedSucceeded = false;

	if (!path.isEmpty()) {
		QImageReader reader(path);
		QString fileName = QFileInfo(path).fileName();
		reader.setDecideFormatFromContent(true);
		if (reader.canRead()) {
			QImage image;
			if (reader.read(&image)) {
				addFile(fileName, path, image);
				addedSucceeded = true;
			}
			else {
				qDebug() << "open is failed...." << reader.errorString();
			}
		}
		else {
			qDebug() << "can not read...." << reader.errorString();
		}
	}
	if (addedSucceeded) {
		emit updateFiles();
	}
}

void FileController::onExecPattern(LabelController* labelController)
{ 
	QString currImageName = getCurrImageName();
	QRect areaChooseSize;
	QRect patternArea;

	QImage cuurImage = m_image[m_currImageName];
	int cuurImageWidth = cuurImage.width();
	int cuurImageHeight = cuurImage.height();

	for (Label* label : labelController->getLabelsOfImage(currImageName)) {
		if (label->getType().contains("搜索区域"))
		{
			if (label) {
				const Area* area = label->getArea();
				if (area) {
					Shape* shape = area->getShape(currImageName);
					QSize shapeSize = shape->m_currSize;
					areaChooseSize = shape->getSize();
					areaChooseREAL_Size.setX(((double)areaChooseSize.x() / (double)shapeSize.width()) * cuurImageWidth);
					areaChooseREAL_Size.setY(((double)areaChooseSize.y() / (double)shapeSize.height()) * cuurImageHeight);
					areaChooseREAL_Size.setWidth(((double)areaChooseSize.width() / (double)shapeSize.width()) * cuurImageWidth);
					areaChooseREAL_Size.setHeight(((double)areaChooseSize.height() / (double)shapeSize.height()) * cuurImageHeight);
				} 
			}
		}
		if (label->getType().contains("特征区域"))
		{
			if (label) {
				const Area* area = label->getArea();
				if (area) {
					Shape* shape = area->getShape(currImageName);
					currType = shape->getType();
					//判断是否当前图像的类型
					if (currType == Shape::Rect)
					{
						QSize shapeSize = shape->m_currSize;
						patternArea = shape->getSize();
						patternAreaREAL_size_rect.setX(((double)patternArea.x() / (double)shapeSize.width()) * cuurImageWidth);
						patternAreaREAL_size_rect.setY(((double)patternArea.y() / (double)shapeSize.height()) * cuurImageHeight);
						patternAreaREAL_size_rect.setWidth(((double)patternArea.width() / (double)shapeSize.width()) * cuurImageWidth);
						patternAreaREAL_size_rect.setHeight(((double)patternArea.height() / (double)shapeSize.height()) * cuurImageHeight);
					}
					else if (currType == Shape::Ellipse)
					{
						QSize shapeSize = shape->m_currSize;
		  				MyGraphicsEllipseItem* patternArea = (MyGraphicsEllipseItem*)shape->getItem();
						auto item = patternArea->rect();
						patternAreaREAL_size_rect.setX(((double)item.x() / (double)shapeSize.width()) * cuurImageWidth);
						patternAreaREAL_size_rect.setY(((double)item.y() / (double)shapeSize.height()) * cuurImageHeight);
						patternAreaREAL_size_rect.setWidth(((double)item.width() / (double)shapeSize.width()) * cuurImageWidth);
						patternAreaREAL_size_rect.setHeight(((double)item.height() / (double)shapeSize.height()) * cuurImageHeight);
					}
					else if (currType == Shape::Polygon)
					{
						patternAreaREAL_size_polygon.clear();
						MyGraphicsPolygonItem* currItem = (MyGraphicsPolygonItem*)shape->getItem();
						double ratio = double(cuurImageWidth) / double(shape->m_currSize.width());

						QPolygonF prev = currItem->polygon();
						for (int i = 0; i < prev.count(); i++) {
							auto point = prev.at(i);
							patternAreaREAL_size_polygon << QPointF(point * ratio);
						}
					}
				}
			}
		}
	}
	//没有搜索区域,则全图
	if (areaChooseREAL_Size.x()==0&& areaChooseREAL_Size.y() == 0 && areaChooseREAL_Size.width() == 0 && areaChooseREAL_Size.height() == 0 )
	{
		areaChooseREAL_Size.setWidth(cuurImageWidth);
		areaChooseREAL_Size.setHeight(cuurImageHeight);
	}
	QImage currentImage = getImage(getCurrImageName()).convertToFormat(QImage::Format_Indexed8);
	cv::Mat MatSrcImage = QImage2Mat(currentImage);
	cv::Rect areaChooseRealSize(areaChooseREAL_Size.x(), areaChooseREAL_Size.y(), areaChooseREAL_Size.width(), areaChooseREAL_Size.height());

	srcImgMat = MatSrcImage(areaChooseRealSize);
	if (currType == Shape::Rect)
	{
		if (patternAreaREAL_size_rect.x() == 0 && patternAreaREAL_size_rect.y() == 0 && patternAreaREAL_size_rect.width() == 0 && patternAreaREAL_size_rect.height() == 0)
		{
			QMessageBox::warning(nullptr, tr("Warning"),
				"没有特征区域");
			return;
		}
		//获取区域
		if (patternAreaREAL_size_rect.x() >= 0 && patternAreaREAL_size_rect.y() >= 0 && patternAreaREAL_size_rect.width() > 0 && patternAreaREAL_size_rect.height() > 0) {
			cv::Rect patternAreaRealSize(patternAreaREAL_size_rect.x(), patternAreaREAL_size_rect.y(), patternAreaREAL_size_rect.width(), patternAreaREAL_size_rect.height());
			cv::Mat patternImg = MatSrcImage(patternAreaRealSize);

			emit sendImageToPattern(Mat2QImage(patternImg), Mat2QImage(srcImgMat));
		}
	}
	else if (currType == Shape::Ellipse)
	{
		if (patternAreaREAL_size_rect.x() == 0 && patternAreaREAL_size_rect.y() == 0 && patternAreaREAL_size_rect.width() == 0 && patternAreaREAL_size_rect.height() == 0)
		{
			QMessageBox::warning(nullptr, tr("Warning"),
				"没有特征区域");
			return;
		}
		//获取区域
		if (patternAreaREAL_size_rect.x() >= 0 && patternAreaREAL_size_rect.y() >= 0 && patternAreaREAL_size_rect.width() > 0 && patternAreaREAL_size_rect.height() > 0) {
			cv::Rect patternAreaRealSize(patternAreaREAL_size_rect.x(), patternAreaREAL_size_rect.y(), patternAreaREAL_size_rect.width(), patternAreaREAL_size_rect.height());
			cv::Mat patternImg = MatSrcImage(patternAreaRealSize);

		// 定义椭圆参数
		cv::Point center(patternAreaREAL_size_rect.x() + patternAreaREAL_size_rect.width()/2, patternAreaREAL_size_rect.y()
		+ patternAreaREAL_size_rect.height() / 2);
		int width = patternAreaREAL_size_rect.width();
		int length = patternAreaREAL_size_rect.height();

		// 创建椭圆的掩码
		cv::Mat mask = cv::Mat::zeros(MatSrcImage.size(), CV_8UC1);
		cv::ellipse(mask, center, cv::Size(width / 2, length / 2), 0, 0, 360, 255, -1);
		cv::Mat maskPattern = mask(patternAreaRealSize);


		emit sendImageToPatternWithMask(Mat2QImage(patternImg), Mat2QImage(srcImgMat), Mat2QImage(maskPattern));
		}
	}
	else if (currType == Shape::Polygon) {
		// 将QPolygonF中的点坐标转换为vector<cv::Point>
		std::vector<cv::Point> points;
		for (const QPointF& point : patternAreaREAL_size_polygon) {
			points.emplace_back(static_cast<int>(point.x()), static_cast<int>(point.y()));
		}
		points.pop_back();
		// 创建一个与图像相同大小的黑色掩码
		cv::Mat mask = cv::Mat::zeros(MatSrcImage.size(), MatSrcImage.type());


		// 将点坐标转换为vector<vector<cv::Point>>形式以符合cv::fillPoly的要求
		std::vector<std::vector<cv::Point>> pts = { points };
		// 在掩码上填充多边形区域
		cv::fillPoly(mask, pts, cv::Scalar(255, 255, 255));

		// 使用掩码提取多边形区域
		cv::Mat result;
		cv::bitwise_and(MatSrcImage, mask, result);
		// 查找包含多边形的最小矩形 边界框
		cv::Rect boundingRect = cv::boundingRect(points);

		// 裁剪图像，去掉旁边的黑色区域
		cv::Mat croppedResult = result(boundingRect).clone();


		// 选择适当的阈值，这里使用 128 作为例子，可以根据实际情况调整
		int threshold_value = 1;

		// 应用二值化操作
		cv::threshold(croppedResult, mask, threshold_value, 255, cv::THRESH_BINARY);

		emit sendImageToPatternWithMask(Mat2QImage(croppedResult), Mat2QImage(srcImgMat), Mat2QImage(mask));
	}
}

void FileController::slot_receiveDrawPoint(QPoint resultPoint,int totalModelTime) {

	cv::Mat srcImgClone = srcImgMat.clone();
	// 在左上方写下totalModelTime文字
	std::string text = "TotalTime : " + std::to_string(totalModelTime)+"MS";

	cv::putText(srcImgClone, text, cv::Point(80, 100), cv::FONT_HERSHEY_SIMPLEX, 4, cv::Scalar(255, 255, 255),2);

	cv::circle(srcImgClone, cv::Point(resultPoint.x(), resultPoint.y()), 30, cv::Scalar(255), -1);
	cv::namedWindow("TestResult", cv::WINDOW_KEEPRATIO);
	cv::imshow("TestResult", srcImgClone);
}
QStringList FileController::findFiles(const QString& startDir, QStringList filters)
{
	QStringList fileNames;
	QDir dir(startDir);

	//current dir
	for (QString file : dir.entryList(filters, QDir::Files))
		fileNames += startDir + '/' + file;

	//sub dirs
	for (QString subdir : dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot))
		fileNames += findFiles(startDir + '/' + subdir, filters);

	return fileNames;
}
const QImage FileController::getImage(const QString& fileName) const {
	if (!m_image.contains(fileName)) {
		qDebug() << "FileController::getImage::file doesn't exist";
		return QImage();
	}
	else {
		qDebug() << "FileController::getImage::found image";
		return m_image.value(fileName);
	}
}

QList<QImage> FileController::getImages() const
{
	QList<QImage> list;
	for (auto p : m_imageList) {
		list.append(p.second);
	}
	return list;
}





int FileController::getIndexOfImage(QString imageName) const
{
	for (int i = 0; i < m_imageList.count(); i++) {
		if (m_imageList[i].first == imageName) {
			return i;
		}
	}
	return -1;
}


QString FileController::getFilePath(const QString& fileName) const {
	if (!m_filePath.contains(fileName)) {
		qDebug() << "FileController::getFilePath::file doesn't exist";
		return QString();
	}
	else {
		return m_filePath.value(fileName);
	}
}


QString FileController::getCurrImageName() const
{
	return m_currImageName;
}

QString FileController::getCurrImagePath() const
{
	if (m_filePath.contains(m_currImageName)) {
		QString ret = m_filePath.value(m_currImageName);
		qDebug() << ret;
		return ret;
	}
	else {
		qDebug() << "FileController::getCurrImagePath";
		return QString();
	}
}

bool FileController::hasCurrImage()
{
	return (!getCurrImagePath().isEmpty() && !getCurrImagePath().isNull());
}
void FileController::setCurrImageName(const QString& name)
{
	if (m_image.contains(name)) {
		m_currImageName = name;
		emit currImageChanged(name);
	}
	else {
		qDebug() << "FileController::setCurrImageName";
	}
}

cv::Mat FileController::QImage2Mat(QImage image)
{
	cv::Mat mat;
	switch (image.format())
	{
	case QImage::Format_RGB32:  //一般Qt读入彩色图后为此格式
		mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
		cv::cvtColor(mat, mat, cv::COLOR_BGRA2BGR);   //转3通道
		break;
	case QImage::Format_RGB888:
		mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
		cv::cvtColor(mat, mat, cv::COLOR_RGB2BGR);
		break;
	case QImage::Format_Indexed8:
		mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
		break;
	}
	return mat;
}

QImage FileController::Mat2QImage(const cv::Mat cvImage)
{
	cv::Mat temp = cvImage.clone();

	std::vector<uchar> imgBuf;
	imencode(".bmp", temp, imgBuf);

	QByteArray baImg((char*)imgBuf.data(), static_cast<int>(imgBuf.size()));
	QImage image;
	image.loadFromData(baImg, "BMP");
	return image;
}
QString FileController::returnImportFilepath() {
	return importFilepath;
}
