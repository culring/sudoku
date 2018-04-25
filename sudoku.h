#ifndef SUDOKU_H
#define SUDOKU_H

#include <QMainWindow>
#include <QUndoStack>
#include "gameboardmodel.h"
#include "customdelegate.h"
#include "controlsdialog.h"

namespace Ui {
class Sudoku;
}

class Sudoku : public QMainWindow
{
    Q_OBJECT

public:
    explicit Sudoku(QWidget *parent = 0);
    ~Sudoku();

private slots:

    void on_gameBoardView_clicked(const QModelIndex &index);

    void on_actionEasy_triggered();

    void on_actionHard_triggered();

    void on_actionNormal_triggered();

    void on_actionControls_triggered();

    void on_shortcutsChanged(QVector<QPair<ControlsDialog::SHORTCUT_TYPE, QKeySequence>> changedShortcuts);

private:
    Ui::Sudoku *ui;
    ControlsDialog controlsDialog;
    GameBoardModel *gameBoardModel;
    int currentRow, currentColumn;
    QModelIndex currentIndex;
    CustomDelegate delegate;
    QUndoStack undoStack;
    QHash<ControlsDialog::SHORTCUT_TYPE, QKeySequence> shortcutToKeySequence;

    void keyPressEvent(QKeyEvent *event);

    void setupGameBoardView();
    void selectCurrentField();
    void deselectCurrentField();
    void selectField(const QModelIndex &fieldIndex);
    bool handleMoveToFieldEvent(QKeyEvent *event);
    bool handleChangeValueEvent(const QKeyEvent *event);
    bool handleUndoEvent(const QKeyEvent *event);
    void resetGame();
    void setLevel(GameBoardModel::LEVEL_TYPE);
    void setupKeySequences();
    void setupControlsDialog();
    Qt::ArrowType convertShortcutTypeToArrowType(ControlsDialog::SHORTCUT_TYPE shortcutType);
    ControlsDialog::SHORTCUT_TYPE convertKeySequenceToShortcutType(QKeySequence keySequence);
};

#endif // SUDOKU_H
