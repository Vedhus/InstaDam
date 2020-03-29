/********************************************************************************
** Form generated from reading UI file 'login.ui'
**
** Created by: Qt User Interface Compiler version 5.11.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Login
{
public:
    QFormLayout *formLayout;
    QLabel *label_2;
    QLineEdit *username;
    QLabel *label_3;
    QLineEdit *password;
    QLineEdit *url;
    QFrame *frame;
    QHBoxLayout *horizontalLayout;
    QPushButton *loginButton;
    QPushButton *registerButton;
    QPushButton *exitButton;
    QLabel *label;
    QFrame *frame_2;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *backButton;
    QLabel *label_4;

    void setupUi(QWidget *Login)
    {
        if (Login->objectName().isEmpty())
            Login->setObjectName(QStringLiteral("Login"));
        Login->resize(558, 280);
        formLayout = new QFormLayout(Login);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        label_2 = new QLabel(Login);
        label_2->setObjectName(QStringLiteral("label_2"));
        QFont font;
        font.setPointSize(16);
        font.setBold(true);
        font.setWeight(75);
        label_2->setFont(font);

        formLayout->setWidget(0, QFormLayout::LabelRole, label_2);

        username = new QLineEdit(Login);
        username->setObjectName(QStringLiteral("username"));

        formLayout->setWidget(0, QFormLayout::FieldRole, username);

        label_3 = new QLabel(Login);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setFont(font);

        formLayout->setWidget(1, QFormLayout::LabelRole, label_3);

        password = new QLineEdit(Login);
        password->setObjectName(QStringLiteral("password"));
        password->setEchoMode(QLineEdit::Password);

        formLayout->setWidget(1, QFormLayout::FieldRole, password);

        url = new QLineEdit(Login);
        url->setObjectName(QStringLiteral("url"));

        formLayout->setWidget(2, QFormLayout::FieldRole, url);

        frame = new QFrame(Login);
        frame->setObjectName(QStringLiteral("frame"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy);
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(frame);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        loginButton = new QPushButton(frame);
        loginButton->setObjectName(QStringLiteral("loginButton"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(loginButton->sizePolicy().hasHeightForWidth());
        loginButton->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(loginButton);

        registerButton = new QPushButton(frame);
        registerButton->setObjectName(QStringLiteral("registerButton"));
        sizePolicy1.setHeightForWidth(registerButton->sizePolicy().hasHeightForWidth());
        registerButton->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(registerButton);

        exitButton = new QPushButton(frame);
        exitButton->setObjectName(QStringLiteral("exitButton"));
        sizePolicy1.setHeightForWidth(exitButton->sizePolicy().hasHeightForWidth());
        exitButton->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(exitButton);


        formLayout->setWidget(3, QFormLayout::FieldRole, frame);

        label = new QLabel(Login);
        label->setObjectName(QStringLiteral("label"));
        QFont font1;
        font1.setPointSize(12);
        font1.setBold(true);
        font1.setWeight(75);
        label->setFont(font1);

        formLayout->setWidget(4, QFormLayout::LabelRole, label);

        frame_2 = new QFrame(Login);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        horizontalLayout_2 = new QHBoxLayout(frame_2);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(306, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        backButton = new QPushButton(frame_2);
        backButton->setObjectName(QStringLiteral("backButton"));
        sizePolicy1.setHeightForWidth(backButton->sizePolicy().hasHeightForWidth());
        backButton->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(backButton);


        formLayout->setWidget(4, QFormLayout::FieldRole, frame_2);

        label_4 = new QLabel(Login);
        label_4->setObjectName(QStringLiteral("label_4"));
        sizePolicy.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy);
        label_4->setFont(font);
        label_4->setTextFormat(Qt::AutoText);
        label_4->setTextInteractionFlags(Qt::NoTextInteraction);

        formLayout->setWidget(2, QFormLayout::LabelRole, label_4);


        retranslateUi(Login);

        QMetaObject::connectSlotsByName(Login);
    } // setupUi

    void retranslateUi(QWidget *Login)
    {
        Login->setWindowTitle(QApplication::translate("Login", "Form", nullptr));
        label_2->setText(QApplication::translate("Login", "Username", nullptr));
        label_3->setText(QApplication::translate("Login", "Password", nullptr));
        password->setText(QString());
        url->setText(QApplication::translate("Login", "http://23.100.23.94", nullptr));
        loginButton->setText(QApplication::translate("Login", "Login", nullptr));
        registerButton->setText(QApplication::translate("Login", "Register", nullptr));
        exitButton->setText(QApplication::translate("Login", "Exit", nullptr));
        label->setText(QApplication::translate("Login", "InstaDam", nullptr));
        backButton->setText(QApplication::translate("Login", "Back", nullptr));
        label_4->setText(QApplication::translate("Login", "URL", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Login: public Ui_Login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
