#include "shouldercontrol.h"
#include "ui_shouldercontrol.h"
#include "stageprogram.h"
#include "stageonemain.h"
#include <QQuaternion>
#include <QDebug>
#include <QMessageBox>
#include <QDomDocument>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QtXml>
#include "manageconfigfile.h"
#include <QSound>
#include "incdecbutton.h"

ShoulderControl::ShoulderControl(QString patientLabel,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShoulderControl)
{
    ui->setupUi(this);
    pLabel = patientLabel;
    //connect(&api, &tetra_grip_api::tetraGripEvent,this, &ShoulderControl::eventHandler);
    //connect(&api, &tetra_grip_api::tetraGripSensorEvent,this, &ShoulderControl::sensorEventHandler);
    connect(&api, &tetra_grip_api::tetraGripSensorFilteredEvent,this, &ShoulderControl::sensorFilteredEventHandler);
    ui->doubleSpinBox_vertical->setSingleStep(0.01);
    ui->doubleSpinBox_protraction->setSingleStep(0.01);
    ui->doubleSpinBox_retraction->setSingleStep(0.01);
    ui->spinBox_twotwitch->setValue(2000);

    ui->textBrowser->setVisible(false);


    ui->label_pid->setText(patientLabel);
    ui->label_pid->setAlignment(Qt::AlignCenter);
    ui->label_pid->setStyleSheet("color: blue;");

    ui->pushButton_nosound -> setIcon(QIcon(":/resources/sound.png"));
    ui->pushButton_nosound -> setIconSize(QSize(18, 18));
    ui->pushButton_settings -> setIcon(QIcon(":/resources/settings.png"));
    ui->pushButton_settings -> setIconSize(QSize(18, 18));

    connect(ui->pushButton_nosound, &QPushButton::clicked, this, &ShoulderControl::noSoundBtn);

    //connect(ui->widget, &IncDecButton::getValue, this, &ShoulderControl::thresholdSet1);

//    ui->lineEdit_q->setText("0.707"); //Q default value
//    ui->lineEdit_db->setText("10"); // Db default value
    //ui->radioButton;

    //tetra_grip_api::get_battery_percentage();
    tetra_grip_api::set_sensor_data_rate(SENSOR_ADDRESS_BROADCAST, 30);
    ui->progressBar_protraction->setRange(0, 0.5*100);
    ui->progressBar_vertical->setRange(0, 0.5*100);
    ui->progressBar_retraction->setRange(0, 0.5*100);

    StyleSheetOn1 = "QRadioButton::indicator {width: 25px; height: 25px; border-radius: 12px;} QRadioButton::indicator:unchecked { background-color: lime; border: 2px solid gray;}";
    StyleSheetOn2 = "QRadioButton::indicator {width: 25px; height: 25px; border-radius: 12px;} QRadioButton::indicator:unchecked { background-color: lime; border: 2px solid gray;}";
    StyleSheetOff = "QRadioButton::indicator {width: 25px; height: 25px; border-radius: 12px;} QRadioButton::indicator:unchecked { background-color: red; border: 2px solid gray;}";
    StyleSheetTwoTwitchOn = "QRadioButton::indicator {width: 25px; height: 25px; border-radius: 12px;} QRadioButton::indicator:unchecked { background-color: yellow; border: 2px solid gray;}";
}

ShoulderControl::~ShoulderControl()
{
    delete ui;
}

void ShoulderControl::eventHandler(STIM_GUI_TOPIC_T topic, uint8_t reg, uint32_t value)
{
    if (topic==TOPIC_STIMULATOR)
    {
        switch(reg)
        {

        case STIM_REG_NUM_SMART_SENSORS:
           if(value==0){
               QMessageBox::critical(this, "Sensor Not Connected!", "Shoulder sensor got disconnected \n\nReconnect the sensor and press Ok to continue");
           }

            break;
        }
    }
}


