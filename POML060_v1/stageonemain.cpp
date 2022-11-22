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
        ui->pushButton_logs->setEnabled(false); // shoulder control
        ui->pushButton_help->setEnabled(false); // stimultor logs

    }

    ui->label_pid->setText(patientLabel);
    ui->label_pid->setAlignment(Qt::AlignCenter);
    ui->label_pid->setStyleSheet("color: blue;");

    ui->pushButton_patients->setToolTip("To setup patient");
    ui->pushButton_programs->setToolTip("To setup stimulation current, frequency and grip profiles ");
    ui->pushButton_logs->setToolTip("To setup shoulder control sensor");
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
    emit textToChange("text updated");
    emit setPushButton(true);
   // emit deviceError(true);


    QString namestring = "log_file_"+pLabel;
    QString filename = "logfilename.txt";
    QString newfilename = "log_file_"+pLabel+".txt";
   // QString newfilename = QCoreApplication::applicationDirPath()+"/data/log_file/"+namestring+".txt";

    QFile originalFile(filename), newFile(newfilename);

    if(!originalFile.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        qDebug() << "Error opening original log file: "<<originalFile.errorString();
    }



    QTextStream instream(& originalFile);







//    while(!instream.atEnd())
//    {

//        QString line = instream.readLine();
//        qDebug()<<line;
//        outstream<<line<<'\n';

