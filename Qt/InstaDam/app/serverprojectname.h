#ifndef SERVERPROJECTNAME_H
#define SERVERPROJECTNAME_H

#include <QWidget>

namespace Ui {
class serverProjectName;
}

class serverProjectName : public QWidget
{
    Q_OBJECT

public:
    explicit serverProjectName(QWidget *parent = nullptr);
    ~serverProjectName();
//    QString projectName;
    QString databaseURL;
    QString accessToken;

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

private:
    Ui::serverProjectName *ui;

};

#endif // SERVERPROJECTNAME_H
