/********************************************************************************
** Form generated from reading UI file 'projectlist.ui'
**
** Created by: Qt User Interface Compiler version 5.11.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROJECTLIST_H
#define UI_PROJECTLIST_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ProjectList
{
public:
    QListWidget *projectsTable;
    QLabel *label;

    void setupUi(QWidget *ProjectList)
    {
        if (ProjectList->objectName().isEmpty())
            ProjectList->setObjectName(QStringLiteral("ProjectList"));
        ProjectList->resize(600, 370);
        projectsTable = new QListWidget(ProjectList);
        projectsTable->setObjectName(QStringLiteral("projectsTable"));
        projectsTable->setGeometry(QRect(25, 25, 550, 301));
        label = new QLabel(ProjectList);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(30, 340, 461, 16));
        QFont font;
        font.setItalic(true);
        label->setFont(font);

        retranslateUi(ProjectList);

        QMetaObject::connectSlotsByName(ProjectList);
    } // setupUi

    void retranslateUi(QWidget *ProjectList)
    {
        ProjectList->setWindowTitle(QApplication::translate("ProjectList", "Form", nullptr));
        label->setText(QApplication::translate("ProjectList", "*Info: double click on a project to perform the requested action ", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ProjectList: public Ui_ProjectList {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROJECTLIST_H
