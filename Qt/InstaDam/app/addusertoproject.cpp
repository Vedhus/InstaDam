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
  Called when button is clicked.
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
  Called when button is clicked.
*/
void AddUserToProject::on_pushButton_2_clicked() {
    this->hide();
}

/*!
  Called when the http response is received.
*/
void AddUserToProject::replyFinished() {
    QByteArray strReply = rep->readAll();
    QJsonParseError jsonError;
    QJsonDocument jsonReply = QJsonDocument::fromJson(strReply, &jsonError); // parse and capture the error flag

    if (jsonError.error != QJsonParseError::NoError) {
        qInfo() << "Error: " << jsonError.errorString();
    } else {
        QJsonObject obj = jsonReply.object();
        qInfo() << obj;
        this->listUsers(obj);
    }
}

/*!
  Lists the users given in \a obj.
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
  Called when button is clicked.
*/
void AddUserToProject::on_addToProject_clicked() {
    this->userInProject = false;
    this->updateUser();
}

/*!
  Called when Update Privliges is clicked.
*/
void AddUserToProject::on_updatePrivilege_clicked() {
    this->userInProject = true;
    this->updateUser();
}

/*!
  Updates the user data in the database.
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
  Adds the annotator role to the user.
*/
void AddUserToProject::addAsAnnotator() {
    this->privilege = "r";
    this->add();
    userPrivilege->hide();
}

/*!
  Adds the admin role to the user.
*/
void AddUserToProject::addAsAdmin() {
    this->privilege = "rw";
    this->add();
    userPrivilege->hide();
}

/*!
  Adds a new user to the database.
*/
void AddUserToProject::add() {
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
  Called when a reply is received from a query update.
*/
void AddUserToProject::privilegeReplyFinished() {
    QByteArray strReply = rep->readAll();
    QJsonParseError jsonError;
    QJsonDocument jsonReply = QJsonDocument::fromJson(strReply, &jsonError); // parse and capture the error flag
    QMessageBox msgBox;
    if (jsonError.error != QJsonParseError::NoError) {
        msgBox.setText("Ooops! Network error, please try again");
        msgBox.exec();
    } else {
        msgBox.setText("User privileges were updated successfully");
        msgBox.exec();
    }
}
