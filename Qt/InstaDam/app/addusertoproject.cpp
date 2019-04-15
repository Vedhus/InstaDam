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


AddUserToProject::AddUserToProject(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddUserToProject)
{
    ui->setupUi(this);
}

AddUserToProject::~AddUserToProject()
{
    delete ui;
}


void AddUserToProject::on_pushButton_clicked()
{
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

void AddUserToProject::on_pushButton_2_clicked()
{
    this->hide();
}

void AddUserToProject::replyFinished()
{
    QByteArray strReply = rep->readAll();
    QJsonParseError jsonError;
    QJsonDocument jsonReply = QJsonDocument::fromJson(strReply, &jsonError); // parse and capture the error flag

    if (jsonError.error != QJsonParseError::NoError) {
        qInfo() << "Error: " << jsonError.errorString();
        }
    else{
        QJsonObject obj = jsonReply.object();
        qInfo() << obj;
        this->listUsers(obj);
        }
}

void AddUserToProject::listUsers(QJsonObject obj)
{
    ui->userList->clear();
    QJsonArray users_list = obj.value("users").toArray();
    for(int i =0; i<users_list.count();i++){
        QJsonObject user = users_list.at(i).toObject();
        QStringList user_details;
        foreach(const QString& k, user.keys()) {
            QJsonValue val = user.value(k);
                if(k == "username"){
                    user_details << val.toString();
                }
                if(k == "email"){
                    user_details << val.toString();
                }
         }
         ui->userList->addItem(user_details.join(" - "));
    }
}

void AddUserToProject::on_addToProject_clicked()
{
    this->userInProject = false;
    this->updateUser();
}

void AddUserToProject::on_updatePrivilege_clicked()
{
    this->userInProject = true;
    this->updateUser();
}

void AddUserToProject::updateUser()
{
    QListWidgetItem *item = ui->userList->selectedItems()[0];
    this->userDetails = item->text();
    userPrivilege->show();
    connect(userPrivilege, SIGNAL(on_pushButton_clicked()), this, SLOT(addAsAdmin()));
    connect(userPrivilege, SIGNAL(on_pushButton_2_clicked()), this, SLOT(addAsAnnotator()));
}

void AddUserToProject::addAsAnnotator(){
    this->privilege = "r";
    this->add();
    userPrivilege->hide();
}

void AddUserToProject::addAsAdmin(){
    this->privilege = "rw";
    this->add();
    userPrivilege->hide();
}

void AddUserToProject::add(){
        QString userName = QString(this->userDetails.split('-')[1]).replace(" ", "");
        QString privilege = this->privilege.toLower();

        QString databaseLoginURL = this->databaseURL+ "/project/"+QString::number(this->projectId)+"/permissions";
//        QString databaseLoginURL = this->databaseURL+"/user/privilege/";
        QUrl dabaseLink = QUrl(databaseLoginURL);
//        qInfo() << databaseLoginURL;

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

void AddUserToProject::privilegeReplyFinished(){
    QByteArray strReply = rep->readAll();
    QJsonParseError jsonError;
    QJsonDocument jsonReply = QJsonDocument::fromJson(strReply, &jsonError); // parse and capture the error flag
    if (jsonError.error != QJsonParseError::NoError) {
        qInfo() << "Error: " << jsonError.errorString();
    } else {
        QJsonObject obj = jsonReply.object();
        qInfo() << obj;
        QMessageBox msgBox;
        msgBox.setText("User privileges were updated successfully");
        msgBox.exec();
    }
}
