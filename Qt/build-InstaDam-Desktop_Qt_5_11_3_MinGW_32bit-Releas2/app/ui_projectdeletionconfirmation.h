/********************************************************************************
** Form generated from reading UI file 'projectdeletionconfirmation.ui'
**
** Created by: Qt User Interface Compiler version 5.11.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROJECTDELETIONCONFIRMATION_H
#define UI_PROJECTDELETIONCONFIRMATION_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_projectDeletionConfirmation
{
public:
    QPushButton *yesDelete;
    QPushButton *noCancel;
    QLabel *label;

    void setupUi(QWidget *projectDeletionConfirmation)
    {
        if (projectDeletionConfirmation->objectName().isEmpty())
            projectDeletionConfirmation->setObjectName(QStringLiteral("projectDeletionConfirmation"));
        projectDeletionConfirmation->resize(420, 130);
        yesDelete = new QPushButton(projectDeletionConfirmation);
        yesDelete->setObjectName(QStringLiteral("yesDelete"));
        yesDelete->setGeometry(QRect(160, 90, 100, 21));
        noCancel = new QPushButton(projectDeletionConfirmation);
        noCancel->setObjectName(QStringLiteral("noCancel"));
        noCancel->setGeometry(QRect(160, 60, 100, 21));
        QFont font;
        font.setPointSize(9);
        font.setBold(true);
        font.setWeight(75);
        noCancel->setFont(font);
        label = new QLabel(projectDeletionConfirmation);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 20, 421, 16));
        QFont font1;
        font1.setBold(true);
        font1.setWeight(75);
        label->setFont(font1);

        retranslateUi(projectDeletionConfirmation);

        QMetaObject::connectSlotsByName(projectDeletionConfirmation);
    } // setupUi

    void retranslateUi(QWidget *projectDeletionConfirmation)
    {
        projectDeletionConfirmation->setWindowTitle(QApplication::translate("projectDeletionConfirmation", "Form", nullptr));
        yesDelete->setText(QApplication::translate("projectDeletionConfirmation", "Yes, delete", nullptr));
        noCancel->setText(QApplication::translate("projectDeletionConfirmation", "Cancel", nullptr));
        label->setText(QApplication::translate("projectDeletionConfirmation", "Deleting a project is irreversible, are you sure you want to proceed?", nullptr));
    } // retranslateUi

};

namespace Ui {
    class projectDeletionConfirmation: public Ui_projectDeletionConfirmation {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROJECTDELETIONCONFIRMATION_H
