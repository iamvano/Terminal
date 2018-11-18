/********************************************************************************
** Form generated from reading UI file 'Terminal.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TERMINAL_H
#define UI_TERMINAL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TerminalClass
{
public:
    QWidget *centralWidget;
    QMenuBar *menuBar;

    void setupUi(QMainWindow *TerminalClass)
    {
        if (TerminalClass->objectName().isEmpty())
            TerminalClass->setObjectName(QStringLiteral("TerminalClass"));
        TerminalClass->resize(600, 400);
        centralWidget = new QWidget(TerminalClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        TerminalClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(TerminalClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 18));
        TerminalClass->setMenuBar(menuBar);

        retranslateUi(TerminalClass);

        QMetaObject::connectSlotsByName(TerminalClass);
    } // setupUi

    void retranslateUi(QMainWindow *TerminalClass)
    {
        TerminalClass->setWindowTitle(QApplication::translate("TerminalClass", "Terminal", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TerminalClass: public Ui_TerminalClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TERMINAL_H