//    }


    QRegExp rx("(\\ |\\,|\\.|\\:|\\t)");
    QRegExp regExp_int (QLatin1Literal("[^0-9]+")); // reg ex to extract integer from a string



    if(newFile.open(QIODevice::WriteOnly|QIODevice::Text))
    {

        QTextStream outstream(& newFile);

        outstream<<"Date and Time of last log access: "<<QDateTime::currentDateTime().toString()<<"\n\n"; // to add date and time

        int linecount_1 = 0;

        while(linecount_1<6){

            QString line = instream.readLine();

            if(linecount_1== 0)
            {
                outstream <<line<< '\n';
            }
            if(linecount_1== 1)
            {
                outstream <<line<< '\n';
            }
            if(linecount_1== 2)
            {

                outstream <<line<< '\n';
            }
            if(linecount_1== 3)
            {
                outstream <<line<< '\n';
            }
            if(linecount_1== 4)
            {
                QStringList slist = line.split(regExp_int);
                qDebug()<<"Integer part of line 4"<<slist;
                int totTime = slist[1].toInt();

                if(totTime > 60 && totTime  < 3600)
                {
                    slist[1] =  QString::number(slist[1].toInt()/(60));
                    QString newline = "Power-on time: " + slist[1] +"min";
                    qDebug() << "newline: " << newline;
                    outstream <<newline<< '\n';
                }
                else if(totTime  > 3600)
                {
                    slist[1] =  QString::number(slist[1].toInt()/(60*60));
                    QString newline = "Power-on time: " + slist[1]+"hrs";
                    qDebug() << "newline: " << newline;
                    outstream <<newline<< '\n';
                }
                else
                {
                    QString newline = "Power-on time: " + slist[1] +"s";
                    qDebug() << "newline: " << newline;
                    outstream <<newline<< '\n';
                }



            }
            if(linecount_1== 5)
            {
                outstream <<line<< '\n';
            }


            ++linecount_1;
        }

        int nb_line = 6;
        while(!instream.atEnd())
        {

            QString line = instream.readLine();


            if(nb_line == 6 )
            {

                QString newline = "Phase\t\t\tCount\tMin time (s)\tMax time (s)\tTotal time (s)";
                outstream <<newline<< '\n';
            }


            if(nb_line == 7 )
            {
                QStringList slist = line.split(rx);
                if(slist.size() >4) {
                    slist[0]= "Rest\t\t";
                    slist[2]= QString::number(slist[2].toInt()/(1000));
                    slist[3]= QString::number(slist[3].toInt()/(1000));
                    slist[4]= QString::number(slist[4].toInt()/(1000));
                    QString newline = slist.join("\t");
                    outstream <<newline<< '\n';
                } else {
                    qDebug() << "Problem with string" << slist << line;
                }

            }


            if(nb_line == 8 )
            {
                QStringList slist = line.split(rx);
                if(slist.size() >4) {
                    slist[0]= "Hand open (Key grip)\t";
                    slist[2]= QString::number(slist[2].toInt()/(1000));
                    slist[3]= QString::number(slist[3].toInt()/(1000));
                    slist[4]= QString::number(slist[4].toInt()/(1000));
                    QString newline = slist.join("\t");
                    outstream <<newline<< '\n';
                } else {
                    qDebug() << "Problem with string" << slist << line;
                }

            }

            if(nb_line == 9 )
            {
                QStringList slist = line.split(rx);
                if(slist.size() >4) {
                    slist[0]= "Finger Flexion (Key grip)\t";
                    slist[2]= QString::number(slist[2].toInt()/(1000));
                    slist[3]= QString::number(slist[3].toInt()/(1000));
                    slist[4]= QString::number(slist[4].toInt()/(1000));
                    QString newline = slist.join("\t");
                    //    ui->textBrowser->append(newline);
                    outstream <<newline<< '\n';
                } else {
                    qDebug() << "Problem with string" << slist << line;
                }

            }

            if(nb_line == 10 )
            {
                QStringList slist = line.split(rx);
                if(slist.size() >4) {
                    slist[0]= "Grip Phase (Key grip)\t";
                    slist[2]= QString::number(slist[2].toInt()/(1000));
                    slist[3]= QString::number(slist[3].toInt()/(1000));
                    slist[4]= QString::number(slist[4].toInt()/(1000));

                    QString newline = slist.join("\t");
                    //   ui->textBrowser->append(newline);
                    outstream <<newline<< '\n';
                } else {
                    qDebug() << "Problem with string" << slist << line;
                }



            }

            if(nb_line == 11 )
            {

                QStringList slist = line.split(rx);
                if(slist.size() >4) {
                    slist[0]= "Release Object (Key grip)\t";
                    slist[2]= QString::number(slist[2].toInt()/(1000));
                    slist[3]= QString::number(slist[3].toInt()/(1000));
                    slist[4]= QString::number(slist[4].toInt()/(1000));
                    QString newline = slist.join("\t");
                    //   ui->textBrowser->append(newline);
                    outstream <<newline<< '\n';
                } else {
                    qDebug() << "Problem with string" << slist << line;
                }


            }

            if(nb_line == 12 )
            {
                QStringList slist = line.split(rx);
                if(slist.size() >4) {
                    slist[0]= "Hand open (Palmar Grasp)\t";
                    slist[2]= QString::number(slist[2].toInt()/(1000));
                    slist[3]= QString::number(slist[3].toInt()/(1000));
                    slist[4]= QString::number(slist[4].toInt()/(1000));
                    QString newline = slist.join("\t");
                    //   ui->textBrowser->append(newline);
                    outstream <<newline<< '\n';
                } else {
                    qDebug() << "Problem with string" << slist << line;
                }
            }
            if(nb_line == 13 )
            {
                QStringList slist = line.split(rx);
                if(slist.size() >4) {
                    slist[0]= "Finger Flexion (Palmar Grasp)\t";
                    slist[2]= QString::number(slist[2].toInt()/(1000));
                    slist[3]= QString::number(slist[3].toInt()/(1000));
                    slist[4]= QString::number(slist[4].toInt()/(1000));
                    QString newline = slist.join("\t");
                    //   ui->textBrowser->append(newline);
                    outstream <<newline<< '\n';
                } else {
                    qDebug() << "Problem with string" << slist << line;
                }
            }
            if(nb_line == 14 )
            {
                QStringList slist = line.split(rx);
                if(slist.size() >4) {
                    slist[0]= "Grip Phase (Palmar Grasp)\t";
                    slist[2]= QString::number(slist[2].toInt()/(1000));
                    slist[3]= QString::number(slist[3].toInt()/(1000));
                    slist[4]= QString::number(slist[4].toInt()/(1000));
                    QString newline = slist.join("\t");
                    //   ui->textBrowser->append(newline);
                    outstream <<newline<< '\n';
                } else {
                    qDebug() << "Problem with string" << slist << line;
                }


            }
            if(nb_line == 15 )
            {
                QStringList slist = line.split(rx);
                if(slist.size() >4) {
                    slist[0]= "Release Object (Palmar Grasp)\t";
                    slist[2]= QString::number(slist[2].toInt()/(1000));
                    slist[3]= QString::number(slist[3].toInt()/(1000));
                    slist[4]= QString::number(slist[4].toInt()/(1000));
                    QString newline = slist.join("\t");
                    //   ui->textBrowser->append(newline);
                    outstream <<newline<< '\n';
                } else {
                    qDebug() << "Problem with string" << slist << line;
                }
            }
            if(nb_line == 16 )
            {
                QStringList slist = line.split(rx);
                if(slist.size() >4) {
                    slist[0]= "Intensity (Key Grip)\t";
                    slist[2]= QString::number(slist[2].toInt()/(1000));
                    slist[3]= QString::number(slist[3].toInt()/(1000));
                    slist[4]= QString::number(slist[4].toInt()/(1000));
                    QString newline = slist.join("\t");
                    //   ui->textBrowser->append(newline);
                    outstream <<newline<< '\n';
                } else {
                    qDebug() << "Problem with string" << slist << line;
                }
            }
            if(nb_line == 17 )
            {
                QStringList slist = line.split(rx);
                if(slist.size() >4) {
                    slist[0]= "Intensity (Palmar Grasp)\t";
                    slist[2]= QString::number(slist[2].toInt()/(1000));
                    slist[3]= QString::number(slist[3].toInt()/(1000));
                    slist[4]= QString::number(slist[4].toInt()/(1000));
                    QString newline = slist.join("\t");
                    //   ui->textBrowser->append(newline);
                    outstream <<newline<< '\n';
                } else {
                    qDebug() << "Problem with string" << slist << line;
                }
            }
            if(nb_line == 18 )
            {
                QStringList slist = line.split(rx);
                if(slist.size() >4) {
                    slist[0]= "Up (Key Grip)\t\t";
                    slist[2]= QString::number(slist[2].toInt()/(1000));
                    slist[3]= QString::number(slist[3].toInt()/(1000));
                    slist[4]= QString::number(slist[4].toInt()/(1000));
                    QString newline = slist.join("\t");
                    //   ui->textBrowser->append(newline);
                    outstream <<newline<< '\n';
                } else {
                    qDebug() << "Problem with string" << slist << line;
                }
            }
            if(nb_line == 19 )
            {
                QStringList slist = line.split(rx);
                if(slist.size() >4) {
                    slist[0]= "Down (Key Grip)\t\t";
                    slist[2]= QString::number(slist[2].toInt()/(1000));
                    slist[3]= QString::number(slist[3].toInt()/(1000));
                    slist[4]= QString::number(slist[4].toInt()/(1000));
                    QString newline = slist.join("\t");
                    //  ui->textBrowser->append(newline);
                    outstream <<newline<< '\n';
                } else {
                    qDebug() << "Problem with string" << slist << line;
                }
            }
            if(nb_line == 20 )
            {
                QStringList slist = line.split(rx);
                if(slist.size() >4) {
                    slist[0]= "Up (Palmar Grasp)\t";
                    slist[2]= QString::number(slist[2].toInt()/(1000));
                    slist[3]= QString::number(slist[3].toInt()/(1000));
                    slist[4]= QString::number(slist[4].toInt()/(1000));
                    QString newline = slist.join("\t");
                    //  ui->textBrowser->append(newline);
                    outstream <<newline<< '\n';
                } else {
                    qDebug() << "Problem with string" << slist << line;
                }
            }
            if(nb_line == 21 )
            {
                QStringList slist = line.split(rx);
                if(slist.size() >4) {
                    slist[0]= "Down (Palmar Grasp)\t";
                    slist[2]= QString::number(slist[2].toInt()/(1000));
                    slist[3]= QString::number(slist[3].toInt()/(1000));
                    slist[4]= QString::number(slist[4].toInt()/(1000));
                    QString newline = slist.join("\t");
                    //  ui->textBrowser->append(newline);
                    outstream <<newline<< '\n';
                } else {
                    qDebug() << "Problem with string" << slist << line;
                }
            }

            if(nb_line == 22 )
            {
                QStringList slist = line.split(rx);
                if(slist.size() >4) {
                    slist[0]= "Exercise phase\t\t";
                    slist[2]= QString::number(slist[2].toInt()/(1000));
                    slist[3]= QString::number(slist[3].toInt()/(1000));
                    slist[4]= QString::number(slist[4].toInt()/(1000));
                    QString newline = slist.join("\t");
                    outstream <<newline<< '\n';
                } else {
                    qDebug() << "Problem with string" << slist << line;
                }
                // ui->textBrowser->append(newline);


            }


            ++nb_line;
        }




        originalFile.close();
        newFile.close();
        //originalFile.remove();
    }

      this->close();
      logwindow = new StimulatorLogWindow(pLabel, nullptr);
      logwindow -> setAttribute(Qt::WA_DeleteOnClose);
      logwindow -> show();

}

void StageOneMain::on_pushButton_logs_clicked()
{
    disconnect(&api, &tetra_grip_api::deviceError, this, &StageOneMain::connectionError);
    disconnect(&api, &tetra_grip_api::tetraGripEvent,this, &StageOneMain::eventHandler);
    this->close();
    window = new ShoulderControl(pLabel, nullptr);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window -> show();
}
