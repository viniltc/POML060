#include "tetra_grip_api.h"
#include "tetra_grip_reporter.h"
#include <QDebug>
#include <stdio.h>
#include <stdlib.h>


#define _CRT_SECURE_NO_DEPRECATE

using namespace::std;

tetra_grip_api::tetra_grip_api(QObject *parent) : QObject(parent)
{

//    serial = new QSerialPort();

//    QList <QSerialPortInfo>stim;
//    QSerialPortInfo info;
//    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
//    {
//       if(info.description() == "USB Serial Port" && info.manufacturer() == "FTDI" && QString::number(info.vendorIdentifier(), 16)== "403" && QString::number(info.productIdentifier(), 16)== "6015")
//        {
//           comPortName = info.portName();
//        }
//    }
//    //serial->setPortName("com5");
//    serial->setPortName(comPortName);
//    serial->setBaudRate(1000000); // baudrate 1000000 ..1M
//    serial->setDataBits(QSerialPort::Data8);
//    serial->setParity(QSerialPort::NoParity);
//    serial->setStopBits(QSerialPort::OneStop);
//    serial->setFlowControl(QSerialPort::HardwareControl); //Hardware flow control (RTS/CTS), NoFlowControl, SoftwareControl
//    if(serial->open(QIODevice::ReadWrite))
//    {
//        qDebug()<<"Open port OK";
//    }
//    else
//    {
//        qDebug()<<"Failed to open port. Error code: "<< serial->error() << serial->errorString();
//    }


//    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
//    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)),this, SLOT(ErrorHandler(QSerialPort::SerialPortError))); // error handling

}

void tetra_grip_api::openSerialPort()//Vendor Identifier: 403 , Product Identifier: 6015
{
    serial = new QSerialPort();

    QList <QSerialPortInfo>stim;
    QSerialPortInfo info;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
       if(info.description() == "USB Serial Port" && info.manufacturer() == "FTDI" && QString::number(info.vendorIdentifier(), 16)== "403" && QString::number(info.productIdentifier(), 16)== "6015")
        {
           comPortName = info.portName();
        }
    }
    //serial->setPortName("com5");
    serial->setPortName(comPortName);
    serial->setBaudRate(1000000); // baudrate 1000000 ..1M
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::HardwareControl); //Hardware flow control (RTS/CTS), NoFlowControl, SoftwareControl
    if(serial->open(QIODevice::ReadWrite))
    {
        qDebug()<<"Opend port OK";
    }
    else
    {
        qDebug()<<"Failed to open port. Error code: "<< serial->error() << serial->errorString();
    }


}

void tetra_grip_api::readData()
{


   const QByteArray data = api.serial->readAll();
    //const QByteArray data = serial->readAll();


//    qDebug()<<"\n---------------------------------\n";
//    qDebug()<<"Bytes read len"<< data.length();
//    qDebug()<<"\n Bytes data " << data.toHex();

    STIM_GUI_PROTOCOL_Process_Received_Bytes((uint8_t*)data.data(), (size_t)data.length());
    // STIM_GUI_PROTOCOL_Process_Received_Bytes(static_cast<uint8_t*>(data.data()), static_cast<size_t>(data.length()));

}


void tetra_grip_api::closeSerialPort()
{
    if (serial->isOpen())
        serial->close();

}


