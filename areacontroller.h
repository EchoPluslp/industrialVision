#ifndef AREACONTROLLER_H
#define AREACONTROLLER_H
#include <QMap>
#include <QObject>
#include "area.h"


//������ʱ�����ͼ�¼���ɺ������Shape����������Area�Թ�Label����ʹ��
class AreaController : public QObject
{
	Q_OBJECT
		Area* m_currArea;

public:
	AreaController(QObject* parent = nullptr);
	~AreaController() {
	}

	// ȡ�ߵ�ǰ��Area����m_currArea�ÿ�
	Area* takeArea();

	// ȡ�õ�ǰ��Area��ĳImage�ϵ�Shape
	Shape* getCurrAreaShape(QString imageName) const;

	bool hasCurrArea() const;
signals:

public slots:
	// ����Image-Shape
	void receiveImageShape(const QString& image, Shape* shape);
};

#endif // AREACONTROLLER_H
