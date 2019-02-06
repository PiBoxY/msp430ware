/********************************************************************************
** Form generated from reading ui file 'settingswindow.ui'
**
** Created: Mon May 23 16:15:11 2011
**      by: Qt User Interface Compiler version 4.4.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SETTINGSWINDOW_H
#define UI_SETTINGSWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGroupBox>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QRadioButton>

QT_BEGIN_NAMESPACE

class Ui_SettingsWindow
{
public:
    QDialogButtonBox *buttonBox;
    QGroupBox *groupBox;
    QLabel *label;
    QRadioButton *radioCelsius;
    QRadioButton *radioFahrenheit;
    QLabel *label_5;
    QLineEdit *edtTimeInterval;
    QLabel *label_2;

    void setupUi(QDialog *SettingsWindow)
    {
    if (SettingsWindow->objectName().isEmpty())
        SettingsWindow->setObjectName(QString::fromUtf8("SettingsWindow"));
    SettingsWindow->resize(340, 164);
    buttonBox = new QDialogButtonBox(SettingsWindow);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setGeometry(QRect(10, 120, 321, 31));
    QFont font;
    font.setPointSize(10);
    buttonBox->setFont(font);
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::NoButton|QDialogButtonBox::Ok);
    groupBox = new QGroupBox(SettingsWindow);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    groupBox->setGeometry(QRect(10, 10, 321, 101));
    QFont font1;
    font1.setPointSize(9);
    groupBox->setFont(font1);
    label = new QLabel(groupBox);
    label->setObjectName(QString::fromUtf8("label"));
    label->setGeometry(QRect(20, 60, 183, 20));
    label->setFont(font);
    radioCelsius = new QRadioButton(groupBox);
    radioCelsius->setObjectName(QString::fromUtf8("radioCelsius"));
    radioCelsius->setGeometry(QRect(160, 30, 61, 19));
    radioFahrenheit = new QRadioButton(groupBox);
    radioFahrenheit->setObjectName(QString::fromUtf8("radioFahrenheit"));
    radioFahrenheit->setGeometry(QRect(230, 30, 81, 19));
    label_5 = new QLabel(groupBox);
    label_5->setObjectName(QString::fromUtf8("label_5"));
    label_5->setGeometry(QRect(20, 30, 101, 16));
    edtTimeInterval = new QLineEdit(groupBox);
    edtTimeInterval->setObjectName(QString::fromUtf8("edtTimeInterval"));
    edtTimeInterval->setGeometry(QRect(211, 60, 61, 21));
    edtTimeInterval->setAlignment(Qt::AlignCenter);
    label_2 = new QLabel(groupBox);
    label_2->setObjectName(QString::fromUtf8("label_2"));
    label_2->setGeometry(QRect(280, 63, 46, 14));

    retranslateUi(SettingsWindow);
    QObject::connect(buttonBox, SIGNAL(accepted()), SettingsWindow, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), SettingsWindow, SLOT(reject()));

    QMetaObject::connectSlotsByName(SettingsWindow);
    } // setupUi

    void retranslateUi(QDialog *SettingsWindow)
    {
    SettingsWindow->setWindowTitle(QApplication::translate("SettingsWindow", "Dialog", 0, QApplication::UnicodeUTF8));
    groupBox->setTitle(QApplication::translate("SettingsWindow", "Settings", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("SettingsWindow", "Time interval for Node removal:", 0, QApplication::UnicodeUTF8));
    radioCelsius->setText(QApplication::translate("SettingsWindow", "Celsius", 0, QApplication::UnicodeUTF8));
    radioFahrenheit->setText(QApplication::translate("SettingsWindow", "Fahrenheit", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    label_5->setToolTip(QApplication::translate("SettingsWindow", "Temperature will be displayed in the chosen unit. ", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP


#ifndef QT_NO_WHATSTHIS
    label_5->setWhatsThis(QString());
#endif // QT_NO_WHATSTHIS

    label_5->setText(QApplication::translate("SettingsWindow", "Temperature Unit:", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("SettingsWindow", "Sec.", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(SettingsWindow);
    } // retranslateUi

};

namespace Ui {
    class SettingsWindow: public Ui_SettingsWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGSWINDOW_H
