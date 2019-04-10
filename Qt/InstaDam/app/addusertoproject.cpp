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
#include <QjsonArray>

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

    qInfo() << "waiting for the reply...";
}

void AddUserToProject::on_pushButton_2_clicked()
{
    this->hide();
}

void AddUserToProject::replyFinished()
{
    qInfo() << "reply received:";
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
    qInfo() << "in listUsers";
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
    qInfo() << this->userDetails;
    UserPrivilege *userPrivilege = new UserPrivilege;
    userPrivilege->show();
    connect(userPrivilege, SIGNAL(on_pushButton_clicked()), this, SLOT(addAsAnnotator()));
    connect(userPrivilege, SIGNAL(on_pushButton_2_clicked()), this, SLOT(addAsAdmin()));
}

void AddUserToProject::addAsAnnotator(){
    this->privilege = "ANNOTATOR";
    this->add();
}

void AddUserToProject::addAsAdmin(){
    this->privilege = "ADMIN";
    this->add();
}

void AddUserToProject::add(){
    qInfo() << this->privilege;
    if(this->userInProject){
        qInfo() << "user in project";
    }
    else{
        qInfo() << "user NOT in project";
    }
}
