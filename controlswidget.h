#ifndef CONTROLSWDIGET_H
#define CONTROLSWDIGET_H

#include <QWidget>

namespace Ui {
class ControlsWdiget;
}

class ControlsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ControlsWidget(QWidget *parent = 0);
    ~ControlsWidget();

private:
    Ui::ControlsWdiget *ui;
};

#endif // CONTROLSWDIGET_H
