#include "stageprogram.h"
#include "ui_stageprogram.h"
#include "currentbuttonone.h"
#include "tetra_grip_api.h"
#include "tetra_grip_reader.h"
#include <QDebug>
#include <QDomDocument>
#include <QFile>
#include "manageconfigfile.h"


stageProgram::stageProgram(QString patientLabel, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::stageProgram)
{
    ui->setupUi(this);
    ui->pushButton_currOnOne->setCheckable(true);
    ui->pushButton_currOnTwo->setCheckable(true);
    ui->pushButton_currOnThree->setCheckable(true);
    ui->pushButton_currOnFour->setCheckable(true);
    ui->pushButton_currOnFive->setCheckable(true);

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


    pLabel = patientLabel;

    connect(&api, &tetra_grip_api::tetraGripEvent,this, &stageProgram::stimStatusEventHandler);
    connect(&api, &tetra_grip_api::tetraGripSensorEvent,this, &stageProgram::sensorEventHandler);

    connect(ui->widget_currentOne, &CurrentButtonOne::getValue, this, &stageProgram::setCurrOnChannelOne);
    connect(ui->widget_currentTwo, &CurrentButtonOne::getValue, this, &stageProgram::setCurrOnChannelTwo);
    connect(ui->widget_currentThree, &CurrentButtonOne::getValue, this, &stageProgram::setCurrOnChannelThree);
    connect(ui->widget_currentFour, &CurrentButtonOne::getValue, this, &stageProgram::setCurrOnChannelFour);
    connect(ui->widget_currentFive, &CurrentButtonOne::getValue, this, &stageProgram::setCurrOnChannelFive);

    connect(ui->pushButton_currOnOne, &QPushButton::clicked, ui->widget_currentOne, &CurrentButtonOne::setEnabled);
    connect(ui->pushButton_currOnTwo, &QPushButton::clicked, ui->widget_currentTwo, &CurrentButtonOne::setEnabled);
    connect(ui->pushButton_currOnThree, &QPushButton::clicked, ui->widget_currentThree, &CurrentButtonOne::setEnabled);
    connect(ui->pushButton_currOnFour, &QPushButton::clicked, ui->widget_currentFour, &CurrentButtonOne::setEnabled);
    connect(ui->pushButton_currOnFive, &QPushButton::clicked, ui->widget_currentFive, &CurrentButtonOne::setEnabled);

    ui->customPlot->addGraph(); // blue line
    ui->customPlot->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    ui->customPlot->addGraph(); // red line
    ui->customPlot->graph(1)->setPen(QPen(QColor(255, 110, 40)));
    ui->customPlot->addGraph(); // red line
    ui->customPlot->graph(2)->setPen(QPen(QColor(110, 255, 40)));
    ui->customPlot->addGraph();
    ui->customPlot->graph(3)->setPen(QPen(QColor(11, 25, 40)));
    ui->customPlot->graph(3)->setChannelFillGraph(ui->customPlot->graph(2));






    //ui->customPlot->graph(3)->setChannelFillGraph(ui->customPlot->graph(1));

//    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
//    timeTicker->setTimeFormat("%h:%m:%s");
//    customPlot->xAxis->setTicker(timeTicker);
//    customPlot->axisRect()->setupFullAxesBox();
//    customPlot->yAxis->setRange(-1.2, 1.2);

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
    ui->customPlot->graph(3)->setName("Z_test");
   // ui->customPlot->graph(3)->setName("Conf Int, Y");

    ui->customPlot->legend->setVisible(true);
    ui->customPlot->legend->setFont(QFont("Helvetica",9));
    ui->customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom|Qt::AlignRight);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect( ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)),  ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect( ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui-> customPlot->yAxis2, SLOT(setRange(QCPRange)));

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    // connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
    // dataTimer.start(0); // Interval 0 means to refresh as fast as possible

    tetra_grip_api::set_sensor_data_rate(SENSOR_ADDRESS_BROADCAST, 10); // sensor data in 100Hz


}

stageProgram::~stageProgram()
{
    delete ui;
   // tetra_grip_api::stimulation_pause(true);
}


