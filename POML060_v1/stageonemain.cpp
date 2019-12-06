#include "stageonemain.h"
#include "ui_stageonemain.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QFile>
#include <QTextStream>
#include "tetra_grip_api.h"
#include "tetra_grip_reader.h"


StageOneMain::StageOneMain( QString patientLabel, QWidget *parent) : QMainWindow(parent)
//  StageOneMain::StageOneMain( tetra_grip_api *api, QWidget *parent) : QMainWindow(parent)
    , ui(new Ui::StageOneMain)
{
    ui->setupUi(this);

  //  this->setAttribute(Qt::WA_QuitOnClose, false);

    this->setStyleSheet("background-color: white;");
    this->setFixedSize(this->width(),this->height());
    statusConfig = new QLabel(this);
    statusBat = new QLabel(this);
    statusBat->setAlignment(Qt::AlignRight);
    ui->statusbar->addPermanentWidget(statusConfig);
    ui->statusbar->addPermanentWidget(statusBat,1);

    // initial appearnece of main window
    if(patientLabel.isEmpty()){
        ui->pushButton_programs->setEnabled(false);
        ui->pushButton_logs->setEnabled(false);

    }

    ui->label_pid->setText(patientLabel);
    ui->label_pid->setAlignment(Qt::AlignCenter);
    ui->label_pid->setStyleSheet("color: blue;");

    pLabel = patientLabel;


   //connect(this, &StageOneMain::setPushButton, ui->pushButton_logs, &QPushButton::setEnabled);// NEW CONNECT SYNTAX
     connect(&api, &tetra_grip_api::deviceError, this, &StageOneMain::connectionError);
     connect(&api, &tetra_grip_api::tetraGripEvent,this, &StageOneMain::eventHandler);



     QFile f(":/resources/config.txt");
     if(!f.open(QFile::ReadOnly))
          {
              QMessageBox::information(0, "config file error", f.errorString());
          }
     else
          {
              QByteArray config = f.readAll();
              tetra_grip_api::send_long_register(STIM_LONG_REG_STIM_CONFIG_FILE, (size_t)config.length(), (uint8_t*)config.data());
              statusConfig->setText("Config file sent");
             // statusBar()->showMessage("Config file sent", 3000);

          }


      tetra_grip_api::get_battery_percentage();
}

StageOneMain::~StageOneMain()
{
    delete ui;
}


void StageOneMain::on_pushButton_exit_clicked()
{
    QApplication::quit();
    //tetra_grip_api::send_config_file()
}

void StageOneMain::on_pushButton_patients_clicked()
{
     this->close();
     stagetwopatients = new StageTwoPatients(this);
     stagetwopatients -> show();

}

void StageOneMain::on_pushButton_programs_clicked()
{

    hide();
    stageprogram = new stageProgram(this);
    stageprogram -> show();


}

void StageOneMain::eventHandler(STIM_GUI_TOPIC_T topic, uint8_t reg, uint32_t value)
{

    if (topic==TOPIC_STIMULATOR)
    {
        switch(reg)
        {
        case STIM_REG_BATTERY_CAPACITY_REMAINING:

             statusBat->setText("Battery remaining: "+QString::number(value)+"%");
             //QTimer::singleShot(3000, ui->statusbar, &QLabel::hide);

            if(value<86)
                ui->qLed->setOnColor(QLed::Red);
            else
                ui->qLed->setOnColor(QLed::Green);
            ui->qLed->setValue(true);
            break;
        }
    }

}

void StageOneMain::connectionError(bool val)
{
    if (val)
    {
        ui->qLed->setShape(QLed::Triangle);
        ui->qLed->setOnColor(QLed::Red);
    }
}

void StageOneMain::on_pushButton_help_clicked()
{
    emit textToChange("text updated");
    emit setPushButton(true);
   // emit deviceError(true);
}
