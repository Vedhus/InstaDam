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
    bool on_pushButton_clicked();
    bool on_pushButton_2_clicked();

//private slots:

private:
    Ui::UserPrivilege *ui;
};

#endif // USERPRIVILEGE_H
