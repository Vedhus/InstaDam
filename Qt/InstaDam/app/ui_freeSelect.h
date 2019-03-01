/********************************************************************************
** Form generated from reading UI file 'freeSelect.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
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
    QLabel *brushSizeLabel;
    QSpinBox *brushSizeSpinner;
    QSlider *brushSizeSlider;
    QPushButton *roundBrushButton;
    QPushButton *squareBrushButton;
    QSpacerItem *freeDrawSpacer;

    void setupUi(QWidget *freeSelectForm)
    {
        if (freeSelectForm->objectName().isEmpty())
            freeSelectForm->setObjectName(QString::fromUtf8("freeSelectForm"));
        freeSelectForm->resize(1421, 152);
        gridLayout = new QGridLayout(freeSelectForm);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setHorizontalSpacing(30);
        brushSizeLabel = new QLabel(freeSelectForm);
        brushSizeLabel->setObjectName(QString::fromUtf8("brushSizeLabel"));

        gridLayout->addWidget(brushSizeLabel, 0, 1, 1, 1);

        brushSizeSpinner = new QSpinBox(freeSelectForm);
        brushSizeSpinner->setObjectName(QString::fromUtf8("brushSizeSpinner"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(brushSizeSpinner->sizePolicy().hasHeightForWidth());
        brushSizeSpinner->setSizePolicy(sizePolicy);

        gridLayout->addWidget(brushSizeSpinner, 0, 3, 1, 1);

        brushSizeSlider = new QSlider(freeSelectForm);
        brushSizeSlider->setObjectName(QString::fromUtf8("brushSizeSlider"));
        brushSizeSlider->setMinimumSize(QSize(200, 0));
        brushSizeSlider->setMaximumSize(QSize(120, 16777215));
        brushSizeSlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(brushSizeSlider, 0, 2, 1, 1);

        roundBrushButton = new QPushButton(freeSelectForm);
        roundBrushButton->setObjectName(QString::fromUtf8("roundBrushButton"));
        sizePolicy.setHeightForWidth(roundBrushButton->sizePolicy().hasHeightForWidth());
        roundBrushButton->setSizePolicy(sizePolicy);
        roundBrushButton->setMinimumSize(QSize(201, 36));

        gridLayout->addWidget(roundBrushButton, 0, 0, 1, 1);

        squareBrushButton = new QPushButton(freeSelectForm);
        squareBrushButton->setObjectName(QString::fromUtf8("squareBrushButton"));
        sizePolicy.setHeightForWidth(squareBrushButton->sizePolicy().hasHeightForWidth());
        squareBrushButton->setSizePolicy(sizePolicy);
        squareBrushButton->setMinimumSize(QSize(201, 36));

        gridLayout->addWidget(squareBrushButton, 1, 0, 1, 1);

        freeDrawSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(freeDrawSpacer, 0, 4, 1, 1);


        retranslateUi(freeSelectForm);

        QMetaObject::connectSlotsByName(freeSelectForm);
    } // setupUi

    void retranslateUi(QWidget *freeSelectForm)
    {
        freeSelectForm->setWindowTitle(QApplication::translate("freeSelectForm", "Form", nullptr));
        brushSizeLabel->setText(QApplication::translate("freeSelectForm", "Brush Size", nullptr));
        roundBrushButton->setText(QApplication::translate("freeSelectForm", "Round", nullptr));
        squareBrushButton->setText(QApplication::translate("freeSelectForm", "Square", nullptr));
    } // retranslateUi

};

namespace Ui {
    class freeSelectForm: public Ui_freeSelectForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FREESELECT_H
