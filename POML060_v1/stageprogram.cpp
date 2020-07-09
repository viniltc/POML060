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

    ui->label_pLabel->setText(patientLabel);
    ui->label_pLabel->setAlignment(Qt::AlignCenter);
    ui->label_pLabel->setStyleSheet("color: blue;");

    ui->doubleSpinBox_thresold->setSingleStep(0.05);
    ui->doubleSpinBox_thresold->setMinimum(0);
    ui->doubleSpinBox_thresold->setMaximum(1);


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

        currentOneSetVal = smallCurrent;
        currentTwoSetVal = smallCurrent;
        currentThreeSetVal = smallCurrent;
        currentFourSetVal = smallCurrent;
        currentFiveSetVal = smallCurrent;
    }



    //connect(ui->pushButton_currOnOne, &QPushButton::clicked, ui->widget_currentOne, &CurrentButtonOne::setEnabled);
    //connect(ui->pushButton_currOnOne, &QPushButton::clicked, [this](){ ui->widget_currentOne->setEnabled(!ui->widget_currentOne->isEnabled()); });

   // connect(ui->pushButton, &QPushButton::clicked, ui->widget_currentOne, &CurrentButtonOne::disableMe);
    connect(ui->pushButton_currOnOne, &QPushButton::clicked, ui->widget_currentOne, &CurrentButtonOne::setEnabled);
    connect(ui->pushButton_currOnTwo, &QPushButton::clicked, ui->widget_currentTwo, &CurrentButtonOne::setEnabled);
    connect(ui->pushButton_currOnThree, &QPushButton::clicked, ui->widget_currentThree, &CurrentButtonOne::setEnabled);
    connect(ui->pushButton_currOnFour, &QPushButton::clicked, ui->widget_currentFour, &CurrentButtonOne::setEnabled);
    connect(ui->pushButton_currOnFive, &QPushButton::clicked, ui->widget_currentFive, &CurrentButtonOne::setEnabled);

    connect(ui->widget_currentOne, &CurrentButtonOne::getValue, this, &stageProgram::setCurrOnChannelOne);
    connect(ui->widget_currentTwo, &CurrentButtonOne::getValue, this, &stageProgram::setCurrOnChannelTwo);
    connect(ui->widget_currentThree, &CurrentButtonOne::getValue, this, &stageProgram::setCurrOnChannelThree);
    connect(ui->widget_currentFour, &CurrentButtonOne::getValue, this, &stageProgram::setCurrOnChannelFour);
    connect(ui->widget_currentFive, &CurrentButtonOne::getValue, this, &stageProgram::setCurrOnChannelFive);

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



    QPen pen0,pen1, pen2, pen3;
    ui->customPlot->addGraph(); // blue line
    pen0.setWidth(1);
    pen0.setColor(QColor(40, 110, 255));
    ui->customPlot->graph(0)->setPen(pen0);
    ui->customPlot->addGraph(); // red line
    pen1.setWidth(1);
    pen1.setColor(QColor(255, 110, 40));
    ui->customPlot->graph(1)->setPen(pen1);
    ui->customPlot->addGraph(); // red line
    pen2.setWidth(1);
    pen2.setColor(QColor(110, 255, 40));
    ui->customPlot->graph(2)->setPen(pen2);
    ui->customPlot->addGraph();
    pen3.setWidth(1);
    pen3.setColor(QColor(250, 0, 0));
    pen3.setStyle(Qt::DotLine);
    ui->customPlot->graph(3)->setPen(pen3);


    ui->customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
    ui->customPlot->xAxis->setAutoTickStep(false);
    ui->customPlot->xAxis->setTickStep(5);
    ui->customPlot->axisRect()->setupFullAxesBox();
    ui->customPlot->yAxis->setRange(-1.5, 1.5);
    ui->customPlot->xAxis->setLabel("Time (s)");
    ui->customPlot->yAxis->setLabel("Acceleration (g)");
    // for legends
    ui->customPlot->graph(0)->setName("X");
    ui->customPlot->graph(1)->setName("Y");
    ui->customPlot->graph(2)->setName("Z");
    ui->customPlot->graph(3)->setName("Threshold");
   // ui->customPlot->graph(3)->setName("Conf Int, Y");

    ui->customPlot->legend->setVisible(true);
    ui->customPlot->legend->setFont(QFont("Helvetica",9));
    ui->customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom|Qt::AlignRight);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect( ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)),  ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect( ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui-> customPlot->yAxis2, SLOT(setRange(QCPRange)));




    if(ui->tab)
    {
         //disconnect(&api, &tetra_grip_api::tetraGripSensorEvent,this, &stageProgram::sensorEventHandler);
         tetra_grip_api::set_sensor_data_rate(SENSOR_ADDRESS_BROADCAST, 0);
    }
    else if(ui->tab_2)
    {
       // connect(&api, &tetra_grip_api::tetraGripSensorEvent,this, &stageProgram::sensorEventHandler);
        tetra_grip_api::set_sensor_data_rate(SENSOR_ADDRESS_BROADCAST, 20);
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




}

