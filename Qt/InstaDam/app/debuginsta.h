#ifndef DEBUGINSTA_H
#define DEBUGINSTA_H

#include <QMainWindow>
#include <QObject>



namespace Ui {
class DebugInsta;
}

class DebugInsta : public QMainWindow
{
    Q_OBJECT

public:
    explicit DebugInsta(QWidget *parent = nullptr);
    ~DebugInsta();

private:
    Ui::DebugInsta *ui;
};






#endif // DEBUGINSTA_H