void ShoulderControl::sensorFilteredEventHandler(int16_t sensor_role, int16_t filter_output)
{

    static QTime time(QTime::currentTime());
    // calculate two new data points:
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds


    static int16_t filter_outputs[6] = {0,0,0,0,0,0};

        filter_outputs[sensor_role] = filter_output;

        if(sensor_role == 5 && startBtnStatus == true)
        {
            realtimeDataSlot((double)filter_outputs[0]/ACCELEROMETER_1G_COUNT,
                    (double)filter_outputs[1]/ACCELEROMETER_1G_COUNT,
                    (double)filter_outputs[2]/ACCELEROMETER_1G_COUNT,
                    (double)filter_outputs[3]/ACCELEROMETER_1G_COUNT,
                    (double)filter_outputs[4]/ACCELEROMETER_1G_COUNT,
                    (double)filter_outputs[5]/ACCELEROMETER_1G_COUNT);

            threshold_vertical = ui->doubleSpinBox_vertical->value();
            threshold_protraction = ui->doubleSpinBox_protraction->value();
            threshold_retraction = ui->doubleSpinBox_retraction->value();

            spinbox_vertical100 = 420-(threshold_vertical*100);
            spinbox_protraction100 = 420-(threshold_protraction*100);
            spinbox_retraction100 = 420-(threshold_retraction*100);

            double spinValueVer = (ui->doubleSpinBox_vertical->value()*100.0);
            double minPVer = ui->progressBar_vertical->minimum();
            double maxPVer = ui->progressBar_vertical->maximum();
            double percentagePVer = (spinValueVer - minPVer)/(maxPVer - minPVer);
            double yCoorVer = 420 - (420-220)*percentagePVer;

            double spinValueRet = (ui->doubleSpinBox_retraction->value()*100.0);
            double minPRet = ui->progressBar_retraction->minimum();
            double maxPRet = ui->progressBar_retraction->maximum();
            double percentagePRet = (spinValueRet - minPRet)/(maxPRet - minPRet);
            double yCoorRet = 420 - (420-220)*percentagePRet;

            double spinValuePro = (ui->doubleSpinBox_protraction->value()*100.0);
            double minPPro = ui->progressBar_protraction->minimum();
            double maxPPro = ui->progressBar_protraction->maximum();
            double percentagePPro = (spinValuePro - minPPro)/(maxPPro - minPPro);
            double yCoorPro = 420 - (420-220)*percentagePPro;

            ui->label_vertical->setGeometry(110,yCoorVer,61,16);
            ui->label_protraction->setGeometry(270,yCoorPro,61,16);
            ui->label_retraction->setGeometry(430,yCoorRet,61,16);

            ui->label_v->setGeometry(115, yCoorVer-8,61,16);
            ui->label_v->setText(QString::number(threshold_vertical, 'f',2 )+"g");
            ui->label_p->setGeometry(275, yCoorPro-8,61,16);
            ui->label_p->setText(QString::number(threshold_protraction, 'f',2 )+"g");
            ui->label_r->setGeometry(435, yCoorRet-8,61,16);
            ui->label_r->setText(QString::number(threshold_retraction, 'f',2 )+"g");
        }
}

