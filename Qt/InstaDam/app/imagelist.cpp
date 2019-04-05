#include "imagelist.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QFileInfo>

#include "ui_imagelist.h"


/*!
  \class ImageList
  \ingroup app
  \inmodule InstaDam
  \inherits QSlider
  \brief Handles list of images form the server
 */

ImageList::ImageList(QWidget *parent, QString databaseUrl, QString token) :
    QWidget(parent), ui(new Ui::ImageList) {
    ui->setupUi(this);
    access_token = token;
    this->databaseURL = databaseUrl;
}

/*!
 * Destructor
 */

ImageList::~ImageList() {
    delete ui;
}

/*!
 * Adds Item \QJsonObject \a obj to the ImageList.
 */
void ImageList::addItems(QJsonObject obj) {
//    QList<QJsonObject> list;
    qInfo() << obj;
    foreach(const QString& key, obj.keys()) {
        if (key.compare("project_images") != 0) {
            qInfo() << "incorrect key";
            return;
        }
        QJsonValue value = obj.value(key);
        if (value.isArray()) {
            QJsonArray valueArray = value.toArray();
            qInfo() << valueArray;
            for (int imageNumber = 0; imageNumber < valueArray.size(); imageNumber++) {
                qInfo() << "image: " << imageNumber << " " << valueArray[imageNumber];
                QJsonObject currentObj = valueArray[imageNumber].toObject();
                QTableWidget* table = ui->tableWidget;
                table->insertRow(table->rowCount());
                int column = 0;
                foreach(const QString& objkey, currentObj.keys()) {
                    qInfo() << "Key = " << objkey << ", Value = " << currentObj.value(objkey);
                    QJsonValue currentValue = currentObj.value(objkey);
                    if (objkey.compare("id") == 0 || objkey.compare("name") == 0 ||
                       objkey.compare("path") == 0) {
                        if (currentValue.isDouble()) {
                            table->setItem(table->rowCount()-1, column, new QTableWidgetItem(QString::number(currentValue.toDouble())));
                        } else if (currentValue.isString()) {
                            table->setItem(table->rowCount()-1, column, new QTableWidgetItem(currentValue.toString()));
                        }
                        column++;
                    }
                }
            }
        } else {
           qInfo() << "value is not an QJsonArray";
           return;
        }
    }
}


/*!
 * Waits for the file to be received.
 */
void ImageList::fileReplyFinished() {
    qInfo() << "got a file";
    if (rep->error()) {
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


/*!
 * Slot called when the load button is clicked.
 */
void ImageList::on_loadButton_clicked() {
    qInfo() << "load button clicked";
    QTableWidget* table = ui->tableWidget;
    QList<QTableWidgetItem*> list = table->selectedItems();
    for (int itemIndex = 2; itemIndex < list.size(); itemIndex+=3) {
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

/*!
 * Slot called when the cancel button is clicked.
 */
void ImageList::on_cancelButton_clicked() {
    close();
}