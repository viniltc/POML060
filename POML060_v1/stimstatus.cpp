#include "stimstatus.h"
#include "ui_stimstatus.h"

stimstatus::stimstatus(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::stimstatus)
{
    ui->setupUi(this);
}

stimstatus::~stimstatus()
{
    delete ui;
}
