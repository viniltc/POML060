#include "stageprogram.h"
#include "ui_stageprogram.h"
#include "currentbuttonone.h"
#include "stageonemain.h"
#include "tetra_grip_api.h"
#include "tetra_grip_reader.h"
#include <QDebug>
#include <QDomDocument>
#include <QFile>
#include <QCloseEvent>
#include "manageconfigfile.h"


stageProgram::stageProgram(QString patientLabel, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::stageProgram),
    watch(new Stopwatch())
{
    ui->setupUi(this);
    ui->pushButton_currOnOne->setCheckable(true);
    ui->pushButton_currOnTwo->setCheckable(true);
    ui->pushButton_currOnThree->setCheckable(true);
    ui->pushButton_currOnFour->setCheckable(true);
    ui->pushButton_currOnFive->setCheckable(true);

    ui->tabWidget->setCurrentWidget(ui->tab);

    ui->label_curr_one->setText(QString("%1 mA").arg(0));
    ui->label_curr_two->setText(QString("%1 mA").arg(0));
    ui->label_curr_three->setText(QString("%1 mA").arg(0));

    ui->label_curr_four->setText(QString("%1 mA").arg(0));
    ui->label_curr_five->setText(QString("%1 mA").arg(0));


    ui->widget_currentOne->setEnabled(false);
    ui->widget_currentTwo->setEnabled(false);
    ui->widget_currentThree->setEnabled(false);
    ui->widget_currentFour->setEnabled(false);
    ui->widget_currentFive->setEnabled(false);

    ui->radioButton->setChecked(true);

    ui->label_pLabel->setText(patientLabel);
    ui->label_pLabel->setAlignment(Qt::AlignCenter);
    ui->label_pLabel->setStyleSheet("color: blue;");


    StyleSheetOn = "border: 1px solid #6593cf; border-radius: 2px; padding: 5px 15px 2px 5px;"
                   "background: qlineargradient(x1 : 0, y1 : 0, x2 : 0, y2 : 1, "
                         "stop : 0.0 #32cd32,stop : 0.5 #1e7b1e, stop : 0.55 #28a428, stop : 1.0 #46d246)";
    StyleSheetOff = "border: 1px solid #6593cf; border-radius: 2px; padding: 5px 15px 2px 5px;"
                          "background: qlineargradient(x1 : 0, y1 : 0, x2 : 0, y2 :   1, stop :   0.0 #f5f9ff,"
                                  "stop :   0.5 #c7dfff, stop :   0.55 #afd2ff, stop :   1.0 #c0dbff);"
                          "color: #0000;";


    pLabel = patientLabel;


    connect(&api, &tetra_grip_api::tetraGripEvent,this, &stageProgram::stimStatusEventHandler);
    //connect(&api, &tetra_grip_api::tetraGripSensorEvent,this, &stageProgram::sensorEventHandler);

    QDomDocument document;

    QString configfilename = "config_keygrip_test_"+pLabel;
    QString xmlName = pLabel;

    QString xmlReadPath = QCoreApplication::applicationDirPath()+"/data/"+xmlName+".xml";
   // QString txtWritePath = ":/resources/"+configfilename+".txt";
    QString txtWritePath = QCoreApplication::applicationDirPath()+"/data/config_file/"+configfilename+".txt";

    QFile xmlfile(xmlReadPath);

    if(!xmlfile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug () << "Error opening XML file: "<<xmlfile.errorString();

    }
    document.setContent(&xmlfile);
    QDomElement root = document.documentElement();
    xmlfile.close();

    QDomNode CurrentNode = root.elementsByTagName("Current").at(0).firstChild();
    QDomElement CurrentNodeVal = CurrentNode.toElement();
    QDomNode Key_PWNode = root.elementsByTagName("PW_KeyGrip").at(0).firstChild();
    QDomElement Key_PWNodeVal = Key_PWNode.toElement();
    QDomNode Palmer_PWNode = root.elementsByTagName("PW_PalmerGrasp").at(0).firstChild();
    QDomElement Palmer_PWNodeVal = Palmer_PWNode.toElement();
    QDomNode Sensor_SettNode = root.elementsByTagName("Sensor_Settings").at(0).firstChild();
    QDomElement Sensor_SettNodeVal = Sensor_SettNode.toElement();

    if (!CurrentNodeVal.isNull())
    {

        currOneStored = root.elementsByTagName("CH1").at(0).firstChild().nodeValue().toFloat()*1000;
        currTwoStored = root.elementsByTagName("CH2").at(0).firstChild().nodeValue().toFloat()*1000;
        currThreeStored = root.elementsByTagName("CH3").at(0).firstChild().nodeValue().toFloat()*1000;
        currFourStored = root.elementsByTagName("CH4").at(0).firstChild().nodeValue().toFloat()*1000;
        currFiveStored = root.elementsByTagName("CH5").at(0).firstChild().nodeValue().toFloat()*1000;

        ui->widget_currentOne->value = currOneStored;
        ui->widget_currentTwo->value = currTwoStored;
        ui->widget_currentThree->value = currThreeStored;
        ui->widget_currentFour->value = currFourStored;
        ui->widget_currentFive->value = currFiveStored;

        ui->label_curr_one->setText(QString("%1 mA").arg(ui->widget_currentOne->value/m_currentDiv));
        ui->label_curr_two->setText(QString("%1 mA").arg(ui->widget_currentTwo->value/m_currentDiv));
        ui->label_curr_three->setText(QString("%1 mA").arg(ui->widget_currentThree->value/m_currentDiv));
        ui->label_curr_four->setText(QString("%1 mA").arg(ui->widget_currentFour->value/m_currentDiv));
        ui->label_curr_five->setText(QString("%1 mA").arg(ui->widget_currentFive->value/m_currentDiv));

        currentOneSetVal = ui->widget_currentOne->value/m_currentDiv;
        currentTwoSetVal = ui->widget_currentTwo->value/m_currentDiv;
        currentThreeSetVal = ui->widget_currentThree->value/m_currentDiv;
        currentFourSetVal = ui->widget_currentFour->value/m_currentDiv;
        currentFiveSetVal = ui->widget_currentFive->value/m_currentDiv;
    }
    else
    {
        ui->widget_currentOne->value = smallCurrent;
        ui->widget_currentTwo->value = smallCurrent;
        ui->widget_currentThree->value = smallCurrent;
        ui->widget_currentFour->value = smallCurrent;
        ui->widget_currentFive->value = smallCurrent;

        ui->label_curr_one->setText(QString("%1 mA").arg(ui->widget_currentOne->value/m_currentDiv));
        ui->label_curr_two->setText(QString("%1 mA").arg(ui->widget_currentTwo->value/m_currentDiv));
        ui->label_curr_three->setText(QString("%1 mA").arg(ui->widget_currentThree->value/m_currentDiv));
        ui->label_curr_four->setText(QString("%1 mA").arg(ui->widget_currentFour->value/m_currentDiv));
        ui->label_curr_five->setText(QString("%1 mA").arg(ui->widget_currentFive->value/m_currentDiv));

        currentOneSetVal = smallCurrent/m_currentDiv;
        currentTwoSetVal = smallCurrent/m_currentDiv;
        currentThreeSetVal = smallCurrent/m_currentDiv;
        currentFourSetVal = smallCurrent/m_currentDiv;
        currentFiveSetVal = smallCurrent/m_currentDiv;
    }

    if(!Key_PWNodeVal.isNull())
    {
        ui->pushButton->setStyleSheet(StyleSheetOn);
    }
    else
    {
        ui->pushButton->setStyleSheet(StyleSheetOff);
    }

    if(!Palmer_PWNodeVal.isNull())
    {
        ui->pushButton_2->setStyleSheet(StyleSheetOn);
    }
    else
    {
         ui->pushButton_2->setStyleSheet(StyleSheetOff);
    }
    if(!Sensor_SettNodeVal.isNull())
    {
        ui->pushButton_3->setStyleSheet(StyleSheetOn);
    }
    else
    {
         ui->pushButton_3->setStyleSheet(StyleSheetOff);
    }



    //connect(ui->pushButton_currOnOne, &QPushButton::clicked, ui->widget_currentOne, &CurrentButtonOne::setEnabled);
    //connect(ui->pushButton_currOnOne, &QPushButton::clicked, [this](){ ui->widget_currentOne->setEnabled(!ui->widget_currentOne->isEnabled()); });
    connect(ui->pushButton_currOnOne, &QPushButton::clicked, ui->widget_currentOne, &CurrentButtonOne::setEnabled);
    connect(ui->pushButton_currOnOne, &QPushButton::clicked, this, &stageProgram::setZeroCurrOnChannelOne);
    connect(ui->pushButton_currOnTwo, &QPushButton::clicked, ui->widget_currentTwo, &CurrentButtonOne::setEnabled);
    connect(ui->pushButton_currOnTwo, &QPushButton::clicked, this, &stageProgram::setZeroCurrOnChannelTwo);
    connect(ui->pushButton_currOnThree, &QPushButton::clicked, ui->widget_currentThree, &CurrentButtonOne::setEnabled);
    connect(ui->pushButton_currOnThree, &QPushButton::clicked, this, &stageProgram::setZeroCurrOnChannelThree);
    connect(ui->pushButton_currOnFour, &QPushButton::clicked, ui->widget_currentFour, &CurrentButtonOne::setEnabled);
    connect(ui->pushButton_currOnFour, &QPushButton::clicked, this, &stageProgram::setZeroCurrOnChannelFour);
    connect(ui->pushButton_currOnFive, &QPushButton::clicked, ui->widget_currentFive, &CurrentButtonOne::setEnabled);
    connect(ui->pushButton_currOnFive, &QPushButton::clicked, this, &stageProgram::setZeroCurrOnChannelFive);

    connect(ui->widget_currentOne, &CurrentButtonOne::getValue, this, &stageProgram::setCurrOnChannelOne);
    connect(ui->widget_currentTwo, &CurrentButtonOne::getValue, this, &stageProgram::setCurrOnChannelTwo);
    connect(ui->widget_currentThree, &CurrentButtonOne::getValue, this, &stageProgram::setCurrOnChannelThree);
    connect(ui->widget_currentFour, &CurrentButtonOne::getValue, this, &stageProgram::setCurrOnChannelFour);
    connect(ui->widget_currentFive, &CurrentButtonOne::getValue, this, &stageProgram::setCurrOnChannelFive);

    //connect(ui->widget_currentOne, &CurrentButtonOne::myButtonPressed, this, &stageProgram::setZeroCurrOnChannelOne);

//    connect(ui->widget_currentOne, &CurrentButtonOne::getValue,
//            [this](unsigned int current_uA) { if (ui->widget_currentOne->isEnabled())  setCurrOnChannelOne( current_uA) ; else  setZeroCurrOnChannelOne();} );

//    connect(ui->widget_currentOne, &CurrentButtonOne::getValue,
//            [this](unsigned int current_uA) {
//                qDebug() << "widget_currentOne->enabled(): " << ui->widget_currentOne->isEnabled();
//                qDebug() << "current_uA: " << current_uA;
//                setCurrOnChannelOne(ui->widget_currentOne->isEnabled() ? current_uA : 0);
//            } );

    //connect(ui->pushButton_currOnOne_2, &QPushButton::clicked, this,&stageProgram::setZeroCurrOnChannelOne);
    //slot
    connect(ui->pushButton_stimStart, &QPushButton::clicked, this, &stageProgram::startStopTimer);
    connect(ui->pushButton_stimStop, &QPushButton::clicked, this, &stageProgram::resetTimer);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    timer->start(10);



    if(ui->tab)
    {
         //disconnect(&api, &tetra_grip_api::tetraGripSensorEvent,this, &stageProgram::sensorEventHandler);
         tetra_grip_api::set_sensor_data_rate(SENSOR_ADDRESS_BROADCAST, 0);
    }

    else if(ui->tab_3)
    {
       // disconnect(&api, &tetra_grip_api::tetraGripSensorEvent,this, &stageProgram::sensorEventHandler);
        tetra_grip_api::set_sensor_data_rate(SENSOR_ADDRESS_BROADCAST, 0);
    }

    QFile f(":/resources/config_setup.txt");
    if(!f.open(QFile::ReadOnly))
         {
             QMessageBox::information(0, "config file error", f.errorString());
         }
    else
         {
             QByteArray config = f.readAll();
             tetra_grip_api::send_long_register(STIM_LONG_REG_STIM_CONFIG_FILE, (size_t)config.length(), (uint8_t*)config.data());

            // statusBar()->showMessage("Config file sent", 3000);

         }

    // reset all user intensity to 50%
     tetra_grip_api::do_stimulator_command( STIM_COMMAND_RESET_USER_INTENSITIES);
}

