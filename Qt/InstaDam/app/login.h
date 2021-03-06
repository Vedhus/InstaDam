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
    void on_registerButton_clicked();

    void on_loginButton_clicked();

    void on_exitButton_clicked();

    void on_backButton_clicked();
    void replyFinished();
#ifdef WASM_BUILD
    void replyFin(QNetworkReply* reply);
#endif

private:
    Ui::Login *ui;
    void launchMainInstadam();
    QNetworkReply *rep;
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QByteArray accessToken;
    QString databaseURL;
};

#endif // LOGIN_H
