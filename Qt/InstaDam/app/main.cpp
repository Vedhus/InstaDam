#include "instadam.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    InstaDam w;
    w.show();

    return a.exec();
}