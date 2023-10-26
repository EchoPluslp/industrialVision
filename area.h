#ifndef AREA_H
#define AREA_H
#pragma execution_character_set("utf-8")

#include <QGraphicsItem>
#include <QList>
#include <QDir>
#include <QFile>
#include <QMap>
#include <QSharedPointer>
#include <QCoreApplication>

#include "shape.h"
#include "rect.h"
#include "ellipse.h"
#include "curve.h"
#include "polygon.h"
#include "circlepen.h"
#include "squarepen.h"
class Label;

// ��������һ��Label��ռ������
// 2D Label��һ��area��Ӧ"ĳ��ͼƬ"�ϵ�"ĳ����״"
// 3D Label��һ��area��Ӧ"����ͼƬ"�ϵ�"�����״"����ÿ������ֻ��һ����״
// ��Ҫ���ٲ�ѯ��ĳ��ͼƬ�ϵ���״���ʲ���Map��ͼƬ����ΪͼƬ��Ψһ��ʶ


class Area
{
	//����Image-Shape��Ϣ
	QMap<QString, Shape*> m_imageShapeMap;
	//��¼������Label
	Label* m_label;

public:

	Area() {}
	explicit Area(Label* label) : m_label(label) { }
	~Area();

	//���һ��Image-Shape
	bool addImageShape(const QString& image, Shape* shape);

	void setLabel(Label* label);

	//ȡ��ĳImage�ϵ�Shape
	Shape* getShape(const QString& image) const;

	const Label* getLabel() const;

	//ȡ�ô�Area��ռ������ͼƬ
	QList<QString> getAllImages() const;

	//��ȡһ��Area�Ŀ���
	Area* clone() const;

	//��ȡ��Area��DOM�ڵ��������ڱ�����Ϣ
	QDomElement getElement(QDomDocument& doc);

	//��DOM���ָ�Area
	Area* createFromElement(QDomElement elem);

	Shape* getFromElementToPattern(QDomElement elem);

	QString getFileName(QDomElement elem);
};

#endif //  AREA_H