void ShoulderControl::realtimeDataSlot(double axS, double ayS, double azS, double aV, double aH, double aP)
{

    static QTime time(QTime::currentTime());
    // calculate two new data points:
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds


    double a_protraction = axS;
    double a_vertical = aV; //aV;
    double a_retraction = axS; //aV;

    ui->progressBar_vertical->setValue(a_vertical*100.0);
    //ui->progressBar_protraction->setValue(a_protraction*100);
   // ui->progressBar_protraction->setValue((a_protraction*0.5)/100.0);
    ui->progressBar_retraction->setValue(a_retraction*100);

    if (axS<0){
        ui->progressBar_retraction->setValue(qAbs(axS)*100);
        ui->progressBar_protraction->setValue(0);
    }
    else{
        ui->progressBar_retraction->setValue(0);
        ui->progressBar_protraction->setValue(axS*100);

    }

    if(a_vertical>0){

       ui->progressBar_vertical->setValue(a_vertical*100);
    }
    else{
       ui->progressBar_vertical->setValue(0);
    }



   // ui->label->setText(QString::number(sampThres,'g',4));

//    QString StyleSheetOn1("QRadioButton::indicator {width: 25px; height: 25px; border-radius: 12px;} QRadioButton::indicator:unchecked { background-color: lime; border: 2px solid gray;}");
//    QString StyleSheetOff1("QRadioButton::indicator {width: 25px; height: 25px; border-radius: 12px;} QRadioButton::indicator:unchecked { background-color: red; border: 2px solid gray;}");
//    QString StyleSheetOn2("QRadioButton::indicator {width: 25px; height: 25px; border-radius: 12px;} QRadioButton::indicator:unchecked { background-color: lime; border: 2px solid gray;}");
//    QString StyleSheetOff2("QRadioButton::indicator {width: 25px; height: 25px; border-radius: 12px;} QRadioButton::indicator:unchecked { background-color: red; border: 2px solid gray;}");

    static double lastVTwitchKey;
    static double lastPTwitchKey;
    static double lastRTwitchKey;

    double percentageValue = 5.0/100.0; // 5%
    double onVThreshold =   threshold_vertical ;
    double offVThreshold = onVThreshold - (onVThreshold*percentageValue);
    double onPThreshold = threshold_protraction;
    double offPThreshold = onPThreshold - (onPThreshold*percentageValue);
    double onRThreshold = threshold_retraction;
    double offRThreshold = onRThreshold - (onRThreshold*percentageValue);

    static int countCrossings = 0;

    ui->textBrowser->setText(
                 "Ver coor: " + QString::number( spinbox_vertical100) + "\n" +
                 "Pro coor: " + QString::number(spinbox_protraction100) + "\n" +
                 "Ret coor): " + QString::number(spinbox_retraction100) + "\n"
                 "labelY: " + QString::number(qRound(threshold_vertical/ ui->progressBar_vertical->maximum() * ui->progressBar_vertical->height())) + "\n" +
                 "Pro bar max: " + QString::number(ui->progressBar_vertical->maximum()) +  "\n" +
                 "Pro bar height: " + QString::number(ui->progressBar_vertical->height()) );



    // if(a_vertical> onVThreshold && onVThreshold > 0.15 && key-lastVTwitchKey >1 )
     if(a_vertical> onVThreshold && onVThreshold > 0.04  && !twitchtimer.isActive())
       {
         //QThread::msleep(1000);
           emit startTimer();
           emit startTwoTwitchTimer();
           countCrossings++;
           ui->rdo_btn_vertical->show();
           ui->rdo_btn_vertical->setStyleSheet(StyleSheetOn1);
           if(!soundBtnStatus){
           QSound::play(":/resources/beep1.wav");
           }
           //lastVTwitchKey = key;

       }
       else
       {
           ui->rdo_btn_vertical->setStyleSheet(StyleSheetOff);
       }

     if(!twoTwitchtimer.isActive()){
         if(countCrossings >= 2){
             ui->rdo_btn_vertical->setStyleSheet(StyleSheetTwoTwitchOn);
             if(!soundBtnStatus){
             QSound::play(":/resources/beep4.wav");
             }
         }
         countCrossings = 0;

     }
     else
     {
         qDebug()<<"No of Twitch: "+QString::number(countCrossings);

     }

       //if(a_horizontal > onPThreshold &&  onPThreshold > 0.15 && key-lastPTwitchKey >1)
       if(a_protraction > onPThreshold &&  onPThreshold > 0.08 && !twitchtimer.isActive())
       {


           emit startTimer();

           ui->rdo_btn_protraction->show();
           ui->rdo_btn_protraction->setStyleSheet(StyleSheetOn2);
           if(!soundBtnStatus){
           QSound::play(":/resources/beep2.wav");
           }
           lastPTwitchKey = key;

       }
       else
       {
           ui->rdo_btn_protraction->setStyleSheet(StyleSheetOff);
       }

     //if(a_retraction < (-1)*onRThreshold &&  onRThreshold > 0.011 && key-lastRTwitchKey >1)
       if(a_retraction < (-1)*onRThreshold &&  onRThreshold > 0.08 && !twitchtimer.isActive())
       {
           //QThread::msleep(1000);
           emit startTimer();
           ui->rdo_btn_retraction->show();
           ui->rdo_btn_retraction->setStyleSheet(StyleSheetOn2);
           if(!soundBtnStatus){
           QSound::play(":/resources/beep3.wav");}
           lastRTwitchKey = key;

       }
       else
       {
           ui->rdo_btn_retraction->setStyleSheet(StyleSheetOff);
       }

//  ui->label_time->setText("Time: "+QString::number(key, 'f', 2));

}

void ShoulderControl::startTimer()
{
    twitchtimer.setSingleShot(true);
    twitchtimer.start(1000);

    qDebug()<<"2 Second Timer called";
}

void ShoulderControl::startTwoTwitchTimer()
{
    doubleTimer = ui->spinBox_twotwitch->value();
    twoTwitchtimer.setSingleShot(true);
    //doubleTwitchtimer.start(2000);
    twoTwitchtimer.start(doubleTimer);

    //qDebug()<<"2 Second Timer called";
}

