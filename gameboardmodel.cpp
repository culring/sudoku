#include "gameboardmodel.h"
#include <exception>

//constexpr unsigned GameBoardModel::BOARD_COLUMNS = 9;
//constexpr unsigned GameBoardModel::BOARD_ROWS = 9;

GameBoardModel::GameBoardModel()
{
    setNewLevel(EASY);
}

int GameBoardModel::rowCount(const QModelIndex &parent) const{
    return parent.isValid() ? 0 : 9;
}

int GameBoardModel::columnCount(const QModelIndex &parent) const {
    return parent.isValid() ? 0 : 9;
}

QVariant GameBoardModel::data(const QModelIndex &index, int role) const {
    if(role == Qt::DisplayRole){
        int value = getValueAtIndex(index);
        if(value == -1){
            return QString("");
        }
        return QString::number(value);
    }

    return QVariant();
};

bool GameBoardModel::setData(const QModelIndex &indx, const QVariant &value, int role){
    if(role == Qt::EditRole){
        Field &field = *getFieldAtIndex(indx);
        field.value = value.toInt();
        emit dataChanged(index(0, 0), index(8, 8));
        return true;
    }
    return false;
}

bool GameBoardModel::setValueInField(const QModelIndex &index, int value){
    return canValueBeSet(index, value) && setData(index, value, Qt::EditRole);
}

bool GameBoardModel::canValueBeSet(const QModelIndex &index, int value){
    if(index.column() > 8 || index.column() < 0 ||
       index.row() > 8 || index.row() < 0){
        return false;
    }
    if(value != -1 && (value < 1 || value > 9)){
        return false;
    }
    if(isLevelField(index)){
        return false;
    }
    return true;
}

bool GameBoardModel::isValueDuplicate(const QModelIndex &index) const{
    int x = index.column(), y = index.row();
    int value = getCopyFieldAtCoordinates(x, y).value;

    if(value == -1){
        return false;
    }

    // check row and column
    for(int i = 0; i<9; ++i){
        if(i != x && getCopyFieldAtCoordinates(i, y).value == value){
            return true;
        }
        if(i != y && getCopyFieldAtCoordinates(x, i).value == value){
            return true;
        }
    }

    // check square
    for(int i = y/3 * 3; i < y/3 * 3 + 3; ++i){
        for(int j = x/3 * 3; j < x/3 * 3 + 3; ++j){
            if(i == y && j == x){
                continue;
            }
            if(getCopyFieldAtCoordinates(j, i).value == value){
                return true;
            }
        }
    }

    return false;
}

void GameBoardModel::setNewLevel(LEVEL_TYPE levelType){
    auto &&values = generateLevel(levelType);

    for(auto i = 0; i<9; ++i){
        for(auto j = 0; j<9; ++j){
            if(values[i][j] < 0 || values[i][j] > 9){
                // error
            }
            Field *field = getFieldAtCoordinates(j, i);
            if(values[i][j] != 0){
                field->fieldType = LEVEL;
                field->value = values[i][j];
            }
            else{
                field->fieldType = USER;
                field->value = -1;
            }
        }
    }

    emit dataChanged(index(0, 0), index(columnCount()-1, rowCount()-1));
}

std::array<std::array<int, GameBoardModel::BOARD_COLUMNS>, GameBoardModel::BOARD_ROWS>
GameBoardModel::generateLevel(LEVEL_TYPE levelType){

    std::array<std::array<int, GameBoardModel::BOARD_COLUMNS>, GameBoardModel::BOARD_ROWS> values = {{
        {0,0,0,2,6,0,7,0,1},
        {6,8,0,0,7,0,0,9,0},
        {1,9,0,0,0,4,5,0,0},
        {8,2,0,1,0,0,0,4,0},
        {0,0,4,6,0,2,9,0,0},
        {0,5,0,0,0,3,0,2,8},
        {0,0,9,3,0,0,0,7,4},
        {0,4,0,0,5,0,0,3,6},
        {7,0,3,0,1,8,0,0,0}
    }};

    return values;
}

bool GameBoardModel::isLevelField(const QModelIndex &index) const{
    return isLevelField(index.column(), index.row());
}

bool GameBoardModel::isLevelField(int x, int y) const{
    return getCopyFieldAtCoordinates(x, y).fieldType == LEVEL;
}

QModelIndex GameBoardModel::getFieldInArrowDirection(const QModelIndex &indx, Qt::ArrowType arrowType) const{
    int xDiff = 0, yDiff = 0;
    // up
    if(arrowType == Qt::UpArrow){
        yDiff = -1;
    }
    // right
    else if(arrowType == Qt::RightArrow){
        xDiff = 1;
    }
    // down
    else if(arrowType == Qt::DownArrow){
        yDiff = 1;
    }
    // left
    else if(arrowType == Qt::LeftArrow){
        xDiff = -1;
    }
    // no_arrow
    else{
        return indx;
    }
    int x = indx.column() + xDiff;
    int y = indx.row() + yDiff;
    auto newIndex = index(y, x);
    if(!newIndex.isValid()){
        throw std::exception();
    }
    return index(y, x);
}

QModelIndex GameBoardModel::getFirstEmptyFieldIndex(const QModelIndex &indx, Qt::ArrowType arrowType) const{
    QModelIndex newIndex = indx;
    try{
        do{
            newIndex = getFieldInArrowDirection(newIndex, arrowType);
        }while(!isFieldEmpty(newIndex));
        return newIndex;
    }
    catch(std::exception e){
        return indx;
    }
}

bool GameBoardModel::areCoordinationsValid(int x, int y) const{
    return areCoordinationsValid(index(y, x));
}

bool GameBoardModel::areCoordinationsValid(const QModelIndex &index) const{
    return index.isValid();
}

int GameBoardModel::getValueAtIndex(const QModelIndex &index) const{
    if(!areCoordinationsValid(index)){
        throw std::exception();
    }

    return getCopyFieldAtIndex(index).value;
}

GameBoardModel::Field* GameBoardModel::getFieldAtIndex(const QModelIndex &index){
    return &fields[index.row()][index.column()];
}

GameBoardModel::Field* GameBoardModel::getFieldAtCoordinates(int x, int y){
    return getFieldAtIndex(index(y, x));
}

GameBoardModel::Field GameBoardModel::getCopyFieldAtIndex(const QModelIndex &index) const{
    return fields[index.row()][index.column()];
}

GameBoardModel::Field GameBoardModel::getCopyFieldAtCoordinates(int x, int y) const{
    return getCopyFieldAtIndex(index(y, x));
}

bool GameBoardModel::isFieldEmpty(const QModelIndex &index) const{
    return getValueAtIndex(index) == -1;
}
