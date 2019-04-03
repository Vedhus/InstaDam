#include "commands.h"

#include <QtWidgets>
#include <QVector>

#include "selectItem.h"
/*!
  \class AddCommand
   \ingroup Selectors
   \inmodule InstaDam
   \inherits QUndoCommand
   \brief The AddCommand class provides undo and redo actions
   for adding a SelectItem to a PhotoScene.

   AddCommand is used to hold a reference to a SelectItem and
   the PhotoScene to which it belongs. Using the undo() and redo()
   commands the item can be added and removed from the scene
   repeatedly.

   \sa DeleteCommand, MoveCommand, MoveVertexCommand, AddVertexCommand, DeleteVertexCommand, ErasePointsCommand, RotateCommand
 */

/*!
  Constructs an AddCommand using the provided \a item and \a scene
  so that the addition of this \a item to the \a scene can be undone
  and/or redone. \a parent refers to parent QUndoCommand, if any.

 */
AddCommand::AddCommand(SelectItem *item,
                       PhotoScene *scene, QUndoCommand *parent)
    : QUndoCommand(parent) {
    myScene = scene;
    myItem = item;
}


/*!
  Destructor
  */
AddCommand::~AddCommand() {
}

/*!
   \reimp
 */
void AddCommand::undo() {
    myItem->hide();
    myItem->mirrorHide();
    myScene->update();
    myItem->updateMirrorScene();
}

/*!
   \reimp
 */
void AddCommand::redo() {
    if (init) {
        myItem->show();
        myItem->mirrorShow();
        myScene->clearSelection();
        myScene->update();
        myItem->updateMirrorScene();
    } else {
        if (myItem->getMirror() != nullptr)
            myItem->itemWasAdded();
        init = true;
    }
}

/*!
  \class DeleteCommand
  \ingroup Selector
  \inmodule InstaDam
  \inherits QUndoCommand
  \brief The DeleteCommand class provides undo and redo actions
   for adding a SelectItem to a PhotoScene.

   DeleteCommand is used to hold a reference to a SelectItem and
   the PhotoScene to which it belongs. Using the undo() and redo()
   commands the item can be added and removed from the scene
   repeatedly.

   \sa AddCommand, MoveCommand, MoveVertexCommand, AddVertexCommand, DeleteVertexCommand, ErasePointsCommand, RotateCommand
 */
/*!
  Constructs a DeleteCommand using the provided \a item and \a scene
  so that the deletion of this \a item from the \a scene can be undone
  and/or redone. \a parent refers to parent QUndoCommand, if any.

 */
DeleteCommand::DeleteCommand(SelectItem* item, PhotoScene *scene,
                             QUndoCommand *parent)
    : QUndoCommand(parent) {
    myScene = scene;
    myItem = item;
}

/*!
   \reimp
 */
void DeleteCommand::undo() {
    myItem->show();
    myItem->mirrorShow();
    myScene->update();
    myItem->updateMirrorScene();
}
/*!
   \reimp
 */
void DeleteCommand::redo() {
    myItem->hide();
    myScene->update();
    myItem->mirrorHide();
    myItem->updateMirrorScene();
}

/*!
  \class MoveCommand
  \ingroup Selector
  \inmodule InstaDam
  \inherits QUndoCommand
  \brief The MoveCommand class provides undo and redo actions
   for moving a SelectItem on a PhotoScene.

   DeleteCommand is used to hold a reference to a SelectItem and
   the starting and ending positions of the move. Using the undo() and redo()
   commands the item can be moved and returned to it's original
   position in the scene repeatedly.

   \sa AddCommand, DeleteCommand, MoveVertexCommand, AddVertexCommand, DeleteVertexCommand, ErasePointsCommand, RotateCommand
*/
/*!
  Constructs a MoveCommand using the provided \a item
  so that the it's movement from \a oldPos to \a newPos on the scene can be
  undone and/or redone. \a parent refers to parent QUndoCommand, if any.

*/
MoveCommand::MoveCommand(SelectItem *item, const QPointF oldPos,
                         const QPointF newPos,
                         QUndoCommand *parent) : QUndoCommand(parent) {
    myItem = item;
    myoldPos = oldPos;
    mynewPos = newPos;
}

/*!
   \reimp
 */
