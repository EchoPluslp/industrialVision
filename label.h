#ifndef LABEL_H
#define LABEL_H
#include <QString>
#include <QList>
#include <QColor>
#include "area.h"
#include"TableWidget.h"
class Label
{
	//���ĳLabel��������Label����
	int m_id;

	QString m_type;
	QColor m_color;
	Area* m_area;
public:
	Label() {}
	Label(const int id, const QString& type, const QColor color, Area* area);
	~Label();

	int getId() const;
	const QString& getType() const;
	const QColor getColor() const;
	const Area* getArea() const;
	QString getName() const;

	//�Դ�Label����Dom�ڵ㣬������Ϣ
	QDomElement getElement(QDomDocument& doc);
	//��Dom�ڵ�ָ�Label
	Label* createFromElement(QDomElement elem);
	Shape* createFromElementToPattern(QDomElement elem);

	//����Label
	Label* clone() const;
};

#endif // LABEL_H
