#include "register.h"
#include "login.h"
#include "ui_register.h"
#include <QDebug>
#include <QFile>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>

Register::Register(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
}

Register::~Register()
{
    delete ui;
}

void Register::on_pushButton_2_clicked()
{
    qInfo() << "Cancelling and going back to the login window";
    Login *log = new Login;
    log->show();
    hide();
}

void Register::on_pushButton_clicked()
{
    QString em = ui->email->toPlainText();
    QString user = ui->username->toPlainText();
    QString pass = ui->password->toPlainText();
    this->databaseURL = ui->url->toPlainText();
    QString databaseRegisterURL = this->databaseURL+"/register";

    QUrl dabaseLink = QUrl(databaseRegisterURL);

    QJsonObject js
    {
        {"email", em},
        {"username", user},
        {"password", pass}
    };

    qInfo() << databaseRegisterURL;
    qInfo() << js;

    QJsonDocument doc(js);
    QByteArray bytes = doc.toJson();
    QNetworkRequest req = QNetworkRequest(dabaseLink);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    rep = manager->post(req, bytes);

    connect(rep, &QNetworkReply::finished,
            this, &Register::replyFinished);

    qInfo() << "waiting for the reply...";
}

void Register::replyFinished()
{
    qInfo() << "reply received:";
    QByteArray strReply = rep->readAll();
//    qInfo() << strReply;

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
              Register::dumpToken();
        }
         else{
              qInfo() << obj;
          }
      }
}

void Register::projectsReplyFinished()
{
    qInfo() << "reply received:";
    QByteArray strReply = rep->readAll();
//    qInfo() << strReply;

    QJsonParseError jsonError;

    QJsonDocument jsonReply = QJsonDocument::fromJson(strReply, &jsonError); // parse and capture the error flag

      if(jsonError.error != QJsonParseError::NoError){
            qInfo() << "Error: " << jsonError.errorString();
      }

      else{
          QJsonObject obj = jsonReply.object();
          qInfo()<< "implement a function to read the returned object";

      }
}

void Register::dumpToken(){
    QFile file("token.txt");
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream << this->accessToken << endl;
    }
}
