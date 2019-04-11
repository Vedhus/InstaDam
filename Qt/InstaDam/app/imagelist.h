#ifndef IMAGELIST_H
#define IMAGELIST_H

#include <QWidget>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QFileInfo>
#include <QUrlQuery>
#include <QPixmap>
#include <QByteArray>
#include "project.h"
#include "ui_imagelist.h"
#include "../Selector/label.h"
namespace Ui {
class ImageList;
}

class ImageList : public QWidget {
    Q_OBJECT

 public:
    explicit ImageList(Project* , QWidget *parent = nullptr, QString databseURL = "",
                       QString token = "");
    ~ImageList();
    void addItems(QJsonObject);
    QList<QTableWidgetItem*> selectedRow;
    QJsonArray jsonLabelArray;
    QJsonObject json;
    Project* currentProject;
    void openAnnotation();



 signals:
    void fileDownloaded(QString path);
    void gotANewAnnotation();
    void allAnnotationsLoaded(QJsonObject,fileTypes);



 private:
    int numAnnotationsReceived;
    Ui::ImageList *ui;
    QString databaseURL;
    QString accessToken;
    QNetworkReply *rep;
    QHash<int, QNetworkReply*> annotationReplies;
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    void getThumbnailReplyFinished();
    void fileReplyFinished();
    void doRequest(QNetworkRequest);

 private slots:
    void on_loadButton_clicked();
    void on_cancelButton_clicked();
 public slots:
    void checkIfAllAnnotationsReceived();
    void annotationReplyFinished(QNetworkReply* );
};

#endif  // imagelist_H