stageProgram::~stageProgram()
{
    delete ui;
   // tetra_grip_api::stimulation_pause(true);
}


void stageProgram::setCurrOnChannelOne(unsigned int current_uA)
{

   // currentOneSetVal = current_uA;
    //ui->label_wi1->setText(QString::number(current_uA));

    tetra_grip_api::stimulation_set_current( m_channelOne, current_uA);
    //tetra_grip_api::stimulation_set_current( m_channelOne, 0);
    //ui->label_11->setText(QString::number(current_uA));


}

void stageProgram::setZeroCurrOnChannelOne()
{

    tetra_grip_api::stimulation_set_current( m_channelOne, 0);
    ui->widget_currentOne->setDisabled(true);
}

void stageProgram::closeEvent(QCloseEvent *event)
{
//    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "TetraGrip",
//                                                                tr("Are you sure want to close TetraGrip Application?\n"),
//                                                                QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
//                                                                QMessageBox::Yes);
//    if (resBtn != QMessageBox::Yes) {
//        event->ignore();
//    } else {
//        event->accept();
       tetra_grip_api::stimulation_pause(true);
//    }
}

void stageProgram::setCurrOnChannelTwo(unsigned int current_uA)
{

    //currentTwoSetVal = current_uA;

    tetra_grip_api::stimulation_set_current( m_channelTwo, current_uA);
}


void stageProgram::setCurrOnChannelThree(unsigned int current_uA)
{

    //currentThreeSetVal = current_uA;
    tetra_grip_api::stimulation_set_current( m_channelThree, current_uA);
}

void stageProgram::setCurrOnChannelFour(unsigned int current_uA)
{

   // currentFourSetVal = current_uA;
    tetra_grip_api::stimulation_set_current( m_channelFour, current_uA);
}

void stageProgram::setCurrOnChannelFive(unsigned int current_uA)
{

   // currentFiveSetVal = current_uA;
    tetra_grip_api::stimulation_set_current( m_channelFive, current_uA);
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
        }
    }
   if (topic==TOPIC_CHANNEL && reg==STIM_ENGINE_REG_TARGET_CURRENT) // Line no: 1088,  stim_gui_protocol_decode.c
   {
        switch(index)
        {
        case 0: // channel 1

            ui->label_curr_one->setText(QString("%1 mA").arg(value/m_currentmA));
            currentOneSetVal = value/m_currentmA;
            break;
        case 1: // channel 2

            ui->label_curr_two->setText(QString("%1 mA").arg(value/m_currentmA));
            currentTwoSetVal = value/m_currentmA;
            break;
        case 2: //channel 3

            ui->label_curr_three->setText(QString("%1 mA").arg(value/m_currentmA));
            currentThreeSetVal = value/m_currentmA;
            break;
        case 3: // channel 4

            ui->label_curr_four->setText(QString("%1 mA").arg(value/m_currentmA));
            currentFourSetVal = value/m_currentmA;
            break;
        case 4: // channle 5

            ui->label_curr_five->setText(QString("%1 mA").arg(value/m_currentmA));
            currentFiveSetVal = value/m_currentmA;        
            break;
        }

    }
}

