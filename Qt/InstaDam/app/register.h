#ifndef REGISTER_H
#define REGISTER_H

#include <QWidget>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QJsonObject>

namespace Ui {
class Register;
}

class Register : public QWidget
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = nullptr);
    ~Register();

private slots:
    void on_cancelButton_clicked();

    void on_registerButton_clicked();
    void replyFinished();
    void replyFin(QNetworkReply* reply);

private:
    Ui::Register *ui;
    void launchMainInstadam();
    QNetworkReply *rep;
    QNetworkAccessManager *manager;
    QString accessToken;
    QString databaseURL;
    QByteArray bytes;
};

#endif // REGISTER_H
