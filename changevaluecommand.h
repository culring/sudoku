#ifndef CHANGEVALUECOMMAND_H
#define CHANGEVALUECOMMAND_H

#include <QUndoCommand>
#include <QModelIndex>
#include "gameboardmodel.h"

class ChangeValueCommand : public QUndoCommand
{
public:
    ChangeValueCommand(GameBoardModel &model, const QModelIndex &indx, int oldValue, int newValue);
    virtual void undo() override;
    virtual void redo() override;

private:
    GameBoardModel &model;
    QModelIndex indx;
    const int oldValue, newValue;
};

#endif // CHANGEVALUECOMMAND_H
