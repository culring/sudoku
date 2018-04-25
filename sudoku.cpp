#include <QKeyEvent>
#include <exception>
#include "sudoku.h"
#include "ui_sudoku.h"
#include "changevaluecommand.h"
#include <QSizeGrip>

Sudoku::Sudoku(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Sudoku),
    controlsDialog(this),
    gameBoardModel(new GameBoardModel())
{
    ui->setupUi(this);

    setWindowIcon(QIcon(":/new/images/images/icon.png"));

    setupControlsDialog();
    setupKeySequences();
    setupGameBoardView();
    resetGame();
}

Sudoku::~Sudoku()
{
    delete ui;
    delete gameBoardModel;
}

void Sudoku::setupControlsDialog(){
    connect(&controlsDialog, &ControlsDialog::shortcutsChanged, this, &Sudoku::on_shortcutsChanged);
}

void Sudoku::setupKeySequences(){
    shortcutToKeySequence[ControlsDialog::LEFT] = QKeySequence("Left");
    shortcutToKeySequence[ControlsDialog::RIGHT] = QKeySequence("Right");
    shortcutToKeySequence[ControlsDialog::UP] = QKeySequence("Up");
    shortcutToKeySequence[ControlsDialog::DOWN] = QKeySequence("Down");
    shortcutToKeySequence[ControlsDialog::NEXT_LEFT] = QKeySequence("Ctrl+Left");
    shortcutToKeySequence[ControlsDialog::NEXT_RIGHT] = QKeySequence("Ctrl+Right");
    shortcutToKeySequence[ControlsDialog::NEXT_UP] = QKeySequence("Ctrl+Up");
    shortcutToKeySequence[ControlsDialog::NEXT_DOWN] = QKeySequence("Ctrl+Down");
    shortcutToKeySequence[ControlsDialog::UNDO] = QKeySequence("Ctrl+Z");
    shortcutToKeySequence[ControlsDialog::REDO] = QKeySequence("Ctrl+Y");
}

void Sudoku::setupGameBoardView(){
    ui->gameBoardView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->gameBoardView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // set model
    ui->gameBoardView->setModel(gameBoardModel);
    // set delagate
    ui->gameBoardView->setItemDelegate(&delegate);
}

void Sudoku::keyPressEvent(QKeyEvent *event){
    if(handleMoveToFieldEvent(event) || handleChangeValueEvent(event) || handleUndoEvent(event)){
        return;
    }
}

bool Sudoku::handleChangeValueEvent(const QKeyEvent *event){
    auto index = gameBoardModel->index(currentRow, currentColumn);
    int oldValue = gameBoardModel->getValueAtIndex(index);
    int keyCode = event->key();

    int key0 = Qt::Key_0;
    for(auto i = 1; i<=9; ++i){
        if(keyCode == key0 + i){
            if(i != oldValue && gameBoardModel->setValueInField(index, i)){
                undoStack.push(new ChangeValueCommand(*gameBoardModel, index, oldValue, i));
            }
            return true;
        }
    }

    if(keyCode == Qt::Key_Backspace){
        if(gameBoardModel->setValueInField(index, -1)){
            undoStack.push(new ChangeValueCommand(*gameBoardModel, index, oldValue, -1));
        }
        return true;
    }

    return false;
}

bool Sudoku::handleUndoEvent(const QKeyEvent *event){
    QKeySequence eventKeySequence = QKeySequence(event->key() + event->modifiers());
    ControlsDialog::SHORTCUT_TYPE shortcutType = convertKeySequenceToShortcutType(eventKeySequence);
    if(shortcutType == ControlsDialog::UNDO){
        undoStack.undo();
        return true;
    }
    if(shortcutType == ControlsDialog::REDO){
        undoStack.redo();
        return true;
    }

    return false;
}

