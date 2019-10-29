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


//    connect(api.serial, SIGNAL(readyRead()), &api, SLOT(readData()));
   //   connect(api.serial, SIGNAL(error(QSerialPort::SerialPortError)),&api, SLOT(ErrorHandler(QSerialPort::SerialPortError))); // error handling
 //   connect(api.serial, SIGNAL(readyRead()), this, SLOT(serialReceived())); // dummy label to test raw serial data
     connect(&api, SIGNAL(tetraGripEvent()), this, SLOT(eventHandler()));



//     QFile f("/config.txt");
//     if(!f.open(QFile::ReadOnly))
//          {
//              QMessageBox::information(0, "config file error", f.errorString());
//          }

//      QByteArray config = f.readAll();
//      tetra_grip_api::send_long_register(STIM_LONG_REG_STIM_CONFIG_FILE, (size_t)config.length(), (uint8_t*)config.data());

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
            ui->label->setText("Battery remaining: "+QString::number(value));
            if(value<84)
                ui->qLed->setOnColor(QLed::Red);
            else
                ui->qLed->setOnColor(QLed::Green);
            ui->qLed->setValue(true);
            break;
        }
    }
    //  ui->label->setText(api.serial->readAll());
}
