#include "myactions.h"
#include <QDebug>
#pragma execution_character_set("utf-8")

MyActions::MyActions(QObject* parent) : QAction(parent)
{
}
MyActions::~MyActions() {}

ExecPatternAction::ExecPatternAction(QObject* parent) : MyActions(parent) {
	setIcon(QIcon("./Image/Icon/open_file"));

	setText("Ч��Ԥ��");
	setFont(QFont(tr("����"), 30, QFont::Bold, false));//����26�ţ��Ӵ֣�б����
	//setShortcuts(QKeySequence::Open);
}

GetImageAction::GetImageAction(QObject* parent) : MyActions(parent) {
	setIcon(QIcon("./Image/Icon/zhixing"));
	setText("�ɼ�ͼƬ");
	setFont(QFont(tr("����"), 30, QFont::Bold, false));//����26�ţ��Ӵ֣�б����
	
	//setShortcuts(QKeySequence::Open);
}

OpenFileAction::OpenFileAction(QObject* parent) : MyActions(parent) {
	//setIcon(QIcon("./Image/Icon/zhixing"));
	//setText("��ͼƬ");
	setFont(QFont(tr("����"), 30, QFont::Bold, false));//����26�ţ��Ӵ֣�б����
	//setShortcuts(QKeySequence::Open);
}



OpenDirAction::OpenDirAction(QObject* parent) : MyActions(parent) {
	//setIcon(QIcon("./Image/Icon/icon-_5"));
	//setText("��Ŀ¼");
	setFont(QFont(tr("����"), 30, QFont::Bold, false));//����26�ţ��Ӵ֣�б����
}


PrevImageAction::PrevImageAction(QObject* parent) : MyActions(parent) {
	//setIcon(QIcon("./Image/Icon/prev_image"));
	//setText("��һ��ͼ");
	setFont(QFont(tr("����"), 30, QFont::Bold, false));//����26�ţ��Ӵ֣�б����
//	setShortcut(tr("Shift+Tab"));
}

NextImageAction::NextImageAction(QObject* parent) : MyActions(parent) {
	//setIcon(QIcon("./Image/Icon/next_image"));
	//setText("��һ��ͼ"); 
	setFont(QFont(tr("����"), 30, QFont::Bold, false));//����26�ţ��Ӵ֣�б����
	//setShortcut(tr("Tab"));
}

DeleteLabelAction::DeleteLabelAction(QObject* parent) : MyActions(parent) {
	setIcon(QIcon("./Image/Icon/delete_label"));
	setText("ɾ������");
	setFont(QFont(tr("����"), 30, QFont::Bold, false));//����26�ţ��Ӵ֣�б����
	//setShortcuts(QKeySequence::Delete);
}

CloseFileAction::CloseFileAction(QObject* parent) : MyActions(parent) {
	setIcon(QIcon("./Image/Icon/x"));
	setText("�رյ�ǰ�ļ�");
	setFont(QFont(tr("����"), 30, QFont::Bold, false));//����26�ţ��Ӵ֣�б����
	//setShortcuts(QKeySequence::Close);
}

SetPointAction::SetPointAction(QObject* parent) : MyActions(parent) {
	setIcon(QIcon("./Image/Icon/sendpoint"));
	setText("�����");
	setFont(QFont(tr("����"), 30, QFont::Bold, false));//����26�ţ��Ӵ֣�б����
	//setShortcut(tr("Ctrl+R"));
}

RectModeAction::RectModeAction(QObject* parent) : MyActions(parent) {
	setIcon(QIcon("./Image/Icon/rect_mode"));
	setText("����");
	setFont(QFont(tr("����"), 30, QFont::Bold, false));//����26�ţ��Ӵ֣�б����
	//setShortcut(tr("Ctrl+R"));
}

EllipseModeAction::EllipseModeAction(QObject* parent) : MyActions(parent) {
	setIcon(QIcon("./Image/Icon/ellipse_mode"));
	setText("��Բ");
	setFont(QFont(tr("����"), 30, QFont::Bold, false));//����26�ţ��Ӵ֣�б����
	//setShortcut(tr("Ctrl+E"));
}

PolygonModeAction::PolygonModeAction(QObject* parent) : MyActions(parent) {
	setIcon(QIcon("./Image/Icon/polygon_mode"));
	setText("�����");
	//setShortcut(tr("Ctrl+P"));
	setFont(QFont(tr("����"), 30, QFont::Bold, false));//����26�ţ��Ӵ֣�б����
}

