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
    qInfo() << "registering a new user";
    Register *reg = new Register;
    reg->show();
    hide();
}

/*!
  Processes the Y button click.
  */
void Login::on_pushButton_clicked() {
    QString user = ui->username->toPlainText();
    QString pass = ui->password->toPlainText();
    this->databaseURL = ui->url->toPlainText();
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

    qInfo() << "waiting for the reply...";
}

/*!
  Something
  */
void Login::replyFinished() {
    qInfo() << "reply received:";
    QByteArray strReply = rep->readAll();
    QJsonParseError jsonError;
    QJsonDocument jsonReply = QJsonDocument::fromJson(strReply, &jsonError); // parse and capture the error flag

    if (jsonError.error != QJsonParseError::NoError) {
        qInfo() << "Error: " << jsonError.errorString();
    } else {
        QJsonObject obj = jsonReply.object();
        if (obj.contains(InstaDamJson::ACCESS_TOKEN)) {
            this->accessToken = obj.value(InstaDamJson::ACCESS_TOKEN).toString().toUtf8();
            //qInfo() << this->accessToken;
            Login::dumpToken();
            Login::listProjects();
        } else {
            qInfo() << obj;
        }
    }
}

/*!
  Something
  */
void Login::projectsReplyFinished() {
    qInfo() << "reply received:";
    QByteArray strReply = rep->readAll();
    QJsonParseError jsonError;
    QJsonDocument jsonReply = QJsonDocument::fromJson(strReply, &jsonError); // parse and capture the error flag

    if (jsonError.error != QJsonParseError::NoError) {
        qInfo() << "Error: " << jsonError.errorString();
    } else {
        QJsonObject obj = jsonReply.object();
        //qInfo()<< "implement a function to read the returned object";
        qInfo() << obj;
        ProjectList *pl = new ProjectList;
        pl->show();
        pl->addItems(jsonReply, this->databaseURL, this->accessToken);
        hide();
    }
}

void debugRequest(QNetworkRequest request) {
    qDebug() << request.url().toString();
    const QList<QByteArray>& rawHeaderList(request.rawHeaderList());
    foreach (QByteArray rawHeader, rawHeaderList) {
        qDebug() << request.rawHeader(rawHeader);
    }
}

/*!
  Lists the projects.
  */
void Login::listProjects() {
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


/*!
  Something
  */
void Login::dumpToken() {
    QFile file("token.txt");
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream << this->accessToken<< endl;
    }
}

/*!
  Processes Z button click.
  */
void Login::on_pushButton_2_clicked() {
    close();
}

/*!
  Processes W button click.
  */
void Login::on_pushButton_4_clicked() {
    qInfo() << "going back to the main widget";
    StartingWidget *wid = new StartingWidget;
    wid->show();
    hide();
}
