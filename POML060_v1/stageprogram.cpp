#include "stageprogram.h"
#include "ui_stageprogram.h"
//#include "currentbuttonone.h"

stageProgram::stageProgram(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::stageProgram)
{
    ui->setupUi(this);

    connect(ui->widget, &CurrentButtonOne::getValue, ui->label_currentOne, &QLabel::setText);
}

stageProgram::~stageProgram()
{
    delete ui;
}
