#include "stageprogram.h"
#include "ui_stageprogram.h"
#include "currentbuttonone.h"
#include "tetra_grip_api.h"
#include "tetra_grip_reader.h"
#include<QDebug>


stageProgram::stageProgram(QString patientLabel, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::stageProgram)
{
    ui->setupUi(this);
    ui->pushButton_currOnOne->setCheckable(true);
    ui->pushButton_currOnTwo->setCheckable(true);
    ui->pushButton_currOnThree->setCheckable(true);
    ui->pushButton_currOnFour->setCheckable(true);
    ui->pushButton_currOnFive->setCheckable(true);


    ui->widget_currentOne->setEnabled(false);
    ui->widget_currentTwo->setEnabled(false);
    ui->widget_currentThree->setEnabled(false);
    ui->widget_currentFour->setEnabled(false);
    ui->widget_currentFive->setEnabled(false);

    ui->label_pLabel->setText(patientLabel);
    ui->label_pLabel->setAlignment(Qt::AlignCenter);
    ui->label_pLabel->setStyleSheet("color: blue;");

    pLabel = patientLabel;

    connect(&api, &tetra_grip_api::tetraGripEvent,this, &stageProgram::stimStatusEventHandler);

    connect(ui->widget_currentOne, &CurrentButtonOne::getValue, this, &stageProgram::setCurrOnChannelOne);
    connect(ui->widget_currentTwo, &CurrentButtonOne::getValue, this, &stageProgram::setCurrOnChannelTwo);
    connect(ui->widget_currentThree, &CurrentButtonOne::getValue, this, &stageProgram::setCurrOnChannelThree);
    connect(ui->widget_currentFour, &CurrentButtonOne::getValue, this, &stageProgram::setCurrOnChannelFour);
    connect(ui->widget_currentFive, &CurrentButtonOne::getValue, this, &stageProgram::setCurrOnChannelFive);

    connect(ui->pushButton_currOnOne, &QPushButton::clicked, ui->widget_currentOne, &CurrentButtonOne::setEnabled);
    connect(ui->pushButton_currOnTwo, &QPushButton::clicked, ui->widget_currentTwo, &CurrentButtonOne::setEnabled);
    connect(ui->pushButton_currOnThree, &QPushButton::clicked, ui->widget_currentThree, &CurrentButtonOne::setEnabled);
    connect(ui->pushButton_currOnFour, &QPushButton::clicked, ui->widget_currentFour, &CurrentButtonOne::setEnabled);
    connect(ui->pushButton_currOnFive, &QPushButton::clicked, ui->widget_currentFive, &CurrentButtonOne::setEnabled);

  tetra_grip_api::get_target_current_channel(0);
}

stageProgram::~stageProgram()
{
    delete ui;
}


void stageProgram::setCurrOnChannelOne(QString value)
{
    unsigned int channel_number = 0;


    unsigned int current_uA=value.toUInt();


    tetra_grip_api::stimulation_set_current(channel_number, current_uA);

}

void stageProgram::setCurrOnChannelTwo(QString value)
{
    unsigned int channel_number = 1;
    unsigned int current_uA = value.toUInt();
    tetra_grip_api::stimulation_set_current(channel_number, current_uA);
}

void stageProgram::setCurrOnChannelThree(QString value)
{
    unsigned int channel_number = 2;
    unsigned int current_uA = value.toUInt();
    tetra_grip_api::stimulation_set_current(channel_number, current_uA);
}

void stageProgram::setCurrOnChannelFour(QString value)
{
    unsigned int channel_number = 3;
    unsigned int current_uA = value.toUInt();
    tetra_grip_api::stimulation_set_current(channel_number, current_uA);
}

void stageProgram::setCurrOnChannelFive(QString value)
{
    unsigned int channel_number = 4;
    unsigned int current_uA = value.toUInt();
    tetra_grip_api::stimulation_set_current(channel_number, current_uA);
}

void stageProgram::on_pushButton_stimStart_clicked()
{
    tetra_grip_api::stimulation_start(true);
}

void stageProgram::on_pushButton_stimStop_clicked()
{
    tetra_grip_api::stimulation_pause(true);
}

void stageProgram::stimStatusEventHandler(STIM_GUI_TOPIC_T topic,uint8_t index, uint8_t reg, uint32_t value)
{
    if (topic==TOPIC_STIMULATOR)
    {
        switch(reg)
        {
        case STIM_REG_ACTIVITY_STATUS: //STIM_REG_ACTIVITY_OPTIONS

            if(value==true)
            {
                 ui->pushButton_stimStop->setText("Stop");
                 ui->pushButton_stimStart->setText("Running");
            }
            else
            {
                 ui->pushButton_stimStop->setText("Stopped");
                 ui->pushButton_stimStart->setText("Start");
            }
            break;
        }
    }
  else if (topic==TOPIC_CHANNEL && index==0){
        switch(reg){
        case STIM_ENGINE_REG_TARGET_CURRENT:

            ui->label_PW->setText(QString::number(value));
        }
    }
}



void stageProgram::on_pushButton_programKeyGrip_clicked()
{
   this->hide();
   keygripv2 = new ProgramKeyGripV2("hello");
   keygripv2 -> show();
}

void stageProgram::on_pushButton_programPalmerGrasp_clicked()
{
    this->hide();
    palmergrasp = new ProgramPalmerGrasp(this);
    palmergrasp->show();
}

void stageProgram::on_pushButton_programOpenHand_clicked()
{
    this->close();
    openhand = new ProgramOpenHand();
    openhand->show();
}

void stageProgram::on_pushButton_programSwitchGrasp_clicked()
{
    this->hide();
    switchgrasp = new ProgramSwitchGrasp();
    switchgrasp->show();

}
