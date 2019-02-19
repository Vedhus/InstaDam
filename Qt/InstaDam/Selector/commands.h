#ifndef COMMANDS_H
#define COMMANDS_H

#include <QUndoCommand>

#include "selectItem.h"
#include "photoScene.h"


class AddCommand : public QUndoCommand
{
public:
    AddCommand(SelectItem *item, PhotoScene *graphicsScene,
               QUndoCommand *parent = nullptr);
    ~AddCommand() override;

    void undo() override;
    void redo() override;

private:
    SelectItem *myItem;
    PhotoScene *myScene;
    QPointF initialPosition;
    bool init = false;
};

class DeleteCommand : public QUndoCommand
{
public:
    explicit DeleteCommand(SelectItem* item, PhotoScene *graphicsScene,
                           QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
    SelectItem *myItem;
    PhotoScene *myScene;
    bool init = false;
};

class MoveCommand : public QUndoCommand{
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

class ResizeCommand : public QUndoCommand{
public:
    ResizeCommand(SelectItem *item, const QPointF oldPos, const QPointF newPos,
                  const unsigned char corner, QUndoCommand *parent = nullptr);
    void undo() override;
    void redo() override;
private:
    SelectItem *myItem;
    QPointF myoldPos, mynewPos;
    unsigned char myCorner;
    bool init = false;
};
#endif /* COMMANDS_H */

