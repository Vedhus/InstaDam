/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   main.cpp
 * Author: friedel
 *
 * Created on February 6, 2019, 8:41 PM
 */

#include <QtWidgets>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    // initialize resources, if needed
    //Q_INIT_RESOURCE(undoframework);

    QApplication app(argc, argv);
    MainWindow mainWindow;
    mainWindow.show();
    // create and show your widgets here

    return app.exec();
}
