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
    QList<int> getIdList();
    QList<QString> getPathList();
    int getSelectedIdIndex();
    void setSelectedIdIndex(int id);
    void fileReplyFinished();
    void setAnnotated();

 signals:
    void fileDownloaded(QString path);
    void allAnnotationsLoaded(QJsonObject json,fileTypes type);
    void clearGUI();

 private:
    QList<int> idList;
    QList<bool> annotatedList;
    int selectedIdIndex;
    QList<QString> pathList;
    Ui::ImageList *ui;
    QString databaseURL;
    QString accessToken;
    QNetworkReply *rep;
    QHash<int, QNetworkReply*> annotationReplies;
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    void doRequest(QNetworkRequest req);
    bool zipFilesTogether(QFileInfoList files);
 private slots:
    void on_loadButton_clicked();
    void on_cancelButton_clicked();
    void on_uploadButton_clicked();
    void getThumbnailReplyFinished();
    void imagesReplyFinished();
    void uploadFileReplyFinished();
    void annotationReplyFinished();
#ifdef WASM_BUILD
    void getThumbnailReplyFin(QNetworkReply* reply);
    void imagesReplyFin(QNetworkReply* reply);
    void uploadFileReplyFin(QNetworkReply* reply);
    void annotationReplyFin(QNetworkReply* reply);
#endif
};

#endif  // imagelist_H
