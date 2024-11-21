#include "stageonemain.h"
#include "stagetwonew.h"
#include "tetra_grip_api.h"
#include "tetra_grip_reader.h"
#include "tetra_grip_writer.h"
#include <QApplication>
#include <QMessageBox>
#include <QSerialPort>


tetra_grip_api api;

int main(int argc, char *argv[])
{

   //QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    //qputenv("QT_AUTO_SCREEN_SCALE_FACTOR", "1");
   //qputenv("QT_SCALE_FACTOR", "1.5");
   // qputenv("QT_AUTO_SCREEN_SCALE_FACTOR", QByteArray("0"));
    //QCoreApplication::setAttribute(Qt::AA_DisableHighDpiScaling);
 //   QApplication::setAttribute(Qt::AA_DisableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);


    QApplication a(argc, argv);

    api.openSerialPort();


   // QSerialPort serial_port;

    QObject::connect(api.serial, SIGNAL(readyRead()), &api, SLOT(readData()));
    QObject::connect(api.serial, SIGNAL(error(QSerialPort::SerialPortError)),&api, SLOT(ErrorHandler(QSerialPort::SerialPortError))); // error handling
   // QObject::connect(api.autoConnectionTimer,SIGNAL(timeout()),&api,SLOT(autoconnect()));

    // splash screen
    QPixmap pixmap(":/resources/tetragrip_logo2.png"); // :/icons/checked.png
    QSplashScreen splash(pixmap, Qt::WindowStaysOnTopHint);
    splash.show();
    //splash.showMessage("Stimulator Detected..");
    QTimer::singleShot(5000, &splash, &QWidget::close); // keep displayed for 5 seconds
//    api.tryToAutoconnect = false;


     StageOneMain w(nullptr);
    // w.setAttribute(Qt::WA_DeleteOnClose);

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

//COM 5 : Vendor Identifier: 403 , Product Identifier: 6015  $
//COM 6 : Vendor Identifier: 403 , Product Identifier: 6001 --> debug port  $

//COM 6 : Vendor Identifier: 403 , Product Identifier: 6001 £
//COM 7 : Vendor Identifier: 403 , Product Identifier: 6015 £

//$ --> Old Stim (Orange),  £ ---> Paul's stim (black)

// COM 6 --> is debug port with settings baud 1M, hardware flow control RTS/CTS...rown/col (40/90) newline mode
