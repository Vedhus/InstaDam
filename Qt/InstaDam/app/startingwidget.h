#ifndef STARTINGWIDGET_H
#define STARTINGWIDGET_H

#include <QWidget>

namespace Ui {
class StartingWidget;
}

class StartingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StartingWidget(QWidget *parent = nullptr);
    ~StartingWidget();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();

private:
    Ui::StartingWidget *ui;
    void runLocalVersion(void);
    void runServerVersion(void);

};

#endif // STARTINGWIDGET_H