stageProgram::~stageProgram()
{
    delete ui;
}


void stageProgram::setCurrOnChannelOne(unsigned int current_uA)
{
    tetra_grip_api::stimulation_set_current( m_channelOne, current_uA);
    tetra_grip_api::set_stimulation_target_pulse_width(m_channelOne,0,180);

}

void stageProgram::setCurrOnChannelTwo(unsigned int current_uA)
{
    //currentTwoSetVal = current_uA;
    tetra_grip_api::stimulation_set_current( m_channelTwo, current_uA);
    tetra_grip_api::set_stimulation_target_pulse_width(m_channelTwo,0,180);
}


void stageProgram::setCurrOnChannelThree(unsigned int current_uA)
{
    //currentThreeSetVal = current_uA;
    tetra_grip_api::stimulation_set_current( m_channelThree, current_uA);
    tetra_grip_api::set_stimulation_target_pulse_width(m_channelThree,0,180);
}

void stageProgram::setCurrOnChannelFour(unsigned int current_uA)
{

   // currentFourSetVal = current_uA;
    tetra_grip_api::stimulation_set_current( m_channelFour, current_uA);
    tetra_grip_api::set_stimulation_target_pulse_width(m_channelFour,0,180);
}

void stageProgram::setCurrOnChannelFive(unsigned int current_uA)
{

   // currentFiveSetVal = current_uA;
    tetra_grip_api::stimulation_set_current( m_channelFive, current_uA);
    tetra_grip_api::set_stimulation_target_pulse_width(m_channelFive,0,180);
}

