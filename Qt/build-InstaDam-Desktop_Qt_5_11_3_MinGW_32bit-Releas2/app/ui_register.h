/********************************************************************************
** Form generated from reading UI file 'register.ui'
**
** Created by: Qt User Interface Compiler version 5.11.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGISTER_H
#define UI_REGISTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Register
{
public:
    QFormLayout *formLayout;
    QLabel *label_5;
    QLineEdit *email;
    QLabel *label_2;
    QLineEdit *username;
    QLabel *label_3;
    QLineEdit *password;
    QLabel *label_4;
    QLineEdit *url;
    QPushButton *registerButton;
    QPushButton *cancelButton;
    QLabel *label;

    void setupUi(QWidget *Register)
    {
        if (Register->objectName().isEmpty())
            Register->setObjectName(QStringLiteral("Register"));
        Register->resize(520, 314);
        formLayout = new QFormLayout(Register);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        label_5 = new QLabel(Register);
        label_5->setObjectName(QStringLiteral("label_5"));
        QFont font;
        font.setPointSize(16);
        font.setBold(true);
        font.setWeight(75);
        label_5->setFont(font);

        formLayout->setWidget(0, QFormLayout::LabelRole, label_5);

        email = new QLineEdit(Register);
        email->setObjectName(QStringLiteral("email"));

        formLayout->setWidget(0, QFormLayout::FieldRole, email);

        label_2 = new QLabel(Register);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setFont(font);

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        username = new QLineEdit(Register);
        username->setObjectName(QStringLiteral("username"));

        formLayout->setWidget(1, QFormLayout::FieldRole, username);

        label_3 = new QLabel(Register);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setFont(font);

        formLayout->setWidget(2, QFormLayout::LabelRole, label_3);

        password = new QLineEdit(Register);
        password->setObjectName(QStringLiteral("password"));
        password->setEchoMode(QLineEdit::Password);

        formLayout->setWidget(2, QFormLayout::FieldRole, password);

        label_4 = new QLabel(Register);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setFont(font);

        formLayout->setWidget(3, QFormLayout::LabelRole, label_4);

        url = new QLineEdit(Register);
        url->setObjectName(QStringLiteral("url"));

        formLayout->setWidget(3, QFormLayout::FieldRole, url);

        registerButton = new QPushButton(Register);
        registerButton->setObjectName(QStringLiteral("registerButton"));

        formLayout->setWidget(4, QFormLayout::FieldRole, registerButton);

        cancelButton = new QPushButton(Register);
        cancelButton->setObjectName(QStringLiteral("cancelButton"));

        formLayout->setWidget(5, QFormLayout::FieldRole, cancelButton);

        label = new QLabel(Register);
        label->setObjectName(QStringLiteral("label"));
        QFont font1;
        font1.setPointSize(12);
        font1.setBold(true);
        font1.setWeight(75);
        label->setFont(font1);

        formLayout->setWidget(6, QFormLayout::LabelRole, label);


        retranslateUi(Register);

        QMetaObject::connectSlotsByName(Register);
    } // setupUi

    void retranslateUi(QWidget *Register)
    {
        Register->setWindowTitle(QApplication::translate("Register", "Form", nullptr));
        label_5->setText(QApplication::translate("Register", "Email", nullptr));
        label_2->setText(QApplication::translate("Register", "UserName", nullptr));
        label_3->setText(QApplication::translate("Register", "Password", nullptr));
        label_4->setText(QApplication::translate("Register", "URL", nullptr));
        url->setText(QApplication::translate("Register", "http://23.100.23.94", nullptr));
        registerButton->setText(QApplication::translate("Register", "Register", nullptr));
        cancelButton->setText(QApplication::translate("Register", "Cancel", nullptr));
        label->setText(QApplication::translate("Register", "InstaDam", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Register: public Ui_Register {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTER_H
