/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "myqgraphicsview.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_choosepicture;
    QAction *action_rect;
    QAction *action_rotaterect;
    QAction *action_polygon;
    QAction *action_circle;
    QAction *action_concircle;
    QAction *action_ringexpansion;
    QAction *action_caliberline;
    QAction *action_calibercircle;
    QAction *action_zoomin;
    QAction *action_zoomout;
    QAction *action_fitwin;
    QAction *action_1_to_1;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    myqgraphicsview *graphicsView;
    QStatusBar *statusbar;
    QMenuBar *menubar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1920, 1080);
        MainWindow->setMouseTracking(true);
        action_choosepicture = new QAction(MainWindow);
        action_choosepicture->setObjectName(QString::fromUtf8("action_choosepicture"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/res/Icon/choosepicture.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_choosepicture->setIcon(icon);
        action_rect = new QAction(MainWindow);
        action_rect->setObjectName(QString::fromUtf8("action_rect"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/res/Icon/rect.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_rect->setIcon(icon1);
        action_rotaterect = new QAction(MainWindow);
        action_rotaterect->setObjectName(QString::fromUtf8("action_rotaterect"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/res/Icon/rotaterect.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_rotaterect->setIcon(icon2);
        action_polygon = new QAction(MainWindow);
        action_polygon->setObjectName(QString::fromUtf8("action_polygon"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/res/Icon/polygon.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_polygon->setIcon(icon3);
        action_circle = new QAction(MainWindow);
        action_circle->setObjectName(QString::fromUtf8("action_circle"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/res/Icon/circle.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_circle->setIcon(icon4);
        action_concircle = new QAction(MainWindow);
        action_concircle->setObjectName(QString::fromUtf8("action_concircle"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/res/Icon/concircle.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_concircle->setIcon(icon5);
        action_ringexpansion = new QAction(MainWindow);
        action_ringexpansion->setObjectName(QString::fromUtf8("action_ringexpansion"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/res/Icon/ringexpension.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_ringexpansion->setIcon(icon6);
        action_caliberline = new QAction(MainWindow);
        action_caliberline->setObjectName(QString::fromUtf8("action_caliberline"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/res/Icon/caliber.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_caliberline->setIcon(icon7);
        action_calibercircle = new QAction(MainWindow);
        action_calibercircle->setObjectName(QString::fromUtf8("action_calibercircle"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/res/Icon/caliber_circle.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_calibercircle->setIcon(icon8);
        action_zoomin = new QAction(MainWindow);
        action_zoomin->setObjectName(QString::fromUtf8("action_zoomin"));
        action_zoomin->setCheckable(false);
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/res/Icon/zoomin.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_zoomin->setIcon(icon9);
        action_zoomout = new QAction(MainWindow);
        action_zoomout->setObjectName(QString::fromUtf8("action_zoomout"));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/res/Icon/zoomout.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_zoomout->setIcon(icon10);
        action_fitwin = new QAction(MainWindow);
        action_fitwin->setObjectName(QString::fromUtf8("action_fitwin"));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/res/Icon/fitwin.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_fitwin->setIcon(icon11);
        action_1_to_1 = new QAction(MainWindow);
        action_1_to_1->setObjectName(QString::fromUtf8("action_1_to_1"));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/res/Icon/1_to_1.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_1_to_1->setIcon(icon12);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        centralwidget->setMouseTracking(true);
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        graphicsView = new myqgraphicsview(centralwidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setMouseTracking(true);

        gridLayout->addWidget(graphicsView, 1, 0, 1, 1);

        gridLayout->setColumnStretch(0, 2);
        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        statusbar->setMouseTracking(true);
        MainWindow->setStatusBar(statusbar);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1920, 22));
        MainWindow->setMenuBar(menubar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        toolBar->setMouseTracking(true);
        toolBar->setOrientation(Qt::Vertical);
        toolBar->setFloatable(true);
        MainWindow->addToolBar(Qt::LeftToolBarArea, toolBar);

        toolBar->addAction(action_choosepicture);
        toolBar->addSeparator();
        toolBar->addAction(action_zoomin);
        toolBar->addSeparator();
        toolBar->addAction(action_zoomout);
        toolBar->addSeparator();
        toolBar->addAction(action_1_to_1);
        toolBar->addSeparator();
        toolBar->addAction(action_fitwin);
        toolBar->addSeparator();
        toolBar->addAction(action_rect);
        toolBar->addSeparator();
        toolBar->addAction(action_rotaterect);
        toolBar->addSeparator();
        toolBar->addAction(action_polygon);
        toolBar->addSeparator();
        toolBar->addAction(action_circle);
        toolBar->addSeparator();
        toolBar->addAction(action_concircle);
        toolBar->addSeparator();
        toolBar->addAction(action_ringexpansion);
        toolBar->addSeparator();
        toolBar->addAction(action_caliberline);
        toolBar->addSeparator();
        toolBar->addAction(action_calibercircle);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        action_choosepicture->setText(QApplication::translate("MainWindow", "\345\212\240\350\275\275\345\233\276\345\203\217", nullptr));
#ifndef QT_NO_TOOLTIP
        action_choosepicture->setToolTip(QApplication::translate("MainWindow", "\345\212\240\350\275\275\345\233\276\345\203\217", nullptr));
#endif // QT_NO_TOOLTIP
        action_rect->setText(QApplication::translate("MainWindow", "\347\237\251\345\275\242", nullptr));
#ifndef QT_NO_TOOLTIP
        action_rect->setToolTip(QApplication::translate("MainWindow", "\347\237\251\345\275\242", nullptr));
#endif // QT_NO_TOOLTIP
        action_rotaterect->setText(QApplication::translate("MainWindow", "\346\227\213\350\275\254\347\237\251\345\275\242", nullptr));
#ifndef QT_NO_TOOLTIP
        action_rotaterect->setToolTip(QApplication::translate("MainWindow", "\346\227\213\350\275\254\347\237\251\345\275\242", nullptr));
#endif // QT_NO_TOOLTIP
        action_polygon->setText(QApplication::translate("MainWindow", "\345\244\232\350\276\271\345\275\242", nullptr));
#ifndef QT_NO_TOOLTIP
        action_polygon->setToolTip(QApplication::translate("MainWindow", "\345\244\232\350\276\271\345\275\242", nullptr));
#endif // QT_NO_TOOLTIP
        action_circle->setText(QApplication::translate("MainWindow", "\345\234\206", nullptr));
#ifndef QT_NO_TOOLTIP
        action_circle->setToolTip(QApplication::translate("MainWindow", "\345\234\206", nullptr));
#endif // QT_NO_TOOLTIP
        action_concircle->setText(QApplication::translate("MainWindow", "\345\220\214\345\277\203\345\234\206", nullptr));
#ifndef QT_NO_TOOLTIP
        action_concircle->setToolTip(QApplication::translate("MainWindow", "\345\220\214\345\277\203\345\234\206", nullptr));
#endif // QT_NO_TOOLTIP
        action_ringexpansion->setText(QApplication::translate("MainWindow", "\345\234\206\347\216\257\345\261\225\345\274\200", nullptr));
#ifndef QT_NO_TOOLTIP
        action_ringexpansion->setToolTip(QApplication::translate("MainWindow", "\345\234\206\347\216\257\345\261\225\345\274\200", nullptr));
#endif // QT_NO_TOOLTIP
        action_caliberline->setText(QApplication::translate("MainWindow", "\345\215\241\345\260\272", nullptr));
#ifndef QT_NO_TOOLTIP
        action_caliberline->setToolTip(QApplication::translate("MainWindow", "\347\233\264\347\272\277\345\215\241\345\260\272", nullptr));
#endif // QT_NO_TOOLTIP
        action_calibercircle->setText(QApplication::translate("MainWindow", "\345\234\206\345\215\241\345\260\272", nullptr));
#ifndef QT_NO_TOOLTIP
        action_calibercircle->setToolTip(QApplication::translate("MainWindow", "\345\234\206\345\215\241\345\260\272", nullptr));
#endif // QT_NO_TOOLTIP
        action_zoomin->setText(QApplication::translate("MainWindow", "\345\233\276\345\203\217\346\224\276\345\244\247", nullptr));
#ifndef QT_NO_TOOLTIP
        action_zoomin->setToolTip(QApplication::translate("MainWindow", "\345\233\276\345\203\217\346\224\276\345\244\247", nullptr));
#endif // QT_NO_TOOLTIP
        action_zoomout->setText(QApplication::translate("MainWindow", "\345\233\276\345\203\217\347\274\251\345\260\217 ", nullptr));
#ifndef QT_NO_TOOLTIP
        action_zoomout->setToolTip(QApplication::translate("MainWindow", "\345\233\276\345\203\217\347\274\251\345\260\217 ", nullptr));
#endif // QT_NO_TOOLTIP
        action_fitwin->setText(QApplication::translate("MainWindow", "\350\207\252\351\200\202\345\272\224\347\252\227\345\217\243", nullptr));
        action_1_to_1->setText(QApplication::translate("MainWindow", "\345\233\276\345\203\217\345\256\236\351\231\205\345\260\272\345\257\270", nullptr));
        toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
