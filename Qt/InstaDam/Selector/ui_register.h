/********************************************************************************
** Form generated from reading UI file 'register.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGISTER_H
#define UI_REGISTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Register
{
public:
    QLabel *label;
    QTextEdit *username;
    QTextEdit *password;
    QTextEdit *url;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QTextEdit *email;

    void setupUi(QWidget *Register)
    {
        if (Register->objectName().isEmpty())
            Register->setObjectName(QString::fromUtf8("Register"));
        Register->resize(600, 350);
        label = new QLabel(Register);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 320, 91, 16));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        username = new QTextEdit(Register);
        username->setObjectName(QString::fromUtf8("username"));
        username->setGeometry(QRect(100, 70, 400, 30));
        password = new QTextEdit(Register);
        password->setObjectName(QString::fromUtf8("password"));
        password->setGeometry(QRect(100, 110, 400, 30));
        url = new QTextEdit(Register);
        url->setObjectName(QString::fromUtf8("url"));
        url->setGeometry(QRect(100, 150, 400, 30));
        pushButton = new QPushButton(Register);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(100, 210, 400, 23));
        pushButton_2 = new QPushButton(Register);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(100, 240, 400, 23));
        email = new QTextEdit(Register);
        email->setObjectName(QString::fromUtf8("email"));
        email->setGeometry(QRect(100, 30, 400, 30));

        retranslateUi(Register);

        QMetaObject::connectSlotsByName(Register);
    } // setupUi

    void retranslateUi(QWidget *Register)
    {
        Register->setWindowTitle(QApplication::translate("Register", "Form", nullptr));
        label->setText(QApplication::translate("Register", "InstaDam", nullptr));
        username->setHtml(QApplication::translate("Register", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#a5a5a5;\">username</span></p></body></html>", nullptr));
        password->setHtml(QApplication::translate("Register", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#a5a5a5;\">password</span></p></body></html>", nullptr));
        url->setHtml(QApplication::translate("Register", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'OpenSans,Helvetica,Arial,sans-serif'; font-size:8pt; color:#505050; background-color:#ffffff;\">http://23.100.23.94</span></p></body></html>", nullptr));
        pushButton->setText(QApplication::translate("Register", "Register", nullptr));
        pushButton_2->setText(QApplication::translate("Register", "Cancel", nullptr));
        email->setHtml(QApplication::translate("Register", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#a5a5a5;\">email</span></p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Register: public Ui_Register {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTER_H
