#include "register.h"

#include <QDebug>
#include <QFile>
#include <QBuffer>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include "instadam.h"
#include "ui_instadam.h"
#include "login.h"
#include "ui_register.h"
#include "../Selector/jsonConstants.h"

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
    manager = new QNetworkAccessManager(this);
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
void Register::on_cancelButton_clicked() {
    Login *log = new Login;
    log->show();
    hide();
}

/*!
  Sends a request to register a new user to the database.
  */
void Register::on_registerButton_clicked() {
    QString em = ui->email->text();
    QString user = ui->username->text();
    QString pass = ui->password->text();
    this->databaseURL = ui->url->text();
    QString databaseRegisterURL = this->databaseURL+"/register";
    QUrl dabaseLink = QUrl(databaseRegisterURL);
    QJsonObject js
    {
        {"email", em},
        {InstaDamJson::USERNAME, user},
        {InstaDamJson::PASSWORD, pass}
    };
    QJsonDocument doc(js);
    bytes = doc.toJson(QJsonDocument::Compact);
    QNetworkRequest req = QNetworkRequest(dabaseLink);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    rep = manager->post(req, bytes);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFin(QNetworkReply*)));
}

void Register::replyFin(QNetworkReply* reply){
    rep = reply;
    replyFinished();
}
/*!
  Receives the reply regarding new user registration.
*/
void Register::replyFinished() {
    QByteArray strReply = rep->readAll();
    QString ss = QString(strReply);
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
            this->accessToken = obj.value(InstaDamJson::ACCESS_TOKEN).toString();
            this->launchMainInstadam();
        } else {
            msgBox.setText(obj.value("msg").toString());
            msgBox.exec();
        }
    }
}

/*!
  Lunches the main InstaDam window.
*/
void Register::launchMainInstadam(){
    InstaDam *instadamWindow = new InstaDam(nullptr, this->databaseURL, this->accessToken);
    instadamWindow->show();
    hide();
}
