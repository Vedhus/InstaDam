/********************************************************************************
** Form generated from reading UI file 'instadam.ui'
**
** Created by: Qt User Interface Compiler version 5.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INSTADAM_H
#define UI_INSTADAM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <maskviewer.h>
#include <photoviewer.h>
#include "picpushbutton.h"

QT_BEGIN_NAMESPACE

class Ui_InstaDam
{
public:
    QAction *action_Open;
    QAction *actionSave_File;
    QAction *actionExit;
    QAction *actionNew;
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionUndo;
    QAction *actionOpen_File;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QFrame *frame;
    QVBoxLayout *verticalLayout_2;
    QFrame *frame_4;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *saveAndBack;
    QPushButton *saveAndNext;
    QSpacerItem *verticalSpacer;
    QPushButton *panButton;
    QPushButton *roundBrush;
    QPushButton *squareBrush;
    QLabel *label;
    QSlider *horizontalSlider;
    QSpinBox *spinBox;
    QPushButton *pushButton_14;
    QSplitter *splitter_4;
    QWidget *horizontalLayoutWidget_3;
    QHBoxLayout *horizontalLayout_5;
    QSplitter *splitter;
    QFrame *frame_2;
    QVBoxLayout *verticalLayout_4;
    QSplitter *splitter_2;
    QScrollArea *scrollArea_2;
    QWidget *scrollAreaWidget;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *labelClassLayout;
    QFrame *frame_3;
    QHBoxLayout *horizontalLayout_2;
    QSplitter *splitter_3;
    PhotoViewer *IdmPhotoViewer;
    MaskViewer *IdmMaskViewer;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QTabWidget *tabWidget;
    QWidget *tab;
    QHBoxLayout *horizontalLayout_3;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_7;
    QVBoxLayout *verticalLayout_6;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_6;
    QLabel *label_5;
    QLabel *label_4;
    QHBoxLayout *horizontalLayout_7;
    PicPushButton *blur_label;
    PicPushButton *canny_label;
    PicPushButton *threshold_label;
    QStatusBar *statusbar;
    QMenuBar *menubar;
    QMenu *menu_File;
    QMenu *menuEdit;
    QMenu *menuProject;

    void setupUi(QMainWindow *InstaDam)
    {
        if (InstaDam->objectName().isEmpty())
            InstaDam->setObjectName(QString::fromUtf8("InstaDam"));
        InstaDam->resize(1023, 614);
        InstaDam->setMouseTracking(true);
        InstaDam->setTabletTracking(true);
        InstaDam->setAcceptDrops(true);
        InstaDam->setAutoFillBackground(true);
        InstaDam->setDocumentMode(true);
        InstaDam->setDockNestingEnabled(true);
        InstaDam->setUnifiedTitleAndToolBarOnMac(true);
        action_Open = new QAction(InstaDam);
        action_Open->setObjectName(QString::fromUtf8("action_Open"));
        actionSave_File = new QAction(InstaDam);
        actionSave_File->setObjectName(QString::fromUtf8("actionSave_File"));
        actionExit = new QAction(InstaDam);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionNew = new QAction(InstaDam);
        actionNew->setObjectName(QString::fromUtf8("actionNew"));
        actionOpen = new QAction(InstaDam);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        actionSave = new QAction(InstaDam);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        actionUndo = new QAction(InstaDam);
        actionUndo->setObjectName(QString::fromUtf8("actionUndo"));
        actionOpen_File = new QAction(InstaDam);
        actionOpen_File->setObjectName(QString::fromUtf8("actionOpen_File"));
        centralwidget = new QWidget(InstaDam);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        frame = new QFrame(centralwidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(frame);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        frame_4 = new QFrame(frame);
        frame_4->setObjectName(QString::fromUtf8("frame_4"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(frame_4->sizePolicy().hasHeightForWidth());
        frame_4->setSizePolicy(sizePolicy);
        frame_4->setMaximumSize(QSize(16777215, 70));
        frame_4->setFrameShape(QFrame::StyledPanel);
        frame_4->setFrameShadow(QFrame::Raised);
        horizontalLayout_4 = new QHBoxLayout(frame_4);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        saveAndBack = new QPushButton(frame_4);
        saveAndBack->setObjectName(QString::fromUtf8("saveAndBack"));

        horizontalLayout_4->addWidget(saveAndBack);

        saveAndNext = new QPushButton(frame_4);
        saveAndNext->setObjectName(QString::fromUtf8("saveAndNext"));

        horizontalLayout_4->addWidget(saveAndNext);

        verticalSpacer = new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);

        horizontalLayout_4->addItem(verticalSpacer);

        panButton = new QPushButton(frame_4);
        panButton->setObjectName(QString::fromUtf8("panButton"));

        horizontalLayout_4->addWidget(panButton);

        roundBrush = new QPushButton(frame_4);
        roundBrush->setObjectName(QString::fromUtf8("roundBrush"));

        horizontalLayout_4->addWidget(roundBrush);

        squareBrush = new QPushButton(frame_4);
        squareBrush->setObjectName(QString::fromUtf8("squareBrush"));

        horizontalLayout_4->addWidget(squareBrush);

        label = new QLabel(frame_4);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_4->addWidget(label);

        horizontalSlider = new QSlider(frame_4);
        horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
        horizontalSlider->setMaximumSize(QSize(120, 16777215));
        horizontalSlider->setOrientation(Qt::Horizontal);

        horizontalLayout_4->addWidget(horizontalSlider);

        spinBox = new QSpinBox(frame_4);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));

        horizontalLayout_4->addWidget(spinBox);

        pushButton_14 = new QPushButton(frame_4);
        pushButton_14->setObjectName(QString::fromUtf8("pushButton_14"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(pushButton_14->sizePolicy().hasHeightForWidth());
        pushButton_14->setSizePolicy(sizePolicy1);
#ifndef QT_NO_ACCESSIBILITY
        pushButton_14->setAccessibleName(QString::fromUtf8(""));
#endif // QT_NO_ACCESSIBILITY
        pushButton_14->setAutoFillBackground(false);
        pushButton_14->setText(QString::fromUtf8("Filter Options"));
        pushButton_14->setIconSize(QSize(200, 200));

        horizontalLayout_4->addWidget(pushButton_14);


        verticalLayout_2->addWidget(frame_4);

        splitter_4 = new QSplitter(frame);
        splitter_4->setObjectName(QString::fromUtf8("splitter_4"));
        splitter_4->setOrientation(Qt::Vertical);
        horizontalLayoutWidget_3 = new QWidget(splitter_4);
        horizontalLayoutWidget_3->setObjectName(QString::fromUtf8("horizontalLayoutWidget_3"));
        horizontalLayout_5 = new QHBoxLayout(horizontalLayoutWidget_3);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        splitter = new QSplitter(horizontalLayoutWidget_3);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Vertical);
        splitter->setHandleWidth(10);
        frame_2 = new QFrame(splitter);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        sizePolicy.setHeightForWidth(frame_2->sizePolicy().hasHeightForWidth());
        frame_2->setSizePolicy(sizePolicy);
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        verticalLayout_4 = new QVBoxLayout(frame_2);
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        splitter_2 = new QSplitter(frame_2);
        splitter_2->setObjectName(QString::fromUtf8("splitter_2"));
        splitter_2->setOrientation(Qt::Horizontal);
        splitter_2->setHandleWidth(10);
        scrollArea_2 = new QScrollArea(splitter_2);
        scrollArea_2->setObjectName(QString::fromUtf8("scrollArea_2"));
        scrollArea_2->setMaximumSize(QSize(150, 16777215));
        scrollArea_2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea_2->setWidgetResizable(true);
        scrollAreaWidget = new QWidget();
        scrollAreaWidget->setObjectName(QString::fromUtf8("scrollAreaWidget"));
        scrollAreaWidget->setGeometry(QRect(0, 0, 94, 305));
        verticalLayout_3 = new QVBoxLayout(scrollAreaWidget);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        labelClassLayout = new QVBoxLayout();
        labelClassLayout->setObjectName(QString::fromUtf8("labelClassLayout"));

        verticalLayout_3->addLayout(labelClassLayout);

        scrollArea_2->setWidget(scrollAreaWidget);
        splitter_2->addWidget(scrollArea_2);
        frame_3 = new QFrame(splitter_2);
        frame_3->setObjectName(QString::fromUtf8("frame_3"));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        horizontalLayout_2 = new QHBoxLayout(frame_3);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        splitter_3 = new QSplitter(frame_3);
        splitter_3->setObjectName(QString::fromUtf8("splitter_3"));
        splitter_3->setMinimumSize(QSize(729, 0));
        splitter_3->setOrientation(Qt::Horizontal);
        IdmPhotoViewer = new PhotoViewer(splitter_3);
        IdmPhotoViewer->setObjectName(QString::fromUtf8("IdmPhotoViewer"));
        IdmPhotoViewer->setMinimumSize(QSize(20, 10));
        splitter_3->addWidget(IdmPhotoViewer);
        IdmMaskViewer = new MaskViewer(splitter_3);
        IdmMaskViewer->setObjectName(QString::fromUtf8("IdmMaskViewer"));
        IdmMaskViewer->setMinimumSize(QSize(20, 10));
        splitter_3->addWidget(IdmMaskViewer);

        horizontalLayout_2->addWidget(splitter_3);

        splitter_2->addWidget(frame_3);

        verticalLayout_4->addWidget(splitter_2);

        splitter->addWidget(frame_2);

        horizontalLayout_5->addWidget(splitter);

        splitter_4->addWidget(horizontalLayoutWidget_3);
        horizontalLayoutWidget = new QWidget(splitter_4);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetMaximumSize);
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        tabWidget = new QTabWidget(horizontalLayoutWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setMaximumSize(QSize(16777215, 300));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        horizontalLayout_3 = new QHBoxLayout(tab);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        scrollArea = new QScrollArea(tab);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 993, 137));
        verticalLayout_7 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        label_6 = new QLabel(scrollAreaWidgetContents);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        horizontalLayout_8->addWidget(label_6, 0, Qt::AlignHCenter|Qt::AlignVCenter);

        label_5 = new QLabel(scrollAreaWidgetContents);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_8->addWidget(label_5, 0, Qt::AlignHCenter);

        label_4 = new QLabel(scrollAreaWidgetContents);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_8->addWidget(label_4, 0, Qt::AlignHCenter);


        verticalLayout_6->addLayout(horizontalLayout_8);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        blur_label = new PicPushButton(scrollAreaWidgetContents);
        blur_label->setObjectName(QString::fromUtf8("blur_label"));

        horizontalLayout_7->addWidget(blur_label, 0, Qt::AlignHCenter);

        canny_label = new PicPushButton(scrollAreaWidgetContents);
        canny_label->setObjectName(QString::fromUtf8("canny_label"));

        horizontalLayout_7->addWidget(canny_label, 0, Qt::AlignHCenter);

        threshold_label = new PicPushButton(scrollAreaWidgetContents);
        threshold_label->setObjectName(QString::fromUtf8("threshold_label"));

        horizontalLayout_7->addWidget(threshold_label, 0, Qt::AlignHCenter);


        verticalLayout_6->addLayout(horizontalLayout_7);


        verticalLayout_7->addLayout(verticalLayout_6);

        scrollArea->setWidget(scrollAreaWidgetContents);

        horizontalLayout_3->addWidget(scrollArea);

        tabWidget->addTab(tab, QString());

        horizontalLayout->addWidget(tabWidget);

        splitter_4->addWidget(horizontalLayoutWidget);

        verticalLayout_2->addWidget(splitter_4);


        verticalLayout->addWidget(frame);

        InstaDam->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(InstaDam);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        InstaDam->setStatusBar(statusbar);
        menubar = new QMenuBar(InstaDam);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setEnabled(true);
        menubar->setGeometry(QRect(0, 0, 1023, 21));
        menubar->setDefaultUp(false);
        menubar->setNativeMenuBar(true);
        menu_File = new QMenu(menubar);
        menu_File->setObjectName(QString::fromUtf8("menu_File"));
        menuEdit = new QMenu(menubar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        menuProject = new QMenu(menubar);
        menuProject->setObjectName(QString::fromUtf8("menuProject"));
        InstaDam->setMenuBar(menubar);

        menubar->addAction(menuProject->menuAction());
        menubar->addAction(menu_File->menuAction());
        menubar->addAction(menuEdit->menuAction());
        menu_File->addAction(actionOpen_File);
        menu_File->addAction(actionSave_File);
        menu_File->addSeparator();
        menu_File->addAction(actionExit);
        menuEdit->addAction(actionUndo);
        menuProject->addAction(actionNew);
        menuProject->addAction(actionOpen);
        menuProject->addAction(actionSave);

        retranslateUi(InstaDam);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(InstaDam);
    } // setupUi

    void retranslateUi(QMainWindow *InstaDam)
    {
        InstaDam->setWindowTitle(QApplication::translate("InstaDam", "MainWindow", nullptr));
        action_Open->setText(QApplication::translate("InstaDam", "Open Project", nullptr));
        actionSave_File->setText(QApplication::translate("InstaDam", "Save File", nullptr));
        actionExit->setText(QApplication::translate("InstaDam", "Exit", nullptr));
        actionNew->setText(QApplication::translate("InstaDam", "New", nullptr));
        actionOpen->setText(QApplication::translate("InstaDam", "Open", nullptr));
        actionSave->setText(QApplication::translate("InstaDam", "Save", nullptr));
        actionUndo->setText(QApplication::translate("InstaDam", "Undo", nullptr));
        actionOpen_File->setText(QApplication::translate("InstaDam", "Open File", nullptr));
        saveAndBack->setText(QApplication::translate("InstaDam", "Save and Back", nullptr));
        saveAndNext->setText(QApplication::translate("InstaDam", "Save and Next", nullptr));
        panButton->setText(QApplication::translate("InstaDam", "Pan", nullptr));
        roundBrush->setText(QApplication::translate("InstaDam", "Round", nullptr));
        squareBrush->setText(QApplication::translate("InstaDam", "Square", nullptr));
        label->setText(QApplication::translate("InstaDam", "Brush Size", nullptr));
        label_6->setText(QApplication::translate("InstaDam", "Blur", nullptr));
        label_5->setText(QApplication::translate("InstaDam", "Canny", nullptr));
        label_4->setText(QApplication::translate("InstaDam", "Threshold", nullptr));
        blur_label->setText(QApplication::translate("InstaDam", "TextLabel", nullptr));
        canny_label->setText(QApplication::translate("InstaDam", "TextLabel", nullptr));
        threshold_label->setText(QApplication::translate("InstaDam", "TextLabel", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("InstaDam", "Filters", nullptr));
        menu_File->setTitle(QApplication::translate("InstaDam", "&File", nullptr));
        menuEdit->setTitle(QApplication::translate("InstaDam", "Edit", nullptr));
        menuProject->setTitle(QApplication::translate("InstaDam", "Project", nullptr));
    } // retranslateUi

};

namespace Ui {
    class InstaDam: public Ui_InstaDam {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INSTADAM_H
