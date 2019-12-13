#include "stageprogram.h"
#include "ui_stageprogram.h"
#include "currentbuttonone.h"
#include "tetra_grip_api.h"
#include "tetra_grip_reader.h"
#include<QDebug>


stageProgram::stageProgram(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::stageProgram)
{
    ui->setupUi(this);

    connect(ui->widget, &CurrentButtonOne::getValue, this, &stageProgram::setCurrOnChannelOne);
}

stageProgram::~stageProgram()
{
    delete ui;
}


void stageProgram::setCurrOnChannelOne(QString value)
{
    ui->label_currentOne->setText(value);
    unsigned int channel_number = 1;
    unsigned int current_uA = value.toUInt();

    qDebug()<<"Current value"<<current_uA;

    tetra_grip_api::stimulation_set_current(channel_number, current_uA);
}

void stageProgram::setCurrOnChannelTwo(QString value)
{

}

void stageProgram::setCurrOnChannelThree(QString value)
{

}

void stageProgram::setCurrOnChannelFour(QString value)
{

}

void stageProgram::setCurrOnChannelFive(QString value)
{

}

void stageProgram::on_pushButton_stimStart_clicked()
{
    tetra_grip_api::stimulation_start(true);
}

void stageProgram::on_pushButton_stimStop_clicked()
{
    tetra_grip_api::stimulation_pause(true);
}
