#ifndef STARTINGWIDGET_H
#define STARTINGWIDGET_H

#include <QWidget>
#include "instadam.h"
namespace Ui {
class StartingWidget;
}

class StartingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StartingWidget(QWidget *parent = nullptr);
    ~StartingWidget();
    InstaDam *w;
    void setInstaDam(InstaDam * id);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::StartingWidget *ui;
    void runLocalVersion();
    void runServerVersion();
};

#endif // STARTINGWIDGET_H
