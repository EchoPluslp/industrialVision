#include"login.h"
login::login(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::login)
{
	
	QString settingPath = QCoreApplication::applicationDirPath() + "/setting.ini";
	QSettings* settings = new QSettings(settingPath, QSettings::IniFormat);
	settings->beginGroup("Idus");
	system_language = settings->value("system_language").toString();

    ui->setupUi(this);
    ui->password->setEchoMode(QLineEdit::EchoMode::Password);
	ui->dabeijing->setPixmap(QPixmap("Image/login/dabeijing.png"));
	ui->label->setPixmap(QPixmap("Image/login/user-fill.png"));
	ui->label_2->setPixmap(QPixmap("Image/login/lock-fill.png"));	

	//  //读取上次关闭时的状态
	QString logoPath = settings->value("logopath", "Image/login/logo.png").toString();
	QString textPath= settings->value("textpath", "Image/login/text.png").toString();

	ui->label_logo->setPixmap(QPixmap(logoPath));
	ui->label_text->setPixmap(QPixmap(textPath));
	if (system_language == "zh")
	{
		setWindowTitle("登录界面");
	}
	else if (system_language == "en")
	{
		setWindowTitle("Login");
		ui->label_3->setText("Login");
		ui->label_4->setText("User name:");
		ui->label_5->setText("Password:");
		ui->pushButton->setText("Login");
		setWindowTitle("Login interface");

		ui->username->clear();
		ui->username->addItem("operator");
		ui->username->addItem("administrator");
	}
	setWindowFlags(Qt::WindowStaysOnTopHint);

	//白色填充左上角图标
	setWindowIcon(QIcon("icon.ico"));

	connect(&w, &industrialVision::sign_switchLogin,this,&login::slot_switchLog,Qt::DirectConnection);
}

void login::login_in() {
	QString username = ui->username->currentText();//获取用户名

    QString pwd = ui->password->text();//获取密码
    if (usersWithRoleAdmin.contains(username)) {
		if(pwd == passwordAdmin){ 
			w.setCURRENT_ROLE("ADMIN");

			w.showNormal();
			this->hide();
			 return;
		}
    }else if (usersWithRoleUser.contains(username))
    {
		if (pwd == passwordUser) {
			w.setCURRENT_ROLE("USER");

			w.showNormal();
			this->hide();
			return;
		}
    }
	if (system_language == "zh")
	{
		QMessageBox::critical(this, "错误信息", "密码或者用户名错误");
	}
	else if (system_language == "en")
	{
		QMessageBox::critical(this, "ERROR", "Password or username error");

	}
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
