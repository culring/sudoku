#include "controlspanel.h"
#include "ui_controlspanel.h"

ControlsPanel::ControlsPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlsPanel)
{
    ui->setupUi(this);
}

ControlsPanel::~ControlsPanel()
{
    delete ui;
}
