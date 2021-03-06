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
#ifdef WASM_BUILD
    ui->backButton->setEnabled(false);
#endif
}

/*!
  Destructor
*/
Login::~Login() {
    delete ui;
}

/*!
  Processes the "Register" button click.
*/
void Login::on_registerButton_clicked() {
    Register *reg = new Register;
    reg->show();
    hide();
}

/*!
  Processes the "Login" button click: Sends a request to login.
*/
void Login::on_loginButton_clicked() {
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
#ifdef WASM_BUILD
    connect(manager, SIGNAL(finished(QNetworkReply*)), this,
            SLOT(replyFin(QNetworkReply*)));
#else
    connect(rep, &QNetworkReply::finished,
            this, &Login::replyFinished);
#endif
}

#ifdef WASM_BUILD
void Login::replyFin(QNetworkReply* reply){
    rep = reply;
    replyFinished();
}
#endif

/*!
  Received the reply for Login.
*/
void Login::replyFinished() {
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
            this->accessToken = obj.value(InstaDamJson::ACCESS_TOKEN).toString().toUtf8();
            Login::launchMainInstadam();
        }
        else{
            msgBox.setText(obj.value("msg").toString());
            msgBox.exec();
        }
    }
}

/*!
  Lunches the main InstaDam Window after loginin in successfully.
*/
void Login::launchMainInstadam(){
    InstaDam *instadamWindow = new InstaDam(nullptr, this->databaseURL, this->accessToken);
    instadamWindow->show();
    hide();
}

/*!
  Processes the "Cancel" button click.
*/
void Login::on_exitButton_clicked() {
    this->close();
}

/*!
  Processes the "Back" button click: Goes back to the starting widget.
*/
void Login::on_backButton_clicked() {
    StartingWidget *startingWidget = new StartingWidget;
    startingWidget->show();
    hide();
}
