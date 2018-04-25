#include "controlswidget.h"
#include "ui_controlswdiget.h"

ControlsWidget::ControlsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlsWdiget)
{
    ui->setupUi(this);
    setWindowTitle("Controls");

}

ControlsWidget::~ControlsWidget()
{
    delete ui;
}
