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
    qInfo() << obj;
    for (int i =0; i<projects_list.count();i++) {
        QJsonValue project = projects_list.at(i);
        if (project.isObject()) {
            QJsonObject subObj = project.toObject();
            QStringList proj_details;
            foreach(const QString& k, subObj.keys()) { // fix the insertions inside the list based on final version of the received json
                QJsonValue val = subObj.value(k);
                if (k == InstaDamJson::ID) {
                    proj_details << QString::number(val.toInt());
                }
                if (k == InstaDamJson::NAME) {
                    proj_details << val.toString();
                }
                if (k == InstaDamJson::IS_ADMIN) {
                    if (val == true) {
                        proj_details << "Admin";
                    } else {
                        proj_details << "Annotator";
                    }
                }
            }
            ui->projectsTable->addItem(proj_details.join(" - "));
            connect(ui->projectsTable, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(openProject(QListWidgetItem *)));
        } else {
            qInfo() << "The returned object does not satisfy the requirements";
            qInfo() << project;
        }
    }
}

/*!
  Opens the Project based on \a project_name.
  */
void ProjectList::openProject(QListWidgetItem *project_name) {
    qInfo() << "inside open a new project" << project_name->text();
    QString id = QString(project_name->text().split('-')[0]);
    selectedProject = id.toInt();
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

/*!
  Something.
  */
void ProjectList::getLabelsReplyFinished() {
    qInfo() << "reply received:";
    QByteArray strReply = rep->readAll();
    QJsonParseError jsonError;
    QJsonDocument jsonReply = QJsonDocument::fromJson(strReply, &jsonError); // parse and capture the error flag

    if (jsonError.error != QJsonParseError::NoError) {
        qInfo() << "Error: " << jsonError.errorString();
    }
    else {
        qInfo() << jsonReply;
        InstaDam *instadamWindow = new InstaDam(nullptr, this->databaseURL, this->accessToken);
        instadamWindow->show();
        hide();
        QJsonObject jsonLabels = jsonReply.object();
        instadamWindow->loadLabelJson(jsonLabels, PROJECT);
        instadamWindow->setCurrentProjectId(selectedProject);
    }
}

void ProjectList::on_pushButton_clicked()
{
    serverProjectName *spn = new serverProjectName();
    spn->databaseURL = this->databaseURL;
    spn->accessToken = this->accessToken;
    spn->show();
    hide();
}


