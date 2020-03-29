/********************************************************************************
** Form generated from reading UI file 'startingwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.11.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STARTINGWIDGET_H
#define UI_STARTINGWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_StartingWidget
{
public:
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QLabel *label;
    QPushButton *pushButton_3;

    void setupUi(QWidget *StartingWidget)
    {
        if (StartingWidget->objectName().isEmpty())
            StartingWidget->setObjectName(QStringLiteral("StartingWidget"));
        StartingWidget->resize(400, 300);
        pushButton = new QPushButton(StartingWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(25, 125, 150, 21));
        pushButton_2 = new QPushButton(StartingWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setEnabled(false);
        pushButton_2->setGeometry(QRect(225, 125, 150, 21));
        label = new QLabel(StartingWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 270, 101, 16));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        pushButton_3 = new QPushButton(StartingWidget);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(310, 270, 80, 21));

        retranslateUi(StartingWidget);

        QMetaObject::connectSlotsByName(StartingWidget);
    } // setupUi

    void retranslateUi(QWidget *StartingWidget)
    {
        StartingWidget->setWindowTitle(QApplication::translate("StartingWidget", "Widget", nullptr));
        pushButton->setText(QApplication::translate("StartingWidget", "LOCAL", nullptr));
        pushButton_2->setText(QApplication::translate("StartingWidget", "SERVER", nullptr));
        label->setText(QApplication::translate("StartingWidget", "InstaDam", nullptr));
        pushButton_3->setText(QApplication::translate("StartingWidget", "Exit", nullptr));
    } // retranslateUi

};

namespace Ui {
    class StartingWidget: public Ui_StartingWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STARTINGWIDGET_H
