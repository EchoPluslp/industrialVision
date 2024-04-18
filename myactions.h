#ifndef MYACTIONS_H
#define MYACTIONS_H
//#pragma execution_character_set("utf-8")

#include <QAction>
//��������Action�����֡�ͼ��
class MyActions : public QAction
{
	Q_OBJECT
public:
	explicit MyActions(QObject* parent = nullptr);
	virtual ~MyActions() = 0;
};

class ExecPatternAction : public MyActions
{
	Q_OBJECT
public:
	explicit ExecPatternAction(QObject* parent = nullptr);

};

class GetImageAction : public MyActions
{
	Q_OBJECT
public:
	explicit GetImageAction(QObject* parent = nullptr);

};

class OpenFileAction : public MyActions
{
	Q_OBJECT
public:
	explicit OpenFileAction(QObject* parent = nullptr);

};

class OpenDirAction : public MyActions
{
	Q_OBJECT
public:
	explicit OpenDirAction(QObject* parent = nullptr);

};

class NextImageAction : public MyActions
{
	Q_OBJECT
public:
	explicit NextImageAction(QObject* parent = nullptr);

};

class SetPointAction : public MyActions
{
	Q_OBJECT
public:
	explicit SetPointAction(QObject* parent = nullptr);
};

class PrevImageAction : public MyActions
{
	Q_OBJECT
public:
	explicit PrevImageAction(QObject* parent = nullptr);
};

class SaveAction : public MyActions
{
	Q_OBJECT
public:
	explicit SaveAction(QObject* parent = nullptr);
};

class SaveAsAction : public MyActions
{
	Q_OBJECT
public:
	explicit SaveAsAction(QObject* parent = nullptr);
};

class ImportAction : public MyActions
{
	Q_OBJECT
public:
	explicit ImportAction(QObject* parent = nullptr);
};


class CloseFileAction : public MyActions
{
	Q_OBJECT
public:
	explicit CloseFileAction(QObject* parent = nullptr);
};

class DeleteLabelAction : public MyActions
{
	Q_OBJECT
public:
	explicit DeleteLabelAction(QObject* parent = nullptr);
};

class RectModeAction : public MyActions
{
	Q_OBJECT
public:
	explicit RectModeAction(QObject* parent = nullptr);
};

class EllipseModeAction : public MyActions
{
	Q_OBJECT
public:
	explicit EllipseModeAction(QObject* parent = nullptr);
};

class PolygonModeAction : public MyActions
{
	Q_OBJECT
public:
	explicit PolygonModeAction(QObject* parent = nullptr);
};

class CurveModeAction : public MyActions
{
	Q_OBJECT
public:
	explicit CurveModeAction(QObject* parent = nullptr);
};

class SquarePenModeAction : public MyActions
{
	Q_OBJECT
public:
	explicit SquarePenModeAction(QObject* parent = nullptr);
};

class CirclePenModeAction : public MyActions
{
	Q_OBJECT
public:
	explicit CirclePenModeAction(QObject* parent = nullptr);
};

class CurseModeAction : public MyActions
{
	Q_OBJECT
public:
	explicit CurseModeAction(QObject* parent = nullptr);
};

class ThreeDModeAction : public MyActions
{
	Q_OBJECT
public:
	explicit ThreeDModeAction(QObject* parent = nullptr);
};

class TwoDModeAction : public MyActions
{
	Q_OBJECT
public:
	explicit TwoDModeAction(QObject* parent = nullptr);
};

class MagnifyAction : public MyActions
{
	Q_OBJECT
public:
	explicit MagnifyAction(QObject* parent = nullptr);
};

class UndoAction : public MyActions
{
	Q_OBJECT
public:
	explicit UndoAction(QObject* parent = nullptr);
};

class RedoAction : public MyActions
{
	Q_OBJECT
public:
	explicit RedoAction(QObject* parent = nullptr);
};
//��������
class searchAreaRectAction : public MyActions
{
	Q_OBJECT
public:
	explicit searchAreaRectAction(QObject* parent = nullptr);
};
//����ƥ������
class featureMatchingRectAction : public MyActions
{
	Q_OBJECT
public:
	explicit featureMatchingRectAction(QObject* parent = nullptr);
};
class helpAction : public MyActions
{
	Q_OBJECT
public:
	explicit helpAction(QObject* parent = nullptr);
};

#endif // MYACTIONS_H