void ShoulderControl::noSoundBtn()
{
    if(!soundBtnStatus)
    {
        //ui->pushButton_nosound -> setStyleSheet("background-color:green;");
        ui->pushButton_nosound -> setIcon(QIcon(":/resources/nosound.png"));
        ui->pushButton_nosound -> setIconSize(QSize(25, 25));
    }
    else
    {
       // ui->pushButton_nosound ->setStyleSheet("background-color:red;");
        ui->pushButton_nosound -> setIcon(QIcon(":/resources/sound.png"));
        ui->pushButton_nosound -> setIconSize(QSize(18, 18));
    }

    soundBtnStatus = !soundBtnStatus;
}



void ShoulderControl::on_pushButton_back_clicked()
{
//    stageProgram *back;
//    this->close();
//    back = new stageProgram(pLabel,this);
//    back -> show();

    StageOneMain *back;
    this->close();
    back = new StageOneMain(pLabel,nullptr);
    back->setAttribute(Qt::WA_DeleteOnClose);
    back -> show();
}

void ShoulderControl::biquadratic_filter_coefficient(double Q, double dB, double FS, double FC, int type, float *coeff)
{
    double n0, n1, n2, d1, d2;
    double b0, b1, b2, a0, a1, a2;
    double w0, cosW, sinW, A, alpha, beta;

        // intermediate parameters
        w0 = 2*M_PI*(FC/FS);
            cosW = cos(w0);
            sinW = sin(w0);
        A = pow(10, (dB/40));
        //printf("A = %lf, dB/40 = %lf\n", A, dB/40);
        alpha = sinW/(2*Q);
        beta = pow(A, 0.5)/Q;

        switch (type) {
        case 0: //Low Pass
            b0 = (1 - cosW)/2;
            b1 = 1 - cosW;
            b2 = (1 - cosW)/2;
            a0 = 1 + alpha;
            a1 = -2*cosW;
            a2 = 1 - alpha;
            break;
        case 1: //High Pass
            b0 = (1 + cosW)/2;
            b1 = -(1 + cosW);
            b2 = (1 + cosW)/2;
            a0 = (1 + alpha);
            a1 = -2*cosW;
            a2 = 1 - alpha;
            break;
        case 2: //Band Pass Peak gain Q
            b0 = sinW/2;
            b1 = 0;
            b0 = -sinW/2;
            a0 = 1 + alpha;
            a1 = -2*cosW;
            a2 = 1 - alpha;
            break;
        case 3: //Band Pass constant 0dB gain
            b0 = alpha;
            b1 = 0;
            b2 = -alpha;
            a0 = 1 + alpha;
            a1 = -2*cosW;
            a2 = 1 - alpha;
            break;
        case 4: //Notch Filter
            b0 = 1;
            b1 = -2*cosW;
            b2 = 1;
            a0 = 1 + alpha;
            a1 = -2*cosW;
            a2 = 1 - alpha;
            break;
        case 5: //All pass
            b0 = 1 - alpha;
            b1 = -2*cosW;
            b2 = 1 + alpha;
            a0 = 1 + alpha;
            a1 = -2*cosW;
            a2 = 1 - alpha;
            break;
        case 6: // Peaking EQ Filter
            b0 = 1 + (alpha*A);
            b1 = -2*cosW;
            b2 = 1 - (alpha*A);
            a0 = 1 + (alpha/A);
            a1 = -2*cosW;
            a2 = 1 - (alpha/A);
            break;
        case 7: //Low Shelving Filter
            b0 = A*((A + 1) - ((A -1)*cosW) + beta*sinW);
            b1 = 2*A*((A - 1) - ((A + 1)*cosW));
            b2 = A*((A + 1) - (A - 1)*cosW - beta*sinW);
            a0 = ((A + 1) + (A - 1)*cosW + beta*sinW);
            a1 = -2*((A - 1) + (A + 1)*cosW);
            a2 = ((A + 1) + (A - 1)*cosW - (beta*sinW));
            break;
        case 8: //High Shelving Filter
            b0 = A*((A + 1) + (A - 1)*cosW + beta*sinW);
            b1 = -2*A*((A - 1) + (A + 1)*cosW);
            b2 = A*((A + 1) + (A - 1)*cosW -beta*sinW);
            a0 = ((A + 1) - (A - 1)*cosW + beta*sinW);
            a1 = 2*((A - 1) - (A + 1)*cosW);
            a2 = ((A + 1) - (A - 1)*cosW - beta*sinW);
            break;
        default:
            break;

        };

        n0 = b0/a0;
        n1 = b1/a0;
        n2 = b2/a0;
        d1 = a1/a0;
        d2 = a2/a0;

        coeff[0] = (float)n0;
        coeff[1] = (float)n1;
        coeff[2] = (float)n2;
        coeff[3] = (float)d1;
        coeff[4] = (float)d2;
        //0:b2, 1:b1, 2:b0, 3:a1, 4:a2
}