void stageProgram::setZeroCurrOnChannelOne()
{
    tetra_grip_api::set_stimulation_target_pulse_width(m_channelOne,0,0); // pw to 0 to turn off the stimulation
  //  ui->widget_currentOne->setDisabled(true);
}

void stageProgram::setZeroCurrOnChannelTwo()
{
    tetra_grip_api::set_stimulation_target_pulse_width(m_channelTwo,0,0); // pw to 0 to turn off the stimulation
   // ui->widget_currentTwo->setDisabled(true);
}

void stageProgram::setZeroCurrOnChannelThree()
{
    tetra_grip_api::set_stimulation_target_pulse_width(m_channelThree,0,0); // pw to 0 to turn off the stimulation
   // ui->widget_currentThree->setDisabled(true);
}

void stageProgram::setZeroCurrOnChannelFour()
{
    tetra_grip_api::set_stimulation_target_pulse_width(m_channelFour,0,0); // pw to 0 to turn off the stimulation
   // ui->widget_currentFour->setDisabled(true);
}

void stageProgram::setZeroCurrOnChannelFive()
{
    tetra_grip_api::set_stimulation_target_pulse_width(m_channelFive,0,0); // pw to 0 to turn off the stimulation
    // ui->widget_currentFive->setDisabled(true);
}


