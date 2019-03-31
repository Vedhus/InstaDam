#ifndef PROJECTLIST_H
#define PROJECTLIST_H

#include <QWidget>
#include <QtNetwork/QNetworkReply>
#include <QListWidgetItem>
#include <QNetworkAccessManager>

namespace Ui {
class ProjectList;
}

class ProjectList : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectList(QWidget *parent = nullptr);
    ~ProjectList();
    void addItems(QJsonObject);
    void listImages();
    void imagesReplyFinished();

    void addItems(QJsonDocument, QString, QString);
public slots:
    void openProject(QListWidgetItem *);

private:
    Ui::ProjectList *ui;
    QString databaseURL;
    QString accessToken;
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkReply *rep;
    void getLabelsReplyFinished();


};

#endif // PROJECTLIST_H
