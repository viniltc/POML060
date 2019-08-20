#include "stageprogram.h"
#include "ui_stageprogram.h"

stageProgram::stageProgram(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::stageProgram)
{
    ui->setupUi(this);
}

stageProgram::~stageProgram()
{
    delete ui;
}