CurveModeAction::CurveModeAction(QObject* parent) : MyActions(parent) {
	//setIcon(QIcon("./Image/Icon/curve_mode"));
	//setText("����");
	//setShortcut(tr("Ctrl+C"));
	setFont(QFont(tr("����"), 30, QFont::Bold, false));//����26�ţ��Ӵ֣�б����
}

SquarePenModeAction::SquarePenModeAction(QObject* parent) : MyActions(parent) {
	setIcon(QIcon("./Image/Icon/square_pen_mode"));
	setText("������Ĥ");
	setFont(QFont(tr("����"), 30, QFont::Bold, false));//����26�ţ��Ӵ֣�б����

}

CirclePenModeAction::CirclePenModeAction(QObject* parent) : MyActions(parent) {
	setIcon(QIcon("./Image/Icon/circle_pen_mode"));
	setText("Բ����Ĥ");
	setFont(QFont(tr("����"), 30, QFont::Bold, false));//����26�ţ��Ӵ֣�б����
}

CurseModeAction::CurseModeAction(QObject* parent) : MyActions(parent) {
	setIcon(QIcon("./Image/Icon/curse_mode"));
	setText("ѡ��");
	//setShortcut(tr("Ctrl+X"));
	setFont(QFont(tr("����"), 30, QFont::Bold, false));//����26�ţ��Ӵ֣�б����
}

TwoDModeAction::TwoDModeAction(QObject* parent) : MyActions(parent) {
	setIcon(QIcon("./Image/Icon/xxxx"));
	//setText("2D");
	setFont(QFont(tr("����"), 30, QFont::Bold, false));//����26�ţ��Ӵ֣�б����
}

MagnifyAction::MagnifyAction(QObject* parent) : MyActions(parent) {
	setIcon(QIcon("./Image/Icon/magnify"));
	setText("�Ŵ�");
	setFont(QFont(tr("����"), 30, QFont::Bold, false));//����26�ţ��Ӵ֣�б����
}

ImportAction::ImportAction(QObject* parent) : MyActions(parent)
{
	setIcon((QIcon("./Image/Icon/import")));
	setText("����ģ��");
	setFont(QFont(tr("����"), 30, QFont::Bold, false));//����26�ţ��Ӵ֣�б����
}

SaveAction::SaveAction(QObject* parent) : MyActions(parent)
{
	setIcon(QIcon("./Image/Icon/save"));
	setText("����");
	//setShortcuts(QKeySequence::Save);
	setFont(QFont(tr("����"), 30, QFont::Bold, false));//����26�ţ��Ӵ֣�б����
}
SaveAsAction::SaveAsAction(QObject* parent) : MyActions(parent)
{
	setIcon(QIcon("./Image/Icon/savefor"));
	setText("���Ϊ");
	//setShortcuts(QKeySequence::Save);
	setFont(QFont(tr("����"), 30, QFont::Bold, false));//����26�ţ��Ӵ֣�б����
}
UndoAction::UndoAction(QObject* parent) : MyActions(parent)
{
	setIcon(QIcon("./Image/Icon/undo"));
	setText("����");
	//setShortcuts(QKeySequence::Undo);
	setFont(QFont(tr("����"), 30, QFont::Bold, false));//����26�ţ��Ӵ֣�б����
}
RedoAction::RedoAction(QObject* parent) : MyActions(parent)
{
	//setIcon(QIcon("./Image/Icon/redo"));
	//setText("ǰ��");
	//setShortcuts(QKeySequence::Redo);
	setFont(QFont(tr("����"), 30, QFont::Bold, false));//����26�ţ��Ӵ֣�б����
}
searchAreaRectAction::searchAreaRectAction(QObject* parent) : MyActions(parent)
{
	setIcon(QIcon("./Image/Icon/rect_mode"));
	setText("��������");
	//setShortcuts(QKeySequence::Redo);
	setFont(QFont(tr("����"), 30, QFont::Bold, false));//����26�ţ��Ӵ֣�б����
}
featureMatchingRectAction::featureMatchingRectAction(QObject* parent) : MyActions(parent)
{
	setIcon(QIcon("./Image/Icon/square_pen_mode"));
	setText("����");
	//setShortcuts(QKeySequence::Redo);
	setFont(QFont(tr("����"), 30, QFont::Bold, false));//����26�ţ��Ӵ֣�б����
}
helpAction::helpAction(QObject* parent) : MyActions(parent)
{
	setIcon(QIcon("./Image/Icon/square_pen_mode"));
	setText("����");
	//setShortcuts(QKeySequence::Redo);
	setFont(QFont(tr("����"), 30, QFont::Bold, false));//����26�ţ��Ӵ֣�б����
}