/********************************************************************************
** Form generated from reading UI file 'newproject.ui'
**
** Created by: Qt User Interface Compiler version 5.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEWPROJECT_H
#define UI_NEWPROJECT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_newproject
{
public:
    QDialogButtonBox *buttonBox;
    QPushButton *pushButton;

    void setupUi(QDialog *newproject)
    {
        if (newproject->objectName().isEmpty())
            newproject->setObjectName(QStringLiteral("newproject"));
        newproject->resize(382, 137);
        buttonBox = new QDialogButtonBox(newproject);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(10, 80, 361, 41));
        buttonBox->setOrientation(Qt::Vertical);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        pushButton = new QPushButton(newproject);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(10, 10, 361, 51));

        retranslateUi(newproject);
        QObject::connect(buttonBox, SIGNAL(rejected()), newproject, SLOT(reject()));
        QObject::connect(buttonBox, SIGNAL(accepted()), newproject, SLOT(accept()));

        QMetaObject::connectSlotsByName(newproject);
    } // setupUi

    void retranslateUi(QDialog *newproject)
    {
        newproject->setWindowTitle(QApplication::translate("newproject", "Dialog", nullptr));
        pushButton->setText(QApplication::translate("newproject", "Add New Label", nullptr));
    } // retranslateUi

};

namespace Ui {
    class newproject: public Ui_newproject {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEWPROJECT_H
