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
#include <QtWidgets/QLabel>
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
    QSlider *brushSizeSlider;
    QSpacerItem *freeDrawSpacer;
    QPushButton *eraseButton;
    QPushButton *drawButton;
    QPushButton *roundBrushButton;
    QLabel *brushSizeLabel;
    QSpinBox *brushSizeSpinner;
    QPushButton *squareBrushButton;

    void setupUi(QWidget *freeSelectForm)
    {
        if (freeSelectForm->objectName().isEmpty())
            freeSelectForm->setObjectName(QStringLiteral("freeSelectForm"));
        freeSelectForm->resize(859, 152);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(2);
        sizePolicy.setHeightForWidth(freeSelectForm->sizePolicy().hasHeightForWidth());
        freeSelectForm->setSizePolicy(sizePolicy);
        freeSelectForm->setMinimumSize(QSize(859, 152));
        gridLayout = new QGridLayout(freeSelectForm);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setHorizontalSpacing(30);
        brushSizeSlider = new QSlider(freeSelectForm);
        brushSizeSlider->setObjectName(QStringLiteral("brushSizeSlider"));
        brushSizeSlider->setMinimumSize(QSize(200, 0));
        brushSizeSlider->setMaximumSize(QSize(120, 16777215));
        brushSizeSlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(brushSizeSlider, 0, 3, 1, 1);

        freeDrawSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(freeDrawSpacer, 0, 5, 1, 1);

        eraseButton = new QPushButton(freeSelectForm);
        eraseButton->setObjectName(QStringLiteral("eraseButton"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(eraseButton->sizePolicy().hasHeightForWidth());
        eraseButton->setSizePolicy(sizePolicy1);
        eraseButton->setMinimumSize(QSize(150, 36));
        eraseButton->setCheckable(true);

        gridLayout->addWidget(eraseButton, 1, 0, 1, 1);

        drawButton = new QPushButton(freeSelectForm);
        drawButton->setObjectName(QStringLiteral("drawButton"));
        sizePolicy1.setHeightForWidth(drawButton->sizePolicy().hasHeightForWidth());
        drawButton->setSizePolicy(sizePolicy1);
        drawButton->setMinimumSize(QSize(150, 36));
        drawButton->setCheckable(true);
        drawButton->setChecked(true);

        gridLayout->addWidget(drawButton, 0, 0, 1, 1);

        roundBrushButton = new QPushButton(freeSelectForm);
        roundBrushButton->setObjectName(QStringLiteral("roundBrushButton"));
        sizePolicy1.setHeightForWidth(roundBrushButton->sizePolicy().hasHeightForWidth());
        roundBrushButton->setSizePolicy(sizePolicy1);
        roundBrushButton->setMinimumSize(QSize(150, 36));
        roundBrushButton->setCheckable(true);
        roundBrushButton->setChecked(true);

        gridLayout->addWidget(roundBrushButton, 0, 1, 1, 1);

        brushSizeLabel = new QLabel(freeSelectForm);
        brushSizeLabel->setObjectName(QStringLiteral("brushSizeLabel"));

        gridLayout->addWidget(brushSizeLabel, 0, 2, 1, 1);

        brushSizeSpinner = new QSpinBox(freeSelectForm);
        brushSizeSpinner->setObjectName(QStringLiteral("brushSizeSpinner"));
        sizePolicy1.setHeightForWidth(brushSizeSpinner->sizePolicy().hasHeightForWidth());
        brushSizeSpinner->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(brushSizeSpinner, 0, 4, 1, 1);

        squareBrushButton = new QPushButton(freeSelectForm);
        squareBrushButton->setObjectName(QStringLiteral("squareBrushButton"));
        sizePolicy1.setHeightForWidth(squareBrushButton->sizePolicy().hasHeightForWidth());
        squareBrushButton->setSizePolicy(sizePolicy1);
        squareBrushButton->setMinimumSize(QSize(150, 36));
        squareBrushButton->setCheckable(true);

        gridLayout->addWidget(squareBrushButton, 1, 1, 1, 1);


        retranslateUi(freeSelectForm);

        QMetaObject::connectSlotsByName(freeSelectForm);
    } // setupUi

    void retranslateUi(QWidget *freeSelectForm)
    {
        freeSelectForm->setWindowTitle(QApplication::translate("freeSelectForm", "Form", nullptr));
        eraseButton->setText(QApplication::translate("freeSelectForm", "Erase", nullptr));
        drawButton->setText(QApplication::translate("freeSelectForm", "Draw", nullptr));
        roundBrushButton->setText(QApplication::translate("freeSelectForm", "Round", nullptr));
        brushSizeLabel->setText(QApplication::translate("freeSelectForm", "Brush Size", nullptr));
        squareBrushButton->setText(QApplication::translate("freeSelectForm", "Square", nullptr));
    } // retranslateUi

};

namespace Ui {
    class freeSelectForm: public Ui_freeSelectForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FREESELECT_H
