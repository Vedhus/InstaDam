#ifndef ADDUSERTOPROJECT_H
#define ADDUSERTOPROJECT_H

#include <QWidget>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QListWidgetItem>
#include "userprivilege.h"
#include "ui_userprivilege.h"

namespace Ui {
class AddUserToProject;
}

class AddUserToProject : public QWidget
{
    Q_OBJECT

public:
    explicit AddUserToProject(QWidget *parent = nullptr);
    ~AddUserToProject();
    QString accessToken;
    QString databaseURL;
    QString userDetails;
    int projectId;
    UserPrivilege *userPrivilege = new UserPrivilege;

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_addToProject_clicked();
    void addAsAnnotator();
    void addAsAdmin();
    void replyFinished();
    void replyFin(QNetworkReply* reply);

    void on_updatePrivilege_clicked();
    void privilegeReplyFin(QNetworkReply* reply);
    void privilegeReplyFinished();

private:
    Ui::AddUserToProject *ui;
    QNetworkReply *rep;
    bool userInProject = false;
    QString privilege;
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    void listUsers(QJsonObject);
    void updateUser();
    void add();
};

#endif // ADDUSERTOPROJECT_H
