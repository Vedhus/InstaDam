/********************************************************************************
** Form generated from reading UI file 'projectlist.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROJECTLIST_H
#define UI_PROJECTLIST_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ProjectList
{
public:
    QListWidget *projectsTable;

    void setupUi(QWidget *ProjectList)
    {
        if (ProjectList->objectName().isEmpty())
            ProjectList->setObjectName(QString::fromUtf8("ProjectList"));
        ProjectList->resize(600, 370);
        projectsTable = new QListWidget(ProjectList);
        projectsTable->setObjectName(QString::fromUtf8("projectsTable"));
        projectsTable->setGeometry(QRect(25, 25, 550, 301));

        retranslateUi(ProjectList);

        QMetaObject::connectSlotsByName(ProjectList);
    } // setupUi

    void retranslateUi(QWidget *ProjectList)
    {
        ProjectList->setWindowTitle(QApplication::translate("ProjectList", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ProjectList: public Ui_ProjectList {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROJECTLIST_H
