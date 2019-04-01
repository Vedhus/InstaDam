/********************************************************************************
** Form generated from reading UI file 'projectDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROJECTDIALOG_H
#define UI_PROJECTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_projectDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QHBoxLayout *horizontalLayout_2;
    QTableWidget *differences;
    QFrame *frame;
    QGridLayout *gridLayout;
    QPushButton *keepButton;
    QPushButton *loadNewButton;
    QLabel *label_2;

    void setupUi(QDialog *projectDialog)
    {
        if (projectDialog->objectName().isEmpty())
            projectDialog->setObjectName(QString::fromUtf8("projectDialog"));
        projectDialog->resize(462, 481);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(projectDialog->sizePolicy().hasHeightForWidth());
        projectDialog->setSizePolicy(sizePolicy);
        projectDialog->setSizeGripEnabled(true);
        verticalLayout = new QVBoxLayout(projectDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(projectDialog);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);
        label->setWordWrap(true);

        verticalLayout->addWidget(label);

        scrollArea = new QScrollArea(projectDialog);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setFrameShape(QFrame::NoFrame);
        scrollArea->setFrameShadow(QFrame::Plain);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 462, 193));
        horizontalLayout_2 = new QHBoxLayout(scrollAreaWidgetContents);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, -1, 0);
        differences = new QTableWidget(scrollAreaWidgetContents);
        if (differences->columnCount() < 2)
            differences->setColumnCount(2);
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        __qtablewidgetitem->setTextAlignment(Qt::AlignLeading|Qt::AlignVCenter);
        __qtablewidgetitem->setFont(font);
        differences->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        __qtablewidgetitem1->setFont(font);
        differences->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        differences->setObjectName(QString::fromUtf8("differences"));
        differences->setFrameShape(QFrame::NoFrame);
        differences->setFrameShadow(QFrame::Plain);
        differences->setEditTriggers(QAbstractItemView::NoEditTriggers);
        differences->setTabKeyNavigation(false);
        differences->setDragDropOverwriteMode(false);
        differences->setCornerButtonEnabled(true);
        differences->setRowCount(0);
        differences->setColumnCount(2);
        differences->horizontalHeader()->setVisible(true);
        differences->horizontalHeader()->setDefaultSectionSize(150);
        differences->horizontalHeader()->setMinimumSectionSize(100);
        differences->horizontalHeader()->setStretchLastSection(false);
        differences->verticalHeader()->setVisible(false);

        horizontalLayout_2->addWidget(differences);

        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout->addWidget(scrollArea);

        frame = new QFrame(projectDialog);
        frame->setObjectName(QString::fromUtf8("frame"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy2);
        frame->setMinimumSize(QSize(0, 0));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(frame);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        keepButton = new QPushButton(frame);
        keepButton->setObjectName(QString::fromUtf8("keepButton"));

        gridLayout->addWidget(keepButton, 1, 1, 1, 1);

        loadNewButton = new QPushButton(frame);
        loadNewButton->setObjectName(QString::fromUtf8("loadNewButton"));

        gridLayout->addWidget(loadNewButton, 1, 2, 1, 1);

        label_2 = new QLabel(frame);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        sizePolicy2.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy2);
        label_2->setSizeIncrement(QSize(2, 0));
        label_2->setWordWrap(true);

        gridLayout->addWidget(label_2, 0, 1, 1, 2);


        verticalLayout->addWidget(frame);


        retranslateUi(projectDialog);

        QMetaObject::connectSlotsByName(projectDialog);
    } // setupUi

    void retranslateUi(QDialog *projectDialog)
    {
        projectDialog->setWindowTitle(QApplication::translate("projectDialog", "New Label Name", nullptr));
        label->setText(QApplication::translate("projectDialog", "The annotation Labels from the current project do not match those from the Label file being read.", nullptr));
        QTableWidgetItem *___qtablewidgetitem = differences->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("projectDialog", "Current", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = differences->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("projectDialog", "From File", nullptr));
        keepButton->setText(QApplication::translate("projectDialog", "Keep", nullptr));
        loadNewButton->setText(QApplication::translate("projectDialog", "Discard", nullptr));
        label_2->setText(QApplication::translate("projectDialog", "Do you want to keep the original Labels (cancels load of file), or discard the current Labels and load the new ones?", nullptr));
    } // retranslateUi

};

namespace Ui {
    class projectDialog: public Ui_projectDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROJECTDIALOG_H
