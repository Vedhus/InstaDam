#include <QApplication>

#include "login.h"
#include "startingwidget.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
#ifdef WASM_BUILD
    Login *log = new Login;
    log->show();
#else
    StartingWidget *sw = new StartingWidget;
    sw->show();
#endif
    return a.exec();
}
