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

class ProjectList : public QDialog
{
    Q_OBJECT

public:
    explicit ProjectList(QWidget *parent = nullptr);
    ~ProjectList();
     QString useCase;

    void addItems(QJsonDocument, QString, QString);
public slots:
    void openProject(QListWidgetItem *);
    void deleteProject(QListWidgetItem *);
    void getLabelsReplyFinished();
    void getLabelsReplyFin(QNetworkReply*);
    void deleteReplyFinished();
    void deleteReplyFin(QNetworkReply*);
signals:
    void projectJsonReceived(QJsonObject);
    void instadamClearAll();

private:
    Ui::ProjectList *ui;
    QString databaseURL;
    QString accessToken;
    int selectedProject = -1;
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkReply *rep;
};

#endif // PROJECTLIST_H
