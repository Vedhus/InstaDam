/********************************************************************************
** Form generated from reading UI file 'instadam.ui'
**
** Created by: Qt User Interface Compiler version 5.11.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INSTADAM_H
#define UI_INSTADAM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
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
    QAction *actionRedo;
    QAction *actionDelete;
    QAction *actionExport;
    QAction *actionExport_zip;
    QAction *actionSave_Annotation;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QFrame *frame;
    QHBoxLayout *horizontalLayout_5;
    QSplitter *splitter_4;
    QFrame *frame_7;
    QVBoxLayout *verticalLayout_2;
    QFrame *frame_4;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *saveAndBack;
    QPushButton *saveAndNext;
    QSpacerItem *verticalSpacer;
    QPushButton *panButton;
    QPushButton *pushButton_14;
    QFrame *frame_6;
    QHBoxLayout *horizontalLayout;
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
    QFrame *frame_8;
    QVBoxLayout *verticalLayout_5;
    QFrame *frame_5;
    QGridLayout *gridLayout;
    QPushButton *polygonSelectButton;
    QPushButton *rectangleSelectButton;
    QFrame *selectControlFrame;
    QPushButton *addSelectionButton;
    QPushButton *ellipseSelectButton;
    QPushButton *cancelSelectionButton;
    QPushButton *freeSelectButton;
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
            InstaDam->setObjectName(QStringLiteral("InstaDam"));
        InstaDam->resize(1918, 1227);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(InstaDam->sizePolicy().hasHeightForWidth());
        InstaDam->setSizePolicy(sizePolicy);
        InstaDam->setMouseTracking(true);
        InstaDam->setTabletTracking(true);
        InstaDam->setAcceptDrops(true);
        InstaDam->setAutoFillBackground(true);
        InstaDam->setDocumentMode(true);
        InstaDam->setDockNestingEnabled(true);
        InstaDam->setUnifiedTitleAndToolBarOnMac(true);
        action_Open = new QAction(InstaDam);
        action_Open->setObjectName(QStringLiteral("action_Open"));
        actionSave_File = new QAction(InstaDam);
        actionSave_File->setObjectName(QStringLiteral("actionSave_File"));
        actionExit = new QAction(InstaDam);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionNew = new QAction(InstaDam);
        actionNew->setObjectName(QStringLiteral("actionNew"));
        actionOpen = new QAction(InstaDam);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionSave = new QAction(InstaDam);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        actionUndo = new QAction(InstaDam);
        actionUndo->setObjectName(QStringLiteral("actionUndo"));
        actionOpen_File = new QAction(InstaDam);
        actionOpen_File->setObjectName(QStringLiteral("actionOpen_File"));
        actionRedo = new QAction(InstaDam);
        actionRedo->setObjectName(QStringLiteral("actionRedo"));
        actionDelete = new QAction(InstaDam);
        actionDelete->setObjectName(QStringLiteral("actionDelete"));
        actionExport = new QAction(InstaDam);
        actionExport->setObjectName(QStringLiteral("actionExport"));
        actionExport_zip = new QAction(InstaDam);
        actionExport_zip->setObjectName(QStringLiteral("actionExport_zip"));
        actionSave_Annotation = new QAction(InstaDam);
        actionSave_Annotation->setObjectName(QStringLiteral("actionSave_Annotation"));
        centralwidget = new QWidget(InstaDam);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        sizePolicy.setHeightForWidth(centralwidget->sizePolicy().hasHeightForWidth());
        centralwidget->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        frame = new QFrame(centralwidget);
        frame->setObjectName(QStringLiteral("frame"));
        sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy);
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        horizontalLayout_5 = new QHBoxLayout(frame);
        horizontalLayout_5->setSpacing(0);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        splitter_4 = new QSplitter(frame);
        splitter_4->setObjectName(QStringLiteral("splitter_4"));
        splitter_4->setOrientation(Qt::Vertical);
        frame_7 = new QFrame(splitter_4);
        frame_7->setObjectName(QStringLiteral("frame_7"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(1);
        sizePolicy1.setHeightForWidth(frame_7->sizePolicy().hasHeightForWidth());
        frame_7->setSizePolicy(sizePolicy1);
        frame_7->setFrameShape(QFrame::StyledPanel);
        frame_7->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(frame_7);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        frame_4 = new QFrame(frame_7);
        frame_4->setObjectName(QStringLiteral("frame_4"));
        sizePolicy.setHeightForWidth(frame_4->sizePolicy().hasHeightForWidth());
        frame_4->setSizePolicy(sizePolicy);
        frame_4->setMaximumSize(QSize(16777215, 70));
        frame_4->setFrameShape(QFrame::StyledPanel);
        frame_4->setFrameShadow(QFrame::Raised);
        horizontalLayout_4 = new QHBoxLayout(frame_4);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        saveAndBack = new QPushButton(frame_4);
        saveAndBack->setObjectName(QStringLiteral("saveAndBack"));

        horizontalLayout_4->addWidget(saveAndBack);

        saveAndNext = new QPushButton(frame_4);
        saveAndNext->setObjectName(QStringLiteral("saveAndNext"));

        horizontalLayout_4->addWidget(saveAndNext);

        verticalSpacer = new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);

        horizontalLayout_4->addItem(verticalSpacer);

        panButton = new QPushButton(frame_4);
        panButton->setObjectName(QStringLiteral("panButton"));
        panButton->setCheckable(true);
        panButton->setChecked(false);

        horizontalLayout_4->addWidget(panButton);

        pushButton_14 = new QPushButton(frame_4);
        pushButton_14->setObjectName(QStringLiteral("pushButton_14"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Maximum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(pushButton_14->sizePolicy().hasHeightForWidth());
        pushButton_14->setSizePolicy(sizePolicy2);
#ifndef QT_NO_ACCESSIBILITY
        pushButton_14->setAccessibleName(QStringLiteral(""));
#endif // QT_NO_ACCESSIBILITY
        pushButton_14->setAutoFillBackground(false);
        pushButton_14->setText(QStringLiteral("Filter Options"));
        pushButton_14->setIconSize(QSize(200, 200));

        horizontalLayout_4->addWidget(pushButton_14);


        verticalLayout_2->addWidget(frame_4);

        frame_6 = new QFrame(frame_7);
        frame_6->setObjectName(QStringLiteral("frame_6"));
        sizePolicy1.setHeightForWidth(frame_6->sizePolicy().hasHeightForWidth());
        frame_6->setSizePolicy(sizePolicy1);
        frame_6->setFrameShape(QFrame::StyledPanel);
        frame_6->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(frame_6);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        splitter = new QSplitter(frame_6);
        splitter->setObjectName(QStringLiteral("splitter"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(splitter->sizePolicy().hasHeightForWidth());
        splitter->setSizePolicy(sizePolicy3);
        splitter->setMinimumSize(QSize(0, 0));
        splitter->setOrientation(Qt::Vertical);
        splitter->setHandleWidth(10);
        frame_2 = new QFrame(splitter);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        sizePolicy.setHeightForWidth(frame_2->sizePolicy().hasHeightForWidth());
        frame_2->setSizePolicy(sizePolicy);
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        verticalLayout_4 = new QVBoxLayout(frame_2);
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        splitter_2 = new QSplitter(frame_2);
        splitter_2->setObjectName(QStringLiteral("splitter_2"));
        sizePolicy.setHeightForWidth(splitter_2->sizePolicy().hasHeightForWidth());
        splitter_2->setSizePolicy(sizePolicy);
        splitter_2->setOrientation(Qt::Horizontal);
        splitter_2->setHandleWidth(10);
        scrollArea_2 = new QScrollArea(splitter_2);
        scrollArea_2->setObjectName(QStringLiteral("scrollArea_2"));
        scrollArea_2->setMaximumSize(QSize(150, 16777215));
        scrollArea_2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea_2->setWidgetResizable(true);
        scrollAreaWidget = new QWidget();
        scrollAreaWidget->setObjectName(QStringLiteral("scrollAreaWidget"));
        scrollAreaWidget->setGeometry(QRect(0, 0, 69, 884));
        sizePolicy.setHeightForWidth(scrollAreaWidget->sizePolicy().hasHeightForWidth());
        scrollAreaWidget->setSizePolicy(sizePolicy);
        verticalLayout_3 = new QVBoxLayout(scrollAreaWidget);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        labelClassLayout = new QVBoxLayout();
        labelClassLayout->setObjectName(QStringLiteral("labelClassLayout"));

        verticalLayout_3->addLayout(labelClassLayout);

        scrollArea_2->setWidget(scrollAreaWidget);
        splitter_2->addWidget(scrollArea_2);
        frame_3 = new QFrame(splitter_2);
        frame_3->setObjectName(QStringLiteral("frame_3"));
        sizePolicy.setHeightForWidth(frame_3->sizePolicy().hasHeightForWidth());
        frame_3->setSizePolicy(sizePolicy);
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        horizontalLayout_2 = new QHBoxLayout(frame_3);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        splitter_3 = new QSplitter(frame_3);
        splitter_3->setObjectName(QStringLiteral("splitter_3"));
        sizePolicy.setHeightForWidth(splitter_3->sizePolicy().hasHeightForWidth());
        splitter_3->setSizePolicy(sizePolicy);
        splitter_3->setMinimumSize(QSize(729, 0));
        splitter_3->setOrientation(Qt::Horizontal);
        IdmPhotoViewer = new PhotoViewer(splitter_3);
        IdmPhotoViewer->setObjectName(QStringLiteral("IdmPhotoViewer"));
        IdmPhotoViewer->setMinimumSize(QSize(20, 10));
        splitter_3->addWidget(IdmPhotoViewer);
        IdmMaskViewer = new MaskViewer(splitter_3);
        IdmMaskViewer->setObjectName(QStringLiteral("IdmMaskViewer"));
        IdmMaskViewer->setMinimumSize(QSize(20, 10));
        splitter_3->addWidget(IdmMaskViewer);

        horizontalLayout_2->addWidget(splitter_3);

        splitter_2->addWidget(frame_3);

        verticalLayout_4->addWidget(splitter_2);

        splitter->addWidget(frame_2);

        horizontalLayout->addWidget(splitter);


        verticalLayout_2->addWidget(frame_6);

        splitter_4->addWidget(frame_7);
        frame_8 = new QFrame(splitter_4);
        frame_8->setObjectName(QStringLiteral("frame_8"));
        frame_8->setFrameShape(QFrame::StyledPanel);
        frame_8->setFrameShadow(QFrame::Raised);
        verticalLayout_5 = new QVBoxLayout(frame_8);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        frame_5 = new QFrame(frame_8);
        frame_5->setObjectName(QStringLiteral("frame_5"));
        QSizePolicy sizePolicy4(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(frame_5->sizePolicy().hasHeightForWidth());
        frame_5->setSizePolicy(sizePolicy4);
        frame_5->setMinimumSize(QSize(0, 40));
        frame_5->setMaximumSize(QSize(16777215, 150));
        frame_5->setFrameShape(QFrame::NoFrame);
        frame_5->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(frame_5);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetMinimumSize);
        gridLayout->setContentsMargins(-1, 0, -1, 0);
        polygonSelectButton = new QPushButton(frame_5);
        polygonSelectButton->setObjectName(QStringLiteral("polygonSelectButton"));
        QSizePolicy sizePolicy5(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(polygonSelectButton->sizePolicy().hasHeightForWidth());
        polygonSelectButton->setSizePolicy(sizePolicy5);
        polygonSelectButton->setMinimumSize(QSize(201, 36));
        polygonSelectButton->setCheckable(true);

        gridLayout->addWidget(polygonSelectButton, 0, 1, 1, 1);

        rectangleSelectButton = new QPushButton(frame_5);
        rectangleSelectButton->setObjectName(QStringLiteral("rectangleSelectButton"));
        sizePolicy5.setHeightForWidth(rectangleSelectButton->sizePolicy().hasHeightForWidth());
        rectangleSelectButton->setSizePolicy(sizePolicy5);
        rectangleSelectButton->setMinimumSize(QSize(201, 36));
        rectangleSelectButton->setCheckable(true);

        gridLayout->addWidget(rectangleSelectButton, 0, 0, 1, 1);

        selectControlFrame = new QFrame(frame_5);
        selectControlFrame->setObjectName(QStringLiteral("selectControlFrame"));
        QSizePolicy sizePolicy6(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(selectControlFrame->sizePolicy().hasHeightForWidth());
        selectControlFrame->setSizePolicy(sizePolicy6);
        selectControlFrame->setMinimumSize(QSize(0, 120));
        selectControlFrame->setSizeIncrement(QSize(0, 0));
        selectControlFrame->setFrameShape(QFrame::StyledPanel);
        selectControlFrame->setFrameShadow(QFrame::Raised);

        gridLayout->addWidget(selectControlFrame, 0, 2, 2, 1);

        addSelectionButton = new QPushButton(frame_5);
        addSelectionButton->setObjectName(QStringLiteral("addSelectionButton"));
        addSelectionButton->setEnabled(false);
        sizePolicy5.setHeightForWidth(addSelectionButton->sizePolicy().hasHeightForWidth());
        addSelectionButton->setSizePolicy(sizePolicy5);
        addSelectionButton->setMaximumSize(QSize(220, 36));
        addSelectionButton->setFlat(false);

        gridLayout->addWidget(addSelectionButton, 0, 3, 1, 1);

        ellipseSelectButton = new QPushButton(frame_5);
        ellipseSelectButton->setObjectName(QStringLiteral("ellipseSelectButton"));
        sizePolicy5.setHeightForWidth(ellipseSelectButton->sizePolicy().hasHeightForWidth());
        ellipseSelectButton->setSizePolicy(sizePolicy5);
        ellipseSelectButton->setMinimumSize(QSize(201, 36));
        ellipseSelectButton->setCheckable(true);

        gridLayout->addWidget(ellipseSelectButton, 1, 0, 1, 1);

        cancelSelectionButton = new QPushButton(frame_5);
        cancelSelectionButton->setObjectName(QStringLiteral("cancelSelectionButton"));
        cancelSelectionButton->setEnabled(false);
        sizePolicy5.setHeightForWidth(cancelSelectionButton->sizePolicy().hasHeightForWidth());
        cancelSelectionButton->setSizePolicy(sizePolicy5);
        cancelSelectionButton->setMinimumSize(QSize(220, 0));
        cancelSelectionButton->setCheckable(true);
        cancelSelectionButton->setChecked(false);

        gridLayout->addWidget(cancelSelectionButton, 1, 3, 1, 1);

        freeSelectButton = new QPushButton(frame_5);
        freeSelectButton->setObjectName(QStringLiteral("freeSelectButton"));
        freeSelectButton->setCheckable(true);

        gridLayout->addWidget(freeSelectButton, 1, 1, 1, 1);


        verticalLayout_5->addWidget(frame_5);

        tabWidget = new QTabWidget(frame_8);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setMaximumSize(QSize(16777215, 300));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        horizontalLayout_3 = new QHBoxLayout(tab);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        scrollArea = new QScrollArea(tab);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 1888, 70));
        verticalLayout_7 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        label_6 = new QLabel(scrollAreaWidgetContents);
        label_6->setObjectName(QStringLiteral("label_6"));

        horizontalLayout_8->addWidget(label_6, 0, Qt::AlignHCenter|Qt::AlignVCenter);

        label_5 = new QLabel(scrollAreaWidgetContents);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout_8->addWidget(label_5, 0, Qt::AlignHCenter);

        label_4 = new QLabel(scrollAreaWidgetContents);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout_8->addWidget(label_4, 0, Qt::AlignHCenter);


        verticalLayout_6->addLayout(horizontalLayout_8);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        blur_label = new PicPushButton(scrollAreaWidgetContents);
        blur_label->setObjectName(QStringLiteral("blur_label"));

        horizontalLayout_7->addWidget(blur_label, 0, Qt::AlignHCenter);

        canny_label = new PicPushButton(scrollAreaWidgetContents);
        canny_label->setObjectName(QStringLiteral("canny_label"));

        horizontalLayout_7->addWidget(canny_label, 0, Qt::AlignHCenter);

        threshold_label = new PicPushButton(scrollAreaWidgetContents);
        threshold_label->setObjectName(QStringLiteral("threshold_label"));

        horizontalLayout_7->addWidget(threshold_label, 0, Qt::AlignHCenter);


        verticalLayout_6->addLayout(horizontalLayout_7);


        verticalLayout_7->addLayout(verticalLayout_6);

        scrollArea->setWidget(scrollAreaWidgetContents);

        horizontalLayout_3->addWidget(scrollArea);

        tabWidget->addTab(tab, QString());

        verticalLayout_5->addWidget(tabWidget);

        splitter_4->addWidget(frame_8);

        horizontalLayout_5->addWidget(splitter_4);


        verticalLayout->addWidget(frame);

        InstaDam->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(InstaDam);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        InstaDam->setStatusBar(statusbar);
        menubar = new QMenuBar(InstaDam);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setEnabled(true);
        menubar->setGeometry(QRect(0, 0, 1918, 21));
        menubar->setDefaultUp(false);
        menubar->setNativeMenuBar(true);
        menu_File = new QMenu(menubar);
        menu_File->setObjectName(QStringLiteral("menu_File"));
        menuEdit = new QMenu(menubar);
        menuEdit->setObjectName(QStringLiteral("menuEdit"));
        menuProject = new QMenu(menubar);
        menuProject->setObjectName(QStringLiteral("menuProject"));
        InstaDam->setMenuBar(menubar);

        menubar->addAction(menuProject->menuAction());
        menubar->addAction(menu_File->menuAction());
        menubar->addAction(menuEdit->menuAction());
        menu_File->addAction(actionOpen_File);
        menu_File->addAction(actionSave_Annotation);
        menu_File->addSeparator();
        menu_File->addAction(actionExit);
        menuEdit->addAction(actionDelete);
        menuProject->addAction(actionNew);
        menuProject->addAction(actionOpen);
        menuProject->addAction(actionSave);
        menuProject->addAction(actionExport);
        menuProject->addAction(actionExport_zip);

        retranslateUi(InstaDam);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(InstaDam);
    } // setupUi

    void retranslateUi(QMainWindow *InstaDam)
    {
        InstaDam->setWindowTitle(QApplication::translate("InstaDam", "MainWindow", nullptr));
        action_Open->setText(QApplication::translate("InstaDam", "Open Project", nullptr));
        actionSave_File->setText(QApplication::translate("InstaDam", "Export", nullptr));
        actionExit->setText(QApplication::translate("InstaDam", "Exit", nullptr));
        actionNew->setText(QApplication::translate("InstaDam", "New", nullptr));
        actionOpen->setText(QApplication::translate("InstaDam", "Open", nullptr));
        actionSave->setText(QApplication::translate("InstaDam", "Save", nullptr));
        actionUndo->setText(QApplication::translate("InstaDam", "Undo", nullptr));
#ifndef QT_NO_SHORTCUT
        actionUndo->setShortcut(QApplication::translate("InstaDam", "Ctrl+Z", nullptr));
#endif // QT_NO_SHORTCUT
        actionOpen_File->setText(QApplication::translate("InstaDam", "Open Image", nullptr));
        actionRedo->setText(QApplication::translate("InstaDam", "Redo", nullptr));
#ifndef QT_NO_SHORTCUT
        actionRedo->setShortcut(QApplication::translate("InstaDam", "Ctrl+S", nullptr));
#endif // QT_NO_SHORTCUT
        actionDelete->setText(QApplication::translate("InstaDam", "Delete", nullptr));
#ifndef QT_NO_SHORTCUT
        actionDelete->setShortcut(QApplication::translate("InstaDam", "Del", nullptr));
#endif // QT_NO_SHORTCUT
        actionExport->setText(QApplication::translate("InstaDam", "Export", nullptr));
        actionExport_zip->setText(QApplication::translate("InstaDam", "Export zip", nullptr));
        actionSave_Annotation->setText(QApplication::translate("InstaDam", "Save Annotation", nullptr));
        saveAndBack->setText(QApplication::translate("InstaDam", "Save and Back", nullptr));
        saveAndNext->setText(QApplication::translate("InstaDam", "Save and Next", nullptr));
        panButton->setText(QApplication::translate("InstaDam", "Pan", nullptr));
        polygonSelectButton->setText(QApplication::translate("InstaDam", "Polygon Select", nullptr));
        rectangleSelectButton->setText(QApplication::translate("InstaDam", "Box Select", nullptr));
        addSelectionButton->setText(QApplication::translate("InstaDam", "Add Selection", nullptr));
        ellipseSelectButton->setText(QApplication::translate("InstaDam", "EllipseSelect", nullptr));
        cancelSelectionButton->setText(QApplication::translate("InstaDam", "Cancel Selection", nullptr));
        freeSelectButton->setText(QApplication::translate("InstaDam", "Free Select", nullptr));
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