bool Sudoku::handleMoveToFieldEvent(QKeyEvent *event){
    QKeySequence eventKeySequence = QKeySequence(event->key() + event->modifiers());
    ControlsDialog::SHORTCUT_TYPE shortcutType = convertKeySequenceToShortcutType(eventKeySequence);
    if(!(shortcutType >= ControlsDialog::LEFT && shortcutType <= ControlsDialog::NEXT_DOWN)){
        return false;
    }

    Qt::ArrowType arrowType = convertShortcutTypeToArrowType(shortcutType);
    QModelIndex index;
    // key for next field in direction
    if(shortcutType >= ControlsDialog::NEXT_LEFT && shortcutType <= ControlsDialog::NEXT_DOWN){
        index = gameBoardModel->getFirstEmptyFieldIndex(currentIndex, arrowType);
    }
    else{
        try{
            index = gameBoardModel->getFieldInArrowDirection(currentIndex, arrowType);
        }
        catch(std::exception e){
            index = currentIndex;
        }
    }

    if(index != currentIndex){
        selectField(index);
    }
    return true;
}

ControlsDialog::SHORTCUT_TYPE Sudoku::convertKeySequenceToShortcutType(QKeySequence keySequence){
    ControlsDialog::SHORTCUT_TYPE shortcutType = ControlsDialog::NO_SHORTCUT;
    for(auto key : shortcutToKeySequence.keys()){
        if(shortcutToKeySequence[key] == keySequence){
            shortcutType = key;
            break;
        }
    }
    return shortcutType;
}

Qt::ArrowType Sudoku::convertShortcutTypeToArrowType(ControlsDialog::SHORTCUT_TYPE shortcutType){
    if(shortcutType == ControlsDialog::LEFT || shortcutType == ControlsDialog::NEXT_LEFT){
        return Qt::LeftArrow;
    }
    else if(shortcutType == ControlsDialog::RIGHT || shortcutType == ControlsDialog::NEXT_RIGHT){
        return Qt::RightArrow;
    }
    else if(shortcutType == ControlsDialog::UP || shortcutType == ControlsDialog::NEXT_UP){
        return Qt::UpArrow;
    }
    else if(shortcutType == ControlsDialog::DOWN || shortcutType == ControlsDialog::NEXT_DOWN){
        return Qt::DownArrow;
    }
    else{
        throw std::exception();
    }
}

void Sudoku::deselectCurrentField(){
    auto selectionModel = ui->gameBoardView->selectionModel();
    QItemSelection selection(currentIndex, currentIndex);
    selectionModel->select(selection, QItemSelectionModel::Deselect);
}

void Sudoku::selectCurrentField(){
    auto selectionModel = ui->gameBoardView->selectionModel();
    QItemSelection selection(currentIndex, currentIndex);
    selectionModel->select(selection, QItemSelectionModel::Select);
}

void Sudoku::selectField(const QModelIndex &fieldIndex){
    deselectCurrentField();
    currentIndex = fieldIndex;
    currentRow = fieldIndex.row();
    currentColumn = fieldIndex.column();
    selectCurrentField();
}

void Sudoku::on_gameBoardView_clicked(const QModelIndex &index)
{
    selectField(index);
}

void Sudoku::resetGame(){
    deselectCurrentField();
    currentColumn = currentRow = 0;
    currentIndex = gameBoardModel->index(0, 0);
    undoStack.clear();
    selectCurrentField();
}

void Sudoku::setLevel(GameBoardModel::LEVEL_TYPE levelType){
    resetGame();
    gameBoardModel->setNewLevel(levelType);
}

void Sudoku::on_actionEasy_triggered()
{
    setLevel(GameBoardModel::EASY);
}

void Sudoku::on_actionNormal_triggered()
{
    setLevel(GameBoardModel::NORMAL);
}

void Sudoku::on_actionHard_triggered()
{
    setLevel(GameBoardModel::HARD);
}

void Sudoku::on_actionControls_triggered()
{
    controlsDialog.show();
}

void Sudoku::on_shortcutsChanged(QVector<QPair<ControlsDialog::SHORTCUT_TYPE, QKeySequence>> changedShortcuts){
    for(auto pair : changedShortcuts){
        auto shortcutType = pair.first;
        auto keySequence = pair.second;
        shortcutToKeySequence[shortcutType] = keySequence;
    }
}
