#include "myactions.h"
#include <QDebug>
#pragma execution_character_set("utf-8")

MyActions::MyActions(QObject* parent) : QAction(parent)
{
}
MyActions::~MyActions() {}

ExecPatternAction::ExecPatternAction(QObject* parent) : MyActions(parent) {
	setIcon(QIcon("./Image/Icon/open_file"));

	setText("效果预览");
	setFont(QFont(tr("宋体"), 30, QFont::Bold, false));//宋体26号，加粗，斜体字
	//setShortcuts(QKeySequence::Open);
}

GetImageAction::GetImageAction(QObject* parent) : MyActions(parent) {
	setIcon(QIcon("./Image/Icon/zhixing"));
	setText("采集图片");
	setFont(QFont(tr("宋体"), 30, QFont::Bold, false));//宋体26号，加粗，斜体字
	
	//setShortcuts(QKeySequence::Open);
}

OpenFileAction::OpenFileAction(QObject* parent) : MyActions(parent) {
	//setIcon(QIcon("./Image/Icon/zhixing"));
	//setText("打开图片");
	setFont(QFont(tr("宋体"), 30, QFont::Bold, false));//宋体26号，加粗，斜体字
	//setShortcuts(QKeySequence::Open);
}



OpenDirAction::OpenDirAction(QObject* parent) : MyActions(parent) {
	//setIcon(QIcon("./Image/Icon/icon-_5"));
	//setText("打开目录");
	setFont(QFont(tr("宋体"), 30, QFont::Bold, false));//宋体26号，加粗，斜体字
}


PrevImageAction::PrevImageAction(QObject* parent) : MyActions(parent) {
	//setIcon(QIcon("./Image/Icon/prev_image"));
	//setText("上一张图");
	setFont(QFont(tr("宋体"), 30, QFont::Bold, false));//宋体26号，加粗，斜体字
//	setShortcut(tr("Shift+Tab"));
}

NextImageAction::NextImageAction(QObject* parent) : MyActions(parent) {
	//setIcon(QIcon("./Image/Icon/next_image"));
	//setText("下一张图"); 
	setFont(QFont(tr("宋体"), 30, QFont::Bold, false));//宋体26号，加粗，斜体字
	//setShortcut(tr("Tab"));
}

DeleteLabelAction::DeleteLabelAction(QObject* parent) : MyActions(parent) {
	setIcon(QIcon("./Image/Icon/delete_label"));
	setText("删除操作");
	setFont(QFont(tr("宋体"), 30, QFont::Bold, false));//宋体26号，加粗，斜体字
	//setShortcuts(QKeySequence::Delete);
}

CloseFileAction::CloseFileAction(QObject* parent) : MyActions(parent) {
	setIcon(QIcon("./Image/Icon/x"));
	setText("关闭当前文件");
	setFont(QFont(tr("宋体"), 30, QFont::Bold, false));//宋体26号，加粗，斜体字
	//setShortcuts(QKeySequence::Close);
}

SetPointAction::SetPointAction(QObject* parent) : MyActions(parent) {
	setIcon(QIcon("./Image/Icon/sendpoint"));
	setText("输出点");
	setFont(QFont(tr("宋体"), 30, QFont::Bold, false));//宋体26号，加粗，斜体字
	//setShortcut(tr("Ctrl+R"));
}

RectModeAction::RectModeAction(QObject* parent) : MyActions(parent) {
	setIcon(QIcon("./Image/Icon/rect_mode"));
	setText("矩形");
	setFont(QFont(tr("宋体"), 30, QFont::Bold, false));//宋体26号，加粗，斜体字
	//setShortcut(tr("Ctrl+R"));
}

EllipseModeAction::EllipseModeAction(QObject* parent) : MyActions(parent) {
	setIcon(QIcon("./Image/Icon/ellipse_mode"));
	setText("椭圆");
	setFont(QFont(tr("宋体"), 30, QFont::Bold, false));//宋体26号，加粗，斜体字
	//setShortcut(tr("Ctrl+E"));
}

PolygonModeAction::PolygonModeAction(QObject* parent) : MyActions(parent) {
	setIcon(QIcon("./Image/Icon/polygon_mode"));
	setText("多边形");
	//setShortcut(tr("Ctrl+P"));
	setFont(QFont(tr("宋体"), 30, QFont::Bold, false));//宋体26号，加粗，斜体字
}

