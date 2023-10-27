#include "myglwidget.h"
#include<QDebug>
MyGLWidget::MyGLWidget(QWidget* parent)
	: QOpenGLWidget(parent)
{
	myText = "";
	crosshair = new QAction(tr("十字线"), this);
	crosshair->setFont(QFont(tr("宋体"), 26, QFont::Bold, false));//宋体26号，加粗，斜体字
	connect(crosshair, &QAction::triggered, this, &MyGLWidget::crosshair_Flag);


	rotate = new QAction(tr("旋转"), this);
	rotate->setFont(QFont(tr("宋体"), 26, QFont::Bold, false));//宋体26号，加粗，斜体字
	connect(rotate, &QAction::triggered, this, &MyGLWidget::rotate_Flag);

	//connect(quitSreenShot, SIGNAL(triggered()), this, SLOT(MenuClose()));
	restore = new QAction(tr("还原图片"), this);
	restore->setFont(QFont(tr("宋体"), 26, QFont::Bold, false));//宋体26号，加粗，斜体字
	connect(restore, &QAction::triggered, this, &MyGLWidget::restore_Flag);

	
	//connect(saveCopyAs, SIGNAL(triggered()), this, SLOT(SaveCopyAs()));
	RightButtonMenu = new QMenu(this);
	RightButtonMenu->setStyleSheet("QMenu{ background - color:rgb(40,87,144); border:1px solid rgb(40,87,144); }\
		QMenu::item{ max-width:20px; font - size: 12px; color: rgb(225,225,225); background:rgba(40,87,144,0.5); border:1px solid rgba(82,130,164,1);}\
		QMenu::item:selected{ background:rgba(82,130,164,1); border:1px solid rgba(82,130,164,1); }  /*选中或者说鼠标滑过状态*/\
		QMenu::item:pressed{ background:rgba(82,130,164,0.4); border:1px solid rgba(82,130,164,1);/*摁下状态*/ }");
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
	//判断是否有十字线
	myPixmap = pixmap;

	if (drawCrossHair_Flag)
	{
		drawcrossHair();
	}
	//旋转图片
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
	if (!this->myPixmap.isNull())//如果图片不为空
	{
		painter.eraseRect(rect);
		painter.translate(0 + XPtInterval, 0 + YPtInterval);//进行平移
		//	//按照高度等比例缩放图片
	int H = this->height();
	int realH = myPixmap.height();
	QPixmap pix = myPixmap.scaledToHeight(H - 5);//图片缩放到指定高度

		painter.drawPixmap(QRect(0, 0, pix.width() * factor, (pix.height() - 25) * factor), pix);
	}
}

void MyGLWidget::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::RightButton)
	{
			RightButtonMenu->clear();
		RightButtonMenu->addAction(crosshair);
		RightButtonMenu->addSeparator();    //分割线
		RightButtonMenu->addAction(rotate);
		RightButtonMenu->addSeparator();    //分割线
		RightButtonMenu->addAction(restore);
		RightButtonMenu->exec(QCursor::pos());  //在当前鼠标处堵住
		//以阻塞方式显示菜单，参数可指示菜单显示位置，另外该函数可返回单击选中项
	}
	oldPos = event->pos();
	Pressed = true;
}


void MyGLWidget::mouseMoveEvent(QMouseEvent* event)//移动
{
	if (Pressed != false)//按下鼠标
	{
		this->setCursor(Qt::SizeAllCursor);//设置光标
		QPoint pos = event->pos();
		int xPtInterval = pos.x() - oldPos.x();//计算移动的距离
		int yPtInterval = pos.y() - oldPos.y();
		XPtInterval += xPtInterval;//加上偏移的距离
		YPtInterval += yPtInterval;
		oldPos = pos;//更新位置
		update();
	}
}

void MyGLWidget::mouseReleaseEvent(QMouseEvent* event)//鼠标释放
{
	Pressed = false;
	this->setCursor(Qt::ArrowCursor);//设置光标
}

void MyGLWidget::mouseDoubleClickEvent(QMouseEvent* event)
{ 
	factor=1.0;
    XPtInterval = 0;
    YPtInterval = 0;
    update();
}

void MyGLWidget::wheelEvent(QWheelEvent* event)     //鼠标滑轮事件
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
	pen.setStyle(Qt::SolidLine);            //定义画笔的风格，直线、虚线等
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
	//传给主界面的值,用于模板图的旋转制作
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
