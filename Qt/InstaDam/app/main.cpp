#include <QApplication>

#include "login.h"
#include "startingwidget.h"
#include "instadam.h"
#include "debuginsta.h"
#include <QDebug>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
#ifdef WASM_BUILD
    Login *log = new Login;
    log->show();
#else
//    DebugInsta *debug = new DebugInsta;
//    debug->show();
//    InstaDam*  = new InstaDam;
    StartingWidget *startingWidgetInstance = new StartingWidget;
//    sw->setInstaDam(w);
//    w->runningLocally = true;
//    w->setButtonsConfiguration();
//    w->show();
    startingWidgetInstance->show();
#endif
    qInfo()<<"Started!";
    return a.exec();
}
