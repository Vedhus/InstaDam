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

    void on_updatePrivilege_clicked();
    void privilegeReplyFinished();

#ifdef WASM_BUILD
    void privilegeReplyFin(QNetworkReply* reply);
    void replyFin(QNetworkReply* reply);
#endif

private:
    Ui::AddUserToProject *ui;
    QNetworkReply *rep;
    bool userInProject = false;
    QString privilege;
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    void listUsers(QJsonObject obj);
    void updateUser();
    void add();
};

#endif // ADDUSERTOPROJECT_H
