#include "myglwidget.h"
#include<QDebug>
MyGLWidget::MyGLWidget(QWidget* parent)
	: QOpenGLWidget(parent)
{
	myText = "";
	crosshair = new QAction(tr("ʮ����"), this);
	crosshair->setFont(QFont(tr("����"), 26, QFont::Bold, false));//����26�ţ��Ӵ֣�б����
	connect(crosshair, &QAction::triggered, this, &MyGLWidget::crosshair_Flag);


	rotate = new QAction(tr("��ת"), this);
	rotate->setFont(QFont(tr("����"), 26, QFont::Bold, false));//����26�ţ��Ӵ֣�б����
	connect(rotate, &QAction::triggered, this, &MyGLWidget::rotate_Flag);

	//connect(quitSreenShot, SIGNAL(triggered()), this, SLOT(MenuClose()));
	restore = new QAction(tr("��ԭͼƬ"), this);
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

MyGLWidget::MyGLWidget()
{
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
	QPixmap pix = myPixmap.scaledToHeight(H - 5);//ͼƬ���ŵ�ָ���߶�

		painter.drawPixmap(QRect(0, 0, pix.width() * factor, (pix.height() - 25) * factor), pix);
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
	factor=1.0;
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
	emit valueChanged(m_rotateIndexInt);
}

void MyGLWidget::restore_Flag() {
	m_rotateIndexInt = 0;
	factor = 1.0;
	XPtInterval = 0;
	YPtInterval = 0;
	update();
	emit valueChanged(m_rotateIndexInt);

}
