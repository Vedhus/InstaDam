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
#include <QtWidgets/QCheckBox>
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
#include <QtWidgets/QTextBrowser>
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
    QAction *openAnnotations;
    QAction *actionSearch;
    QAction *actionUpdate_Privilege;
    QAction *actionDelete_2;
    QAction *actionEdit_Label;
    QAction *actionImport;
    QAction *actionClear_All_can_t_undo;
    QAction *actionExport_mat;
    QAction *actionTest;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QFrame *frame;
    QHBoxLayout *horizontalLayout_5;
    QSplitter *splitter_4;
    QFrame *frame_7;
    QVBoxLayout *verticalLayout_2;
    QFrame *frame_4;
    QVBoxLayout *verticalLayout_8;
    QGridLayout *gridLayout_3;
    QPushButton *addSelectionButton;
    QTextBrowser *photoViewTime;
    QPushButton *polygonSelectButton;
    QTextBrowser *filelabel;
    QSpacerItem *horizontalSpacer;
    QPushButton *saveAndBack;
    QPushButton *ellipseSelectButton;
    QPushButton *freeSelectButton;
    QPushButton *cancelSelectionButton;
    QTextBrowser *maskViewTime;
    QPushButton *panButton;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *saveAndNext;
    QCheckBox *showMaskSelections;
    QPushButton *filterOptions;
    QPushButton *rectangleSelectButton;
    QPushButton *time;
    QFrame *selectControlFrame;
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
    QLabel *label_2;
    QLabel *label_3;
    QHBoxLayout *horizontalLayout_7;
    PicPushButton *blur_label;
    PicPushButton *canny_label;
    PicPushButton *threshold_label;
    PicPushButton *labelmask_label;
    PicPushButton *colorthreshold_label;
    QStatusBar *statusbar;
    QMenuBar *menubar;
    QMenu *menu_File;
    QMenu *menuEdit;
    QMenu *menuProject;
    QMenu *menuUser;

    void setupUi(QMainWindow *InstaDam)
    {
        if (InstaDam->objectName().isEmpty())
            InstaDam->setObjectName(QStringLiteral("InstaDam"));
        InstaDam->resize(1517, 623);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(InstaDam->sizePolicy().hasHeightForWidth());
        InstaDam->setSizePolicy(sizePolicy);
        InstaDam->setMouseTracking(true);
        InstaDam->setTabletTracking(true);
        InstaDam->setAcceptDrops(true);
        InstaDam->setAutoFillBackground(true);
        InstaDam->setStyleSheet(QStringLiteral(""));
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
        openAnnotations = new QAction(InstaDam);
        openAnnotations->setObjectName(QStringLiteral("openAnnotations"));
        actionSearch = new QAction(InstaDam);
        actionSearch->setObjectName(QStringLiteral("actionSearch"));
        actionUpdate_Privilege = new QAction(InstaDam);
        actionUpdate_Privilege->setObjectName(QStringLiteral("actionUpdate_Privilege"));
        actionDelete_2 = new QAction(InstaDam);
        actionDelete_2->setObjectName(QStringLiteral("actionDelete_2"));
        actionEdit_Label = new QAction(InstaDam);
        actionEdit_Label->setObjectName(QStringLiteral("actionEdit_Label"));
        actionImport = new QAction(InstaDam);
        actionImport->setObjectName(QStringLiteral("actionImport"));
        actionClear_All_can_t_undo = new QAction(InstaDam);
        actionClear_All_can_t_undo->setObjectName(QStringLiteral("actionClear_All_can_t_undo"));
        actionExport_mat = new QAction(InstaDam);
        actionExport_mat->setObjectName(QStringLiteral("actionExport_mat"));
        actionTest = new QAction(InstaDam);
        actionTest->setObjectName(QStringLiteral("actionTest"));
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
        frame_4->setMaximumSize(QSize(16777215, 100));
        frame_4->setFrameShape(QFrame::NoFrame);
        frame_4->setFrameShadow(QFrame::Plain);
        frame_4->setLineWidth(0);
        verticalLayout_8 = new QVBoxLayout(frame_4);
        verticalLayout_8->setSpacing(1);
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        verticalLayout_8->setContentsMargins(1, 1, 1, 1);
        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        addSelectionButton = new QPushButton(frame_4);
        addSelectionButton->setObjectName(QStringLiteral("addSelectionButton"));
        addSelectionButton->setEnabled(false);
        sizePolicy.setHeightForWidth(addSelectionButton->sizePolicy().hasHeightForWidth());
        addSelectionButton->setSizePolicy(sizePolicy);
        addSelectionButton->setMaximumSize(QSize(16777215, 16777215));
        addSelectionButton->setFlat(false);

        gridLayout_3->addWidget(addSelectionButton, 6, 23, 1, 1);

        photoViewTime = new QTextBrowser(frame_4);
        photoViewTime->setObjectName(QStringLiteral("photoViewTime"));
        photoViewTime->setMinimumSize(QSize(0, 0));
        photoViewTime->setMaximumSize(QSize(85, 22));
        photoViewTime->setFrameShape(QFrame::NoFrame);

        gridLayout_3->addWidget(photoViewTime, 6, 18, 1, 1);

        polygonSelectButton = new QPushButton(frame_4);
        polygonSelectButton->setObjectName(QStringLiteral("polygonSelectButton"));
        sizePolicy.setHeightForWidth(polygonSelectButton->sizePolicy().hasHeightForWidth());
        polygonSelectButton->setSizePolicy(sizePolicy);
        polygonSelectButton->setMinimumSize(QSize(36, 24));
        polygonSelectButton->setCheckable(true);

        gridLayout_3->addWidget(polygonSelectButton, 6, 5, 1, 1);

        filelabel = new QTextBrowser(frame_4);
        filelabel->setObjectName(QStringLiteral("filelabel"));
        filelabel->setMaximumSize(QSize(16777215, 22));
        filelabel->setFrameShape(QFrame::NoFrame);
        filelabel->setFrameShadow(QFrame::Plain);

        gridLayout_3->addWidget(filelabel, 6, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer, 6, 3, 1, 1);

        saveAndBack = new QPushButton(frame_4);
        saveAndBack->setObjectName(QStringLiteral("saveAndBack"));
        sizePolicy.setHeightForWidth(saveAndBack->sizePolicy().hasHeightForWidth());
        saveAndBack->setSizePolicy(sizePolicy);
        saveAndBack->setMinimumSize(QSize(36, 0));

        gridLayout_3->addWidget(saveAndBack, 6, 1, 1, 1);

        ellipseSelectButton = new QPushButton(frame_4);
        ellipseSelectButton->setObjectName(QStringLiteral("ellipseSelectButton"));
        sizePolicy.setHeightForWidth(ellipseSelectButton->sizePolicy().hasHeightForWidth());
        ellipseSelectButton->setSizePolicy(sizePolicy);
        ellipseSelectButton->setMinimumSize(QSize(36, 24));
        ellipseSelectButton->setCheckable(true);

        gridLayout_3->addWidget(ellipseSelectButton, 6, 11, 1, 1);

        freeSelectButton = new QPushButton(frame_4);
        freeSelectButton->setObjectName(QStringLiteral("freeSelectButton"));
        sizePolicy.setHeightForWidth(freeSelectButton->sizePolicy().hasHeightForWidth());
        freeSelectButton->setSizePolicy(sizePolicy);
        freeSelectButton->setMinimumSize(QSize(36, 24));
        freeSelectButton->setCheckable(true);

        gridLayout_3->addWidget(freeSelectButton, 6, 4, 1, 1);

        cancelSelectionButton = new QPushButton(frame_4);
        cancelSelectionButton->setObjectName(QStringLiteral("cancelSelectionButton"));
        cancelSelectionButton->setEnabled(false);
        sizePolicy.setHeightForWidth(cancelSelectionButton->sizePolicy().hasHeightForWidth());
        cancelSelectionButton->setSizePolicy(sizePolicy);
        cancelSelectionButton->setMinimumSize(QSize(35, 0));
        cancelSelectionButton->setCheckable(true);
        cancelSelectionButton->setChecked(false);

        gridLayout_3->addWidget(cancelSelectionButton, 6, 27, 1, 1);

        maskViewTime = new QTextBrowser(frame_4);
        maskViewTime->setObjectName(QStringLiteral("maskViewTime"));
        maskViewTime->setMaximumSize(QSize(85, 22));
        maskViewTime->setFrameShape(QFrame::NoFrame);
        maskViewTime->setFrameShadow(QFrame::Plain);

        gridLayout_3->addWidget(maskViewTime, 6, 19, 1, 1);

        panButton = new QPushButton(frame_4);
        panButton->setObjectName(QStringLiteral("panButton"));
        sizePolicy.setHeightForWidth(panButton->sizePolicy().hasHeightForWidth());
        panButton->setSizePolicy(sizePolicy);
        panButton->setMinimumSize(QSize(36, 0));
        panButton->setCheckable(true);
        panButton->setChecked(false);

        gridLayout_3->addWidget(panButton, 6, 13, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_2, 6, 21, 1, 1);

        saveAndNext = new QPushButton(frame_4);
        saveAndNext->setObjectName(QStringLiteral("saveAndNext"));
        sizePolicy.setHeightForWidth(saveAndNext->sizePolicy().hasHeightForWidth());
        saveAndNext->setSizePolicy(sizePolicy);
        saveAndNext->setMinimumSize(QSize(36, 0));

        gridLayout_3->addWidget(saveAndNext, 6, 2, 1, 1);

        showMaskSelections = new QCheckBox(frame_4);
        showMaskSelections->setObjectName(QStringLiteral("showMaskSelections"));

        gridLayout_3->addWidget(showMaskSelections, 6, 28, 1, 1);

        filterOptions = new QPushButton(frame_4);
        filterOptions->setObjectName(QStringLiteral("filterOptions"));
        sizePolicy.setHeightForWidth(filterOptions->sizePolicy().hasHeightForWidth());
        filterOptions->setSizePolicy(sizePolicy);
        filterOptions->setMinimumSize(QSize(36, 0));
#ifndef QT_NO_ACCESSIBILITY
        filterOptions->setAccessibleName(QStringLiteral(""));
#endif // QT_NO_ACCESSIBILITY
        filterOptions->setAutoFillBackground(false);
        filterOptions->setText(QStringLiteral("Filter Options"));
        filterOptions->setIconSize(QSize(200, 200));

        gridLayout_3->addWidget(filterOptions, 6, 22, 1, 1);

        rectangleSelectButton = new QPushButton(frame_4);
        rectangleSelectButton->setObjectName(QStringLiteral("rectangleSelectButton"));
        sizePolicy.setHeightForWidth(rectangleSelectButton->sizePolicy().hasHeightForWidth());
        rectangleSelectButton->setSizePolicy(sizePolicy);
        rectangleSelectButton->setMinimumSize(QSize(36, 24));
        rectangleSelectButton->setCheckable(true);

        gridLayout_3->addWidget(rectangleSelectButton, 6, 6, 1, 1);

        time = new QPushButton(frame_4);
        time->setObjectName(QStringLiteral("time"));
        time->setMaximumSize(QSize(35, 16777215));
        time->setCheckable(true);

        gridLayout_3->addWidget(time, 6, 17, 1, 1);


        verticalLayout_8->addLayout(gridLayout_3);

        selectControlFrame = new QFrame(frame_4);
        selectControlFrame->setObjectName(QStringLiteral("selectControlFrame"));
        sizePolicy.setHeightForWidth(selectControlFrame->sizePolicy().hasHeightForWidth());
        selectControlFrame->setSizePolicy(sizePolicy);
        selectControlFrame->setMinimumSize(QSize(859, 24));
        selectControlFrame->setMaximumSize(QSize(16777215, 50));
        selectControlFrame->setSizeIncrement(QSize(0, 0));
        selectControlFrame->setFrameShape(QFrame::NoFrame);
        selectControlFrame->setFrameShadow(QFrame::Plain);

        verticalLayout_8->addWidget(selectControlFrame);


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
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(splitter->sizePolicy().hasHeightForWidth());
        splitter->setSizePolicy(sizePolicy2);
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
        scrollArea_2->setMaximumSize(QSize(400, 16777215));
        scrollArea_2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea_2->setWidgetResizable(true);
        scrollAreaWidget = new QWidget();
        scrollAreaWidget->setObjectName(QStringLiteral("scrollAreaWidget"));
        scrollAreaWidget->setGeometry(QRect(0, 0, 69, 396));
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
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 1487, 70));
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
        label_5->setAutoFillBackground(true);

        horizontalLayout_8->addWidget(label_5, 0, Qt::AlignHCenter);

        label_4 = new QLabel(scrollAreaWidgetContents);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout_8->addWidget(label_4, 0, Qt::AlignHCenter);

        label_2 = new QLabel(scrollAreaWidgetContents);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_8->addWidget(label_2, 0, Qt::AlignHCenter);

        label_3 = new QLabel(scrollAreaWidgetContents);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_8->addWidget(label_3, 0, Qt::AlignHCenter);


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

        labelmask_label = new PicPushButton(scrollAreaWidgetContents);
        labelmask_label->setObjectName(QStringLiteral("labelmask_label"));

        horizontalLayout_7->addWidget(labelmask_label, 0, Qt::AlignHCenter);

        colorthreshold_label = new PicPushButton(scrollAreaWidgetContents);
        colorthreshold_label->setObjectName(QStringLiteral("colorthreshold_label"));

        horizontalLayout_7->addWidget(colorthreshold_label, 0, Qt::AlignHCenter);


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
        menubar->setGeometry(QRect(0, 0, 1517, 21));
        menubar->setDefaultUp(false);
        menubar->setNativeMenuBar(true);
        menu_File = new QMenu(menubar);
        menu_File->setObjectName(QStringLiteral("menu_File"));
        menuEdit = new QMenu(menubar);
        menuEdit->setObjectName(QStringLiteral("menuEdit"));
        menuProject = new QMenu(menubar);
        menuProject->setObjectName(QStringLiteral("menuProject"));
        menuUser = new QMenu(menubar);
        menuUser->setObjectName(QStringLiteral("menuUser"));
        InstaDam->setMenuBar(menubar);

        menubar->addAction(menuProject->menuAction());
        menubar->addAction(menu_File->menuAction());
        menubar->addAction(menuEdit->menuAction());
        menubar->addAction(menuUser->menuAction());
        menu_File->addAction(actionOpen_File);
        menu_File->addSeparator();
        menu_File->addAction(actionSave_Annotation);
        menu_File->addAction(actionImport);
        menu_File->addAction(actionExport);
        menu_File->addAction(actionExport_zip);
        menu_File->addAction(actionExport_mat);
        menu_File->addSeparator();
        menuEdit->addAction(actionDelete);
        menuEdit->addAction(actionEdit_Label);
        menuEdit->addSeparator();
        menuEdit->addAction(actionClear_All_can_t_undo);
        menuProject->addAction(actionNew);
        menuProject->addAction(actionOpen);
        menuProject->addAction(actionSave);
        menuProject->addAction(actionDelete_2);
        menuUser->addAction(actionSearch);
        menuUser->addAction(actionUpdate_Privilege);

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
        actionSave_Annotation->setText(QApplication::translate("InstaDam", "Save", nullptr));
        openAnnotations->setText(QApplication::translate("InstaDam", "Open", nullptr));
        actionSearch->setText(QApplication::translate("InstaDam", "Search", nullptr));
        actionUpdate_Privilege->setText(QApplication::translate("InstaDam", "Update Privilege", nullptr));
        actionDelete_2->setText(QApplication::translate("InstaDam", "Delete", nullptr));
        actionEdit_Label->setText(QApplication::translate("InstaDam", "Edit Label Class", nullptr));
        actionImport->setText(QApplication::translate("InstaDam", "Import", nullptr));
        actionClear_All_can_t_undo->setText(QApplication::translate("InstaDam", "Clear All (can't undo)", nullptr));
        actionExport_mat->setText(QApplication::translate("InstaDam", "Export .npz", nullptr));
        actionTest->setText(QApplication::translate("InstaDam", "Test", nullptr));
        addSelectionButton->setText(QApplication::translate("InstaDam", "Add Selection", nullptr));
        polygonSelectButton->setText(QApplication::translate("InstaDam", "Polygon Select", nullptr));
        saveAndBack->setText(QApplication::translate("InstaDam", "Save and Back", nullptr));
        ellipseSelectButton->setText(QApplication::translate("InstaDam", "EllipseSelect", nullptr));
        freeSelectButton->setText(QApplication::translate("InstaDam", "Free Select", nullptr));
        cancelSelectionButton->setText(QApplication::translate("InstaDam", "Cancel Selection", nullptr));
        panButton->setText(QApplication::translate("InstaDam", "Pan", nullptr));
        saveAndNext->setText(QApplication::translate("InstaDam", "Save and Next", nullptr));
        showMaskSelections->setText(QApplication::translate("InstaDam", "Show Selections on Mask", nullptr));
        rectangleSelectButton->setText(QApplication::translate("InstaDam", "Box Select", nullptr));
        time->setText(QApplication::translate("InstaDam", "Time", nullptr));
        label_6->setText(QApplication::translate("InstaDam", "Blur", nullptr));
        label_5->setText(QApplication::translate("InstaDam", "Canny", nullptr));
        label_4->setText(QApplication::translate("InstaDam", "Threshold", nullptr));
        label_2->setText(QApplication::translate("InstaDam", "Label Mask", nullptr));
        label_3->setText(QApplication::translate("InstaDam", "Color Range", nullptr));
        blur_label->setText(QApplication::translate("InstaDam", "TextLabel", nullptr));
        canny_label->setText(QApplication::translate("InstaDam", "TextLabel", nullptr));
        threshold_label->setText(QApplication::translate("InstaDam", "TextLabel", nullptr));
        labelmask_label->setText(QApplication::translate("InstaDam", "TextLabel", nullptr));
        colorthreshold_label->setText(QApplication::translate("InstaDam", "TextLabel", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("InstaDam", "Filters", nullptr));
        menu_File->setTitle(QApplication::translate("InstaDam", "Annotations", nullptr));
        menuEdit->setTitle(QApplication::translate("InstaDam", "Edit", nullptr));
        menuProject->setTitle(QApplication::translate("InstaDam", "Project", nullptr));
        menuUser->setTitle(QApplication::translate("InstaDam", "User", nullptr));
    } // retranslateUi

};

namespace Ui {
    class InstaDam: public Ui_InstaDam {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INSTADAM_H
