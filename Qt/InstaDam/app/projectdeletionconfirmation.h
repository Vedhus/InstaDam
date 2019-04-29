#ifndef PROJECTDELETIONCONFIRMATION_H
#define PROJECTDELETIONCONFIRMATION_H

#include <QWidget>
#include <QListWidgetItem>

namespace Ui {
class projectDeletionConfirmation;
}

class projectDeletionConfirmation : public QWidget
{
    Q_OBJECT

public:
    explicit projectDeletionConfirmation(QWidget *parent = nullptr);
    ~projectDeletionConfirmation();
    QListWidgetItem * project_name;

signals:
    void projectDeleted(QListWidgetItem *);

private slots:
    void on_noCancel_clicked();

    void on_yesDelete_clicked();

private:
    Ui::projectDeletionConfirmation *ui;
};

#endif // PROJECTDELETIONCONFIRMATION_H
