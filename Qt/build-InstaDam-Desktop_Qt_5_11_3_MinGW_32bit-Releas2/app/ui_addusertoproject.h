/********************************************************************************
** Form generated from reading UI file 'addusertoproject.ui'
**
** Created by: Qt User Interface Compiler version 5.11.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADDUSERTOPROJECT_H
#define UI_ADDUSERTOPROJECT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AddUserToProject
{
public:
    QTextEdit *userInfoInput;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QListWidget *userList;
    QPushButton *addToProject;
    QPushButton *updatePrivilege;
    QLabel *label;

    void setupUi(QWidget *AddUserToProject)
    {
        if (AddUserToProject->objectName().isEmpty())
            AddUserToProject->setObjectName(QStringLiteral("AddUserToProject"));
        AddUserToProject->resize(400, 357);
        userInfoInput = new QTextEdit(AddUserToProject);
        userInfoInput->setObjectName(QStringLiteral("userInfoInput"));
        userInfoInput->setGeometry(QRect(25, 40, 350, 21));
        pushButton = new QPushButton(AddUserToProject);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(25, 300, 170, 21));
        pushButton_2 = new QPushButton(AddUserToProject);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(204, 300, 170, 21));
        userList = new QListWidget(AddUserToProject);
        userList->setObjectName(QStringLiteral("userList"));
        userList->setGeometry(QRect(25, 70, 350, 221));
        addToProject = new QPushButton(AddUserToProject);
        addToProject->setObjectName(QStringLiteral("addToProject"));
        addToProject->setGeometry(QRect(25, 330, 170, 21));
        updatePrivilege = new QPushButton(AddUserToProject);
        updatePrivilege->setObjectName(QStringLiteral("updatePrivilege"));
        updatePrivilege->setGeometry(QRect(204, 330, 170, 21));
        label = new QLabel(AddUserToProject);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(30, 10, 341, 20));
        QFont font;
        font.setBold(true);
        font.setItalic(false);
        font.setWeight(75);
        label->setFont(font);

        retranslateUi(AddUserToProject);

        QMetaObject::connectSlotsByName(AddUserToProject);
    } // setupUi

    void retranslateUi(QWidget *AddUserToProject)
    {
        AddUserToProject->setWindowTitle(QApplication::translate("AddUserToProject", "Form", nullptr));
        userInfoInput->setHtml(QApplication::translate("AddUserToProject", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", nullptr));
        pushButton->setText(QApplication::translate("AddUserToProject", "Search", nullptr));
        pushButton_2->setText(QApplication::translate("AddUserToProject", "Close", nullptr));
        addToProject->setText(QApplication::translate("AddUserToProject", "Add to Project", nullptr));
        updatePrivilege->setText(QApplication::translate("AddUserToProject", "Update Privilege", nullptr));
        label->setText(QApplication::translate("AddUserToProject", "Enter Username or Email address to search for a user:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AddUserToProject: public Ui_AddUserToProject {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDUSERTOPROJECT_H
