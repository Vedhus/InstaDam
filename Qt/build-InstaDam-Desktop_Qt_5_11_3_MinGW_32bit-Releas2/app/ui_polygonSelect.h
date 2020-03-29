/********************************************************************************
** Form generated from reading UI file 'polygonSelect.ui'
**
** Created by: Qt User Interface Compiler version 5.11.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_POLYGONSELECT_H
#define UI_POLYGONSELECT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_polygonSelectForm
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *finishPolygonButton;
    QPushButton *insertPointButton;
    QPlainTextEdit *polygonMessageBox;

    void setupUi(QWidget *polygonSelectForm)
    {
        if (polygonSelectForm->objectName().isEmpty())
            polygonSelectForm->setObjectName(QStringLiteral("polygonSelectForm"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(polygonSelectForm->sizePolicy().hasHeightForWidth());
        polygonSelectForm->setSizePolicy(sizePolicy);
        polygonSelectForm->setMinimumSize(QSize(859, 0));
        polygonSelectForm->setMaximumSize(QSize(16777215, 26));
        gridLayout = new QGridLayout(polygonSelectForm);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(-1, 0, -1, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(2);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        finishPolygonButton = new QPushButton(polygonSelectForm);
        finishPolygonButton->setObjectName(QStringLiteral("finishPolygonButton"));
        finishPolygonButton->setEnabled(false);
        sizePolicy.setHeightForWidth(finishPolygonButton->sizePolicy().hasHeightForWidth());
        finishPolygonButton->setSizePolicy(sizePolicy);
        finishPolygonButton->setMinimumSize(QSize(0, 0));
        finishPolygonButton->setMaximumSize(QSize(16777215, 24));

        horizontalLayout->addWidget(finishPolygonButton);

        insertPointButton = new QPushButton(polygonSelectForm);
        insertPointButton->setObjectName(QStringLiteral("insertPointButton"));
        sizePolicy.setHeightForWidth(insertPointButton->sizePolicy().hasHeightForWidth());
        insertPointButton->setSizePolicy(sizePolicy);
        insertPointButton->setMinimumSize(QSize(0, 0));
        insertPointButton->setMaximumSize(QSize(16777215, 24));

        horizontalLayout->addWidget(insertPointButton);

        polygonMessageBox = new QPlainTextEdit(polygonSelectForm);
        polygonMessageBox->setObjectName(QStringLiteral("polygonMessageBox"));
        polygonMessageBox->setMinimumSize(QSize(1200, 0));
        polygonMessageBox->setMaximumSize(QSize(10000, 24));
        polygonMessageBox->setFrameShape(QFrame::NoFrame);
        polygonMessageBox->setUndoRedoEnabled(false);
        polygonMessageBox->setReadOnly(true);
        polygonMessageBox->setTextInteractionFlags(Qt::NoTextInteraction);

        horizontalLayout->addWidget(polygonMessageBox);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);


        retranslateUi(polygonSelectForm);

        QMetaObject::connectSlotsByName(polygonSelectForm);
    } // setupUi

    void retranslateUi(QWidget *polygonSelectForm)
    {
        polygonSelectForm->setWindowTitle(QApplication::translate("polygonSelectForm", "Form", nullptr));
        finishPolygonButton->setText(QApplication::translate("polygonSelectForm", "Done Editing", nullptr));
        insertPointButton->setText(QApplication::translate("polygonSelectForm", "Insert Point Between", nullptr));
    } // retranslateUi

};

namespace Ui {
    class polygonSelectForm: public Ui_polygonSelectForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_POLYGONSELECT_H
