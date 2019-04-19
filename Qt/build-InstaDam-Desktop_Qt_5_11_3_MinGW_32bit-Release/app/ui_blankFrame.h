/********************************************************************************
** Form generated from reading UI file 'blankFrame.ui'
**
** Created by: Qt User Interface Compiler version 5.11.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BLANKFRAME_H
#define UI_BLANKFRAME_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_blankForm
{
public:
    QHBoxLayout *horizontalLayout;
    QFrame *blankFrame;
    QHBoxLayout *horizontalLayout_2;
    QPlainTextEdit *blankText;

    void setupUi(QWidget *blankForm)
    {
        if (blankForm->objectName().isEmpty())
            blankForm->setObjectName(QStringLiteral("blankForm"));
        blankForm->resize(859, 300);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(blankForm->sizePolicy().hasHeightForWidth());
        blankForm->setSizePolicy(sizePolicy);
        blankForm->setMinimumSize(QSize(859, 152));
        horizontalLayout = new QHBoxLayout(blankForm);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        blankFrame = new QFrame(blankForm);
        blankFrame->setObjectName(QStringLiteral("blankFrame"));
        sizePolicy.setHeightForWidth(blankFrame->sizePolicy().hasHeightForWidth());
        blankFrame->setSizePolicy(sizePolicy);
        blankFrame->setFrameShape(QFrame::NoFrame);
        blankFrame->setFrameShadow(QFrame::Plain);
        blankFrame->setLineWidth(0);
        horizontalLayout_2 = new QHBoxLayout(blankFrame);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        blankText = new QPlainTextEdit(blankFrame);
        blankText->setObjectName(QStringLiteral("blankText"));

        horizontalLayout_2->addWidget(blankText);


        horizontalLayout->addWidget(blankFrame);


        retranslateUi(blankForm);

        QMetaObject::connectSlotsByName(blankForm);
    } // setupUi

    void retranslateUi(QWidget *blankForm)
    {
        blankForm->setWindowTitle(QApplication::translate("blankForm", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class blankForm: public Ui_blankForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BLANKFRAME_H
