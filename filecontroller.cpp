#include "filecontroller.h"

cv::Point FileController::MatchPicture(cv::Mat templateImage, cv::Mat srcImage, bool threSholdFlag)
{

	int result_cols = srcImage.cols - templateImage.cols + 1;
	int result_rows = srcImage.rows - templateImage.rows + 1;


	if (result_cols < 0 || result_rows < 0)
	{
		//ShowErrorMsg(TEXT("ģ��ͼ��С����ԭͼ��С"), 0);
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

	//ȡ��ֵ(��ƥ�䷽������)
	//    matchLoc = minLoc;
	matchLoc = maxLoc;



	/*if (threSholdFlag) {
		threShold = ScoreThreshold_1;
	}*/

	if (maxVal >= threShold) {
		//������ƥ�������
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
	int result_w = src.cols - temp.cols + 1;    //���ͼ��ĳߴ�
	int result_h = src.rows - temp.rows + 1;
	result.create(result_h, result_w, CV_32FC1);    //���ͼ���ǵ�ͨ��32λ������


	matchTemplate(src, temp, result, cv::TM_CCOEFF_NORMED); //ģ��ƥ�䣬����1������ԭͼ�� ��������ͼ����Ӧ���ǵ�ͨ����8-���ػ�32-���� ������ͼ��temp��ƥ��Сͼ���ܴ�������ͼ�񣬲���3�ȽϽ����ӳ��ͼ��   CV_TM_CCOEFF_NORMED  �������ϵ��ƥ�䷨�����ƥ��Ϊ1��
	//normalize(result, result, 0, 1, NORM_MINMAX, -1);//��һ��0��1
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
		//ͨ�����������,ֻ�ı仭ͼ�Ŀ�,���ܸı�ͼƬ������
	if (!importFilepath.isNull()) {
		//�Ѿ����ڣ��򸲸�
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
		QMessageBox::warning(0, "֪ͨ", "����ɹ�");
		emit modelFilePATH(importFilepath);
	}
	else {
		//δ���ڣ����½�
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
			//�������򴴽�
			dir.mkdir(dirpath); //ֻ����һ����Ŀ¼�������뱣֤�ϼ�Ŀ¼����
		}

		QString fullpath = dirpath + getCurrImageName();

		std::string str = codec->fromUnicode(fullpath).data();

		cv::imwrite(str, MatSrcImage);
		emit modelFilePATH(path);
		file.close();
		QMessageBox::warning(0, "֪ͨ", "����ɹ�");

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

	//���Ϊ
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
		//�������򴴽�
		dir.mkdir(dirpath); //ֻ����һ����Ŀ¼�������뱣֤�ϼ�Ŀ¼����
	}

	QString fullpath = dirpath + getCurrImageName();

	std::string str = codec->fromUnicode(fullpath).data();

	cv::imwrite(str, MatSrcImage);
	emit modelFilePATH(path);
	file.close();	
	QMessageBox::warning(0, "֪ͨ", "���Ϊ�ɹ�");

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

				//���imageName
		QString icomString = getImageNameFromXML(root);
		if (icomString.isEmpty()) {
			QMessageBox::warning(nullptr, tr("�Ӿ�ģ��"),
				tr("�Ӿ�ģ�����ô���,����������ģ����Ϣ"));
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
			tr("δѡ���ļ�."));
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
				//��patternImageName��ֵ���ڶ�ȡģ��ͼ
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
	//ͨ���ɼ�ͼƬ��ʱ��,���õ���·��
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

//ģ�����ý��浼�빦������
//nameָ����ͼƬ�ļ���,�����޸�,��Ϊ��label��
bool FileController::getImageFromCameraFromImport(QString imageName)
{
	bool addedSucceeded = false;

	//����ͼƬ����
	QStringList stringModelNameAndPath = 	imageName.split("=");
	 QString fileName = stringModelNameAndPath.at(1);
	QImage image(stringModelNameAndPath.at(0)+ fileName);
	int x = image.width();
	if (x==0)
	{
		QMessageBox::warning(nullptr, tr("ͼƬ�쳣"),
			"ģ��ͼƬ�쳣,����ʧ��");
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
		if (label->getType().contains("��������"))
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
		if (label->getType().contains("��������"))
		{
			if (label) {
				const Area* area = label->getArea();
				if (area) {
					Shape* shape = area->getShape(currImageName);
					currType = shape->getType();
					//�ж��Ƿ�ǰͼ�������
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
	//û����������,��ȫͼ
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
				"û����������");
			return;
		}
		//��ȡ����
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
				"û����������");
			return;
		}
		//��ȡ����
		if (patternAreaREAL_size_rect.x() >= 0 && patternAreaREAL_size_rect.y() >= 0 && patternAreaREAL_size_rect.width() > 0 && patternAreaREAL_size_rect.height() > 0) {
			cv::Rect patternAreaRealSize(patternAreaREAL_size_rect.x(), patternAreaREAL_size_rect.y(), patternAreaREAL_size_rect.width(), patternAreaREAL_size_rect.height());
			cv::Mat patternImg = MatSrcImage(patternAreaRealSize);

		// ������Բ����
		cv::Point center(patternAreaREAL_size_rect.x() + patternAreaREAL_size_rect.width()/2, patternAreaREAL_size_rect.y()
		+ patternAreaREAL_size_rect.height() / 2);
		int width = patternAreaREAL_size_rect.width();
		int length = patternAreaREAL_size_rect.height();

		// ������Բ������
		cv::Mat mask = cv::Mat::zeros(MatSrcImage.size(), CV_8UC1);
		cv::ellipse(mask, center, cv::Size(width / 2, length / 2), 0, 0, 360, 255, -1);
		cv::Mat maskPattern = mask(patternAreaRealSize);


		emit sendImageToPatternWithMask(Mat2QImage(patternImg), Mat2QImage(srcImgMat), Mat2QImage(maskPattern));
		}
	}
	else if (currType == Shape::Polygon) {
		// ��QPolygonF�еĵ�����ת��Ϊvector<cv::Point>
		std::vector<cv::Point> points;
		for (const QPointF& point : patternAreaREAL_size_polygon) {
			points.emplace_back(static_cast<int>(point.x()), static_cast<int>(point.y()));
		}
		points.pop_back();
		// ����һ����ͼ����ͬ��С�ĺ�ɫ����
		cv::Mat mask = cv::Mat::zeros(MatSrcImage.size(), MatSrcImage.type());


		// ��������ת��Ϊvector<vector<cv::Point>>��ʽ�Է���cv::fillPoly��Ҫ��
		std::vector<std::vector<cv::Point>> pts = { points };
		// �������������������
		cv::fillPoly(mask, pts, cv::Scalar(255, 255, 255));

		// ʹ��������ȡ���������
		cv::Mat result;
		cv::bitwise_and(MatSrcImage, mask, result);
		// ���Ұ�������ε���С���� �߽��
		cv::Rect boundingRect = cv::boundingRect(points);

		// �ü�ͼ��ȥ���Աߵĺ�ɫ����
		cv::Mat croppedResult = result(boundingRect).clone();


		// ѡ���ʵ�����ֵ������ʹ�� 128 ��Ϊ���ӣ����Ը���ʵ���������
		int threshold_value = 1;

		// Ӧ�ö�ֵ������
		cv::threshold(croppedResult, mask, threshold_value, 255, cv::THRESH_BINARY);

		emit sendImageToPatternWithMask(Mat2QImage(croppedResult), Mat2QImage(srcImgMat), Mat2QImage(mask));
	}
}

void FileController::slot_receiveDrawPoint(QPoint resultPoint,int totalModelTime) {

	cv::Mat srcImgClone = srcImgMat.clone();
	// �����Ϸ�д��totalModelTime����
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
	case QImage::Format_RGB32:  //һ��Qt�����ɫͼ��Ϊ�˸�ʽ
		mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
		cv::cvtColor(mat, mat, cv::COLOR_BGRA2BGR);   //ת3ͨ��
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
