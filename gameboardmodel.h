#ifndef GAMEBOARDMODEL_H
#define GAMEBOARDMODEL_H

#include <QAbstractTableModel>
#include <QModelIndex>
#include <QUndoStack>
#include <array>

class GameBoardModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum LEVEL_TYPE{
        EASY,
        NORMAL,
        HARD
    };

    GameBoardModel();
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

    bool setValueInField(const QModelIndex &index, int value);
    bool canValueBeSet(const QModelIndex &index, int vaue);
    bool isLevelField(const QModelIndex &index) const;
    QModelIndex getFirstEmptyFieldIndex(const QModelIndex &index, Qt::ArrowType arrowType) const;
    QModelIndex getFieldInArrowDirection(const QModelIndex &index, Qt::ArrowType) const;
    bool isValueDuplicate(const QModelIndex &index) const;
    int getValueAtIndex(const QModelIndex &index) const;
    bool isFieldEmpty(const QModelIndex &index) const;
    void setNewLevel(LEVEL_TYPE levelType);

private:
    enum FIELD_TYPE{
        USER,
        LEVEL
    };
    struct Field{
        FIELD_TYPE fieldType;
        int value;

        Field() : fieldType(USER), value(1) {}
    };
    static constexpr unsigned BOARD_COLUMNS = 9, BOARD_ROWS = 9;
    std::array<std::array<Field, BOARD_COLUMNS>, BOARD_ROWS> fields;

    std::array<std::array<int, BOARD_COLUMNS>, BOARD_ROWS> generateLevel(LEVEL_TYPE levelType);
    bool areCoordinationsValid(int x, int y) const;
    bool areCoordinationsValid(const QModelIndex &index) const;
    bool isLevelField(int x, int y) const;
    Field* getFieldAtIndex(const QModelIndex &index);
    Field* getFieldAtCoordinates(int x, int y);
    Field getCopyFieldAtIndex(const QModelIndex &index) const;
    Field getCopyFieldAtCoordinates(int x, int y) const;
};

#endif // GAMEBOARDMODEL_H
