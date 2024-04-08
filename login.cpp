#include"login.h"
login::login(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
    ui->password->setEchoMode(QLineEdit::EchoMode::Password);
	ui->dabeijing->setPixmap(QPixmap("Image/login/dabeijing2.png"));

	//  //读取上次关闭时的状态
	QString settingPath = QCoreApplication::applicationDirPath() + "/setting.ini";
	QSettings* settings = new QSettings(settingPath, QSettings::IniFormat);
	settings->beginGroup("Idus");
	QString logoPath = settings->value("logopath", "Image/login/logo.png").toString();
	QString textPath= settings->value("textpath", "Image/login/text.png").toString();

//	ui->label_logo->setPixmap(QPixmap(logoPath));
	//ui->label_text->setPixmap(QPixmap(textPath));
	setWindowTitle("登录界面");
	setWindowFlags(Qt::WindowStaysOnTopHint);

	//白色填充左上角图标
	QPixmap pixmap(100, 100);
	pixmap.fill(Qt::white);
	setWindowIcon(QIcon(pixmap));
	connect(&w, &industrialVision::sign_switchLogin,this,&login::slot_switchLog,Qt::DirectConnection);
}

void login::login_in() {
	QString username = ui->username->currentText();//获取用户名

    QString pwd = ui->password->text();//获取密码
    if (username == ROLE_ADMIN) {
		if(pwd == passwordAdmin){ 
			w.setCURRENT_ROLE(ROLE_ADMIN);

			w.showNormal();
			this->hide();
			 return;
		}
    }else if (username == ROLE_USER)
    {
		if (pwd == passwordUser) {
			w.setCURRENT_ROLE(ROLE_USER);

			w.showNormal();
			this->hide();
			return;
		}
    }
    QMessageBox::critical(this, "错误信息","密码或者用户名错误");
}

login::~login()
{
    delete ui;
}

void login::slot_switchLog()
{
	 ui->password->setText("");//获取密码
	// w = new industrialVision();

		w.hide();
	// 重新初始化 w 窗口
	w.reinitialize();  // 假设你在 industrialVision 类中实现了 reinitialize 方法

	this->show();

}

void login::setPassword(QString passwordAdmin, QString passwordUser)
{
	this->passwordAdmin = passwordAdmin;
	this->passwordUser = passwordUser;
}