void ShoulderControl::on_pushButton_2_clicked()
{
//    QString configfilename = "config_filterSettiings_"+pLabel;
//    QString xmlName = pLabel;

//    QString xmlReadPath = QCoreApplication::applicationDirPath()+"/data/"+xmlName+".xml";
//   // QString txtWritePath = ":/resources/"+configfilename+".txt";
//    QFile file(xmlReadPath);
//    if(!file.open(QIODevice::ReadOnly  | QIODevice::Text))
//    {

//        QMessageBox::information(this, "Unable to open XML file to read", file.errorString());
//        return;
//    }

//     QDomDocument document;
//     document.setContent(&file);
//     QDomElement root = document.documentElement();


//    file.close();
//    QDomNode FiltNode = root.elementsByTagName("Filter_Coeffs").at(0).firstChild();
//    QDomElement FiltNodeVal = FiltNode.toElement();

//    if (FiltNodeVal.isNull())
//    {
//        QMessageBox::information(this, "TetraGrip", "Goto Settings and tune filter");
//        return;
//    }

//    else
//    {

//    QString txtWritePath = QCoreApplication::applicationDirPath()+"/data/config_file/"+configfilename+".txt";

//    QFile f(txtWritePath);
//    if(!f.open(QFile::ReadOnly))
//         {
//             QMessageBox::information(0, "config file error", f.errorString());
//         }
//    else
//         {
//             QByteArray config = f.readAll();
//             tetra_grip_api::send_long_register(STIM_LONG_REG_STIM_CONFIG_FILE, (size_t)config.length(), (uint8_t*)config.data());


//         }
//    }

//    tetra_grip_api::stimulation_start(true);
}