void stageProgram::on_pushButton_stimStart_clicked()
{
    tetra_grip_api::stimulation_start(true);

}

void stageProgram::on_pushButton_stimStop_clicked()
{
    tetra_grip_api::stimulation_pause(true);

}

void stageProgram::closeEvent(QCloseEvent *event)
{

       tetra_grip_api::stimulation_pause(true);

}

void stageProgram::stimStatusEventHandler(STIM_GUI_TOPIC_T topic,uint8_t index, uint8_t reg, uint32_t value)
{
    if (topic==TOPIC_STIMULATOR)
    {
        switch(reg)
        {
        case STIM_REG_ACTIVITY_STATUS: // Line no: 714,  stim_gui_protocol_decode.c

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
        case STIM_REG_NUM_SMART_SENSORS:
           if(value==0){
               QMessageBox::critical(this, "Sensor Not Connected!", "Shoulder sensor got disconnected \n\nReconnect the sensor and press Ok to continue");
           }

            break;
        }
    }
   if (topic==TOPIC_CHANNEL && reg==STIM_ENGINE_REG_TARGET_CURRENT) // Line no: 1088,  stim_gui_protocol_decode.c
   {
        switch(index)
        {
        case 0: // channel 1

            ui->label_curr_one->setText(QString("%1 mA").arg(value/m_currentmA));

            if(value/m_currentmA >= 120)
            {
              ui->label_curr_one->setText(QString("MAX"));
            }

            currentOneSetVal = value/m_currentmA;
            break;
        case 1: // channel 2

            ui->label_curr_two->setText(QString("%1 mA").arg(value/m_currentmA));

            if(value/m_currentmA >= 120)
            {
              ui->label_curr_two->setText(QString("MAX"));
            }
            currentTwoSetVal = value/m_currentmA;
            break;
        case 2: //channel 3

            ui->label_curr_three->setText(QString("%1 mA").arg(value/m_currentmA));

            if(value/m_currentmA >= 120)
            {
              ui->label_curr_three->setText(QString("MAX"));
            }

            currentThreeSetVal = value/m_currentmA;
            break;
        case 3: // channel 4

            ui->label_curr_four->setText(QString("%1 mA").arg(value/m_currentmA));

            if(value/m_currentmA >= 120)
            {
              ui->label_curr_four->setText(QString("MAX"));
            }

            currentFourSetVal = value/m_currentmA;
            break;
        case 4: // channle 5

            ui->label_curr_five->setText(QString("%1 mA").arg(value/m_currentmA));

            if(value/m_currentmA >= 120)
            {
              ui->label_curr_five->setText(QString("MAX"));
            }
            currentFiveSetVal = value/m_currentmA;        
            break;
        }

    }
}



