#include "stageonemain.h"
#include "tetra_grip_api.h"
#include "tetra_grip_reader.h"
#include "tetra_grip_writer.h"

#include <QApplication>

tetra_grip_api api;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    api.openSerialPort();
    QObject::connect(api.serial, SIGNAL(readyRead()), &api, SLOT(readData()));
    QObject::connect(api.serial, SIGNAL(error(QSerialPort::SerialPortError)),&api, SLOT(ErrorHandler(QSerialPort::SerialPortError))); // error handling
//    QObject::connect(api.autoConnectionTimer,SIGNAL(timeout()),&api,SLOT(autoconnect()));
//    api.tryToAutoconnect = false;


    StageOneMain w(nullptr);
    w.show();
    return a.exec();
}

/*
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        QString s = QObject::tr("Port: ") + info.portName() + "\n"
                + QObject::tr("Location: ") + info.systemLocation() + "\n"
                + QObject::tr("Description: ") + info.description() + "\n"
                + QObject::tr("Manufacturer: ") + info.manufacturer() + "\n"
                + QObject::tr("Vendor Identifier: ") + (info.hasVendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : QString()) + "\n"
                + QObject::tr("Product Identifier: ") + (info.hasProductIdentifier() ? QString::number(info.productIdentifier(), 16) : QString()) + "\n"
                + QObject::tr("Busy: ") + (info.isBusy() ? QObject::tr("Yes") : QObject::tr("No")) + "\n";
        qDebug()<<s;}
        */

//COM 5 : Vendor Identifier: 403 , Product Identifier: 6015
//COM 6 : Vendor Identifier: 403 , Product Identifier: 6001
