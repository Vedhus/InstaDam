#include "addusertoproject.h"
#include "userprivilege.h"
#include "ui_addusertoproject.h"
#include "ui_userprivilege.h"

#include <QDebug>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QJsonDocument>
#include <QUrlQuery>
#include <QJsonArray>
#include <QMessageBox>

/*!
  \class AddUserToProject
  \ingroup app
  \inmodule InstaDam
  \inherits QWidget
  \brief The AddUserToProject class provides an interface for adding a user to a project.
*/

/*!
  Constructs an instance of AddUserToProject with parent \a parent, if any.
*/
AddUserToProject::AddUserToProject(QWidget *parent) :
    QWidget(parent), ui(new Ui::AddUserToProject) {
    ui->setupUi(this);
}

/*!
  Destructor
*/
AddUserToProject::~AddUserToProject() {
    delete ui;
}

/*!
  Reads the user query and sends a request to receive the search results from
  the server.
*/
void AddUserToProject::on_pushButton_clicked() {
    QString userInfo = ui->userInfoInput->toPlainText();
    QString databaseLoginURL = this->databaseURL+"/users/search?q=illinois";
    QUrl databaseLink = QUrl(databaseLoginURL);
    QUrlQuery query;
    query.addQueryItem("q", userInfo);
    databaseLink.setQuery(query.query());
    QNetworkRequest req = QNetworkRequest(databaseLink);
    QString loginToken = "Bearer "+this->accessToken.replace("\"", "");
    req.setRawHeader(QByteArray("Authorization"), loginToken.QString::toUtf8());
    rep = manager->get(req);
    connect(rep, &QNetworkReply::finished,
            this, &AddUserToProject::replyFinished);
}

/*!
  Processes the "Cancel" Button.
*/
void AddUserToProject::on_pushButton_2_clicked() {
    this->hide();
}

/*!
    Receives the reply for the search request.
*/
void AddUserToProject::replyFinished() {
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
    } else {
        QJsonObject obj = jsonReply.object();
        qInfo() << obj;
        this->listUsers(obj);
    }
}

/*!
  Lists all the users that share a similarity with the search query as received
  from server as \a obj.
*/
void AddUserToProject::listUsers(QJsonObject obj) {
    ui->userList->clear();
    QJsonArray users_list = obj.value("users").toArray();
    for (int i =0; i<users_list.count();i++) {
        QJsonObject user = users_list.at(i).toObject();
        QStringList user_details;
        foreach(const QString& k, user.keys()) {
            QJsonValue val = user.value(k);
            if (k == "username") {
                user_details << val.toString();
            }
            if (k == "email") {
                user_details << val.toString();
            }
        }
        ui->userList->addItem(user_details.join(" - "));
    }
}

/*!
    Adds a user to the project
    (at this stage: it's the same whether the user is in the
    project or not)
*/
void AddUserToProject::on_addToProject_clicked() {
    this->userInProject = false;
    this->updateUser();
}

/*!
    Updates the privilege of an existing user in the project
    (at this stage: it's the same whether the user is in the
    project or not)
*/
void AddUserToProject::on_updatePrivilege_clicked() {
    this->userInProject = true;
    this->updateUser();
}

/*!
    Displays a widget to allow the user to select new user privilege and waits for respond
*/
void AddUserToProject::updateUser() {
    if (ui->userList->selectedItems().size()!= 0) {
        QListWidgetItem *item = ui->userList->selectedItems()[0];
        this->userDetails = item->text();
        userPrivilege->show();
        connect(userPrivilege, SIGNAL(on_pushButton_clicked()), this, SLOT(addAsAdmin()));
        connect(userPrivilege, SIGNAL(on_pushButton_2_clicked()), this, SLOT(addAsAnnotator()));
    } else {
        QMessageBox msgBox;
        msgBox.setText("Please search and select a user first");
        msgBox.exec();
    }
}

/*!
    Adds a user to a project as annotator
*/
void AddUserToProject::addAsAnnotator() {
    this->privilege = "r";
    this->add();
    userPrivilege->hide();
}

/*!
    Adds a user to a project as admin
*/
void AddUserToProject::addAsAdmin() {
    this->privilege = "rw";
    this->add();
    userPrivilege->hide();
}
/*!
  Sends the request to update the user privielge and waits for a reponse
*/
void AddUserToProject::add(){
    QString userName = QString(this->userDetails.split('-')[1]).replace(" ", "");
    QString privilege = this->privilege.toLower();
    QString databaseLoginURL = this->databaseURL+ "/project/"+QString::number(this->projectId)+"/permissions";
    QUrl dabaseLink = QUrl(databaseLoginURL);
    QJsonObject js
    {
        {"access_type", privilege},
        {"username", userName}
    };
    QJsonDocument doc(js);
    QByteArray bytes = doc.toJson();
    QNetworkRequest req = QNetworkRequest(dabaseLink);
    QString loginToken = "Bearer "+this->accessToken.replace("\"", "");
    req.setRawHeader(QByteArray("Authorization"), loginToken.QString::toUtf8());
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    rep = manager->put(req, bytes);
    connect(rep, &QNetworkReply::finished,
            this, &AddUserToProject::privilegeReplyFinished);
}

/*!
    Receives the update user privilege response from backend.
*/
void AddUserToProject::privilegeReplyFinished() {
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
    } else {
        msgBox.setText("User privileges were updated successfully");
        msgBox.exec();
    }
}