void stageProgram::setCurrOnChannelOne(unsigned int current_uA)
{

   // currentOneSetVal = current_uA;
    tetra_grip_api::stimulation_set_current( m_channelOne, current_uA);

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
//            if(value != currentOneSetVal/10)
//            {
//            ui->label_curr_one->setText("Value mismatch!");
//            tetra_grip_api::stimulation_pause(true);
//            }
            break;
        case 1:

            ui->label_curr_two->setText(QString("%1 mA").arg(value/m_currentmA));
            currentTwoSetVal = value/m_currentmA;
//            if(value != currentTwoSetVal/10)
//            {
//            ui->label_curr_two->setText("Value mismatch!");
//            tetra_grip_api::stimulation_pause(true);
//            }
            break;
        case 2:

            ui->label_curr_three->setText(QString("%1 mA").arg(value/m_currentmA));
            currentThreeSetVal = value/m_currentmA;
//            if(value != currentThreeSetVal/10)
//            {
//            ui->label_curr_three->setText("Value mismatch!");
//            tetra_grip_api::stimulation_pause(true);
//            }
            break;
        case 3:

            ui->label_curr_four->setText(QString("%1 mA").arg(value/m_currentmA));
            currentFourSetVal = value/m_currentmA;
//            if(value != currentFourSetVal/10)
//            {
//            ui->label_curr_four->setText("Value mismatch!");
//            tetra_grip_api::stimulation_pause(true);
//            }
            break;
        case 4:

            ui->label_curr_five->setText(QString("%1 mA").arg(value/m_currentmA));
            currentFiveSetVal = value/m_currentmA;
//            if(value != currentFiveSetVal/10)
//            {
//            ui->label_curr_five->setText("Value mismatch!");
//            tetra_grip_api::stimulation_pause(true);
//            }
            break;
        }

    }
}

