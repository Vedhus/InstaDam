#include "imagelist.h"
#include "ui_imagelist.h"
#include "instadam.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QUrlQuery>
#include <QPixmap>
#include <QByteArray>
#include <QtConcurrent/QtConcurrent>
#include "windows.h"

ImageList::ImageList(QWidget *parent, QString databaseUrl, QString token) :
    QWidget(parent),
    ui(new Ui::ImageList)
{
    ui->setupUi(this);
    accessToken = token;
    this->databaseURL = databaseUrl;
}

ImageList::~ImageList()
{
    delete ui;
}

void ImageList::getThumbnailReplyFinished()
{
    qInfo() << "got a thumbnailreply";

    QByteArray strReply = rep->readAll();
    qInfo() << rep->error();
    qInfo() << rep->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QJsonParseError jsonError;
    QJsonDocument jsonReply = QJsonDocument::fromJson(strReply, &jsonError); // parse and capture the error flag
    QJsonObject currentObject = jsonReply.object();
    if(jsonError.error != QJsonParseError::NoError){
        qInfo() << "Error: " << jsonError.errorString();
        return;
    }

    foreach(const QString& objkey, currentObject.keys()) //Everything except the thumbnail
    {
        QJsonValue currentValue = currentObject.value(objkey);
        //qInfo() << "Key = " << objkey << ", Value = " << currentObject.value(objkey);
        if(objkey.compare("base64_image")==0)
        {
            if(currentValue.isString())
            {
                QByteArray byteArray = QByteArray::fromBase64(currentValue.toString().toUtf8());
                QPixmap thumbnailPixmap;
                thumbnailPixmap.loadFromData(byteArray, currentObject.value("format").toString().toUtf8());
                QTableWidgetItem *thumbnail = new QTableWidgetItem;
                thumbnail->setData(Qt::DecorationRole, thumbnailPixmap);
                QTableWidget* table = ui->tableWidget;
                table->setItem(table->rowCount()-1, table->columnCount()-1, thumbnail);

            }
        }
    }
}



void ImageList::doRequest(QNetworkRequest req)
{
    rep = manager->get(req);

    connect(rep, &QNetworkReply::finished,
            this, &ImageList::getThumbnailReplyFinished);
    /* wait for reply because backend can't handle multiple requests at same time I think */
    QEventLoop loop;
    connect(rep, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

}


void ImageList::addItems(QJsonObject obj)
{
    qInfo() << obj;
    foreach(const QString& key, obj.keys()) {
        if(key.compare("project_images")!=0)
        {
            qInfo() << "incorrect key";
            return;
        }
        QJsonValue value= obj.value(key);
        if(value.isArray())
        {
            QJsonArray valueArray = value.toArray();
            qInfo() << valueArray;
            for(int imageNumber=0; imageNumber<valueArray.size(); imageNumber++)
            {
                qInfo() << "image: " << imageNumber << " " << valueArray[imageNumber];
                QJsonObject currentObj = valueArray[imageNumber].toObject();
                QTableWidget* table = ui->tableWidget;
                table->insertRow(table->rowCount());
                int column =0;
                foreach(const QString& objkey, currentObj.keys()) //Everything except the thumbnail
                {
                    QJsonValue currentValue = currentObj.value(objkey);
                    if(objkey.compare("id")==0 || objkey.compare("name")==0 || objkey.compare("path")==0)
                    {
                        if(currentValue.isDouble())
                        {
                            table->setItem(table->rowCount()-1, column, new QTableWidgetItem(QString::number(currentValue.toDouble())));
                        }
                        else if(currentValue.isString())
                        {
                            table->setItem(table->rowCount()-1, column, new QTableWidgetItem(currentValue.toString()));
                        }
                        column++;
                    }
                }
                //implement thumbnail here.
                QString databaseGetProjectURL = this->databaseURL+"/image/" + QString::number(imageNumber+1) + "/thumbnail";
                QUrl databaseLink = QUrl(databaseGetProjectURL);

                QUrlQuery query;

                query.addQueryItem("size_w", "150");
                query.addQueryItem("size_h", "50");

                databaseLink.setQuery(query.query());
                QNetworkRequest req = QNetworkRequest(databaseLink);
                qInfo() << req.url();
                QString loginToken = "Bearer "+this->accessToken;
                req.setRawHeader(QByteArray("Authorization"), loginToken.QString::toUtf8());
                doRequest(req);
            }
        }
        else {
           qInfo() << "value is not an QJsonArray";
           return;
        }
    }
}


void ImageList::fileReplyFinished()
{
    qInfo() << "got a file";
    if(rep->error())
    {
        qInfo() << "error getting file";
    }
    QUrl url = rep->url();
    QString path = QFileInfo(url.path()).fileName();
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        qInfo() << "could not save to disk";
    }

    file.write(rep->readAll());
    file.close();

    emit fileDownloaded(path);


}


void ImageList::on_loadButton_clicked()
{
    qInfo() << "load button clicked";
    QTableWidget* table = ui->tableWidget;
    QList<QTableWidgetItem*> list = table->selectedItems();
    for(int itemIndex = 2; itemIndex<list.size(); itemIndex+=3)
    {
        QTableWidgetItem* item = list.at(itemIndex);
        qInfo() << item->text();
        QString filepath = this->databaseURL + "/" + item->text();
        qInfo() << filepath;
        QNetworkRequest req = QNetworkRequest(filepath);
        rep = manager->get(req);

        connect(rep, &QNetworkReply::finished,
                this, &ImageList::fileReplyFinished);
    }

}

void ImageList::on_cancelButton_clicked()
{
    close();
}

