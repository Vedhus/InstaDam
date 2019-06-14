/********************************************************************************
** Form generated from reading UI file 'freeSelect.ui'
**
** Created by: Qt User Interface Compiler version 5.11.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FREESELECT_H
#define UI_FREESELECT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_freeSelectForm
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *drawButton;
    QPushButton *eraseButton;
    QSpacerItem *horizontalSpacer;
    QPushButton *roundBrushButton;
    QPushButton *squareBrushButton;
    QSpacerItem *horizontalSpacer_2;
    QSlider *brushSizeSlider;
    QSpinBox *brushSizeSpinner;

    void setupUi(QWidget *freeSelectForm)
    {
        if (freeSelectForm->objectName().isEmpty())
            freeSelectForm->setObjectName(QStringLiteral("freeSelectForm"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(freeSelectForm->sizePolicy().hasHeightForWidth());
        freeSelectForm->setSizePolicy(sizePolicy);
        freeSelectForm->setMinimumSize(QSize(859, 0));
        freeSelectForm->setMaximumSize(QSize(16777215, 26));
        gridLayout = new QGridLayout(freeSelectForm);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setHorizontalSpacing(30);
        gridLayout->setVerticalSpacing(0);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(1);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        drawButton = new QPushButton(freeSelectForm);
        drawButton->setObjectName(QStringLiteral("drawButton"));
        sizePolicy.setHeightForWidth(drawButton->sizePolicy().hasHeightForWidth());
        drawButton->setSizePolicy(sizePolicy);
        drawButton->setMinimumSize(QSize(150, 0));
        drawButton->setMaximumSize(QSize(16777215, 24));
        drawButton->setCheckable(true);
        drawButton->setChecked(true);

        horizontalLayout_2->addWidget(drawButton);

        eraseButton = new QPushButton(freeSelectForm);
        eraseButton->setObjectName(QStringLiteral("eraseButton"));
        sizePolicy.setHeightForWidth(eraseButton->sizePolicy().hasHeightForWidth());
        eraseButton->setSizePolicy(sizePolicy);
        eraseButton->setMinimumSize(QSize(150, 0));
        eraseButton->setMaximumSize(QSize(16777215, 24));
        eraseButton->setCheckable(true);

        horizontalLayout_2->addWidget(eraseButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        roundBrushButton = new QPushButton(freeSelectForm);
        roundBrushButton->setObjectName(QStringLiteral("roundBrushButton"));
        sizePolicy.setHeightForWidth(roundBrushButton->sizePolicy().hasHeightForWidth());
        roundBrushButton->setSizePolicy(sizePolicy);
        roundBrushButton->setMinimumSize(QSize(150, 20));
        roundBrushButton->setMaximumSize(QSize(16777215, 24));
        roundBrushButton->setCheckable(true);
        roundBrushButton->setChecked(true);

        horizontalLayout_2->addWidget(roundBrushButton);

        squareBrushButton = new QPushButton(freeSelectForm);
        squareBrushButton->setObjectName(QStringLiteral("squareBrushButton"));
        sizePolicy.setHeightForWidth(squareBrushButton->sizePolicy().hasHeightForWidth());
        squareBrushButton->setSizePolicy(sizePolicy);
        squareBrushButton->setMinimumSize(QSize(150, 0));
        squareBrushButton->setMaximumSize(QSize(16777215, 24));
        squareBrushButton->setCheckable(true);

        horizontalLayout_2->addWidget(squareBrushButton);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        brushSizeSlider = new QSlider(freeSelectForm);
        brushSizeSlider->setObjectName(QStringLiteral("brushSizeSlider"));
        sizePolicy.setHeightForWidth(brushSizeSlider->sizePolicy().hasHeightForWidth());
        brushSizeSlider->setSizePolicy(sizePolicy);
        brushSizeSlider->setMinimumSize(QSize(400, 0));
        brushSizeSlider->setMaximumSize(QSize(16712, 24));
        brushSizeSlider->setMaximum(500);
        brushSizeSlider->setOrientation(Qt::Horizontal);

        horizontalLayout_2->addWidget(brushSizeSlider);

        brushSizeSpinner = new QSpinBox(freeSelectForm);
        brushSizeSpinner->setObjectName(QStringLiteral("brushSizeSpinner"));
        sizePolicy.setHeightForWidth(brushSizeSpinner->sizePolicy().hasHeightForWidth());
        brushSizeSpinner->setSizePolicy(sizePolicy);
        brushSizeSpinner->setMaximumSize(QSize(16777215, 24));
        brushSizeSpinner->setMaximum(500);

        horizontalLayout_2->addWidget(brushSizeSpinner);


        gridLayout->addLayout(horizontalLayout_2, 0, 0, 1, 1);


        retranslateUi(freeSelectForm);

        QMetaObject::connectSlotsByName(freeSelectForm);
    } // setupUi

    void retranslateUi(QWidget *freeSelectForm)
    {
        freeSelectForm->setWindowTitle(QApplication::translate("freeSelectForm", "Form", nullptr));
        drawButton->setText(QApplication::translate("freeSelectForm", "Draw", nullptr));
        eraseButton->setText(QApplication::translate("freeSelectForm", "Erase", nullptr));
        roundBrushButton->setText(QApplication::translate("freeSelectForm", "Round", nullptr));
        squareBrushButton->setText(QApplication::translate("freeSelectForm", "Square", nullptr));
    } // retranslateUi

};

namespace Ui {
    class freeSelectForm: public Ui_freeSelectForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FREESELECT_H
