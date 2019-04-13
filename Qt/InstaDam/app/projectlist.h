#ifndef PROJECTLIST_H
#define PROJECTLIST_H

#include "instadam.h"
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
     InstaDam *instadam;

    void addItems(QJsonDocument, QString, QString);
public slots:
    void openProject(QListWidgetItem *);

private:
    Ui::ProjectList *ui;
    QString databaseURL;
    QString accessToken;
    int selectedProject = -1;
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkReply *rep;
    void getLabelsReplyFinished();
};

#endif // PROJECTLIST_H
