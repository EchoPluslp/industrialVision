#ifndef LABELCONTROLLER_H
#define LABELCONTROLLER_H

#include <QSet>
#include <QMap>
#include <QList>
#include <QDebug>
#include <QColor>
#include <QObject>
#include "label.h"
#include "area.h"
#include "shape.h"



// ��������Label��Type
// ��������Label
// ����ַ�id
// ʵ��Label��������������

class LabelController : public QObject
{
	Q_OBJECT
	//��¼ΪĳType��Label�ַ���Id
	QMap<QString, int> m_typeToCount;
	//��¼ĳType�µ�����Label
	QMap<QString, QList<Label*> > m_typeToLabel;
	//��¼ĳType��Ӧ��Color,����Color��Controller�Զ�����
	QMap<QString, QColor> m_typeToColor;

	//��¼�Ҷ�ƥ��label �µ�����
	QMap<QString, ParamsItem> m_labelToAttr;

	bool createType(const QString& type);
public:
	LabelController(QObject* parent = nullptr);
	~LabelController();


public slots:
	//��Area��Type����Label
	Label* createLabel(const QString& type, Area* area);

	//�������Label
	bool addLabel(Label* label);

	//ɾ��Label
	bool deleteLabel(const QString& type, int id);
	//ɾ������label
	bool LabelController::deleteAllLabel();
	

	//����Label
	Label* getLabel(const QString& type, int id);

	//ȡ��ĳ��ͼƬ������Label
	QList<Label*> getLabelsOfImage(const QString& imageName);

	//ȡ��ĳ��ͼƬ������Shape
	QList<Shape*> getShapesOfImage(const QString& imageName);

	//ȡ��ĳ��GraphicsItem������Label
	Label* findParentLabelOfItem(QGraphicsItem* item);
	//ɾ��ĳ��GraphicsItem�������Ӧ��Label��
	void deleteItem(QGraphicsItem* item);

	//�Դ�LabelController����DOM�ڵ㣬����������Ϣ
	QDomElement getElement(QDomDocument& doc);
	//������ĳ��ͼƬ��Ӧ��DOM�ڵ㣬���浥��ͼƬ��Label��Ϣ
	QDomElement getElementOfImage(QDomDocument& doc, QString imageName);
	//��DOM�ڵ�ָ���Ϣ
	LabelController* createFromElement(QDomElement elem);

signals:
	void labelCreated(const QString&, int);
	void labelDeleted(const QString&, int);
	void typeCreated(const QString&, QColor color);
	void typeDeleted(const QString&);
};

#endif // LABELCONTROLLER_H
