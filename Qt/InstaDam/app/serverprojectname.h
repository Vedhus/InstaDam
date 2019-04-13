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
    Ui::serverProjectName *ui;

signals:
    void on_pushButton_clicked();

private slots:
    void on_pushButton_2_clicked();

};

#endif // SERVERPROJECTNAME_H
