#include "projectlist.h"
#include "ui_projectlist.h"
#include "instadam.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>

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

void ProjectList::addItems(QJsonDocument obj, QString databaseURL, QString accessToken){
    this->databaseURL = databaseURL;
    this->accessToken = accessToken;
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
                    if(k == "id"){
                        proj_details << val.toString();
                    }
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
               connect(ui->projectsTable, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(openProject(QListWidgetItem *)));
            }
            else{
                qInfo() << "The returned object does not satisfy the requirements";
                qInfo() << project;
            }
        }


}

void ProjectList::openProject(QListWidgetItem *project_name){
    qInfo() << "inside open a new project" << project_name->text();
    QString id="1";
    QString databaseGetProjectURL = this->databaseURL+"/project/"+id+"/labels";
    QUrl dabaseLink = QUrl(databaseGetProjectURL);
    qInfo() << databaseGetProjectURL;
    QNetworkRequest req = QNetworkRequest(dabaseLink);
    QString loginToken = "Bearer "+this->accessToken;
    req.setRawHeader(QByteArray("Authorization"), loginToken.QString::toUtf8());
    rep = manager->get(req);

    connect(rep, &QNetworkReply::finished,
            this, &ProjectList::getLabelsReplyFinished);


}

void ProjectList::getLabelsReplyFinished()
{
    qInfo() << "reply received:";
    QByteArray strReply = rep->readAll();
    QJsonParseError jsonError;
    QJsonDocument jsonReply = QJsonDocument::fromJson(strReply, &jsonError); // parse and capture the error flag

      if(jsonError.error != QJsonParseError::NoError){
            qInfo() << "Error: " << jsonError.errorString();
      }

      else{
            qInfo() << jsonReply;
              InstaDam *w = new InstaDam;
              w->show();
              hide();
              Project newPr = Project();

              QJsonObject labels = jsonReply.object();
              foreach(const QString& k, labels.keys()) {
                  if(k=="labels"){
                    QJsonValue labels_values = labels.value(k);
//                    if(labels_values.isObject()){
                       QJsonArray labels_values_list = labels_values.toArray();
                       for(int i=0;i<labels_values_list.count();i++){
                           QJsonValue labelValue = labels_values_list.at(i);
                               if(labelValue.isObject()){
                                   QJsonObject label = labelValue.toObject();
                                   Label lb = Label(label, label.value("id").toInt());
                                   Label *lb_ = &lb;
                                   QSharedPointer<Label> LB = QSharedPointer<Label>(lb_);
                                   newPr.addLabel(LB);
                               }
                        }
//                    }
                  }
              }

              w->setCurrentProject(newPr);
              w->setLabels();
      }
}
