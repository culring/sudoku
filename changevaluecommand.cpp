#include "changevaluecommand.h"

ChangeValueCommand::ChangeValueCommand(GameBoardModel &model, const QModelIndex &indx, int oldValue, int newValue) :
    model(model), indx(indx), oldValue(oldValue), newValue(newValue) {}

void ChangeValueCommand::undo(){
    model.setValueInField(indx, oldValue);
}

void ChangeValueCommand::redo(){
    model.setValueInField(indx, newValue);
}
