#include "register.h"

#include <QDebug>
#include <QFile>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include "instadam.h"
#include "ui_instadam.h"
#include "login.h"
#include "ui_register.h"

/*!
  \class Register
  \ingroup app
  \inmodule InstaDam
  \inherits QWidget
  \brief Registers a user.
  */

/*!
  Creates a Register instance with parent QWidget \a parent.
  */
Register::Register(QWidget *parent) :
    QWidget(parent), ui(new Ui::Register) {
    ui->setupUi(this);
}

/*!
  Destructor
  */
Register::~Register() {
    delete ui;
}

/*!
  Responds to the cancel button being clicked.
  */
void Register::on_pushButton_2_clicked() {
    Login *log = new Login;
    log->show();
    hide();
}

/*!
  Sends a request to register a new user to the database.
  */
void Register::on_pushButton_clicked() {
    QString em = ui->email->text();
    QString user = ui->username->text();
    QString pass = ui->password->text();
    this->databaseURL = ui->url->text();
    QString databaseRegisterURL = this->databaseURL+"/register";
    QUrl dabaseLink = QUrl(databaseRegisterURL);
    QJsonObject js
    {
        {"email", em},
        {"username", user},
        {"password", pass}
    };
    QJsonDocument doc(js);
    QByteArray bytes = doc.toJson();
    QNetworkRequest req = QNetworkRequest(dabaseLink);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    rep = manager->post(req, bytes);
    connect(rep, &QNetworkReply::finished,
            this, &Register::replyFinished);
}

/*!
  Receives the reply regarding new user registration.
  */
void Register::replyFinished() {
    QByteArray strReply = rep->readAll();
    QJsonParseError jsonError;
    QJsonDocument jsonReply = QJsonDocument::fromJson(strReply, &jsonError); // parse and capture the error flag
    QMessageBox msgBox;
    if (jsonError.error != QJsonParseError::NoError) {
        msgBox.setText("Ooops! Network error, please double check your URL and try again");
        msgBox.exec();
    } else {
        QJsonObject obj = jsonReply.object();
        if (obj.contains("access_token")) {
            this->accessToken = obj.value("access_token").toString();
            this->lunchMainInstadam();
        } else {
            msgBox.setText(obj.value("msg").toString());
            msgBox.exec();
        }
    }
}

/*!
  Lunches the main InstaDam window.
  */
void Register::lunchMainInstadam(){
    InstaDam *instadamWindow = new InstaDam(nullptr, this->databaseURL, this->accessToken);
    instadamWindow->show();
    hide();
}
