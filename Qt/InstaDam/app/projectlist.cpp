#include "projectlist.h"
#include "ui_projectlist.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>

ProjectList::ProjectList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectList)
{
    ui->setupUi(this);
}

ProjectList::~ProjectList()
{
    delete ui;
}

void ProjectList::addItems(QJsonDocument obj){
    QJsonArray projects_list = obj.array();
    qInfo() << obj;
    for(int i =0; i<projects_list.count();i++){
        QJsonValue project = projects_list.at(i);
            if(project.isObject()){
                QJsonObject subObj = project.toObject();
                QStringList proj_details;
                foreach(const QString& k, subObj.keys()) {
                    QJsonValue val = subObj.value(k);
                    qInfo() << "Key = " << k << ", Value = " << val;
                    if(k == "name"){
                        proj_details << val.toString();
                        }
                    if(k=="is_admin"){
                        if(val==true){
                            proj_details << "Admin";
                            }
                        else{
                            proj_details << "Annotator";
                            }
                        }
                    }
                ui->projectsTable->addItem(proj_details.join(" - "));
//               QListWidgetItem *new_item = ui->projectsTable->item(i);
               connect(ui->projectsTable, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(openProject(QListWidgetItem *)));
            }
            else{
                qInfo() << "The returned object does not satisfy the requirements";
                qInfo() << project;
            }
        }

//    for item in list:
//      QListWidget::itemClicked()

}

void ProjectList::openProject(QListWidgetItem *project_name){
    qInfo() << project_name;
    qInfo() << "inside open a new project";
}
