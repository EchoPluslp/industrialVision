#include"login.h"
login::login(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::login)
{
	//if (checkOnly() == false)
	//	return ;
    ui->setupUi(this);
    ui->password->setEchoMode(QLineEdit::EchoMode::Password);
	ui->dabeijing->setPixmap(QPixmap("Image/login/dabeijing.png"));
	ui->label->setPixmap(QPixmap("Image/login/user-fill.png"));
	ui->label_2->setPixmap(QPixmap("Image/login/lock-fill.png"));	
	ui->label_logo->setPixmap(QPixmap("Image/login/logo.png"));
	ui->label_text->setPixmap(QPixmap("Image/login/text.png"));
	setWindowTitle(" ");
	setWindowFlags(Qt::WindowStaysOnTopHint);

	//白色填充左上角图标
	QPixmap pixmap(100, 100);
	pixmap.fill(Qt::white);
	setWindowIcon(QIcon(pixmap));
}

void login::login_in() {
    QString pwd = ui->password->text();//获取密码
    if (pwd == password) {
        w.showNormal();
        this->close();
        return;
    }
    QMessageBox::critical(this, "错误信息","密码或者用户名错误");
}

//bool login::checkOnly()
//{
//	//  创建互斥量  
//	HANDLE m_hMutex = CreateMutex(NULL, FALSE, L"fortest_abc123");
//	//  检查错误代码  
//	if (GetLastError() == ERROR_ALREADY_EXISTS) {
//		//  如果已有互斥量存在则释放句柄并复位互斥量  
//		CloseHandle(m_hMutex);
//		m_hMutex = NULL;
//		//  程序退出  
//		return  false;
//	}
//	else
//		return true;
//}

login::~login()
{
    delete ui;
}

void login::setPassword(QString passWordFromMain)
{
	password = passWordFromMain;
}
