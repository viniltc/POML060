#include "programopenhand.h"
#include "ui_programopenhand.h"
#include "stageprogram.h"

ProgramOpenHand::ProgramOpenHand(QString patientLabel, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ProgramOpenHand)
{
    ui->setupUi(this);
}

ProgramOpenHand::~ProgramOpenHand()
{
    delete ui;
}

void ProgramOpenHand::on_pushButton_back_keypro_clicked()
{
    stageProgram *backprogram;
    this->close();
    backprogram = new stageProgram("",this);
    backprogram -> show();
}
