#ifndef CONTROLSPANEL_H
#define CONTROLSPANEL_H

#include <QWidget>

namespace Ui {
class ControlsPanel;
}

class ControlsPanel : public QWidget
{
    Q_OBJECT

public:
    explicit ControlsPanel(QWidget *parent = 0);
    ~ControlsPanel();

private:
    Ui::ControlsPanel *ui;
};

#endif // CONTROLSPANEL_H
