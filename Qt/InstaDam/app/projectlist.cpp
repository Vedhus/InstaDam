#include "projectlist.h"
#include "ui_projectlist.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include "imagelist.h"

ProjectList::ProjectList(QWidget *parent, QString databaseUrl, QByteArray token) :
    QWidget(parent),
    ui(new Ui::ProjectList)
{
    ui->setupUi(this);
    access_token = token;
    this->databaseURL = databaseUrl;
}

ProjectList::~ProjectList()
{
    delete ui;
}

void ProjectList::addItems(QJsonObject obj){
//    QList<QJsonObject> list;
    foreach(const QString& key, obj.keys()) {
            QJsonValue value = obj.value(key);
            if(value.isObject()){
                QJsonObject subObj = value.toObject();
                    foreach(const QString& k, subObj.keys()) {
                        QJsonValue val = subObj.value(k);
                        qInfo() << "Key = " << k << ", Value = " << val.toString();
                    }
            }
            else{
                qInfo() << "The returned object does not satisfy the requirements";
                qInfo() << "Server Message: Key = " << key << ", Value = " << value.toString();
            }
        }
}




void ProjectList::imagesReplyFinished()
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
          ImageList* il = new ImageList(nullptr, this->databaseURL, this->access_token);
          il->show();
          il->addItems(obj);
      }
}


void ProjectList::listImages(/*there will be a paramter for a specific project later, using project id=1 for now*/){
    QString databaseImagesURL = this->databaseURL+"/projects/1/images";
    QUrl dabaseLink = QUrl(databaseImagesURL);

    qInfo() << databaseImagesURL;

    QNetworkRequest req = QNetworkRequest(dabaseLink);
    req.setRawHeader("Authorization", this->access_token);
   // debugRequest(req);


    rep = manager->get(req);

    connect(rep, &QNetworkReply::finished,
            this, &ProjectList::imagesReplyFinished);

    qInfo() << "waiting for the reply...";


}

void ProjectList::on_loadImagesButton_clicked()
{
    qInfo() << "load images button clicked";
    listImages();
}