void stageProgram::startStopTimer()
{
    if(watch->isRunning()) {
        //ui->startStopButton->setText("Restart");
        watch->pause();
    }
    else {
        //ui->startStopButton->setText("Pause");
        watch->start();
    }

}

void stageProgram::resetTimer()
{
    ui->hundredthsText->setText("00");
    ui->secondsText->setText("00");
    ui->minutesText->setText("00");
    watch->reset();
    QPalette p = ui->secondsText->palette();
    p.setColor(QPalette::Text, Qt::black);
    ui->secondsText->setPalette(p);
    ui->minutesText->setPalette(p);
    ui->hundredthsText->setPalette(p);

}



void stageProgram::onTimeout()
{
    QPalette p = ui->secondsText->palette();
    if(watch->isRunning())
    {
        qint64 time = watch->getTime();
        int h = time / 1000 / 60 / 60;
        int m = (time / 1000 / 60) - (h * 60);
        int s = (time / 1000) - (m * 60);
        int ms = time - ( s + ( m + ( h * 60)) * 60) * 1000;
        int ms_dis = ms / 10;
        if(ms_dis < 10) {
            ui->hundredthsText->setText(QStringLiteral("0%1").arg(ms_dis));
        }
        else {
            ui->hundredthsText->setText(QStringLiteral("%1").arg(ms_dis));
        }
        if(s < 10) {
            ui->secondsText->setText(QStringLiteral("0%1").arg(s));
           // p.setColor(QPalette::Base, Qt::white);
            //ui->secondsText->setPalette(p);
        }
        else {
            ui->secondsText->setText(QStringLiteral("%1").arg(s));

        }
        if(m < 10) {
            ui->minutesText->setText(QStringLiteral("0%1").arg(m));
        }
        else {
            ui->minutesText->setText(QStringLiteral("%1").arg(m));
        }

        if (s==5)
        {
            p.setColor(QPalette::Text, Qt::red);
            ui->secondsText->setPalette(p);
            ui->minutesText->setPalette(p);
            ui->hundredthsText->setPalette(p);


        }

    }

}



