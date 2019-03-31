/********************************************************************************
** Form generated from reading UI file 'newproject.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEWPROJECT_H
#define UI_NEWPROJECT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_newproject
{
public:
    QGridLayout *gridLayout;
    QPushButton *pushButton;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *newproject)
    {
        if (newproject->objectName().isEmpty())
            newproject->setObjectName(QString::fromUtf8("newproject"));
        newproject->resize(382, 156);
        newproject->setLayoutDirection(Qt::LeftToRight);
        gridLayout = new QGridLayout(newproject);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        pushButton = new QPushButton(newproject);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setMinimumSize(QSize(210, 0));

        gridLayout->addWidget(pushButton, 0, 0, 1, 1);

        buttonBox = new QDialogButtonBox(newproject);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setMinimumSize(QSize(150, 72));
        buttonBox->setMaximumSize(QSize(150, 16777215));
        buttonBox->setBaseSize(QSize(150, 0));
        buttonBox->setOrientation(Qt::Vertical);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(true);

        gridLayout->addWidget(buttonBox, 1, 0, 1, 1);


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
