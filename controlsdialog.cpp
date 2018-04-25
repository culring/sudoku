#include "controlsdialog.h"
#include "ui_controlsdialog.h"

ControlsDialog::ControlsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ControlsDialog)
{
    ui->setupUi(this);
    setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);
    setupShortcuts();
    setupShortcutsTypesToSequenceEdits();
    setupKeySequenceEdits();
}

ControlsDialog::~ControlsDialog()
{
    delete ui;
}

void ControlsDialog::on_buttonBox_accepted()
{
    QVector<QPair<SHORTCUT_TYPE, QKeySequence>> changedShortcuts;
    for(auto i = 0; i<NO_SHORTCUT; ++i){
        auto shortcutType = static_cast<SHORTCUT_TYPE>(i);
        if(shortcuts[shortcutType] != shortcutTypeToSequenceEdit[shortcutType]->keySequence()){
            shortcuts[shortcutType] = shortcutTypeToSequenceEdit[shortcutType]->keySequence();
            changedShortcuts.append(QPair<SHORTCUT_TYPE, QKeySequence>(shortcutType, shortcuts[shortcutType]));
        }
    }

    emit shortcutsChanged(changedShortcuts);
}

void ControlsDialog::setupKeySequenceEdits(){
    for(auto i = 0; i<NO_SHORTCUT; ++i){
        auto shortcutType = static_cast<SHORTCUT_TYPE>(i);
        shortcutTypeToSequenceEdit[shortcutType]->setKeySequence(shortcuts[shortcutType]);
    }
}

void ControlsDialog::setupShortcuts(){
    shortcuts[LEFT] = QKeySequence("Left");
    shortcuts[RIGHT] = QKeySequence("Right");
    shortcuts[UP] = QKeySequence("Up");
    shortcuts[DOWN] = QKeySequence("Down");
    shortcuts[NEXT_LEFT] = QKeySequence("Ctrl+Left");
    shortcuts[NEXT_RIGHT] = QKeySequence("Ctrl+Right");
    shortcuts[NEXT_UP] = QKeySequence("Ctrl+Up");
    shortcuts[NEXT_DOWN] = QKeySequence("Ctrl+Down");
    shortcuts[UNDO] = QKeySequence("Ctrl+Z");
    shortcuts[REDO] = QKeySequence("Ctrl+Y");
}

void ControlsDialog::setupShortcutsTypesToSequenceEdits(){
    shortcutTypeToSequenceEdit[LEFT] = ui->moveLeftKeySequenceEdit;
    shortcutTypeToSequenceEdit[RIGHT] = ui->moveRightKeySequenceEdit;
    shortcutTypeToSequenceEdit[UP] = ui->moveUpKeySequenceEdit;
    shortcutTypeToSequenceEdit[DOWN] = ui->moveDownKeySequenceEdit;
    shortcutTypeToSequenceEdit[NEXT_LEFT] = ui->moveNextLeftKeySequenceEdit;
    shortcutTypeToSequenceEdit[NEXT_RIGHT] = ui->moveNextRightKeySequenceEdit;
    shortcutTypeToSequenceEdit[NEXT_UP] = ui->moveNextUpKeySequenceEdit;
    shortcutTypeToSequenceEdit[NEXT_DOWN] = ui->moveNextDownKeySequenceEdit;
    shortcutTypeToSequenceEdit[UNDO] = ui->undoKeySequenceEdit;
    shortcutTypeToSequenceEdit[REDO] = ui->redoKeySequenceEdit;
}

void ControlsDialog::on_buttonBox_rejected()
{
    setupKeySequenceEdits();
}
