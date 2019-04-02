#include "instadam.h"
#include "startingwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    InstaDam w;
//    w.show();
    StartingWidget *sw = new StartingWidget;
    sw->show();

    return a.exec();
}
