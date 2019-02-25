/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   mainwindow.h
 * Author: friedel
 *
 * Created on February 6, 2019, 8:48 PM
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "selectItem.h"

QT_BEGIN_NAMESPACE
class QAction;
class QToolBar;
class QMenu;
class QUndoStack;
class QUndoView;
class QPointF;
QT_END_NAMESPACE
class PhotoScene;
class SelectItem;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

//public slots:
//    void itemMoved(DiagramItem *movedDiagram, const QPointF &moveStartPosition);

private slots:
    //void deleteItem();
    //void newItem(QPointF pos);
    //void movePoint(QPointF pos);
    void processMouseMoved(QPointF fromPos, QPointF toPos);
    //void selected(SelectItem* selected, QPointF pos);
    void processPointClicked(SelectItem *item, QPointF pos);  //OK
    void processLeftMouseReleased(QPointF oldPos, QPointF newPos);
    //void myupdate();
    //void deleteItem(SelectItem* item);
    void processKeyPressed(const int key);  //OK

private:
    void createActions();
    void createMenus();
    void createUndoView();

    QAction *deleteAction;
    QAction *addBoxAction;
    QAction *undoAction;
    QAction *redoAction;
    QAction *exitAction;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *itemMenu;

    PhotoScene *diagramScene;
    QUndoStack *undoStack;
    QUndoView *undoView;
    SelectType type;
    //SelectItem *item;
    SelectItem *currentItem;
};

#endif /* MAINWINDOW_H */

