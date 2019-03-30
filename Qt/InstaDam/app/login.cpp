#include "login.h"
#include "ui_login.h"
#include "register.h"
#include "instadam.h"
#include "startingwidget.h"
#include "projectlist.h"
#include <QDebug>
#include <QFile>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>

Login::Login(QWidget *parent) :
    QWidget(parent),
   ui(new Ui::Login)
{
    ui->setupUi(this);
}


Login::~Login()
{
    delete ui;
}

void Login::on_pushButton_3_clicked()
{
    qInfo() << "registering a new user";
    Register *reg = new Register;
    reg->show();
    hide();
}

void Login::on_pushButton_clicked()
{
    QString user = ui->username->toPlainText();
    QString pass = ui->password->toPlainText();
    this->databaseURL = ui->url->toPlainText();
    QString databaseLoginURL = this->databaseURL+"/login";

    QUrl dabaseLink = QUrl(databaseLoginURL);

    QJsonObject js
    {
        {"username", user},
        {"password", pass}
    };

    qInfo() << databaseLoginURL;
    qInfo() << js;

    QJsonDocument doc(js);
    QByteArray bytes = doc.toJson();
    QNetworkRequest req = QNetworkRequest(dabaseLink);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    rep = manager->post(req, bytes);

    connect(rep, &QNetworkReply::finished,
            this, &Login::replyFinished);

    qInfo() << "waiting for the reply...";
}

void Login::replyFinished()
{
    qInfo() << "reply received:";
    QByteArray strReply = rep->readAll();
    QJsonParseError jsonError;
    QJsonDocument jsonReply = QJsonDocument::fromJson(strReply, &jsonError); // parse and capture the error flag

      if(jsonError.error != QJsonParseError::NoError){
            qInfo() << "Error: " << jsonError.errorString();
      }

      else{
          QJsonObject obj = jsonReply.object();
          if(obj.contains("access_token")){
              this->accessToken = obj.value("access_token").toString();
              qInfo() << this->accessToken;
              Login::dumpToken();
              Login::listProjects();
              /////////////////////////
              /// move the following code to after the project is picked
//              InstaDam *w = new InstaDam;
//              w->show();
//              hide();
              /////////////////////////
        }
         else{
              qInfo() << obj;
          }
      }
}

void Login::projectsReplyFinished()
{
    qInfo() << "reply received:";
    QByteArray strReply = rep->readAll();
    QJsonParseError jsonError;
    QJsonDocument jsonReply = QJsonDocument::fromJson(strReply, &jsonError); // parse and capture the error flag

      if(jsonError.error != QJsonParseError::NoError){
            qInfo() << "Error: " << jsonError.errorString();
      }

      else{
//          QJsonObject obj = jsonReply.object();
//          qInfo()<< "jsonReply:";
//          qInfo()<< jsonReply;
//          qInfo()<< "obj:";
//          qInfo()<< obj;
          ProjectList *pl = new ProjectList;
          pl->show();
          pl->addItems(jsonReply);
          hide();
      }
}

void Login::listProjects(){
    QString databaseProjectsURL = this->databaseURL+"/projects";
    QUrl dabaseLink = QUrl(databaseProjectsURL);
    qInfo() << databaseProjectsURL;
    QNetworkRequest req = QNetworkRequest(dabaseLink);
    QString loginToken = "Bearer "+this->accessToken;
    qInfo() << loginToken;
    req.setRawHeader(QByteArray("Authorization"), loginToken.QString::toUtf8());
    rep = manager->get(req);

    connect(rep, &QNetworkReply::finished,
            this, &Login::projectsReplyFinished);

    qInfo() << "waiting for the reply...";


}
void Login::dumpToken(){
    QFile file("token.txt");
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream << this->accessToken<< endl;
    }
}


void Login::on_pushButton_2_clicked()
{
    close();
}

void Login::on_pushButton_4_clicked()
{
    qInfo() << "going back to the main widget";
    StartingWidget *wid = new StartingWidget;
    wid->show();
    hide();
}