void MoveCommand::undo() {
    myItem->resetActiveVertex();
    myItem->moveItem(mynewPos, myoldPos);
    myItem->scene()->update();
    myItem->updateMirrorScene();
}

/*!
   \reimp
 */
void MoveCommand::redo() {
    if (init) {
        myItem->resetActiveVertex();
        myItem->moveItem(myoldPos, mynewPos);
        myItem->scene()->update();
        myItem->updateMirrorScene();
    } else {
        init = true;
    }
}


/*!
  \class MoveVertexCommand
  \ingroup Selector
  \inmodule InstaDam
  \inherits QUndoCommand
  \brief The MoveVertexCommand class provides undo and redo actions
   for moving a vertex of a SelectItem on a PhotoScene.

   MoveVertexCommand is used to hold a reference to a SelectItem, the starting
   and ending positions, and which vertex is moving. Using the undo() and redo()
   commands the vertex can be moved from and to its original position
   in the scene repeatedly.

   \sa AddCommand, MoveCommand, DeleteCommand, AddVertexCommand, DeleteVertexCommand, ErasePointsCommand, RotateCommand
 */
/*!
  Constructs a MoveVeretxCommand using the provided \a item and \a vertex
  so that it's movement from \a oldPos to \a newPos on the scene can be undone
  and/or redone. \a parent refers to parent QUndoCommand, if any.

 */
MoveVertexCommand::MoveVertexCommand(SelectItem *item, const QPointF oldPos,
                                     const QPointF newPos,
                             const int vertex, QUndoCommand *parent)
    : QUndoCommand(parent) {
    myItem = item;
    myoldPos = oldPos;
    mynewPos = newPos;
    myVertex = vertex;
}

/*!
   \reimp
 */
void MoveVertexCommand::undo() {
    myItem->resizeItem(myVertex, myoldPos);
    myItem->scene()->update();
    myItem->updateMirrorScene();
}

/*!
  \reimp
*/
void MoveVertexCommand::redo() {
    if (init) {
        myItem->resizeItem(myVertex, mynewPos);
        myItem->scene()->update();
        myItem->updateMirrorScene();
    } else {
        init = true;
    }
}


/*!
  \class AddVertexCommand
  \ingroup Selector
  \inmodule InstaDam
  \inherits QUndoCommand
  \brief The AddVertexCommand class provides undo and redo actions
   for adding a vertex to a SelectItem on a PhotoScene.

   AddVertexCommand is used to hold a reference to a SelectItem and the position
   of the new vertex \a point. Using the undo() and redo()
   commands the vertex can removed from and to the SelectItem repeatedly.

   \sa AddCommand, MoveCommand, DeleteCommand, MoveVertexCommand, DeleteVertexCommand, ErasePointsCommand, RotateCommand
 */
/*!
  Constructs an AddVeretxCommand using the provided \a item and \a point so that
  the vertex can be added/removed from \a item repeatedly. \a parent refers
  to parent QUndoCommand, if any.
 */
AddVertexCommand::AddVertexCommand(SelectItem *item, const QPointF point,
                                   QUndoCommand *parent)
    : QUndoCommand(parent) {
    myItem = item;
    myPoint = point;
    init = false;
}

/*!
   \reimp
 */
void AddVertexCommand::undo() {
    myItem->removeVertex();
    myItem->scene()->update();
    myItem->updateMirrorScene();
}

/*!
   \reimp
 */
void AddVertexCommand::redo() {
    if (init) {
        myItem->setActiveVertex(SelectItem::UNSELECTED);
        myItem->addPoint(myPoint);
        myItem->scene()->update();
        myItem->updateMirrorScene();
    } else {
        init = true;
    }
}

/*!
  \class DeleteVertexCommand
  \ingroup Selector
  \inmodule InstaDam
  \inherits QUndoCommand
  \brief The DeleteVertexCommand class provides undo and redo actions
   for deleting a vertex from a SelectItem on a PhotoScene.

   DeleteVertexCommand is used to hold a reference to a SelectItem and it's
   activeVertex. Using the undo() and redo() commands the vertex can be added to
   and removed from the SelectItem repeatedly.

   \sa AddCommand, MoveCommand, DeleteCommand, MoveVertexCommand, AddVertexCommand, ErasePointsCommand, RotateCommand
 */
