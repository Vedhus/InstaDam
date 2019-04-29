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
     QString useCase;

    void addItems(QJsonDocument, QString, QString);
public slots:
    void openProject(QListWidgetItem *);
    void deleteProject(QListWidgetItem *);

signals:
    void projectJsonReceived(QJsonObject json);
    void projectIdChanged(int id);
    void instadamClearAll();
    void projectDeleted(int id);

public slots:
    void openProject(QListWidgetItem *project_name);
    void deleteProject(QListWidgetItem *project_name);
    void confirmProjectDeletion(QListWidgetItem *project_name);

private:
    Ui::ProjectList *ui;
    QString databaseURL;
    QString accessToken;
    int selectedProject = -1;
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkReply *rep;
};

#endif // PROJECTLIST_H

class ProjectList : public QDialog
{
class ProjectList : public QWidget {
public slots:
    void openProject(QListWidgetItem *);
    void deleteProject(QListWidgetItem *);
    void getLabelsReplyFinished();
    void getLabelsReplyFin(QNetworkReply*);
    void deleteReplyFinished();
    void deleteReplyFin(QNetworkReply*);