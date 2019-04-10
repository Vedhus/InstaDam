#ifndef USERPRIVILEGE_H
#define USERPRIVILEGE_H

#include <QWidget>

namespace Ui {
class UserPrivilege;
}

class UserPrivilege : public QWidget
{
    Q_OBJECT

public:
    explicit UserPrivilege(QWidget *parent = nullptr);
    ~UserPrivilege();
    QString userDetails;
    QString privilege;

signals:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

//private slots:

private:
    Ui::UserPrivilege *ui;
};

#endif // USERPRIVILEGE_H
