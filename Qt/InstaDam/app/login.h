#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QJsonObject>

namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

private slots:
    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_username_selectionChanged();

    void on_password_textChanged();

    void on_url_textChanged();

private:
    Ui::Login *ui;
    void replyFinished();
    void projectsReplyFinished();
    void dumpToken();
    void listProjects();
    QNetworkReply *rep;
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QByteArray accessToken;
    QString databaseURL;
};

#endif // LOGIN_H
