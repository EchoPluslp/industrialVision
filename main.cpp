#include "login.h"
#include <QtWidgets/QApplication>
#include <QCoreApplication>
bool checkOnly();
//
int main(int argc, char *argv[]){


#if (QT_VERSION > QT_VERSION_CHECK(5,6,0))
	QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
	if (!checkOnly())
	{
		QMessageBox::critical(NULL, "������Ϣ", "�Ӿ�����Ѿ�����,��رպ������´�");
		return NULL;
	}

	QApplication a(argc, argv);
	 
	  //��ȡ�ϴιر�ʱ��״̬
	QString settingPath = QCoreApplication::applicationDirPath() + "/setting.ini";
	QSettings* settings = new QSettings(settingPath, QSettings::IniFormat);
	settings->beginGroup("Idus");
	//����Ա����
	QString passwordAdmin = settings->value("passwordAdmin","2222").toString();
	//����Ա����
	QString passwordUser = settings->value("passwordUser","1111").toString();

	
	if (passwordUser.isNull() || passwordUser.size() == 0)
	{
		industrialVision w;

		//û������ʱ,ֱ�ӽ�ȥ
		w.showNormal();
		return a.exec();
	}
	login x;
	x.setPassword(passwordAdmin, passwordUser);
	x.show();
	return a.exec();
}
//// 
// �Զ��庯��������SSIM
//
//int main() {
//	// ��ȡͼ��
//	cv::Mat img = cv::imread("123.bmp", cv::IMREAD_GRAYSCALE);
//	if (img.empty()) {
//		std::cout << "Image load failed!" << std::endl;
//		return -1;
//	}
//
//	// ����CLAHE����
//	// ���Ե���clipLimit���Աȶ����ƣ���tileGridSize������Ӧ��CLAHE�������С��
//	cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
//	clahe->setClipLimit(2.0);
//	clahe->setTilesGridSize(cv::Size(2, 2));
//
//	// Ӧ��CLAHE
//	cv::Mat img_clahe;
//	clahe->apply(img, img_clahe);
//
//	// ʹ��˫���˲�ƽ��ͼ��
//	cv::Mat filteredImg;
//	// ˫���˲�������diameter, sigmaColor, sigmaSpace
//	cv::bilateralFilter(img_clahe, filteredImg, 9, 25, 25);
//
//	int newWidth = img.cols / 2;
//	int newHeight = img.rows / 2;
//
//	// ʹ��cv::resize������Сͼ��
//	cv::resize(img_clahe, img_clahe, cv::Size(newWidth, newHeight));
//	cv::resize(filteredImg, filteredImg, cv::Size(newWidth, newHeight));
//
//	cv::resize(img, img, cv::Size(newWidth, newHeight));
//	
//	// ����PSNR
//	double psnrCLAHE = cv::PSNR(img, img_clahe);
//	double psnrFiltered = cv::PSNR(img, filteredImg);
//	double psnrFiltered2 = cv::PSNR(img_clahe, filteredImg);
//
//	std::cout << "PSNR between original and CLAHE image: " << psnrCLAHE << " dB" << std::endl;
//	std::cout << "PSNR between original and bilateral filtered image: " << psnrFiltered << " dB" << std::endl;
//	std::cout << "PSNR betweenpsnrFiltered: " << psnrFiltered2 << " dB" << std::endl;
//
//	double ssim_va12 = computeSSIM(img, img_clahe);
//	double ssim_val3 = computeSSIM(img, filteredImg);
//	double ssim_val4 = computeSSIM(img_clahe, filteredImg);
//
//
//	// ��ʾԭʼͼ���CLAHE���
//	cv::imshow("Original Image", img);
//	cv::imshow("CLAHE Image", img_clahe);
//	cv::imshow("filteredImg Image", filteredImg);
//
//
//	cv::waitKey(0); // �ȴ�����
//	return 0;
//}
// ����PSNR����
double calculatePSNR(const cv::Mat& I1, const cv::Mat& I2) {
	cv::Mat s1;
	absdiff(I1, I2, s1);       // |I1 - I2|
	s1.convertTo(s1, CV_32F);  // ת��Ϊ32λ�������Ա������
	s1 = s1.mul(s1);           // |I1 - I2|^2

	cv::Scalar s = sum(s1);         // ����������ֵ���
	double sse = s.val[0] + s.val[1] + s.val[2]; // ������ͨ�����

	if (sse <= 1e-10) { // ���ڷǳ�С��ֵ���ǽ�������Ϊ���
		return 0;
	}
	else {
		double mse = sse / (double)(I1.channels() * I1.total());
		double psnr = 10.0 * log10((255 * 255) / mse);
		return psnr;
	}
}
//int main() {
//	//��ȡ���ź���������ͼ��
//	Mat img1 = imread("321.bmp", IMREAD_ANYCOLOR);
//	Mat img2 = imread("321.bmp", IMREAD_ANYCOLOR);
//	if (img1.empty() || img2.empty())
//	{
//		cout << "��ȷ��ͼ���ļ������Ƿ���ȷ" << endl;
//		return -1;
//	}
//	Mat result3, result4,result5;
//
//
//	//��֤��ͬ��׼��ֵ���˲�Ч��
//	bilateralFilter(img2, result3, 9, 9, 9);
//	bilateralFilter(img2, result4, 25, 200, 200);
//
//	bilateralFilter(img2, result5, 25, 500, 500);
//
//	//��ʾԭͼ
//	imshow("img1", img1);
//	imshow("img2", img2);
//
//	//��ͬ��׼��ֵ�˲����
//	imshow("result3 ", result3);
//	imshow("result4", result4);
//	imshow("result5", result5);
//
//	waitKey(0);
//	return 0;
//}

