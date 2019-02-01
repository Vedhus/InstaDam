/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_2;
    QSplitter *connectionSplitter;
    QTreeWidget *tablesView;
    QTabWidget *dbhTabs;
    QWidget *editor;
    QVBoxLayout *verticalLayout;
    QSplitter *resultsSplitter;
    QWidget *verticalLayoutWidget_3;
    QVBoxLayout *topPanelLayout;
    QPlainTextEdit *plainTextEdit;
    QVBoxLayout *sqlLayout;
    QHBoxLayout *buttonLayout;
    QSpacerItem *editorSpacer;
    QPushButton *commit;
    QPushButton *rollback;
    QPushButton *execute;
    QPushButton *cancel;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *resultsLayout;
    QTableWidget *results;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1413, 1263);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout_2 = new QVBoxLayout(centralwidget);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        connectionSplitter = new QSplitter(centralwidget);
        connectionSplitter->setObjectName(QString::fromUtf8("connectionSplitter"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(connectionSplitter->sizePolicy().hasHeightForWidth());
        connectionSplitter->setSizePolicy(sizePolicy);
        connectionSplitter->setOrientation(Qt::Horizontal);
        connectionSplitter->setHandleWidth(10);
        connectionSplitter->setChildrenCollapsible(false);
        tablesView = new QTreeWidget(connectionSplitter);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        tablesView->setHeaderItem(__qtreewidgetitem);
        QTreeWidgetItem *__qtreewidgetitem1 = new QTreeWidgetItem(tablesView);
        new QTreeWidgetItem(__qtreewidgetitem1);
        new QTreeWidgetItem(__qtreewidgetitem1);
        new QTreeWidgetItem(tablesView);
        tablesView->setObjectName(QString::fromUtf8("tablesView"));
        sizePolicy.setHeightForWidth(tablesView->sizePolicy().hasHeightForWidth());
        tablesView->setSizePolicy(sizePolicy);
        tablesView->setMinimumSize(QSize(0, 0));
        tablesView->setMaximumSize(QSize(600, 16777210));
        tablesView->setSizeIncrement(QSize(0, 0));
        tablesView->setBaseSize(QSize(0, 0));
        tablesView->viewport()->setProperty("cursor", QVariant(QCursor(Qt::ArrowCursor)));
        tablesView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tablesView->setUniformRowHeights(true);
        tablesView->setSortingEnabled(false);
        connectionSplitter->addWidget(tablesView);
        tablesView->header()->setVisible(false);
        dbhTabs = new QTabWidget(connectionSplitter);
        dbhTabs->setObjectName(QString::fromUtf8("dbhTabs"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(dbhTabs->sizePolicy().hasHeightForWidth());
        dbhTabs->setSizePolicy(sizePolicy1);
        dbhTabs->setBaseSize(QSize(0, 0));
        editor = new QWidget();
        editor->setObjectName(QString::fromUtf8("editor"));
        verticalLayout = new QVBoxLayout(editor);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        resultsSplitter = new QSplitter(editor);
        resultsSplitter->setObjectName(QString::fromUtf8("resultsSplitter"));
        sizePolicy.setHeightForWidth(resultsSplitter->sizePolicy().hasHeightForWidth());
        resultsSplitter->setSizePolicy(sizePolicy);
        resultsSplitter->setOrientation(Qt::Vertical);
        resultsSplitter->setHandleWidth(10);
        resultsSplitter->setChildrenCollapsible(false);
        verticalLayoutWidget_3 = new QWidget(resultsSplitter);
        verticalLayoutWidget_3->setObjectName(QString::fromUtf8("verticalLayoutWidget_3"));
        topPanelLayout = new QVBoxLayout(verticalLayoutWidget_3);
        topPanelLayout->setSpacing(0);
        topPanelLayout->setObjectName(QString::fromUtf8("topPanelLayout"));
        topPanelLayout->setContentsMargins(0, 0, 0, 0);
        plainTextEdit = new QPlainTextEdit(verticalLayoutWidget_3);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));
        plainTextEdit->setDocumentTitle(QString::fromUtf8(""));
        plainTextEdit->setPlainText(QString::fromUtf8(""));

        topPanelLayout->addWidget(plainTextEdit);

        sqlLayout = new QVBoxLayout();
        sqlLayout->setSpacing(0);
        sqlLayout->setObjectName(QString::fromUtf8("sqlLayout"));
        buttonLayout = new QHBoxLayout();
        buttonLayout->setSpacing(6);
        buttonLayout->setObjectName(QString::fromUtf8("buttonLayout"));
        buttonLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
        buttonLayout->setContentsMargins(-1, -1, 3, -1);
        editorSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        buttonLayout->addItem(editorSpacer);

        commit = new QPushButton(verticalLayoutWidget_3);
        commit->setObjectName(QString::fromUtf8("commit"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(commit->sizePolicy().hasHeightForWidth());
        commit->setSizePolicy(sizePolicy2);
        commit->setMaximumSize(QSize(200, 16777215));
        commit->setText(QString::fromUtf8("Commit"));

        buttonLayout->addWidget(commit);

        rollback = new QPushButton(verticalLayoutWidget_3);
        rollback->setObjectName(QString::fromUtf8("rollback"));
        sizePolicy2.setHeightForWidth(rollback->sizePolicy().hasHeightForWidth());
        rollback->setSizePolicy(sizePolicy2);
        rollback->setMaximumSize(QSize(200, 16777215));
        rollback->setText(QString::fromUtf8("Rollback"));

        buttonLayout->addWidget(rollback);

        execute = new QPushButton(verticalLayoutWidget_3);
        execute->setObjectName(QString::fromUtf8("execute"));
        sizePolicy2.setHeightForWidth(execute->sizePolicy().hasHeightForWidth());
        execute->setSizePolicy(sizePolicy2);
        execute->setMaximumSize(QSize(200, 16777215));
        execute->setText(QString::fromUtf8("Execute"));

        buttonLayout->addWidget(execute);

        cancel = new QPushButton(verticalLayoutWidget_3);
        cancel->setObjectName(QString::fromUtf8("cancel"));
        sizePolicy2.setHeightForWidth(cancel->sizePolicy().hasHeightForWidth());
        cancel->setSizePolicy(sizePolicy2);
        cancel->setMaximumSize(QSize(200, 16777215));
        cancel->setText(QString::fromUtf8("Cancel"));

        buttonLayout->addWidget(cancel);


        sqlLayout->addLayout(buttonLayout);


        topPanelLayout->addLayout(sqlLayout);

        resultsSplitter->addWidget(verticalLayoutWidget_3);
        verticalLayoutWidget_2 = new QWidget(resultsSplitter);
        verticalLayoutWidget_2->setObjectName(QString::fromUtf8("verticalLayoutWidget_2"));
        resultsLayout = new QVBoxLayout(verticalLayoutWidget_2);
        resultsLayout->setSpacing(0);
        resultsLayout->setObjectName(QString::fromUtf8("resultsLayout"));
        resultsLayout->setContentsMargins(0, 0, 0, 0);
        results = new QTableWidget(verticalLayoutWidget_2);
        if (results->columnCount() < 3)
            results->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        results->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        results->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        results->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        if (results->rowCount() < 3)
            results->setRowCount(3);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        results->setVerticalHeaderItem(0, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        results->setVerticalHeaderItem(1, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        results->setVerticalHeaderItem(2, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        results->setItem(0, 0, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        results->setItem(0, 1, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        results->setItem(0, 2, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        results->setItem(1, 0, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        results->setItem(1, 1, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        results->setItem(1, 2, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        results->setItem(2, 0, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        results->setItem(2, 1, __qtablewidgetitem13);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        results->setItem(2, 2, __qtablewidgetitem14);
        results->setObjectName(QString::fromUtf8("results"));
        sizePolicy.setHeightForWidth(results->sizePolicy().hasHeightForWidth());
        results->setSizePolicy(sizePolicy);

        resultsLayout->addWidget(results);

        resultsSplitter->addWidget(verticalLayoutWidget_2);

        verticalLayout->addWidget(resultsSplitter);

        dbhTabs->addTab(editor, QString());
        dbhTabs->setTabText(dbhTabs->indexOf(editor), QString::fromUtf8("Editor"));
        connectionSplitter->addWidget(dbhTabs);

        verticalLayout_2->addWidget(connectionSplitter);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1413, 34));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        dbhTabs->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));

        const bool __sortingEnabled = tablesView->isSortingEnabled();
        tablesView->setSortingEnabled(false);
        QTreeWidgetItem *___qtreewidgetitem = tablesView->topLevelItem(0);
        ___qtreewidgetitem->setText(0, QApplication::translate("MainWindow", "Item1", nullptr));
        QTreeWidgetItem *___qtreewidgetitem1 = ___qtreewidgetitem->child(0);
        ___qtreewidgetitem1->setText(0, QApplication::translate("MainWindow", "SubItem1", nullptr));
        QTreeWidgetItem *___qtreewidgetitem2 = ___qtreewidgetitem->child(1);
        ___qtreewidgetitem2->setText(0, QApplication::translate("MainWindow", "SubItem2", nullptr));
        QTreeWidgetItem *___qtreewidgetitem3 = tablesView->topLevelItem(1);
        ___qtreewidgetitem3->setText(0, QApplication::translate("MainWindow", "Item2", nullptr));
        tablesView->setSortingEnabled(__sortingEnabled);

        QTableWidgetItem *___qtablewidgetitem = results->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("MainWindow", "First Name", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = results->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("MainWindow", "Last Name", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = results->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("MainWindow", "Age", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = results->verticalHeaderItem(0);
        ___qtablewidgetitem3->setText(QApplication::translate("MainWindow", "1", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = results->verticalHeaderItem(1);
        ___qtablewidgetitem4->setText(QApplication::translate("MainWindow", "2", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = results->verticalHeaderItem(2);
        ___qtablewidgetitem5->setText(QApplication::translate("MainWindow", "3", nullptr));

        const bool __sortingEnabled1 = results->isSortingEnabled();
        results->setSortingEnabled(false);
        QTableWidgetItem *___qtablewidgetitem6 = results->item(0, 0);
        ___qtablewidgetitem6->setText(QApplication::translate("MainWindow", "Bill", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = results->item(0, 1);
        ___qtablewidgetitem7->setText(QApplication::translate("MainWindow", "Smith", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = results->item(0, 2);
        ___qtablewidgetitem8->setText(QApplication::translate("MainWindow", "56", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = results->item(1, 0);
        ___qtablewidgetitem9->setText(QApplication::translate("MainWindow", "Frank", nullptr));
        QTableWidgetItem *___qtablewidgetitem10 = results->item(1, 1);
        ___qtablewidgetitem10->setText(QApplication::translate("MainWindow", "Johnson", nullptr));
        QTableWidgetItem *___qtablewidgetitem11 = results->item(1, 2);
        ___qtablewidgetitem11->setText(QApplication::translate("MainWindow", "22", nullptr));
        QTableWidgetItem *___qtablewidgetitem12 = results->item(2, 0);
        ___qtablewidgetitem12->setText(QApplication::translate("MainWindow", "Sue", nullptr));
        QTableWidgetItem *___qtablewidgetitem13 = results->item(2, 1);
        ___qtablewidgetitem13->setText(QApplication::translate("MainWindow", "Black", nullptr));
        QTableWidgetItem *___qtablewidgetitem14 = results->item(2, 2);
        ___qtablewidgetitem14->setText(QApplication::translate("MainWindow", "45", nullptr));
        results->setSortingEnabled(__sortingEnabled1);

    } // retranslateUi

};


    class MainWindow: public Ui_MainWindow {};


QT_END_NAMESPACE

#endif // MAINWINDOW_H
