#include "stageonemain.h"
#include "ui_stageonemain.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QFile>
#include <QTextStream>
#include "tetra_grip_api.h"
#include "tetra_grip_reader.h"
#include <QCloseEvent>




StageOneMain::StageOneMain(QString patientLabel, QWidget *parent) : QMainWindow(parent)
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
        //ui->pushButton_logs->setEnabled(false); // shoulder control
        ui->pushButton_help->setEnabled(false); // stimultor logs

    }

    ui->label_pid->setText(patientLabel);
    ui->label_pid->setAlignment(Qt::AlignCenter);
    ui->label_pid->setStyleSheet("color: blue;");

    ui->pushButton_patients->setToolTip("To setup patient");
    ui->pushButton_programs->setToolTip("To setup stimulation current, frequency and grip profiles ");
 //   ui->pushButton_logs->setToolTip("To setup shoulder control sensor");
    ui->pushButton_help->setToolTip("To view stimulator log");
    ui->pushButton_exit->setToolTip("To exit from the Tetragrip Application");
    ui->qLed->setToolTip("Stimulator Battery and Connection Status LED");

    pLabel = patientLabel;


   //connect(this, &StageOneMain::setPushButton, ui->pushButton_logs, &QPushButton::setEnabled);// NEW CONNECT SYNTAX
     connect(&api, &tetra_grip_api::deviceError, this, &StageOneMain::connectionError);
     connect(&api, &tetra_grip_api::tetraGripEvent,this, &StageOneMain::eventHandler);



     QFile f(":/resources/config_setup.txt");
     if(!f.open(QFile::ReadOnly))
          {
              QMessageBox::information(0, "config file error", f.errorString());
          }
     else
          {
              QByteArray config = f.readAll();
              tetra_grip_api::send_long_register(STIM_LONG_REG_STIM_CONFIG_FILE, (size_t)config.length(), (uint8_t*)config.data());
            //  statusConfig->setText("Config file sent");

          }

      tetra_grip_api::read_long_register(STIM_LONG_REG_LOG_FILE, 0);
      tetra_grip_api::get_battery_percentage();
      tetra_grip_api::get_sensor_status();
      tetra_grip_api::set_sensor_data_rate(SENSOR_ADDRESS_BROADCAST, 0);
}

StageOneMain::~StageOneMain()
{
    delete ui;
    //tetra_grip_api::stimulation_pause(true);
}


void StageOneMain::on_pushButton_exit_clicked()
{
    QApplication::quit();
    //tetra_grip_api::send_config_file()
}

void StageOneMain::on_pushButton_patients_clicked()
{
     this->close();

     stagetwopatients = new StageTwoPatients(nullptr);
     stagetwopatients->setAttribute(Qt::WA_DeleteOnClose);
     stagetwopatients -> show();

}

void StageOneMain::on_pushButton_programs_clicked()
{

   // hide();

    QDomDocument document;


    QString xmlName = pLabel;

    QString xmlReadPath = QCoreApplication::applicationDirPath()+"/data/"+xmlName+".xml";


    QFile xmlfile(xmlReadPath);

    if(!xmlfile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug () << "Error opening XML file: "<<xmlfile.errorString();

    }
    document.setContent(&xmlfile);
    QDomElement root = document.documentElement();
    xmlfile.close();

    QDomNode FrequencyVal_SettNode = root.elementsByTagName("Frequency").at(0).firstChild();
    QDomElement FrequencyVal_SettNodeVal = FrequencyVal_SettNode.toElement();

    if(!FrequencyVal_SettNode.isNull())
    {

        ManageConfigFile newFile;
        newFile.currentTest(pLabel);

    }


    this->close();
    stageprogram = new stageProgram(pLabel, nullptr);
    stageprogram->setAttribute(Qt::WA_DeleteOnClose);
    stageprogram -> show();

//    ClinicalProtocolForm *clinwindow;
//    clinwindow = new  ClinicalProtocolForm(pLabel, nullptr);
//    clinwindow -> setAttribute(Qt::WA_DeleteOnClose);
//    clinwindow -> show();





}

void StageOneMain::eventHandler(STIM_GUI_TOPIC_T topic, uint8_t index, uint8_t reg, uint32_t value)
{



    if (topic==TOPIC_STIMULATOR)
    {      
        switch(reg)
        {
        case STIM_REG_BATTERY_CAPACITY_REMAINING:

             statusBat->setText("Battery remaining: "+QString::number(value)+"%");
             //QTimer::singleShot(3000, ui->statusbar, &QLabel::hide);

            if(value<60)
                ui->qLed->setOnColor(QLed::Red);
            else
                ui->qLed->setOnColor(QLed::Green);
            ui->qLed->setValue(true);
            break;

        case STIM_REG_NUM_SMART_SENSORS:
           if(value==0){
               QMessageBox::critical(this, "Sensor Not Connected!", "Shoulder sensor got disconnected \n\nReconnect the sensor and press Ok to continue");
               return;
           }

            break;
        }
    }

}

void StageOneMain::closeEvent(QCloseEvent *event1)
{

    tetra_grip_api::stimulation_pause(true);


    if(event1->spontaneous()){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::information(this, "TetraGrip", "Are you sure want to quit Tetragrip App?\n\nClick Yes to quit or No to remain in this window",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {

            event1->accept();

        }
        else if(reply == QMessageBox::No) {
            event1->ignore();
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


//    // Define file names
//       QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");
//       QString namestring = "log_file_" + pLabel + "_" + timestamp; // Add timestamp to file name
//       QString originalFilename = "logfilename.txt";

//       // Construct the path for the new file
//       QString saveDir = QCoreApplication::applicationDirPath() + "/data/log file";
//       QDir dir(saveDir);

//       // Ensure the directory exists, create if it doesn't
//       if (!dir.exists()) {
//           if (!dir.mkpath(saveDir)) {
//               qDebug() << "Failed to create directory:" << saveDir;
//               return;
//           }
//       }

//       QString newFilename = saveDir + "/" + namestring + ".txt";

//       QFile originalFile(originalFilename);
//       QFile newFile(newFilename);

//       // Open the original log file
//       if (!originalFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
//           qDebug() << "Error opening original log file: " << originalFile.errorString();
//           return; // Exit if the file can't be opened
//       }

//       // Open the new file for writing
//       if (!newFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
//           qDebug() << "Error creating new log file: " << newFile.errorString();
//           originalFile.close(); // Ensure the original file is closed
//           return; // Exit if the file can't be created
//       }

//       QTextStream instream(&originalFile);
//       QTextStream outstream(&newFile);

//       // Add date and time information to the new file
//       outstream << "Date and Time of last log access: "
//                 << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") << "\n\n";

//       // Copy content from the original file to the new file
//       while (!instream.atEnd()) {
//           QString line = instream.readLine();
//           outstream << line << "\n";
//       }

//       // Close the files
//       originalFile.close();
//       newFile.close();

//       qDebug() << "New log file created successfully: " << newFilename;


      this->close();
      logwindow = new StimulatorLogWindow(pLabel, nullptr);
      logwindow -> setAttribute(Qt::WA_DeleteOnClose);
      logwindow -> show();

}

//void StageOneMain::on_pushButton_logs_clicked()
//{
//    disconnect(&api, &tetra_grip_api::deviceError, this, &StageOneMain::connectionError);
//    disconnect(&api, &tetra_grip_api::tetraGripEvent,this, &StageOneMain::eventHandler);
//    this->close();
//    window = new ShoulderControl(pLabel, nullptr);
//    window->setAttribute(Qt::WA_DeleteOnClose);
//    window -> show();
//}
