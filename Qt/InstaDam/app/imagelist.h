#ifndef IMAGELIST_H
#define IMAGELIST_H

#include <QWidget>
#include <QtNetwork/QNetworkReply>
namespace Ui {
class ImageList;
}

class ImageList : public QWidget
{
    Q_OBJECT

public:
    explicit ImageList(QWidget *parent = nullptr, QString databseURL ="", QByteArray token = "");
    ~ImageList();
    void addItems(QJsonObject);

private:
    Ui::ImageList *ui;
    QByteArray access_token;
    QString databaseURL;
    QNetworkReply *rep;
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
private slots:
    void on_loadButton_clicked();
    void on_cancelButton_clicked();


};

#endif // imagelist_H