void stageProgram::on_pushButton_programKeyGrip_clicked()
{

   QString xmlName = pLabel;
   QString xmlReadPath = QCoreApplication::applicationDirPath()+"/data/"+xmlName+".xml";
   QFile xmlfile(xmlReadPath);

   if(!xmlfile.open(QIODevice::ReadOnly | QIODevice::Text))
   {
       qDebug () << "Error opening XML file: "<<xmlfile.errorString();

   }
   QDomDocument document;
   document.setContent(&xmlfile);
   QDomElement root = document.documentElement();
   xmlfile.close();
   QDomNode PWNode = root.elementsByTagName("PW_KeyGrip").at(0).firstChild();
   QDomElement PWNodeVal = PWNode.toElement();
   QDomNode CurrentNode = root.elementsByTagName("Current").at(0).firstChild();
   QDomElement CurrentNodeVal = CurrentNode.toElement();
   if(!PWNode.isNull())
   {
       ManageConfigFile configFile;
       configFile.keyGripFinal(pLabel);
   }

   else
   {
       ManageConfigFile configFile;
       configFile.keyGripTest(pLabel);
   }

   if(CurrentNode.isNull())
   {
       QMessageBox::information(this, "Current not saved", "Please goto previous tab and set the current levels in all the channels and press save button");
       return;
   }



   tetra_grip_api::set_sensor_data_rate(SENSOR_ADDRESS_BROADCAST, 0);

   //disconnect(&api, &tetra_grip_api::tetraGripEvent,this, &stageProgram::stimStatusEventHandler);
   //disconnect(&api, &tetra_grip_api::tetraGripSensorEvent,this, &stageProgram::sensorEventHandler);

   this->close();
   keygripv2 = new ProgramKeyGripV2(pLabel, nullptr);
   keygripv2->setAttribute(Qt::WA_DeleteOnClose);
   keygripv2 -> show();
}

void stageProgram::on_pushButton_programPalmerGrasp_clicked()
{
    QString xmlName = pLabel;
     QString xmlReadPath = QCoreApplication::applicationDirPath()+"/data/"+xmlName+".xml";
    QFile xmlfile(xmlReadPath);

    if(!xmlfile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug () << "Error opening XML file: "<<xmlfile.errorString();

    }
    QDomDocument document;
    document.setContent(&xmlfile);
    QDomElement root = document.documentElement();
    xmlfile.close();
    QDomNode PWNode = root.elementsByTagName("PW_PalmerGrasp").at(0).firstChild();
    QDomElement PWNodeVal = PWNode.toElement();
    QDomNode CurrentNode = root.elementsByTagName("Current").at(0).firstChild();
    QDomElement CurrentNodeVal = CurrentNode.toElement();
    if(!PWNode.isNull())
    {
        ManageConfigFile configFile;
        configFile.palmerGraspFinal(pLabel);
    }
    else
    {
        ManageConfigFile configFile;
        configFile.palmerGraspTest(pLabel);
    }

    if(CurrentNode.isNull())
    {
        QMessageBox::information(this, "Current not saved", "Please goto previous tab and set the current levels in all the channels and press save button");
        return;
    }

    tetra_grip_api::set_sensor_data_rate(SENSOR_ADDRESS_BROADCAST, 0);

   // disconnect(&api, &tetra_grip_api::tetraGripEvent,this, &stageProgram::stimStatusEventHandler);
  //  disconnect(&api, &tetra_grip_api::tetraGripSensorEvent,this, &stageProgram::sensorEventHandler);

    this->close();
    palmergrasp = new ProgramPalmerGrasp(pLabel,nullptr);
    palmergrasp->setAttribute(Qt::WA_DeleteOnClose);
    palmergrasp->show();
}