void tetra_grip_api::ErrorHandler(QSerialPort::SerialPortError error)
{
    switch(error)
    {
    case QSerialPort::NoError:
        return;
        break;
    case QSerialPort::DeviceNotFoundError:
        break;
    case QSerialPort::PermissionError:
        QMessageBox::warning(0,"title","Warning: Could not connect: Permission Error!!"
                             "Are you sure that you have write permission and no other Program is using this port?",QMessageBox::Ok	,QMessageBox::NoButton);
        break;
    case QSerialPort::OpenError:
        QMessageBox::warning(0,"Serial port","Serial Port Open error!!",QMessageBox::Ok	,QMessageBox::NoButton);
        break;
    case QSerialPort::ParityError:
        break;
    case QSerialPort::FramingError:
        QMessageBox::warning(0,"title","Warning: Probably the baud rates doesn´t match!",QMessageBox::Ok	,QMessageBox::NoButton);
        break;
    case QSerialPort::BreakConditionError:
        break;
    case QSerialPort::WriteError:
        QMessageBox::warning(0,"Serial port","Serial Port Write Error!!",QMessageBox::Ok	,QMessageBox::NoButton);
        qDebug()<<"Serial Port WriteError!!";
        disconnect();
        break;
    case QSerialPort::ReadError:
        QMessageBox::warning(0,"Serial port","Serial Port Read Error!!",QMessageBox::Ok	,QMessageBox::NoButton);
        qDebug()<<"Serial Port ReadError!!";
        disconnect();
        break;
    case QSerialPort::ResourceError:
        QMessageBox::warning(0,"Serial port","Serial Port ResourceError!!",QMessageBox::Ok	,QMessageBox::NoButton);
        qDebug()<<"Serial Port ResourceError!!";
        emit api.deviceError(true);
        //disconnect();
        break;
    case QSerialPort::UnsupportedOperationError:
        break;
    case QSerialPort::UnknownError:
        break;
    default:
        break;
    }
    qDebug()<<"Serial Port error: Number "<<error;
    //serialPort.clearError();
}

bool tetra_grip_api::autoconnect()
{
    QList <QSerialPortInfo>stim;
    QSerialPortInfo info;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        if(info.description() == "USB Serial Port" && info.manufacturer() == "FTDI" && !info.isBusy())
        {
            stim.append(info);
        }
    }
    if(stim.size()>1)
    {
        qWarning()<<"Warning: You tried to autoconnect, but there are more than one Stimulators";
    }
    else if(stim.size() == 1)
    {
        qDebug()<<"try to autoconnect to: "<<stim.at(0).portName();
        qDebug()<<"error status: "<<serial->error();
        return connectTo(stim.at(0).portName());

    }
}

bool tetra_grip_api::connectTo(QString port)
{
    serial->setPortName(port);
    serial->setBaudRate(1000000);
    serial->clearError();
    if(serial->open(QIODevice::ReadWrite))
    {
        serial->clear();
        comPortName = port;
        qDebug()<<"connected to: "<<port;
        emit successfullyConnectedTo(port);
        if(autoConnectionTimer.isActive())
        {
            autoConnectionTimer.stop();
        }
        return true;
    }
    return false;
}

void tetra_grip_api::setAutoconnect(bool value)
{
    tryToAutoconnect = value;
    emit AutoconnectionIsSet(value);
    if(value){autoconnect();}
    if(!serial->isOpen())
    {
        autoConnectionTimer.start();
    }
}


bool tetra_grip_api::send_short_block(STIM_GUI_MESSAGE_S_BLOCK_T *pblock)
{
    printf("\n>>>>>>>>>>>>>>>>>>>>>>>\nSending: ");
    return STIM_GUI_Send_message(STIMULATOR_ADDRESS1, GUI_ADDRESS, pblock);
}

void tetra_grip_api::clear_stim_config(void)
{
    STIM_GUI_MESSAGE_S_BLOCK_T block={};

    uint8_t command=STIM_COMMAND_CLEAR_STIM_CONFIG;

    block.msg_type=WRITE_COMMAND;
    block.topic=TOPIC_STIMULATOR;
    block.index=0;
    block.reg_address=STIM_REG_COMMAND;
    block.data_length=1;
    block.data=&command;
    if(!send_short_block(&block))
        printf("Failed to send the command to clear stim config.\n");
}

void tetra_grip_api::send_long_register(uint8_t reg, uint32_t data_len, uint8_t *data)

{


    STIM_GUI_MESSAGE_L_BLOCK_T block={};

    if(!data)
    {
        printf("Error: null pointer argument to send_long_register().\n");
        return;
    }

    block.msg_type=WRITE_COMMAND;
    block.reg_address=reg;
    block.data_length=data_len;
    block.data=data;
    printf("Writing %d bytes to long register %d: '%*s'...\n", data_len, reg, data_len, data);


    if(STIM_GUI_Send_long_message(STIMULATOR_ADDRESS1, GUI_ADDRESS, &block))
    {
        printf("Write OK\n");
    }
    else
    {
        printf("Write Failed.\n");
    }
}

