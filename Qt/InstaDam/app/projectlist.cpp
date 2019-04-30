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
#include "projectdeletionconfirmation.h"

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
    QDialog(parent), ui(new Ui::ProjectList) {
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
    for (int i = 0; i < projects_list.count(); i++) {
        QJsonValue project = projects_list.at(i);
        if (project.isObject()) {
            QJsonObject subObj = project.toObject();
            QStringList proj_details;
            foreach(const QString& k, subObj.keys()) {
                QJsonValue val = subObj.value(k);
                if (k == InstaDamJson::ID) {
                    proj_details << QString::number(val.toInt());
                }
                if (k == InstaDamJson::NAME) {
                    proj_details << val.toString();
                }
                if (k == InstaDamJson::IS_ADMIN) {
                    if (val==true) {
                        proj_details << "Admin";
                    } else {
                        proj_details << "Annotator";
                    }
                }
            }
            ui->projectsTable->addItem(proj_details.join(" - "));
        }
    }
    if (this->useCase == InstaDamJson::OPEN) {
         connect(ui->projectsTable, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(openProject(QListWidgetItem *)));
    } else if (this->useCase == InstaDamJson::IDELETE) {
        connect(ui->projectsTable, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(confirmProjectDeletion(QListWidgetItem *)));
    }
}

/*!
  Starts a widget to confirm project deletion of \a project_name.
  */
void ProjectList::confirmProjectDeletion(QListWidgetItem *project_name){
    projectDeletionConfirmation *confirmation = new projectDeletionConfirmation();
    confirmation->project_name = project_name;
    connect(confirmation, SIGNAL(projectDeleted(QListWidgetItem *)), this,
            SLOT(deleteProject(QListWidgetItem *)));
    confirmation->show();
}

/*!
  Slot triggered to open the Project based on \a project_name.
*/
void ProjectList::openProject(QListWidgetItem *project_name) {
    QString id = QString(project_name->text().split('-')[0]);
    id.replace(" ", "");
    selectedProject = id.toInt();
    QString databaseGetProjectURL = this->databaseURL + "/" +
            InstaDamJson::PROJECT + "/" + id + "/" + InstaDamJson::LABELS;
    QUrl dabaseLink = QUrl(databaseGetProjectURL);
    QNetworkRequest req = QNetworkRequest(dabaseLink);
    QString loginToken = InstaDamJson::BEARER + this->accessToken.replace("\"", "");
    req.setRawHeader(InstaDamJson::AUTHORIZATION, loginToken.QString::toUtf8());
    rep = manager->get(req);
#ifdef WASM_BUILD
    connect(manager, SIGNAL(finished(QNetworkReply*)), this,
            SLOT(getLabelsReplyFin(QNetworkReply*)));
#else
    connect(rep, &QNetworkReply::finished,
            this, &ProjectList::getLabelsReplyFinished);
#endif
}

#ifdef WASM_BUILD
void ProjectList::getLabelsReplyFin(QNetworkReply* reply){
    rep = reply;
    getLabelsReplyFinished();
}
#endif

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
  Sends a project deletion request for \a project_name.
*/
void ProjectList::deleteProject(QListWidgetItem *project_name) {
    QString id = QString(project_name->text().split('-')[0]);
    id.replace(" ", "");
    selectedProject = id.toInt();
    QString databaseGetProjectURL = this->databaseURL + "/" +
            InstaDamJson::PROJECT + "/" + id;
    QUrl dabaseLink = QUrl(databaseGetProjectURL);
    QNetworkRequest req = QNetworkRequest(dabaseLink);
    QString loginToken = InstaDamJson::BEARER + this->accessToken.replace("\"", "");
    req.setRawHeader(InstaDamJson::AUTHORIZATION, loginToken.QString::toUtf8());
    rep = manager->deleteResource(req);
#ifdef WASM_BUILD
    connect(manager, SIGNAL(finished(QNetworkReply*)), this,
            SLOT(deleteReplyFin(QNetworkReply*)));
#else
    connect(rep, &QNetworkReply::finished,
            this, &ProjectList::deleteReplyFinished);
#endif
}

#ifdef WASM_BUILD
void ProjectList::deleteReplyFin(QNetworkReply* reply){
    rep = reply;
    deleteReplyFinished();
}
#endif

/*!
 Receives the reply of deleting a project from the server
*/
void ProjectList::deleteReplyFinished() {
    qInfo() << "reply received:";
    QByteArray strReply = rep->readAll();
    QJsonParseError jsonError;
    QJsonDocument jsonReply = QJsonDocument::fromJson(strReply, &jsonError);
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

/*!
  \fn void ProjectList::projectJsonReceived(QJsonObject json)

  This signal is emitted when a new project Json object, \a json, is received.
*/

/*!
  \fn void ProjectList::projectIdChanged(int id)

  This signal is emitted when a project id is changed to \a id.
*/

/*!
  \fn void ProjectList::instadamClearAll()

  This signal is emitted when a project list is cleared.
*/

/*!
  \fn void ProjectList::projectDeleted(int id)
  Signal sent when a project, \a id,  is deleted.
*/