void stageProgram::on_pushButton_programOpenHand_clicked()
{
    this->close();
    openhand = new ProgramOpenHand(pLabel);
    openhand->show();
}

void stageProgram::on_pushButton_programSwitchGrasp_clicked()
{
    this->hide();
    switchgrasp = new ProgramSwitchGrasp(pLabel);
    switchgrasp->show();

}

void stageProgram::on_pushButton_stimSave_clicked()
{

  saveToXMLFile();
  saveClicked = true;

}

void stageProgram::saveToXMLFile()
{
    QString filename = pLabel;
    QString path = QCoreApplication::applicationDirPath()+"/data/"+filename+".xml";
    QFile file(path);

    /* QT Append wont work!
     * Open the file read-only, read it all in, close it.
     * Make changes in-memory document.
     * Then open the file for overwrite, write all content, close file. */

    if(!file.open(QIODevice::ReadOnly  | QIODevice::Text))
    {

        QMessageBox::information(this, "Unable to open XML file to read", file.errorString());
        return;
    }

     QDomDocument document;
     document.setContent(&file);
     QDomElement root = document.documentElement();


    file.close();

    QDomElement newCurrentTag = document.createElement(QString("Current"));


    QDomNode CurrentNode = root.elementsByTagName("Current").at(0).firstChild();
    QDomElement CurrentNodeVal = CurrentNode.toElement();


    if (CurrentNodeVal.isNull())
    {
        QDomElement Ch1Tag = document.createElement(QString("CH1"));
        QDomText Ch1Val = document.createTextNode(QString::number(currentOneSetVal));
        Ch1Tag.appendChild(Ch1Val);
        newCurrentTag.appendChild(Ch1Tag);

        QDomElement Ch2Tag = document.createElement(QString("CH2"));
        QDomText Ch2Val = document.createTextNode(QString::number(currentTwoSetVal));
        Ch2Tag.appendChild(Ch2Val);
        newCurrentTag.appendChild(Ch2Tag);

        QDomElement Ch3Tag = document.createElement(QString("CH3"));
        QDomText Ch3Val = document.createTextNode(QString::number(currentThreeSetVal));
        Ch3Tag.appendChild(Ch3Val);
        newCurrentTag.appendChild(Ch3Tag);

        QDomElement Ch4Tag = document.createElement(QString("CH4"));
        QDomText Ch4Val = document.createTextNode(QString::number(currentFourSetVal));
        Ch4Tag.appendChild(Ch4Val);
        newCurrentTag.appendChild(Ch4Tag);

        QDomElement Ch5Tag = document.createElement(QString("CH5"));
        QDomText Ch5Val = document.createTextNode(QString::number(currentFiveSetVal));
        Ch5Tag.appendChild(Ch5Val);
        newCurrentTag.appendChild(Ch5Tag);

        root.appendChild(newCurrentTag);
    }

    else
    {
          QDomElement root = document.documentElement();
          QDomNode SettingsNode = root.namedItem("Current");

          QDomNode cur1 = SettingsNode.namedItem("CH1");
          cur1.firstChild().setNodeValue(QString::number(currentOneSetVal));
          QDomNode cur2 = SettingsNode.namedItem("CH2");
          cur2.firstChild().setNodeValue(QString::number(currentTwoSetVal));
          QDomNode cur3 = SettingsNode.namedItem("CH3");
          cur3.firstChild().setNodeValue(QString::number(currentThreeSetVal));
          QDomNode cur4 = SettingsNode.namedItem("CH4");
          cur4.firstChild().setNodeValue(QString::number(currentFourSetVal));
          QDomNode cur5 = SettingsNode.namedItem("CH5");
          cur5.firstChild().setNodeValue(QString::number(currentFiveSetVal));

    }



    if(!file.open(QIODevice::WriteOnly  | QIODevice::Text))
    {
        qDebug () << "Error saving XML file....";
        QMessageBox::information(this, "Unable to open XML file to write", file.errorString());
        return;
    }

    QTextStream output(&file);
    output << document.toString();
    file.close();

    qDebug()<< "Finished";
}






