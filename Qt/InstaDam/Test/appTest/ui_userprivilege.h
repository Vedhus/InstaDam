/********************************************************************************
** Form generated from reading UI file 'userprivilege.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_USERPRIVILEGE_H
#define UI_USERPRIVILEGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UserPrivilege
{
public:
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QLabel *label;

    void setupUi(QWidget *UserPrivilege)
    {
        if (UserPrivilege->objectName().isEmpty())
            UserPrivilege->setObjectName(QString::fromUtf8("UserPrivilege"));
        UserPrivilege->resize(358, 112);
        pushButton = new QPushButton(UserPrivilege);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(20, 40, 311, 21));
        pushButton_2 = new QPushButton(UserPrivilege);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(20, 70, 311, 21));
        label = new QLabel(UserPrivilege);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 10, 311, 16));

        retranslateUi(UserPrivilege);

        QMetaObject::connectSlotsByName(UserPrivilege);
    } // setupUi

    void retranslateUi(QWidget *UserPrivilege)
    {
        UserPrivilege->setWindowTitle(QApplication::translate("UserPrivilege", "Form", nullptr));
        pushButton->setText(QApplication::translate("UserPrivilege", "Admin", nullptr));
        pushButton_2->setText(QApplication::translate("UserPrivilege", "Annotator", nullptr));
        label->setText(QApplication::translate("UserPrivilege", "Please specify user privilege:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UserPrivilege: public Ui_UserPrivilege {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USERPRIVILEGE_H
