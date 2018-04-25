#ifndef CONTROLSDIALOG_H
#define CONTROLSDIALOG_H

#include <QDialog>
#include <QVector>
#include <QKeySequenceEdit>

namespace Ui {
class ControlsDialog;
}

class ControlsDialog : public QDialog
{
    Q_OBJECT

public:
    enum SHORTCUT_TYPE{
        LEFT,
        RIGHT,
        UP,
        DOWN,
        NEXT_LEFT,
        NEXT_RIGHT,
        NEXT_UP,
        NEXT_DOWN,
        UNDO,
        REDO,
        NO_SHORTCUT
    };

    explicit ControlsDialog(QWidget *parent = 0);
    ~ControlsDialog();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::ControlsDialog *ui;
    QHash<SHORTCUT_TYPE, QKeySequence> shortcuts;
    QHash<SHORTCUT_TYPE, QKeySequenceEdit*> shortcutTypeToSequenceEdit;

    void setupKeySequenceEdits();
    void setupShortcuts();
    void setupShortcutsTypesToSequenceEdits();

signals:
    void shortcutsChanged(QVector<QPair<SHORTCUT_TYPE, QKeySequence>> changedShorcuts);
};

#endif // CONTROLSDIALOG_H
