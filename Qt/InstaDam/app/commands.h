#ifndef COMMANDS_H
#define COMMANDS_H

#include <QUndoCommand>
#include <QColor>

#include "Selector/selectItem.h"
#include "Selector/polygonSelect.h"
#include "Selector/freeDrawErase.h"
#include "Selector/photoScene.h"

class InstaDam;

class AddCommand : public QUndoCommand{
 public:
    AddCommand(SelectItem *item, PhotoScene *graphicsScene,
               InstaDam *idam, QUndoCommand *parent = nullptr);
    ~AddCommand() override;

    void undo() override;
    void redo() override;

 private:
    SelectItem *myItem;
    PhotoScene *myScene;
    InstaDam *myParent;
    QPointF initialPosition;
    bool init = false;
};

class DeleteCommand : public QUndoCommand {
 public:
    explicit DeleteCommand(SelectItem* item, PhotoScene *graphicsScene,
                           InstaDam *idam, QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

 private:
    SelectItem *myItem;
    PhotoScene *myScene;
    InstaDam *myParent;
    bool init = false;
};

class MoveCommand : public QUndoCommand {
 public:
    MoveCommand(SelectItem *item, const QPointF oldPos,
                const QPointF newPos, QUndoCommand *parent = nullptr);
    void undo() override;
    void redo() override;
 private:
    SelectItem *myItem;
    QPointF myoldPos;
    QPointF mynewPos;
    bool init = false;
};

class MoveVertexCommand : public QUndoCommand {
 public:
    MoveVertexCommand(SelectItem *item, const QPointF oldPos,
                      const QPointF newPos, const int vertex,
                      QUndoCommand *parent = nullptr);
    void undo() override;
    void redo() override;
 private:
    SelectItem *myItem;
    QPointF myoldPos, mynewPos;
    int myVertex;
    bool init = false;
};

class AddVertexCommand : public QUndoCommand {
 public:
    AddVertexCommand(SelectItem *item, const QPointF point,
                     QUndoCommand *parent = nullptr);
    void undo() override;
    void redo() override;
 private:
    SelectItem *myItem;
    QPointF myPoint;
    int vertex;
    bool init = false;
};

class DeleteVertexCommand : public QUndoCommand {
 public:
    explicit DeleteVertexCommand(SelectItem *item,
                                 QUndoCommand *parent = nullptr);
    void undo() override;
    void redo() override;
 private:
    SelectItem *myItem;
    QPointF myPoint;
    int myVertex;
    bool init = false;
};

class ErasePointsCommand : public QUndoCommand {
 public:
    ErasePointsCommand(FreeDrawErase *item, PhotoScene *graphicsScene,
                       PhotoScene *maskScene, QUndoCommand *parent = nullptr);
    void undo() override;
    void redo() override;
 private:
    FreeDrawErase *myItem;
    PhotoScene *myScene;
    PhotoScene *myMask;
    bool init = false;
    QPen myPen;
};

class RotateCommand : public QUndoCommand {
 public:
    RotateCommand(SelectItem *item, const QPointF oldPos,
                const QPointF newPos, QUndoCommand *parent = nullptr);
    void undo() override;
    void redo() override;
 private:
    SelectItem *myItem;
    QPointF myoldPos;
    QPointF mynewPos;
    bool init = false;
};

class EditLabelCommand : public QUndoCommand{
public:
    EditLabelCommand(SelectItem *item, QSharedPointer<Label> newLabel,
                     QSharedPointer<Label> oldLabel,
                     PhotoScene *scene,InstaDam *idam,
                     QUndoCommand *parent = nullptr);
    void undo() override;
    void redo() override;
    SelectItem *myItem;
    PhotoScene *myScene;
    InstaDam *myParent;
    QSharedPointer<Label> myOldLabel;
    QSharedPointer<Label> myNewLabel;
};


#endif /* COMMANDS_H */