/*!
  Constructs a DeleteVeretxCommand using the provided \a item
  the vertex can be added/removed from \a item repeatedly. \a parent refers to
  parent QUndoCommand, if any.
 */
DeleteVertexCommand::DeleteVertexCommand(SelectItem *item, QUndoCommand *parent)
    : QUndoCommand(parent) {
    myItem = item;
    myPoint = item->getActivePoint();
    myVertex = item->getActiveVertex();
}

/*!
   \reimp
 */
void DeleteVertexCommand::undo() {
    myItem->setActiveVertex(SelectItem::UNSELECTED);
    myItem->addPoint(myPoint, myVertex);
    myItem->scene()->update();
    myItem->updateMirrorScene();
}

/*!
   \reimp
 */
void DeleteVertexCommand::redo() {
    myItem->removeVertex(myVertex);
    myItem->scene()->update();
    myItem->updateMirrorScene();
}

/*!
  \class ErasePointsCommand
  \ingroup Selector
  \inmodule InstaDam
  \inherits QUndoCommand
  \brief The ErasePointsCommand class provides undo and redo actions
   for erasing points from a FreeDrawSelect object.

   ErasePointsCommand is used to hold a reference to a FreeDrawSelect and
   the PhotoScenes to which it and its mirror belong. Using the undo() and redo()
   commands the erasure can be undone and redone
   repeatedly.

   \sa AddCommand, MoveCommand, AddVertexCommand, MoveVertexCommand, DeleteVertexCommand, DeleteCommand, RotateCommand
 */
/*!
  Constructs an ErasePointsCommand using the provided \a item and it's owning
  \a scene and the \a maskScene which owns it's mirror so that the erasure of
  points from this \a item can be undone/redone. \a parent refers to parent
  QUndoCommand, if any.
 */
ErasePointsCommand::ErasePointsCommand(FreeDrawErase *item, PhotoScene *scene,
                                       PhotoScene *maskScene,
                                       QUndoCommand *parent)
    : QUndoCommand(parent) {
    myItem = item;
    myScene = scene;
    myMask = maskScene;
    myPen.setColor(Qt::transparent);
}

/*!
   \reimp
 */
void ErasePointsCommand::undo() {
    EraseMapIterator it((*myItem->getMap()));
    while (it.hasNext()) {
        it.next();
        it.key()->addPoints(it.value());
    }
    myScene->update();
    myMask->update();
}
/*!
   \reimp
 */
void ErasePointsCommand::redo() {
    if (init) {
        EraseMapIterator it((*myItem->getMap()));
        while (it.hasNext()) {
            it.next();
            it.key()->deletePoints(myPen, it.value());
        }
    } else {
        init = true;
    }
    myScene->update();
    myMask->update();
}

/*!
  \class RotateCommand
  \ingroup Selector
  \inmodule InstaDam
  \inherits QUndoCommand
  \brief The RotateCommand class provides undo and redo actions
   for rotating a SelectItem on a PhotoScene.

   RotateCommand is used to hold a reference to a SelectItem and
   the starting and ending points of rotation. Using the undo() and redo()
   commands the rotation of the item can be undone and redone
   repeatedly.

   \sa AddCommand, MoveCommand, AddVertexCommand, MoveVertexCommand, DeleteVertexCommand, ErasePointsCommand, DeleteCommand
 */
/*!
  Constructs a RotateCommand using the provided \a item and the starting
  (\a oldPos) and ending (\a newPos) points of rotation so that the rotation of
  this \a item can be undone and/or redone. \a parent refers to parent
  QUndoCommand, if any.
 */
RotateCommand::RotateCommand(SelectItem *item, const QPointF oldPos,
                         const QPointF newPos,
                         QUndoCommand *parent) : QUndoCommand(parent) {
    myItem = item;
    myoldPos = oldPos;
    mynewPos = newPos;
}

/*!
   \reimp
 */
void RotateCommand::undo() {
    myItem->resetActiveVertex();
    myItem->rotate(mynewPos, myoldPos);
    myItem->scene()->update();
    myItem->updateMirrorScene();
}

/*!
   \reimp
 */
void RotateCommand::redo() {
    if (init) {
        myItem->resetActiveVertex();
        myItem->rotate(myoldPos, mynewPos);
        myItem->scene()->update();
        myItem->updateMirrorScene();
    } else {
        init = true;
    }
}
