/********************************************************************************
** Form generated from reading UI file 'serverprojectname.ui'
**
** Created by: Qt User Interface Compiler version 5.11.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERVERPROJECTNAME_H
#define UI_SERVERPROJECTNAME_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_serverProjectName
{
public:
    QTextEdit *projectName;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QLabel *label;

    void setupUi(QWidget *serverProjectName)
    {
        if (serverProjectName->objectName().isEmpty())
            serverProjectName->setObjectName(QStringLiteral("serverProjectName"));
        serverProjectName->resize(400, 143);
        projectName = new QTextEdit(serverProjectName);
        projectName->setObjectName(QStringLiteral("projectName"));
        projectName->setGeometry(QRect(25, 39, 350, 31));
        pushButton = new QPushButton(serverProjectName);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(25, 80, 350, 21));
        pushButton_2 = new QPushButton(serverProjectName);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(25, 110, 350, 21));
        label = new QLabel(serverProjectName);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(30, 10, 341, 20));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);

        retranslateUi(serverProjectName);

        QMetaObject::connectSlotsByName(serverProjectName);
    } // setupUi

    void retranslateUi(QWidget *serverProjectName)
    {
        serverProjectName->setWindowTitle(QApplication::translate("serverProjectName", "Form", nullptr));
        projectName->setHtml(QApplication::translate("serverProjectName", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", nullptr));
        pushButton->setText(QApplication::translate("serverProjectName", "Save", nullptr));
        pushButton_2->setText(QApplication::translate("serverProjectName", "Cancel", nullptr));
        label->setText(QApplication::translate("serverProjectName", "Please specify a name for your new project:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class serverProjectName: public Ui_serverProjectName {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERVERPROJECTNAME_H