void stageProgram::sensorEventHandler(uint8_t index, SENSOR_DATA_T *sample)
{
//    ui->textBrowser->setText("Index = " + QString::number(index) + "\n" +
//                "Acceleration, X: " + QString::number(sample->acceleration_g[0]) + "\n" +
//                "Acceleration, Y: " + QString::number(sample->acceleration_g[1]) + "\n" +
//                "Acceleration, Z: " + QString::number(sample->acceleration_g[2]) + "\n\n" +

//                "Quaternion w: " + QString::number(sample->quaternion[0]) + "\n" +
//                "Quaternion x: " + QString::number(sample->quaternion[1]) + "\n" +
//                "Quaternion y: " + QString::number(sample->quaternion[2]) + "\n" +
//                "Quaternion z: " + QString::number(sample->quaternion[3]));


////   // from http://www.varesano.net/blog/fabio/simple-gravity-compensation-9-dom-imus
//   double q0 = sample->quaternion[0];
//   double q1 = sample->quaternion[1];
//   double q2 = sample->quaternion[2];
//   double q3 = sample->quaternion[3];

//   double a0 = sample->acceleration_g[0];
//   double a1 = sample->acceleration_g[1];
//   double a2 = sample->acceleration_g[2];

////    double a_mag=sqrt(a0*a0 + a1*a1 + a2*a2);
////    a0/=a_mag;
////    a1/=a_mag;
////    a2/=a_mag;

////   double w=q0;
////   q0=q1;
////   q1=q2;
////   q2=q3;
////   q3=w;
//   double q_mag=sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
//  // double q_mag=sqrt(q1*q1 + q2*q2 + q3*q3);
//   q0/=q_mag;
//   q1/=q_mag;
//   q2/=q_mag;
//   q3/=q_mag;
//   double g0 = 2 * (q1 * q3 - q0 * q2);
//   double g1 = 2 * (q0 * q1 + q2 * q3);
//   double g2 = q0 * q0 - q1 * q1 - q2 * q2 + q3 * q3;
//   //double g2 =    2 * (q0 * q0 - 1 + 2 * q3 * q3);
//   gCompensation_x = g0;
//   gCompensation_y = g1;
//   gCompensation_z = g2;



////   double rotation[] = {
////       2*(q0*q0)-1+ 2*(q1*q1), 2*(q1*q2 + q0*q3), 2*(q1*q3 - q0*q2),
////       2*(q1*q2 - q0*q3), 2*(q0*q0)-1+ 2*(q2*q2), 2*(q2*q3 + q0*q1),
////       2*(q1*q3 + q0*q2), 2*(q2*q3 - q0*q1), 2*(q0*q0)-1+ 2*(q3*q3)
////   };
////   QGenericMatrix<3,3,double> R_matrix(rotation);
////   //qDebug() <<  " th Matrix is:"<< R_matrix;

////   double a[] = {
////       a0, a1, a2
////   };
////   QGenericMatrix<1,3,double> A_matrix(a);
////  // qDebug() <<  " the Acceleration Matrix is:"<< A_matrix;

////   double g[] = {
////       0, 0, -1
////   };
////    QGenericMatrix<1,3,double> G_matrix(g);
////    QGenericMatrix<1,3,double> Totmatrix = (R_matrix*A_matrix)+G_matrix;
//    //qDebug() << "TOTMATRIX" << Totmatrix;

//    QQuaternion quat(q0,q1,q2,q3);
//    QVector3D acc(a0,a1,a2);
//    QVector3D gg(0,0,-1.0);

//    QVector3D vpost = quat.rotatedVector(acc);
//    QVector3D res = vpost;
//    QQuaternion quatinv= quat.inverted();
//    QVector3D vpostt = quatinv.rotatedVector(res);
//  // qDebug()<< "The index" << res.x();





    realtimeDataSlot(sample->acceleration_g[0]-gCompensation_x, sample->acceleration_g[1]-gCompensation_y, sample->acceleration_g[2]-gCompensation_z, sample->acceleration_g[2]);
   // realtimeDataSlot(a0-gCompensation_x, a1-gCompensation_y, a2-gCompensation_z, sample->acceleration_g[2]);
   //  realtimeDataSlot(Totmatrix(0,0), Totmatrix(1,0),Totmatrix(2,0), sample->acceleration_g[2]);
   // realtimeDataSlot(vpostt.x(), vpostt.y(), vpostt.z(), sample->acceleration_g[2]);
   // realtimeDataSlot(res.x(), res.y(), res.z(), sample->acceleration_g[2]);

}
void stageProgram::realtimeDataSlot(double x_acceleration_g, double y_acceleration_g, double z_acceleration_g, double z_acceleration_gnew)
{
    static QTime time(QTime::currentTime());
    // calculate two new data points:
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;
    if (key-lastPointKey > 0.02) // at most add point every 2 ms
    {
      // add data to lines:
    //    ui->customPlot->graph(0)->addData(key, qSin(key)+qrand()/(double)RAND_MAX*1*qSin(key/0.3843));
    //    ui->customPlot->graph(1)->addData(key, qCos(key)+qrand()/(double)RAND_MAX*0.5*qSin(key/0.4364));
    //    ui->customPlot->graph(2)->addData(key, qCos(key)+qrand()/(double)RAND_MAX*0.5*qSin(key/0.5364));
        ui->customPlot->graph(0)->addData(key, x_acceleration_g);
        ui->customPlot->graph(1)->addData(key, y_acceleration_g);
        ui->customPlot->graph(2)->addData(key, z_acceleration_g);

        ui->customPlot->graph(3)->addData(key, z_acceleration_gnew);

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
    QString StyleSheetOn("QRadioButton::indicator {width: 25px; height: 25px; border-radius: 12px;} QRadioButton::indicator:unchecked { background-color: lime; border: 2px solid gray;}");
    QString StyleSheetOff("QRadioButton::indicator {width: 25px; height: 25px; border-radius: 12px;} QRadioButton::indicator:unchecked { background-color: red; border: 2px solid gray;}");

    double a_sum = sqrt(y_acceleration_g*y_acceleration_g+x_acceleration_g*x_acceleration_g+z_acceleration_g*z_acceleration_g);

    if(a_sum > 0.5 && gCompensation_y == 1 && y_acceleration_g > 0)
    //if(a_sum > 0.5 && gCompensation_y==1)
        //ui->btn_twitch->setStyleSheet("background:red;");
    {
    ui->rdo_btn->show();
    setStyleSheet(StyleSheetOn);
    }

    else
    {
        //ui->btn_twitch->setStyleSheet("");
    setStyleSheet(StyleSheetOff);
    }



//    if(y_acceleration_g>0.25  && key > 0.02)
//    {
//        pCount++;
//        ui->btn_twitch->setStyleSheet("background:red;");
//    }
//    else
//        ui->btn_twitch->setStyleSheet("");

//    ui->lbl_peak->setText("Twitch count: "+QString::number(pCount));
}

void stageProgram::on_pushButton_programKeyGrip_clicked()
{
   ManageConfigFile configFile;
   configFile.keyGripTest(pLabel);

   this->hide();
   keygripv2 = new ProgramKeyGripV2(pLabel);
   keygripv2 -> show();
}

void stageProgram::on_pushButton_programPalmerGrasp_clicked()
{
    this->hide();
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

void stageProgram::on_pushButton_reset_clicked()
{
    gCompensation_x = 0;
    gCompensation_y = 0;
    gCompensation_z = 0;
}

void stageProgram::on_btn_gx_clicked()
{
    gCompensation_x = 1;
}

void stageProgram::on_btn_gy_clicked()
{
    gCompensation_y = 1;
}

void stageProgram::on_btn_gz_clicked()
{
    gCompensation_z = 1;
}
