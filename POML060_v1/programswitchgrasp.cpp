#include "programswitchgrasp.h"
#include "ui_programswitchgrasp.h"
#include "stageprogram.h"

ProgramSwitchGrasp::ProgramSwitchGrasp(QString patientLabel, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ProgramSwitchGrasp)
{
    ui->setupUi(this);
}

ProgramSwitchGrasp::~ProgramSwitchGrasp()
{
    delete ui;
}

void ProgramSwitchGrasp::on_pushButton_back_keypro_clicked()
{
    stageProgram *backprogram;
    this->close();
    backprogram = new stageProgram("",this);
    backprogram -> show();
}
