/********************************************************************************
** Form generated from reading UI file 'labeldialog.ui'
**
** Created by: Qt User Interface Compiler version 5.11.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LABELDIALOG_H
#define UI_LABELDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_labelDialog
{
public:
    QGridLayout *gridLayout;
    QLabel *labelNameLabel;
    QLineEdit *labelName;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *labelDialog)
    {
        if (labelDialog->objectName().isEmpty())
            labelDialog->setObjectName(QStringLiteral("labelDialog"));
        labelDialog->resize(400, 212);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(labelDialog->sizePolicy().hasHeightForWidth());
        labelDialog->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(labelDialog);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        labelNameLabel = new QLabel(labelDialog);
        labelNameLabel->setObjectName(QStringLiteral("labelNameLabel"));

        gridLayout->addWidget(labelNameLabel, 0, 0, 1, 1);

        labelName = new QLineEdit(labelDialog);
        labelName->setObjectName(QStringLiteral("labelName"));

        gridLayout->addWidget(labelName, 0, 1, 1, 1);

        buttonBox = new QDialogButtonBox(labelDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 1, 0, 1, 2);


        retranslateUi(labelDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), labelDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), labelDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(labelDialog);
    } // setupUi

    void retranslateUi(QDialog *labelDialog)
    {
        labelDialog->setWindowTitle(QApplication::translate("labelDialog", "New Label Name", nullptr));
        labelNameLabel->setText(QApplication::translate("labelDialog", "Label Name:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class labelDialog: public Ui_labelDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LABELDIALOG_H