CurveModeAction::CurveModeAction(QObject* parent) : MyActions(parent) {
	//setIcon(QIcon("./Image/Icon/curve_mode"));
	//setText("曲线");
	//setShortcut(tr("Ctrl+C"));
	setFont(QFont(tr("宋体"), 30, QFont::Bold, false));//宋体26号，加粗，斜体字
}

SquarePenModeAction::SquarePenModeAction(QObject* parent) : MyActions(parent) {
	setIcon(QIcon("./Image/Icon/square_pen_mode"));
	setText("方形掩膜");
	setFont(QFont(tr("宋体"), 30, QFont::Bold, false));//宋体26号，加粗，斜体字

}

CirclePenModeAction::CirclePenModeAction(QObject* parent) : MyActions(parent) {
	setIcon(QIcon("./Image/Icon/circle_pen_mode"));
	setText("圆形掩膜");
	setFont(QFont(tr("宋体"), 30, QFont::Bold, false));//宋体26号，加粗，斜体字
}

CurseModeAction::CurseModeAction(QObject* parent) : MyActions(parent) {
	setIcon(QIcon("./Image/Icon/curse_mode"));
	setText("选择");
	//setShortcut(tr("Ctrl+X"));
	setFont(QFont(tr("宋体"), 30, QFont::Bold, false));//宋体26号，加粗，斜体字
}

TwoDModeAction::TwoDModeAction(QObject* parent) : MyActions(parent) {
	setIcon(QIcon("./Image/Icon/xxxx"));
	//setText("2D");
	setFont(QFont(tr("宋体"), 30, QFont::Bold, false));//宋体26号，加粗，斜体字
}

MagnifyAction::MagnifyAction(QObject* parent) : MyActions(parent) {
	setIcon(QIcon("./Image/Icon/magnify"));
	setText("放大镜");
	setFont(QFont(tr("宋体"), 30, QFont::Bold, false));//宋体26号，加粗，斜体字
}

ImportAction::ImportAction(QObject* parent) : MyActions(parent)
{
	setIcon((QIcon("./Image/Icon/import")));
	setText("导入模板");
	setFont(QFont(tr("宋体"), 30, QFont::Bold, false));//宋体26号，加粗，斜体字
}

SaveAction::SaveAction(QObject* parent) : MyActions(parent)
{
	setIcon(QIcon("./Image/Icon/save"));
	setText("保存");
	//setShortcuts(QKeySequence::Save);
	setFont(QFont(tr("宋体"), 30, QFont::Bold, false));//宋体26号，加粗，斜体字
}
SaveAsAction::SaveAsAction(QObject* parent) : MyActions(parent)
{
	setIcon(QIcon("./Image/Icon/savefor"));
	setText("另存为");
	//setShortcuts(QKeySequence::Save);
	setFont(QFont(tr("宋体"), 30, QFont::Bold, false));//宋体26号，加粗，斜体字
}
UndoAction::UndoAction(QObject* parent) : MyActions(parent)
{
	setIcon(QIcon("./Image/Icon/undo"));
	setText("撤销");
	//setShortcuts(QKeySequence::Undo);
	setFont(QFont(tr("宋体"), 30, QFont::Bold, false));//宋体26号，加粗，斜体字
}
RedoAction::RedoAction(QObject* parent) : MyActions(parent)
{
	//setIcon(QIcon("./Image/Icon/redo"));
	//setText("前进");
	//setShortcuts(QKeySequence::Redo);
	setFont(QFont(tr("宋体"), 30, QFont::Bold, false));//宋体26号，加粗，斜体字
}
searchAreaRectAction::searchAreaRectAction(QObject* parent) : MyActions(parent)
{
	setIcon(QIcon("./Image/Icon/rect_mode"));
	setText("搜索区域");
	//setShortcuts(QKeySequence::Redo);
	setFont(QFont(tr("宋体"), 30, QFont::Bold, false));//宋体26号，加粗，斜体字
}
featureMatchingRectAction::featureMatchingRectAction(QObject* parent) : MyActions(parent)
{
	setIcon(QIcon("./Image/Icon/square_pen_mode"));
	setText("矩形");
	//setShortcuts(QKeySequence::Redo);
	setFont(QFont(tr("宋体"), 30, QFont::Bold, false));//宋体26号，加粗，斜体字
}
helpAction::helpAction(QObject* parent) : MyActions(parent)
{
	setIcon(QIcon("./Image/Icon/square_pen_mode"));
	setText("帮助");
	//setShortcuts(QKeySequence::Redo);
	setFont(QFont(tr("宋体"), 30, QFont::Bold, false));//宋体26号，加粗，斜体字
}