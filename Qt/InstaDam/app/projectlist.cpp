#include "projectlist.h"

#include "ui_projectlist.h"
#include "ui_newproject.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>

#include "instadam.h"
#include "newproject.h"
#include "serverprojectname.h"
#include "imagelist.h"
/*!
  \class ProjectList
  \ingroup app
  \inmodule InstaDam
  \inherits QWidget
  \brief A list of projects.
  */

/*!
  Creates a ProjectList with parent QWidget \a parent, if any.
  */
ProjectList::ProjectList(QWidget *parent) :
    QWidget(parent), ui(new Ui::ProjectList) {
    ui->setupUi(this);
}

/*!
  Destructor.
  */
ProjectList::~ProjectList() {
    delete ui;
}

/*!
  Adds Projects to this object based on the input \a obj, \a databaseURL,
  and \a accessToken.
  */
void ProjectList::addItems(QJsonDocument obj, QString databaseURL, QString accessToken) {
    this->databaseURL = databaseURL;
    this->accessToken = accessToken;
    QJsonArray projects_list = obj.array();
    for(int i =0; i<projects_list.count();i++){
        QJsonValue project = projects_list.at(i);
            if(project.isObject()){
                QJsonObject subObj = project.toObject();
                QStringList proj_details;
                foreach(const QString& k, subObj.keys()) { // fix the insertions inside the list based on final version of the received json
                    QJsonValue val = subObj.value(k);
                    if(k == "id"){
                        proj_details << QString::number(val.toInt());
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
               if(this->useCase=="OPEN"){
                    connect(ui->projectsTable, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(openProject(QListWidgetItem *)));
                }
               else if (this->useCase=="DELETE") {
                   connect(ui->projectsTable, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(deleteProject(QListWidgetItem *)));
                }
            }
    }
}

/*!
  Opens the Project based on \a project_name.
  */
void ProjectList::openProject(QListWidgetItem *project_name) {
    QString id = QString(project_name->text().split('-')[0]);
    id.replace(" ", "");
    selectedProject = id.toInt();
    QString databaseGetProjectURL = this->databaseURL+"/project/"+id+"/labels";
    QUrl dabaseLink = QUrl(databaseGetProjectURL);
    QNetworkRequest req = QNetworkRequest(dabaseLink);
    QString loginToken = "Bearer "+this->accessToken.replace("\"", "");
    req.setRawHeader(QByteArray("Authorization"), loginToken.QString::toUtf8());
    rep = manager->get(req);
    connect(rep, &QNetworkReply::finished,
            this, &ProjectList::getLabelsReplyFinished);
}

/*!
  Waits until a reply regarding the labels request is received
  emits the received labels to InstaDam
  */
void ProjectList::getLabelsReplyFinished() {
    emit instadamClearAll();
    QByteArray strReply = rep->readAll();
    QJsonParseError jsonError;
    QJsonDocument jsonReply = QJsonDocument::fromJson(strReply, &jsonError);
    if (jsonError.error == QJsonParseError::NoError) {
      QJsonObject jsonLabels = jsonReply.object();
      emit projectJsonReceived(jsonLabels);
      emit projectIdChanged(selectedProject);
      this->close();
      }
}

/*!
  Sends a project deletion request
 */
void ProjectList::deleteProject(QListWidgetItem *project_name) {
    QString id = QString(project_name->text().split('-')[0]);
    id.replace(" ", "");
    selectedProject = id.toInt();
    QString databaseGetProjectURL = this->databaseURL+"/project/"+id;
    QUrl dabaseLink = QUrl(databaseGetProjectURL);
    QNetworkRequest req = QNetworkRequest(dabaseLink);
    QString loginToken = "Bearer "+this->accessToken.replace("\"", "");
    req.setRawHeader(QByteArray("Authorization"), loginToken.QString::toUtf8());
    rep = manager->deleteResource(req);
    connect(rep, &QNetworkReply::finished,
            this, &ProjectList::deleteReplyFinished);
}

/*!
 Receives the reply of deleting a project from the server
 */
void ProjectList::deleteReplyFinished() {
    qInfo() << "reply received:";
    QByteArray strReply = rep->readAll();
    QJsonParseError jsonError;
    QJsonDocument jsonReply = QJsonDocument::fromJson(strReply, &jsonError); // parse and capture the error flag
    QMessageBox msgBox;
    if (jsonError.error != QJsonParseError::NoError) {
        QString message = jsonReply.object().value("msg").toString();
        if(message!=""){
            msgBox.setText(message);
            msgBox.exec();
        }
    }  else {
            this->hide();
            msgBox.setText("The project has been successfully deleted");
            msgBox.exec();
            emit projectDeleted(selectedProject);
      }
}