void addSaltAndPepperNoise(Mat& image, double noiseRatio) {
	int n = static_cast<int>(noiseRatio * image.cols * image.rows);
	for (int i = 0; i < n; i++) {
		int r = rand() % image.rows;
		int c = rand() % image.cols;
		uchar* pixel = image.ptr<uchar>(r) + c * image.channels();
		for (int j = 0; j < image.channels(); j++) {
			pixel[j] = (i % 2 == 0) ? 255 : 0; // ������Ӱ�ɫ�ͺ�ɫ����
		}
	}
}

//
//int main() {
//	Mat img1 = imread("1.bmp", IMREAD_GRAYSCALE);
//	Mat img2 = imread("2.bmp", IMREAD_GRAYSCALE);
//	if (img1.empty() || img2.empty()) {
//		cout << "Could not open or find the images!" << endl;
//		return -1;
//	}
//	resize(img1, img1, Size(), 0.5, 0.5, INTER_LINEAR); // ��СΪԭʼ�ߴ��һ��
//	resize(img2, img2, Size(), 0.5, 0.5, INTER_LINEAR); // ��СΪԭʼ�ߴ��һ��
//
//	// ��ʼ��SIFT�����
//	Ptr<SIFT> sift = SIFT::create(50);
//
//	// ������ͼ��ֱ���������������������ȡ
//	vector<KeyPoint> keypoints1, keypoints2;
//	Mat descriptors1, descriptors2;
//	sift->detectAndCompute(img1, noArray(), keypoints1, descriptors1);
//	sift->detectAndCompute(img2, noArray(), keypoints2, descriptors2);
//
//	// ʹ��BFMatcher����������ƥ��
//	BFMatcher matcher(NORM_L2);
//	vector<DMatch> matches;
//	matcher.match(descriptors1, descriptors2, matches);
//
//	// ����ƥ��������ƥ��ͼ��
//	Mat img_matches;
//	drawMatches(img1, keypoints1, img2, keypoints2, matches, img_matches, Scalar::all(-1),
//		Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
//
//	// ��ʾƥ��ͼ��
//	imshow("Matches", img_matches);
//	imwrite("09.BMP", img_matches);
//	waitKey(0);
//
//}

bool checkOnly()
{
	//  ����������  
	HANDLE m_hMutex = CreateMutex(NULL, FALSE, L"fortest_abc123");
	//  ���������  
	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		//  ������л������������ͷž������λ������  
		CloseHandle(m_hMutex);
		m_hMutex = NULL;
		//  �����˳�  
		return  false;
	}
	else
		return true;
}
