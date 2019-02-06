/********************************************************************************
** Form generated from reading ui file 'console.ui'
**
** Created: Mon May 23 16:15:12 2011
**      by: Qt User Interface Compiler version 4.4.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_CONSOLE_H
#define UI_CONSOLE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QPushButton>
#include <QtGui/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_dialogConsole
{
public:
    QTextEdit *txtConsole;
    QPushButton *btnClose;

    void setupUi(QDialog *dialogConsole)
    {
    if (dialogConsole->objectName().isEmpty())
        dialogConsole->setObjectName(QString::fromUtf8("dialogConsole"));
    dialogConsole->resize(569, 527);
    QFont font;
    font.setStyleStrategy(QFont::PreferDefault);
    dialogConsole->setFont(font);
    txtConsole = new QTextEdit(dialogConsole);
    txtConsole->setObjectName(QString::fromUtf8("txtConsole"));
    txtConsole->setGeometry(QRect(20, 40, 531, 441));
    QFont font1;
    font1.setFamily(QString::fromUtf8("Terminal"));
    font1.setPointSize(10);
    font1.setKerning(true);
    font1.setStyleStrategy(QFont::NoAntialias);
    txtConsole->setFont(font1);
    txtConsole->setLineWrapMode(QTextEdit::NoWrap);
    txtConsole->setReadOnly(true);
    btnClose = new QPushButton(dialogConsole);
    btnClose->setObjectName(QString::fromUtf8("btnClose"));
    btnClose->setGeometry(QRect(460, 490, 91, 31));
    QFont font2;
    font2.setPointSize(10);
    btnClose->setFont(font2);

    retranslateUi(dialogConsole);

    QMetaObject::connectSlotsByName(dialogConsole);
    } // setupUi

    void retranslateUi(QDialog *dialogConsole)
    {
    dialogConsole->setWindowTitle(QApplication::translate("dialogConsole", "Console", 0, QApplication::UnicodeUTF8));
    btnClose->setText(QApplication::translate("dialogConsole", "Close", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(dialogConsole);
    } // retranslateUi

};

namespace Ui {
    class dialogConsole: public Ui_dialogConsole {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONSOLE_H
