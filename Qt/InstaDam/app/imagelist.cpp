#include "imagelist.h"
#include "ui_imagelist.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

ImageList::ImageList(QWidget *parent, QString databaseUrl, QByteArray token) :
    QWidget(parent),
    ui(new Ui::ImageList)
{
    ui->setupUi(this);
    access_token = token;
    this->databaseURL = databaseUrl;
}

ImageList::~ImageList()
{
    delete ui;
}

void ImageList::addItems(QJsonObject obj){
//    QList<QJsonObject> list;
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
                foreach(const QString& objkey, currentObj.keys())
                {
                    qInfo() << "Key = " << objkey << ", Value = " << currentObj.value(objkey);
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
            }
        }
        else {
           qInfo() << "value is not an QJsonArray";
           return;
        }
    }
}




void ImageList::on_loadButton_clicked()
{
    qInfo() << "load button clicked";
    /*open a canvas with the selected image, check if annotations exist for image and load those as well*/
}

void ImageList::on_cancelButton_clicked()
{
    close();
}