void tetra_grip_api::send_config_file(QByteArray config, bool nonvolatile)

{
      //  clear_stim_config(); >>>>>>>>>>>   check this

        if(config.length())
        {
            send_long_register((nonvolatile?STIM_LONG_REG_NV_STIM_CONFIG_FILE:STIM_LONG_REG_STIM_CONFIG_FILE), config.length(),(uint8_t *)config.data());
        }
        else
        {
            printf("Could not read any bytes from file.\n");
        }

}

void tetra_grip_api::stimulation_start(bool start)

{
    STIM_GUI_MESSAGE_S_BLOCK_T block={}; // previously it was {0}

    uint8_t en = start;
    block.msg_type=WRITE_COMMAND;
    block.topic=TOPIC_STIMULATOR;
    block.index=0;
    block.reg_address=STIM_REG_ACTIVITY_STATUS;
    block.data_length=1;
    block.data=&en;
    if(!send_short_block(&block))
        printf("Failed to send %s command to stimulator.\n", start?"start":"stop");
    else
        printf("Asked stimulator to %s.\n", start?"start":"stop");
}


void tetra_grip_api::stimulation_pause(bool paused)

{
    STIM_GUI_MESSAGE_S_BLOCK_T block={}; // previously it was {0}

    uint8_t en=!paused;
    block.msg_type=WRITE_COMMAND;
    block.topic=TOPIC_STIMULATOR;
    block.index=0;
    block.reg_address=STIM_REG_ACTIVITY_STATUS;
    block.data_length=1;
    block.data=&en;
    if(!send_short_block(&block))
        printf("Failed to send %s command to stimulator.\n", paused?"pause":"unpause");
    else
        printf("Asked stimulator to %s.\n", paused?"pause":"unpause");
}

void tetra_grip_api::stimulation_set_current(unsigned int channel_number, unsigned int current_uA)

{
    STIM_GUI_MESSAGE_S_BLOCK_T block={}; // previously it was {0}

    block.msg_type=WRITE_COMMAND;
    block.topic=TOPIC_CHANNEL;
    block.index=channel_number;
    block.reg_address=54;
    block.data_length=2;
    uint8_t data_array[2];
    data_array[1]=(current_uA/10) >> 8;
    data_array[0]=(current_uA/10) & 0xFF;
    block.data=&data_array[0];
    if(!send_short_block(&block))
        printf("Failed to set current at channel %d", channel_number);
}



void tetra_grip_api::toggle_pause(void)

{
    bool remote_mode=false;

    static uint8_t paused=true;
    if(!remote_mode)
    {
        paused=!paused;
        stimulation_pause(!paused);
    }
}

void tetra_grip_api::read_stim_status_reg(void)
{
    STIM_GUI_MESSAGE_S_BLOCK_T block={};

    block.msg_type=READ_COMMAND;
    block.topic=TOPIC_STIMULATOR;
    block.index=0;
    block.reg_address=STIM_REG_STATUS;
    block.data_length=1;
    block.data=NULL;
    if(!send_short_block(&block))
    {
        printf("Failed to send the read command for the status register.\n");
    }
}

void tetra_grip_api::battery_percentage(void)
{
    STIM_GUI_MESSAGE_S_BLOCK_T block={};

    block.msg_type=READ_COMMAND;
    block.topic=TOPIC_STIMULATOR;
    block.index=0;
    block.reg_address=STIM_REG_BATTERY_CAPACITY_REMAINING;
    block.data_length=1;
    block.data=NULL;
    if(!send_short_block(&block))
    {
        printf("Failed to send the read command for the status register.\n");
    }
}


void tetra_grip_reporter(STIM_GUI_TOPIC_T topic, uint8_t reg, uint32_t value)
{
    emit api.tetraGripEvent(topic, reg, value);
}