void ShoulderControl::saveToXMLFile()
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

    QDomElement newFiltTag = document.createElement(QString("Filter_Coeffs"));

    QDomNode FiltNode = root.elementsByTagName("Filter_Coeffs").at(0).firstChild();
    QDomElement FiltNodeVal = FiltNode.toElement();

    if (FiltNodeVal.isNull())
    {
        QDomElement a1Tag = document.createElement(QString("a1"));
        QDomText a1Val = document.createTextNode(QString::number(coeff[3]));
        a1Tag.appendChild(a1Val);
        newFiltTag.appendChild(a1Tag);

        QDomElement a2Tag = document.createElement(QString("a2"));
        QDomText a2Val = document.createTextNode(QString::number(coeff[4]));
        a2Tag.appendChild(a2Val);
        newFiltTag.appendChild(a2Tag);

        QDomElement b0Tag = document.createElement(QString("b0"));
        QDomText b0Val = document.createTextNode(QString::number(coeff[0]));
        b0Tag.appendChild(b0Val);
        newFiltTag.appendChild(b0Tag);

        QDomElement b1Tag = document.createElement(QString("b1"));
        QDomText b1Val = document.createTextNode(QString::number(coeff[1]));
        b1Tag.appendChild(b1Val);
        newFiltTag.appendChild(b1Tag);

        QDomElement b2Tag = document.createElement(QString("b2"));
        QDomText b2Val = document.createTextNode(QString::number(coeff[2]));
        b2Tag.appendChild(b2Val);
        newFiltTag.appendChild(b2Tag);

        root.appendChild(newFiltTag);
    }

    else
    {
          QDomElement root = document.documentElement();
          QDomNode SettingsNode = root.namedItem("Filter_Coeffs");

          QDomNode a1 = SettingsNode.namedItem("a1");
          a1.firstChild().setNodeValue(QString::number(coeff[3]));
          QDomNode a2 = SettingsNode.namedItem("a2");
          a2.firstChild().setNodeValue(QString::number(coeff[4]));
          QDomNode b0 = SettingsNode.namedItem("b0");
          b0.firstChild().setNodeValue(QString::number(coeff[0]));
          QDomNode b1 = SettingsNode.namedItem("b1");
          b1.firstChild().setNodeValue(QString::number(coeff[1]));
          QDomNode b2 = SettingsNode.namedItem("b2");
          b2.firstChild().setNodeValue(QString::number(coeff[2]));

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


void ShoulderControl::on_pushButton_4_clicked()
{

    QString configfilename = "config_filterSettiings_"+pLabel;
    QString xmlName = pLabel;

    QString xmlReadPath = QCoreApplication::applicationDirPath()+"/data/"+xmlName+".xml";
   // QString txtWritePath = ":/resources/"+configfilename+".txt";
    QFile file(xmlReadPath);
    if(!file.open(QIODevice::ReadOnly  | QIODevice::Text))
    {

        QMessageBox::information(this, "Unable to open XML file to read", file.errorString());
        return;
    }

     QDomDocument document;
     document.setContent(&file);
     QDomElement root = document.documentElement();


    file.close();
    QDomNode FiltNode = root.elementsByTagName("Filter_Coeffs").at(0).firstChild();
    QDomElement FiltNodeVal = FiltNode.toElement();

    if (FiltNodeVal.isNull())
    {
        QMessageBox::information(this, "TetraGrip", "Goto Settings and tune filter");
        return;
    }

    else
    {

    QString txtWritePath = QCoreApplication::applicationDirPath()+"/data/config_file/"+configfilename+".txt";

    QFile f(txtWritePath);
    if(!f.open(QFile::ReadOnly))
         {
             QMessageBox::information(0, "config file error", f.errorString());
         }
    else
         {
             QByteArray config = f.readAll();
             tetra_grip_api::send_long_register(STIM_LONG_REG_STIM_CONFIG_FILE, (size_t)config.length(), (uint8_t*)config.data());


         }
    }

    tetra_grip_api::stimulation_start(true);











    startBtnStatus = true;
}



void ShoulderControl::on_pushButton_settings_clicked()
{
    FilterSettings *filterw;
    //this->hide();
    filterw = new FilterSettings(pLabel, nullptr);
    filterw->setAttribute(Qt::WA_DeleteOnClose);
    filterw -> show();
}



void ShoulderControl::on_pushButton_3_clicked()
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

    QDomElement newSensorTag = document.createElement(QString("Sensor_Settings"));

    QDomNode SensorNode = root.elementsByTagName("Sensor_Settings").at(0).firstChild();
    QDomElement SensorNodeVal = SensorNode.toElement();

    if (SensorNodeVal.isNull())
    {
        QDomElement verTag = document.createElement(QString("vertical_threshold"));
        QDomText verVal = document.createTextNode(QString::number((int)(threshold_vertical*ACCELEROMETER_1G_COUNT)));
        verTag.appendChild(verVal);
        newSensorTag.appendChild(verTag);

        QDomElement proTag = document.createElement(QString("protraction_threshold"));
        QDomText proVal = document.createTextNode(QString::number((int)(threshold_protraction*ACCELEROMETER_1G_COUNT)));
        proTag.appendChild(proVal);
        newSensorTag.appendChild(proTag);

        QDomElement retTag = document.createElement(QString("retraction_treshold"));
        QDomText retVal = document.createTextNode(QString::number((int)(-1*threshold_retraction*ACCELEROMETER_1G_COUNT)));
        retTag.appendChild(retVal);
        newSensorTag.appendChild(retTag);

        QDomElement timeTag = document.createElement(QString("two_twith_time"));
        QDomText timeVal = document.createTextNode(QString::number( ui->spinBox_twotwitch->value() ));
        timeTag.appendChild(timeVal);
        newSensorTag.appendChild(timeTag);


        root.appendChild(newSensorTag);
    }

    else
    {
          QDomElement root = document.documentElement();
          QDomNode SettingsNode = root.namedItem("Sensor_Settings");

          QDomNode a1 = SettingsNode.namedItem("vertical_threshold");
          a1.firstChild().setNodeValue(QString::number((int)(threshold_vertical*ACCELEROMETER_1G_COUNT)));
          QDomNode a2 = SettingsNode.namedItem("protraction_threshold");
          a2.firstChild().setNodeValue(QString::number((int)(threshold_protraction*ACCELEROMETER_1G_COUNT)));
          QDomNode b0 = SettingsNode.namedItem("retraction_treshold");
          b0.firstChild().setNodeValue(QString::number((int)(-1*threshold_retraction*ACCELEROMETER_1G_COUNT)));
          QDomNode b1 = SettingsNode.namedItem("two_twith_time");
          b1.firstChild().setNodeValue(QString::number( ui->spinBox_twotwitch->value() ));


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
