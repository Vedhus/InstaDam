#ifndef INSTADAM_H
#define INSTADAM_H

#include <QMainWindow>

namespace Ui {
class InstaDam;
}

class InstaDam : public QMainWindow
{
    Q_OBJECT

public:
    explicit InstaDam(QWidget *parent = nullptr);
    ~InstaDam();

private:
    Ui::InstaDam *ui;
};

#endif // INSTADAM_H
