#include "stageonemain.h"
#include "ui_stageonemain.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QFile>
#include <QTextStream>
#include "tetra_grip_api.h"
#include "tetra_grip_reader.h"

StageOneMain::StageOneMain(QWidget *parent) : QMainWindow(parent)
    , ui(new Ui::StageOneMain)
{
    ui->setupUi(this);
    this->setStyleSheet("background-color: white;");
    this->setFixedSize(this->width(),this->height());
    statusConfig = new QLabel(this);
    statusBat = new QLabel(this);
    statusBat->setAlignment(Qt::AlignRight);
    ui->statusbar->addPermanentWidget(statusConfig);
    ui->statusbar->addPermanentWidget(statusBat,1);

    // initial appearnece of main window
    ui->label_pid->setText("Set up new patient");
    ui->pushButton_programs->setEnabled(false);





    // connect(this, SIGNAL(textToChange(QString)), this->ui->label_pid, SLOT(setText(QString))); // dummy label to test raw serial data
     connect(this, &StageOneMain::textToChange, ui->label_pid, &QLabel::setText);
     connect(this, &StageOneMain::setPushButton, ui->pushButton_logs, &QPushButton::setEnabled);// NEW CONNECT SYNTAX
     //connect(&api, SIGNAL(deviceError(bool)), this->ui->qLed, SLOT(setShape(QLed::Triangle)));//
     connect(this, SIGNAL(deviceError(bool)), this, SLOT(connectionError(bool)));
     connect(&api, SIGNAL(deviceError(bool)), this, SLOT(connectionError(bool)));
     connect(&api, SIGNAL(tetraGripEvent(STIM_GUI_TOPIC_T, uint8_t, uint32_t )), this, SLOT(eventHandler(STIM_GUI_TOPIC_T , uint8_t , uint32_t )));



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
//test git

      tetra_grip_api::battery_percentage();
}

StageOneMain::~StageOneMain()
{
    delete ui;
}

//extern "C" size_t StageOneMain::send_using_qtserial(uint8_t *data, size_t len)
//{
//    //return serial->write((const char *)data, (qint64)len);
//}

void StageOneMain::on_pushButton_exit_clicked()
{
    QApplication::quit();
    //tetra_grip_api::send_config_file()
}

void StageOneMain::on_pushButton_patients_clicked()
{

    hide();
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
