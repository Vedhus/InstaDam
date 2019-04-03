#include <QApplication>

#include "instadam.h"
#include "startingwidget.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    StartingWidget *sw = new StartingWidget;
    sw->show();

    return a.exec();
}
