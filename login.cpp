#include"login.h"
login::login(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
    ui->password->setEchoMode(QLineEdit::EchoMode::Password);
	ui->dabeijing->setPixmap(QPixmap("Image/login/dabeijing.png"));
	ui->label->setPixmap(QPixmap("Image/login/user-fill.png"));
	ui->label_2->setPixmap(QPixmap("Image/login/lock-fill.png"));	

	//  //读取上次关闭时的状态
	QString settingPath = QCoreApplication::applicationDirPath() + "/setting.ini";
	QSettings* settings = new QSettings(settingPath, QSettings::IniFormat);
	settings->beginGroup("Idus");
	QString logoPath = settings->value("logopath", "Image/login/logo.png").toString();
	QString textPath= settings->value("textpath", "Image/login/text.png").toString();

	ui->label_logo->setPixmap(QPixmap(logoPath));
	ui->label_text->setPixmap(QPixmap(textPath));
	setWindowFlags(Qt::WindowStaysOnTopHint);

	currentLanguage = settings->value("language","zh").toString();
	slot_seting_language(currentLanguage);

	
	connect(&w, &industrialVision::sign_switchLogin,this,&login::slot_switchLog,Qt::DirectConnection);
	connect(&w, &industrialVision::send_login_language, this, &login::slot_seting_language, Qt::DirectConnection);
	settings->endGroup();
	setWindowIcon(QIcon("Image/login/icon.ico"));

}

void login::login_in() {
	QString username = ui->username->currentText();//获取用户名
	if (usersWithRoleUser.contains(username)) {
		username = ROLE_USER;
	}else if (usersWithRoleAdmin.contains(username))
	{
		username = ROLE_ADMIN;
	}

    QString pwd = ui->password->text();//获取密码
    if (username == ROLE_ADMIN) {
		if(pwd == passwordAdmin){ 
			w.setCURRENT_ROLE(ROLE_ADMIN);
			w.setLanguageInfo(currentLanguage);
			w.showNormal();

			this->hide();
			 return;
		}
    }else if (username == ROLE_USER)
    {
		if (pwd == passwordUser) {
			w.setCURRENT_ROLE(ROLE_USER);
			w.setLanguageInfo(currentLanguage);
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
	 w.setLanguageInfo(currentLanguage);

	 
		w.hide();
	// 重新初始化 w 窗口
	w.reinitialize();  // 假设你在 industrialVision 类中实现了 reinitialize 方法

	this->show();

}

void login::slot_seting_language(QString language)
{

	if (language=="zh")
	{
		currentLanguage = "zh";

		ui->label_3->setText("用户登录");
		ui->label_4->setText("用户名:");
		ui->label_5->setText("密码:");
		ui->pushButton->setText("登录");
		setWindowTitle("登录界面");

		ui->username->clear();
		ui->username->addItem("操作员");
		ui->username->addItem("管理员");


	}else if (language == "en")
	{
		currentLanguage = "en";

		ui->label_3->setText("Login");
		ui->label_4->setText("User name:");
		ui->label_5->setText("Password:");
		ui->pushButton->setText("Login");
		setWindowTitle("Login interface");

		ui->username->clear();
		ui->username->addItem("operator");
		ui->username->addItem("administrator");
	}else if (language == "es")
	{
		currentLanguage = "es";
		ui->label_3->setText("Iniciar sesión");
		ui->label_4->setText("Nombre de usuario:");
		ui->label_5->setText("Contraseña:");
		ui->pushButton->setText("Sesión");
		setWindowTitle("Interfaz de inicio de sesión");

		ui->username->clear();
		ui->username->addItem("operador");
		ui->username->addItem("administrador");
	}
}

void login::setPassword(QString passwordAdmin, QString passwordUser)
{
	this->passwordAdmin = passwordAdmin;
	this->passwordUser = passwordUser;
}
