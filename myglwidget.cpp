#include "myglwidget.h"
#include<QDebug>
MyGLWidget::MyGLWidget(QWidget* parent)
	: QOpenGLWidget(parent)
{
	QString settingPath = QCoreApplication::applicationDirPath() + "/setting.ini";
	QSettings* settings = new QSettings(settingPath, QSettings::IniFormat);
	settings->beginGroup("Idus");
	system_language = settings->value("system_language").toString();

	myText = "";
	crosshair = new QAction(this);
	if (system_language == "zh")
	{
		crosshair->setText("ʮ����");
	}
	else if (system_language == "en")
	{
		crosshair->setText("Crosshair");
	}
	crosshair->setFont(QFont(tr("����"), 26, QFont::Bold, false));//����26�ţ��Ӵ֣�б����
	connect(crosshair, &QAction::triggered, this, &MyGLWidget::crosshair_Flag);

	if (system_language == "zh")
	{
		rotate = new QAction(tr("��ת"), this);
	}
	else if (system_language == "en")
	{
		rotate = new QAction(tr("Rotate"), this);
	}
	rotate->setFont(QFont(tr("����"), 26, QFont::Bold, false));//����26�ţ��Ӵ֣�б����
	connect(rotate, &QAction::triggered, this, &MyGLWidget::rotate_Flag);

	//connect(quitSreenShot, SIGNAL(triggered()), this, SLOT(MenuClose()));
	if (system_language == "zh")
	{
		restore = new QAction(tr("��ԭͼƬ"), this);
	}
	else if (system_language == "en")
	{
		restore = new QAction(tr("Restore image"), this);
	}
	restore->setFont(QFont(tr("����"), 26, QFont::Bold, false));//����26�ţ��Ӵ֣�б����
	connect(restore, &QAction::triggered, this, &MyGLWidget::restore_Flag);

	
	//connect(saveCopyAs, SIGNAL(triggered()), this, SLOT(SaveCopyAs()));
	RightButtonMenu = new QMenu(this);
	RightButtonMenu->setStyleSheet("QMenu{ background - color:rgb(40,87,144); border:1px solid rgb(40,87,144); }\
		QMenu::item{ max-width:20px; font - size: 12px; color: rgb(225,225,225); background:rgba(40,87,144,0.5); border:1px solid rgba(82,130,164,1);}\
		QMenu::item:selected{ background:rgba(82,130,164,1); border:1px solid rgba(82,130,164,1); }  /*ѡ�л���˵��껬��״̬*/\
		QMenu::item:pressed{ background:rgba(82,130,164,0.4); border:1px solid rgba(82,130,164,1);/*����״̬*/ }");
	RightButtonMenu->setMaximumWidth(300);
	RightButtonMenu->setMaximumHeight(300);
}


MyGLWidget::~MyGLWidget()
{

}
bool falg = true;
void MyGLWidget::setPixmap(QPixmap pixmap, QString text)
{	
	//�ж��Ƿ���ʮ����
	myPixmap = pixmap;

	if (drawCrossHair_Flag)
	{
		drawcrossHair();
	}
	//��תͼƬ
	//rotatePic();
	
	myText = text;
	m_width = pixmap.width();
	m_height = pixmap.height();

	this->update();
}

void MyGLWidget::setMouseClickFlag(bool flag)
{
	if (flag)
	{

		// ���� crosshair ���ı������õ�������¼�
		if (system_language == "zh")
		{
			crosshair->setText(tr("ʮ���ߣ���������"));
		}
		else if (system_language == "en")
		{
			crosshair->setText(tr("Cross line (locked)"));
		}
		crosshair->setDisabled(flag);

		//// ���� rotate ���ı������õ�������¼�
		if (system_language == "zh")
		{
			rotate->setText(tr("��ת����������"));
		}
		else if (system_language == "en")
		{
			rotate->setText(tr("Rotation (locked)"));
		}
		rotate->setDisabled(flag);

		//// ���� restore ���ı������õ�������¼�
		if (system_language == "zh")
		{
			restore->setText(tr("��ԭͼƬ����������"));
		}
		else if (system_language == "en")
		{
			restore->setText(tr("Restore image (locked)"));
		}
		restore->setDisabled(flag);
	}else {
		// ���� crosshair ���ı������õ�������¼�
		if (system_language == "zh")
		{
			crosshair->setText(tr("ʮ����"));
		}
		else if (system_language == "en")
		{
			crosshair->setText(tr("Crosshair"));
		}
		crosshair->setDisabled(flag);

		//// ���� rotate ���ı������õ�������¼�
		if (system_language == "zh")
		{
			rotate->setText(tr("��ת"));
		}
		else if (system_language == "en")
		{
			rotate->setText(tr("rotate"));
		}
		rotate->setDisabled(flag);

		//// ���� restore ���ı������õ�������¼�
		if (system_language == "zh")
		{
			restore->setText(tr("��ԭͼƬ"));
		}
		else if (system_language == "en")
		{
			restore->setText(tr("Restore image"));
		}
		restore->setDisabled(flag);
	}
}


