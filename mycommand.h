#ifndef MYCOMMAND_H
#define MYCOMMAND_H


#include <QUndoCommand>
#include <QDomDocument>
#include "paintscene.h"
#include "labelcontroller.h"
#include "filecontroller.h"
#include "areacontroller.h"
#include "mygraphicsitems.h"



/*
 * Undo/Redoָ����
 * ֧�ֵ�Undo/Redo��Label�Ĵ�����ɾ����ͼƬ���л�
*/


class CreateLabelCommand :public QUndoCommand
{
public:
	CreateLabelCommand(const QString& type, Area* area, LabelController* labelController, QUndoCommand* parent = nullptr);
	~CreateLabelCommand();
	// ����һ��label
	void redo() override;
	// ��������һ��label
	void undo() override;
private:
	QString m_type;// ����type
	int m_id;// ����id
	Area* m_area;// ����area��Ϣ
	LabelController* m_labelController;
};



class DeleteCurrLabelCommand :public QUndoCommand
{
public:
	DeleteCurrLabelCommand(LabelController* labelController, QString type, int id, QUndoCommand* parent = nullptr);
	~DeleteCurrLabelCommand();
	// ɾ����ǰlabel
	void redo() override;
	// ����ɾ��label
	void undo() override;
private:
	QString m_type;
	int m_id;
	Label* m_label;
	LabelController* m_labelController;
};

class SwitchImageCommand : public QUndoCommand
{
public:
	SwitchImageCommand(QString prevImageName, QString nextImageName, FileController* fileController, QUndoCommand* parent = nullptr);
	// �л�
	void redo() override;
	// ����
	void undo() override;
private:
	QString m_prev, m_next;
	FileController* m_fileController;
};

class CloseImageCommand : public QUndoCommand
{
public:
	CloseImageCommand(FileController* fileController, QUndoCommand* parent = nullptr);
	// �ر�
	void redo() override;
	// ��
	void undo() override;
private:
	FileController* m_fileController;
	QString m_imageName;
	QString m_imagePath;
};





#endif // MYCOMMAND_H
