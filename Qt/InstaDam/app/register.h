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
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::Register *ui;
    void replyFinished();
    void dumpToken();
    void lunchMainInstadam();
    QNetworkReply *rep;
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QString accessToken;
    QString databaseURL;
};

#endif // REGISTER_H