void MyGLWidget::paintEvent(QPaintEvent* event)
{
	const QRect& rect = event->rect();

	QPainter painter(this);
	if (!this->myPixmap.isNull())//���ͼƬ��Ϊ��
	{
		painter.eraseRect(rect);
		painter.translate(0 + XPtInterval, 0 + YPtInterval);//����ƽ��
		//	//���ո߶ȵȱ�������ͼƬ
	int H = this->height();
	int realH = myPixmap.height();
	//QPixmap pix = myPixmap.scaledToHeight(H - 5);//ͼƬ���ŵ�ָ���߶�

		painter.drawPixmap(QRect(0, 0, myPixmap.width() * factor, (myPixmap.height() - 25) * factor), myPixmap);
	}
}

void MyGLWidget::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::RightButton)
	{
		RightButtonMenu->clear();
		RightButtonMenu->addAction(crosshair);
		RightButtonMenu->addSeparator();    //�ָ���
		RightButtonMenu->addAction(rotate);
		RightButtonMenu->addSeparator();    //�ָ���
		RightButtonMenu->addAction(restore);
		RightButtonMenu->exec(QCursor::pos());  //�ڵ�ǰ��괦��ס
		//��������ʽ��ʾ�˵���������ָʾ�˵���ʾλ�ã�����ú����ɷ��ص���ѡ����
	}
	oldPos = event->pos();
	Pressed = true;
}


void MyGLWidget::mouseMoveEvent(QMouseEvent* event)//�ƶ�
{
	if (Pressed != false)//�������
	{
		this->setCursor(Qt::SizeAllCursor);//���ù��
		QPoint pos = event->pos();
		int xPtInterval = pos.x() - oldPos.x();//�����ƶ��ľ���
		int yPtInterval = pos.y() - oldPos.y();
		XPtInterval += xPtInterval;//����ƫ�Ƶľ���
		YPtInterval += yPtInterval;
		oldPos = pos;//����λ��
		update();
	}
}

void MyGLWidget::mouseReleaseEvent(QMouseEvent* event)//����ͷ�
{
	Pressed = false;
	this->setCursor(Qt::ArrowCursor);//���ù��
}

void MyGLWidget::mouseDoubleClickEvent(QMouseEvent* event)
{ 
	factor=0.3;
    XPtInterval = 0;
    YPtInterval = 0;
    update();
}

void MyGLWidget::wheelEvent(QWheelEvent* event)     //��껬���¼�
{
	double numDegrees = event->delta() / 8.0;
	double numSteps = numDegrees / 15.0;
	factor *= pow(1.1, numSteps);
	if (factor < 0.07)
	{
		factor = 0.07;
	}
	else if (factor > 50)
	{
		factor = 50;
	}
	update();
}

void MyGLWidget::drawcrossHair()
{
	QPen pen;
	pen.setStyle(Qt::SolidLine);            //���廭�ʵķ��ֱ�ߡ����ߵ�
	pen.setWidth(10);
	pen.setBrush(Qt::green);
	QPainter painter(&myPixmap);
	painter.setPen(pen);
	painter.drawLine(QLine(myPixmap.width() / 2, 0, myPixmap.width() / 2, myPixmap.height()));
	painter.drawLine(QLine(0, myPixmap.height() / 2, myPixmap.width(), myPixmap.height() / 2));
}


void MyGLWidget::crosshair_Flag() {
	drawCrossHair_Flag = !drawCrossHair_Flag;
}

void MyGLWidget::rotate_Flag() {
	m_rotateIndexInt++;
	if ((m_rotateIndexInt % angleArc.size()) == 0 )
	{
		m_rotateIndexInt = 0;
	}
	//�����������ֵ,����ģ��ͼ����ת����
	emit rotateIndexValueChanged(m_rotateIndexInt);
}

void MyGLWidget::restore_Flag() {
	m_rotateIndexInt = defaultRotateIndexValue;
	factor = 1.0;
	XPtInterval = 0;
	YPtInterval = 0;
	update();
	emit rotateIndexValueChanged(m_rotateIndexInt);

}
