/********************************************************************************
** Form generated from reading UI file 'PaperDoll.ui'
**
** Created by: Qt User Interface Compiler version 6.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PAPERDOLL_H
#define UI_PAPERDOLL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PaperDollClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *PaperDollClass)
    {
        if (PaperDollClass->objectName().isEmpty())
            PaperDollClass->setObjectName("PaperDollClass");
        PaperDollClass->resize(600, 400);
        menuBar = new QMenuBar(PaperDollClass);
        menuBar->setObjectName("menuBar");
        PaperDollClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(PaperDollClass);
        mainToolBar->setObjectName("mainToolBar");
        PaperDollClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(PaperDollClass);
        centralWidget->setObjectName("centralWidget");
        PaperDollClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(PaperDollClass);
        statusBar->setObjectName("statusBar");
        PaperDollClass->setStatusBar(statusBar);

        retranslateUi(PaperDollClass);

        QMetaObject::connectSlotsByName(PaperDollClass);
    } // setupUi

    void retranslateUi(QMainWindow *PaperDollClass)
    {
        PaperDollClass->setWindowTitle(QCoreApplication::translate("PaperDollClass", "PaperDoll", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PaperDollClass: public Ui_PaperDollClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PAPERDOLL_H
