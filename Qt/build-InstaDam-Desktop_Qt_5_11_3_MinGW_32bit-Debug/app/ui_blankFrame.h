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
        blankForm->resize(859, 26);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(blankForm->sizePolicy().hasHeightForWidth());
        blankForm->setSizePolicy(sizePolicy);
        blankForm->setMinimumSize(QSize(859, 0));
        blankForm->setMaximumSize(QSize(16777215, 26));
        horizontalLayout = new QHBoxLayout(blankForm);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        blankFrame = new QFrame(blankForm);
        blankFrame->setObjectName(QStringLiteral("blankFrame"));
        sizePolicy.setHeightForWidth(blankFrame->sizePolicy().hasHeightForWidth());
        blankFrame->setSizePolicy(sizePolicy);
        blankFrame->setMaximumSize(QSize(16777215, 60));
        blankFrame->setFrameShape(QFrame::NoFrame);
        blankFrame->setFrameShadow(QFrame::Plain);
        blankFrame->setLineWidth(0);
        horizontalLayout_2 = new QHBoxLayout(blankFrame);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        blankText = new QPlainTextEdit(blankFrame);
        blankText->setObjectName(QStringLiteral("blankText"));
        blankText->setMaximumSize(QSize(16777215, 24));
        blankText->setFrameShape(QFrame::NoFrame);
        blankText->setFrameShadow(QFrame::Plain);
        blankText->setLineWidth(0);
        blankText->setLineWrapMode(QPlainTextEdit::WidgetWidth);

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
