/********************************************************************************
** Form generated from reading UI file 'polygonSelect.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_POLYGONSELECT_H
#define UI_POLYGONSELECT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_polygonSelectForm
{
public:
    QHBoxLayout *horizontalLayout;
    QPushButton *finishPolygonButton;
    QSpacerItem *polygonSeletSpacer;

    void setupUi(QWidget *polygonSelectForm)
    {
        if (polygonSelectForm->objectName().isEmpty())
            polygonSelectForm->setObjectName(QString::fromUtf8("polygonSelectForm"));
        polygonSelectForm->resize(1413, 158);
        horizontalLayout = new QHBoxLayout(polygonSelectForm);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        finishPolygonButton = new QPushButton(polygonSelectForm);
        finishPolygonButton->setObjectName(QString::fromUtf8("finishPolygonButton"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(finishPolygonButton->sizePolicy().hasHeightForWidth());
        finishPolygonButton->setSizePolicy(sizePolicy);
        finishPolygonButton->setMinimumSize(QSize(201, 36));

        horizontalLayout->addWidget(finishPolygonButton);

        polygonSeletSpacer = new QSpacerItem(1185, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(polygonSeletSpacer);


        retranslateUi(polygonSelectForm);

        QMetaObject::connectSlotsByName(polygonSelectForm);
    } // setupUi

    void retranslateUi(QWidget *polygonSelectForm)
    {
        polygonSelectForm->setWindowTitle(QApplication::translate("polygonSelectForm", "Form", nullptr));
        finishPolygonButton->setText(QApplication::translate("polygonSelectForm", "Finish Polygon", nullptr));
    } // retranslateUi

};

namespace Ui {
    class polygonSelectForm: public Ui_polygonSelectForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_POLYGONSELECT_H