void stageProgram::on_tabWidget_currentChanged(int index)
{
 if (index == 1)
 {
    // tetra_grip_api::set_sensor_data_rate(SENSOR_ADDRESS_BROADCAST, 20);
   //  connect(&api, &tetra_grip_api::tetraGripSensorEvent,this, &stageProgram::sensorEventHandler);
 }
 else
 {
     tetra_grip_api::set_sensor_data_rate(SENSOR_ADDRESS_BROADCAST, 0);
   //  disconnect(&api, &tetra_grip_api::tetraGripSensorEvent,this, &stageProgram::sensorEventHandler);
 }

// if (index == 2)
// {
//     if(saveClicked == false)
//     {

//       QMessageBox::warning(this,"TetraGrip","Save Current Settings",QMessageBox::Ok	,QMessageBox::NoButton);

//     }
// }

}

void stageProgram::on_tabWidget_tabCloseRequested(int index)
{

}

void stageProgram::on_pushButton_shoulderControl_clicked()
{
//    this->close();
//    window = new ShoulderControl(pLabel);
//    window -> show();

    disconnect(&api, &tetra_grip_api::tetraGripEvent,this, &stageProgram::stimStatusEventHandler);
   // disconnect(&api, &tetra_grip_api::tetraGripSensorEvent,this, &stageProgram::sensorEventHandler);

    this-> close();
    StageOneMain *newPatient = new StageOneMain(pLabel);
   //newPatient->setAttribute(Qt::WA_DeleteOnClose);
    newPatient-> show();
}

void stageProgram::on_pushButton_5_clicked()


{

    ManageConfigFile configFile1;
    configFile1.TetraGripFinal(pLabel);

    disconnect(&api, &tetra_grip_api::tetraGripEvent,this, &stageProgram::stimStatusEventHandler);
    //disconnect(&api, &tetra_grip_api::tetraGripSensorEvent,this, &stageProgram::sensorEventHandler);
    this->close();
    statewindow = new StateTestWindow(pLabel,nullptr);
    statewindow->setAttribute(Qt::WA_DeleteOnClose);
    statewindow -> show();
}

void stageProgram::on_radioButton_3_clicked()
{
    tetra_grip_api::set_stimulation_ramp_rate(m_channelOne, 0, 9000); // 0.5 seconds
    tetra_grip_api::set_stimulation_ramp_rate(m_channelTwo, 0, 9000);
    tetra_grip_api::set_stimulation_ramp_rate(m_channelThree , 0, 9000);
    tetra_grip_api::set_stimulation_ramp_rate(m_channelFour, 0, 9000);
    tetra_grip_api::set_stimulation_ramp_rate(m_channelFive, 0, 9000);
}

void stageProgram::on_radioButton_clicked()
{
    tetra_grip_api::set_stimulation_ramp_rate(m_channelOne, 0, 4500); // 1 second ((180-0)/(40*1))*1000
    tetra_grip_api::set_stimulation_ramp_rate(m_channelTwo, 0, 4500);
    tetra_grip_api::set_stimulation_ramp_rate(m_channelThree, 0, 4500);
    tetra_grip_api::set_stimulation_ramp_rate(m_channelFour, 0, 4500);
    tetra_grip_api::set_stimulation_ramp_rate(m_channelFive, 0, 4500);
}

void stageProgram::on_radioButton_2_clicked()
{
    tetra_grip_api::set_stimulation_ramp_rate(m_channelOne, 0, 2250); // 2seconds
    tetra_grip_api::set_stimulation_ramp_rate(m_channelTwo, 0, 2250);
    tetra_grip_api::set_stimulation_ramp_rate(m_channelThree, 0, 2250);
    tetra_grip_api::set_stimulation_ramp_rate(m_channelFour, 0, 2250);
    tetra_grip_api::set_stimulation_ramp_rate(m_channelFive, 0, 2250);
}
