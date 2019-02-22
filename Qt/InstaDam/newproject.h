#ifndef NEWPROJECT_H
#define NEWPROJECT_H

#include <QDialog>
#include <QVector>
#include "project.h"

namespace Ui {
class newproject;
}

class newproject : public QDialog
{
    Q_OBJECT

public:
    explicit newproject(QWidget *parent = nullptr);
    ~newproject();

    Project newPr;

private slots:
    void on_pushButton_clicked();

    Project on_buttonBox_accepted();

private:
    Ui::newproject *ui;
};

#endif // NEWPROJECT_H
