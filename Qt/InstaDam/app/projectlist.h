#ifndef PROJECTLIST_H
#define PROJECTLIST_H

#include <QWidget>
#include <QtNetwork/QNetworkReply>
namespace Ui {
class ProjectList;
}

class ProjectList : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectList(QWidget *parent = nullptr, QString databseURL ="", QByteArray token = "");
    ~ProjectList();
    void addItems(QJsonObject);
    void listImages();
    void imagesReplyFinished();
private:
    Ui::ProjectList *ui;
    QByteArray access_token;
    QString databaseURL;
    QNetworkReply *rep;
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
private slots:
    void on_loadImagesButton_clicked();


};

#endif // PROJECTLIST_H
