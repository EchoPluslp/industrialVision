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
		QMessageBox::critical(NULL, "错误信息", "视觉软件已经启动,请关闭后再重新打开");
		return NULL;
	}

	QApplication a(argc, argv);
	 
	  //读取上次关闭时的状态
	QString settingPath = QCoreApplication::applicationDirPath() + "/setting.ini";
	QSettings* settings = new QSettings(settingPath, QSettings::IniFormat);
	settings->beginGroup("Idus");
	//管理员密码
	QString passwordAdmin = settings->value("passwordAdmin","2222").toString();
	//操作员密码
	QString passwordUser = settings->value("passwordUser","1111").toString();

	
	if (passwordUser.isNull() || passwordUser.size() == 0)
	{
		industrialVision w;

		//没有密码时,直接进去
		w.showNormal();
		return a.exec();
	}
	login x;
	x.setPassword(passwordAdmin, passwordUser);
	x.show();
	return a.exec();
}
//// 
// 自定义函数，计算SSIM
//
//int main() {
//	// 读取图像
//	cv::Mat img = cv::imread("123.bmp", cv::IMREAD_GRAYSCALE);
//	if (img.empty()) {
//		std::cout << "Image load failed!" << std::endl;
//		return -1;
//	}
//
//	// 创建CLAHE对象
//	// 可以调整clipLimit（对比度限制）和tileGridSize（用于应用CLAHE的区域大小）
//	cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
//	clahe->setClipLimit(2.0);
//	clahe->setTilesGridSize(cv::Size(2, 2));
//
//	// 应用CLAHE
//	cv::Mat img_clahe;
//	clahe->apply(img, img_clahe);
//
//	// 使用双边滤波平滑图像
//	cv::Mat filteredImg;
//	// 双边滤波参数：diameter, sigmaColor, sigmaSpace
//	cv::bilateralFilter(img_clahe, filteredImg, 9, 25, 25);
//
//	int newWidth = img.cols / 2;
//	int newHeight = img.rows / 2;
//
//	// 使用cv::resize函数缩小图像
//	cv::resize(img_clahe, img_clahe, cv::Size(newWidth, newHeight));
//	cv::resize(filteredImg, filteredImg, cv::Size(newWidth, newHeight));
//
//	cv::resize(img, img, cv::Size(newWidth, newHeight));
//	
//	// 计算PSNR
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
//	// 显示原始图像和CLAHE结果
//	cv::imshow("Original Image", img);
//	cv::imshow("CLAHE Image", img_clahe);
//	cv::imshow("filteredImg Image", filteredImg);
//
//
//	cv::waitKey(0); // 等待按键
//	return 0;
//}
// 计算PSNR函数
double calculatePSNR(const cv::Mat& I1, const cv::Mat& I2) {
	cv::Mat s1;
	absdiff(I1, I2, s1);       // |I1 - I2|
	s1.convertTo(s1, CV_32F);  // 转换为32位浮点数以避免溢出
	s1 = s1.mul(s1);           // |I1 - I2|^2

	cv::Scalar s = sum(s1);         // 对所有像素值求和
	double sse = s.val[0] + s.val[1] + s.val[2]; // 对所有通道求和

	if (sse <= 1e-10) { // 对于非常小的值我们将它们视为相等
		return 0;
	}
	else {
		double mse = sse / (double)(I1.channels() * I1.total());
		double psnr = 10.0 * log10((255 * 255) / mse);
		return psnr;
	}
}
//int main() {
//	//读取两张含有人脸的图像
//	Mat img1 = imread("321.bmp", IMREAD_ANYCOLOR);
//	Mat img2 = imread("321.bmp", IMREAD_ANYCOLOR);
//	if (img1.empty() || img2.empty())
//	{
//		cout << "请确认图像文件名称是否正确" << endl;
//		return -1;
//	}
//	Mat result3, result4,result5;
//
//
//	//验证不同标准差值的滤波效果
//	bilateralFilter(img2, result3, 9, 9, 9);
//	bilateralFilter(img2, result4, 25, 200, 200);
//
//	bilateralFilter(img2, result5, 25, 500, 500);
//
//	//显示原图
//	imshow("img1", img1);
//	imshow("img2", img2);
//
//	//不同标准差值滤波结果
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
			pixel[j] = (i % 2 == 0) ? 255 : 0; // 交替添加白色和黑色噪声
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
//	resize(img1, img1, Size(), 0.5, 0.5, INTER_LINEAR); // 缩小为原始尺寸的一半
//	resize(img2, img2, Size(), 0.5, 0.5, INTER_LINEAR); // 缩小为原始尺寸的一半
//
//	// 初始化SIFT检测器
//	Ptr<SIFT> sift = SIFT::create(50);
//
//	// 对两张图像分别进行特征点检测和描述符提取
//	vector<KeyPoint> keypoints1, keypoints2;
//	Mat descriptors1, descriptors2;
//	sift->detectAndCompute(img1, noArray(), keypoints1, descriptors1);
//	sift->detectAndCompute(img2, noArray(), keypoints2, descriptors2);
//
//	// 使用BFMatcher进行特征点匹配
//	BFMatcher matcher(NORM_L2);
//	vector<DMatch> matches;
//	matcher.match(descriptors1, descriptors2, matches);
//
//	// 根据匹配结果绘制匹配图像
//	Mat img_matches;
//	drawMatches(img1, keypoints1, img2, keypoints2, matches, img_matches, Scalar::all(-1),
//		Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
//
//	// 显示匹配图像
//	imshow("Matches", img_matches);
//	imwrite("09.BMP", img_matches);
//	waitKey(0);
//
//}

bool checkOnly()
{
	//  创建互斥量  
	HANDLE m_hMutex = CreateMutex(NULL, FALSE, L"fortest_abc123");
	//  检查错误代码  
	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		//  如果已有互斥量存在则释放句柄并复位互斥量  
		CloseHandle(m_hMutex);
		m_hMutex = NULL;
		//  程序退出  
		return  false;
	}
	else
		return true;
}