void stageProgram::sensorEventHandler(uint8_t index, SENSOR_DATA_T *sample)
{
    ////   // from http://www.varesano.net/blog/fabio/simple-gravity-compensation-9-dom-imus
       double q0 = sample->quaternion[0];
       double q1 = sample->quaternion[1];
       double q2 = sample->quaternion[2];
       double q3 = sample->quaternion[3];


       QQuaternion quat; // quaternion value change q0, (-)q1, (-)q2, q3
       quat.setScalar(q0);
       quat.setX(-q1);
       quat.setY(-q2);
       quat.setZ(q3);

       QQuaternion qNormalized = quat.normalized();

       double a0 = sample->acceleration_g[0];
       double a1 = sample->acceleration_g[1];
       double a2 = sample->acceleration_g[2];
       accThreshold = ui->doubleSpinBox_thresold->value();


       QVector3D acc(a0,a1,a2);
       QVector3D gg(0,0,1);

        //QVector3D vpost = qNormalized.rotatedVector(acc);
       QVector3D res = qNormalized.rotatedVector(acc)-gg; //rotate the acceleration from the local body reference frame to the local Earth reference frame and gravity if substracted


       QVector3D rotRes = qNormalized.inverted().rotatedVector(res); // converted it back to the local body frame by taking inverse quaternion


     //realtimeDataSlot(res.x(), res.y(), res.z());
       realtimeDataSlot(rotRes.x(), rotRes.y(), rotRes.z(), accThreshold);
}
void stageProgram::realtimeDataSlot(double x_acceleration_g, double y_acceleration_g, double z_acceleration_g, double threshold)
{
    static QTime time(QTime::currentTime());
    // calculate two new data points:
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;
    if (key-lastPointKey > 0.02) // at most add point every 2 ms
    {

        ui->customPlot->graph(0)->addData(key, x_acceleration_g);
        ui->customPlot->graph(1)->addData(key, y_acceleration_g);
        ui->customPlot->graph(2)->addData(key, z_acceleration_g);
        ui->customPlot->graph(3)->addData(key, threshold);

       // ui->customPlot->graph(3)->addData(key, z_acceleration_gnew);

        // rescale value (vertical) axis to fit the current data:
      //ui->customPlot->graph(0)->rescaleValueAxis();
      //ui->customPlot->graph(1)->rescaleValueAxis(true);
      lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->customPlot->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->customPlot->replot();


    // calculate frames per second:
    static double lastFpsKey;
    static int frameCount;
    ++frameCount;
    if (key-lastFpsKey >2) // average fps over 2 seconds
    {
      ui->statusbar->showMessage(
            QString("%1 FPS, Total Data points: %2")
            .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
            .arg(ui->customPlot->graph(0)->data()->size()+ui->customPlot->graph(1)->data()->size())
            , 0);
      lastFpsKey = key;
      frameCount = 0;

    }

  // twitch detection
    static double lastTwitchKey;
    QString StyleSheetOn("QRadioButton::indicator {width: 25px; height: 25px; border-radius: 12px;} QRadioButton::indicator:unchecked { background-color: lime; border: 2px solid gray;}");
    QString StyleSheetOff("QRadioButton::indicator {width: 25px; height: 25px; border-radius: 12px;} QRadioButton::indicator:unchecked { background-color: red; border: 2px solid gray;}");

    //double a_sum = sqrt(y_acceleration_g*y_acceleration_g+x_acceleration_g*x_acceleration_g+z_acceleration_g*z_acceleration_g);
   // double a_sum = sqrt(y_acceleration_g*y_acceleration_g);

    if(setThreshold && y_acceleration_g> accThreshold &&  y_acceleration_g > 0 && key-lastTwitchKey > 0.5)
      {

        ui->rdo_btn->show();
        setStyleSheet(StyleSheetOn);
//        QElapsedTimer ttime;
//        ttime.start();
        lastTwitchKey = key;
        }


    else
      {
        //ui->btn_twitch->setStyleSheet("");
        setStyleSheet(StyleSheetOff);
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


   ManageConfigFile configFile;
   configFile.keyGripTest(pLabel);
   tetra_grip_api::set_sensor_data_rate(SENSOR_ADDRESS_BROADCAST, 0);

   //disconnect(&api, &tetra_grip_api::tetraGripEvent,this, &stageProgram::stimStatusEventHandler);
   disconnect(&api, &tetra_grip_api::tetraGripSensorEvent,this, &stageProgram::sensorEventHandler);

   this->close();
   keygripv2 = new ProgramKeyGripV2(pLabel);
   keygripv2 -> show();
}

void stageProgram::on_pushButton_programPalmerGrasp_clicked()
{
    ManageConfigFile configFile;
    configFile.palmerGraspTest(pLabel);
    tetra_grip_api::set_sensor_data_rate(SENSOR_ADDRESS_BROADCAST, 0);

    disconnect(&api, &tetra_grip_api::tetraGripEvent,this, &stageProgram::stimStatusEventHandler);
    disconnect(&api, &tetra_grip_api::tetraGripSensorEvent,this, &stageProgram::sensorEventHandler);

    this->close();
    palmergrasp = new ProgramPalmerGrasp(pLabel);
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




void stageProgram::on_pushButton_setThreshold_clicked()
{
    accThreshold = ui->doubleSpinBox_thresold->value();
    setThreshold = true;
}

void stageProgram::on_tabWidget_currentChanged(int index)
{
 if (index == 1)
 {
     tetra_grip_api::set_sensor_data_rate(SENSOR_ADDRESS_BROADCAST, 20);
     connect(&api, &tetra_grip_api::tetraGripSensorEvent,this, &stageProgram::sensorEventHandler);
 }
 else
 {
     tetra_grip_api::set_sensor_data_rate(SENSOR_ADDRESS_BROADCAST, 0);
     disconnect(&api, &tetra_grip_api::tetraGripSensorEvent,this, &stageProgram::sensorEventHandler);
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
    disconnect(&api, &tetra_grip_api::tetraGripSensorEvent,this, &stageProgram::sensorEventHandler);

    this-> close();
    StageOneMain *newPatient = new StageOneMain(pLabel);
   //newPatient->setAttribute(Qt::WA_DeleteOnClose);
    newPatient-> show();
}
