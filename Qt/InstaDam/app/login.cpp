#include "login.h"
#include <QDebug>
#include <QFile>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>

#include "ui_login.h"
#include "register.h"
#include "instadam.h"
#include "startingwidget.h"
#include "projectlist.h"

/*!
  \class Login
  \ingroup app
  \inmodule InstaDam
  \inherits QWidget
  \brief Creates a login dialog.
  */

/*!
  Creates an instance with parent QWidget \a parent, if any.
  */
Login::Login(QWidget *parent) :
    QWidget(parent), ui(new Ui::Login) {
    ui->setupUi(this);
}

/*!
  Destructor
  */
Login::~Login() {
    delete ui;
}

/*!
  Processes the X button click.
  */
void Login::on_pushButton_3_clicked() {
    Register *reg = new Register;
    reg->show();
    hide();
}

/*!
  Sends a request to login
  */
void Login::on_pushButton_clicked() {
    QString user = ui->username->text();
    QString pass = ui->password->text();
    this->databaseURL = ui->url->text();
    QString databaseLoginURL = this->databaseURL+"/login";
    QUrl dabaseLink = QUrl(databaseLoginURL);
    QJsonObject js
    {
        {InstaDamJson::USERNAME, user},
        {InstaDamJson::PASSWORD, pass}
    };
    QJsonDocument doc(js);
    QByteArray bytes = doc.toJson();
    QNetworkRequest req = QNetworkRequest(dabaseLink);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    rep = manager->post(req, bytes);
    connect(rep, &QNetworkReply::finished,
            this, &Login::replyFinished);
}

/*!
  Received the reply for login
  */
void Login::replyFinished() {
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
        if (obj.contains(InstaDamJson::ACCESS_TOKEN)) {
            this->accessToken = obj.value(InstaDamJson::ACCESS_TOKEN).toString().toUtf8();
            Login::lunchMainInstadam();
        }
        else{
            msgBox.setText(obj.value("msg").toString());
            msgBox.exec();
        }
    }
}

/*!
  Lists the projects.
  */

void Login::
lunchMainInstadam(){
    InstaDam *instadamWindow = new InstaDam(nullptr, this->databaseURL, this->accessToken);
    instadamWindow->show();
    hide();
}


/*!
  Processes Z button click.
  */
void Login::on_pushButton_2_clicked() {
    this->close();
}

/*!
  Goes back to the starting widget when the button is clicked
  */
void Login::on_pushButton_4_clicked() {
    StartingWidget *startingWidget = new StartingWidget;
    startingWidget->show();
    hide();
}
