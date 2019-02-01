#include <QtWidgets/QApplication>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCommandLineOption>
#include <QtWidgets/QMainWindow>

#include "MainWindow.h"

int main(int argc, char *argv[])
{
    //Q_INIT_RESOURCE(application);

    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("QtProject");
    QCoreApplication::setApplicationName("Application Example");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);
    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::applicationName());
    parser.addHelpOption();
    parser.addVersionOption();
    parser.process(app);

    QMainWindow* mainWin = new QMainWindow();
    //if (!parser.positionalArguments().isEmpty())
    //    mainWin.loadFile(parser.positionalArguments().first());
    MainWindow* mw = new MainWindow();
    mw->setupUi(mainWin);
    mainWin->show();
    return app.exec();
}
