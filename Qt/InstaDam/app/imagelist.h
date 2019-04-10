#ifndef IMAGELIST_H
#define IMAGELIST_H

#include <QWidget>
#include <QtNetwork/QNetworkReply>
#include <QFile>
namespace Ui {
class ImageList;
}

class ImageList : public QWidget
{
    Q_OBJECT

public:
    explicit ImageList(QWidget *parent = nullptr, QString databseURL ="", QString token = "");
    ~ImageList();
    void addItems(QJsonObject);

signals:
    void fileDownloaded(QString path);

private:
    Ui::ImageList *ui;
    QString databaseURL;
    QString accessToken;
    QNetworkReply *rep;
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    void getThumbnailReplyFinished();
    void fileReplyFinished();
    void doRequest(QNetworkRequest);

private slots:
    void on_loadButton_clicked();
    void on_cancelButton_clicked();


};

#endif // imagelist_H
