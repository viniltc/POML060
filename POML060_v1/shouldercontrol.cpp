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

ShoulderControl::ShoulderControl(QString patientLabel,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShoulderControl)
{
    ui->setupUi(this);
    pLabel = patientLabel;
    //connect(&api, &tetra_grip_api::tetraGripEvent,this, &ShoulderControl::eventHandler);
    //connect(&api, &tetra_grip_api::tetraGripSensorEvent,this, &ShoulderControl::sensorEventHandler);
    connect(&api, &tetra_grip_api::tetraGripSensorFilteredEvent,this, &ShoulderControl::sensorFilteredEventHandler);
    ui->doubleSpinBox_vertical->setSingleStep(0.05);
    ui->doubleSpinBox_horizontal->setSingleStep(0.05);

    ui->lineEdit_q->setText("0.707"); //Q default value
    ui->lineEdit_db->setText("10"); // Db default value

    //tetra_grip_api::get_battery_percentage();
    tetra_grip_api::set_sensor_data_rate(SENSOR_ADDRESS_BROADCAST, 30);
    ui->progressBar_horizontal->setRange(0, 0.5*100);
    ui->progressBar_vertical->setRange(0, 0.5*100);
}

ShoulderControl::~ShoulderControl()
{
    delete ui;
}

//void ShoulderControl::eventHandler(STIM_GUI_TOPIC_T topic, uint8_t reg, uint32_t value)
//{

//}

//void ShoulderControl::sensorEventHandler(uint8_t index, SENSOR_DATA_T *sample)
//{
//    static QTime time(QTime::currentTime());
//    // calculate two new data points:
//    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds

//    double q0 = sample->quaternion[0];
//    double q1 = sample->quaternion[1];
//    double q2 = sample->quaternion[2];
//    double q3 = sample->quaternion[3];

//    QQuaternion quat; // quaternion value change q0, (-)q1, (-)q2, q3
//    quat.setScalar(q0);
//    quat.setX(-q1);
//    quat.setY(-q2);
//    quat.setZ(q3);

//    QQuaternion qNormalized = quat.normalized();

//    float a0 = sample->acceleration_g[0];
//    double a1 = sample->acceleration_g[1];
//    double a2 = sample->acceleration_g[2];






//     QVector3D acc(a0,a1,a2);
//     QVector3D gg(0,0,1);

//     //QVector3D vpost = qNormalized.rotatedVector(acc);
//     QVector3D res = qNormalized.rotatedVector(acc)-gg;
//    //QVector3D res = acc-gg;

//     QVector3D rotRes = qNormalized.inverted().rotatedVector(res);
//    //qDebug()<< "The index" << res.x();


//    //threshold = ui->doubleSpinBox->value();

//    double a_horizontal = sqrt(rotRes.x()*rotRes.x()+rotRes.z()*rotRes.z());
//    double a_vertical = rotRes.y();

//    ui->progressBar_vertical->setValue(a_vertical*100);
//    ui->progressBar_horizontal->setValue(a_horizontal*100);


//    //twitchDetection
//    static double lastTwitchKey_v;
//   // if(a_vertical> ui->doubleSpinBox_vertical->value() &&  a_vertical > 0 && key-lastTwitchKey_v > 0.3)
//    if(a_vertical> ui->doubleSpinBox_vertical->value() &&  a_vertical > 0)
//      {

//        lastTwitchKey_v = key;
//        ui->rdo_btn_vertical->show();
//        ui->rdo_btn_vertical->setStyleSheet(StyleSheetOn1);


//      }
//    else
//      {
//        //ui->btn_twitch->setStyleSheet("");
//        ui->rdo_btn_vertical->setStyleSheet(StyleSheetOff1);
//      }

//    if(a_horizontal> ui->doubleSpinBox_vertical->value() &&  a_horizontal > 0)
//      {

//        ui->rdo_btn_horizontal->show();
//        ui->rdo_btn_horizontal->setStyleSheet(StyleSheetOn2);

//      }
//    else
//      {
//        //ui->btn_twitch->setStyleSheet("");
//        ui->rdo_btn_horizontal->setStyleSheet(StyleSheetOff2);

//      }

//  ui->label_vtime->setText("V Time: "+QString::number(key-lastTwitchKey_v, 'f', 2));

//}

void ShoulderControl::sensorFilteredEventHandler(int16_t sensor_role, int16_t filter_output)
{

    static QTime time(QTime::currentTime());
    // calculate two new data points:
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds


    static int16_t filter_outputs[6] = {0,0,0,0,0,0};

        filter_outputs[sensor_role] = filter_output;

        if(sensor_role == 5)
        {
            realtimeDataSlot((double)filter_outputs[0]/ACCELEROMETER_1G_COUNT,
                    (double)filter_outputs[1]/ACCELEROMETER_1G_COUNT,
                    (double)filter_outputs[2]/ACCELEROMETER_1G_COUNT,
                    (double)filter_outputs[3]/ACCELEROMETER_1G_COUNT,
                    (double)filter_outputs[4]/ACCELEROMETER_1G_COUNT,
                    (double)filter_outputs[5]/ACCELEROMETER_1G_COUNT);

            //ui->label->setText(QString::number(ui->doubleSpinBox->value(), 'f', 3));
            spinbox_vertical100 = 570-(2.2*ui->doubleSpinBox_vertical->value()*100);
            spinbox_horizontal100 = 570-(2.2*ui->doubleSpinBox_horizontal->value()*100);
            ui->label_vertical->setGeometry(110, spinbox_vertical100,81,16);
            ui->label_horizontal->setGeometry(320,spinbox_horizontal100,81,16);

            //ui->label_vertical->setStyleSheet("{color: #C0BBFE}");
            //ui->label_vertical->setText("<font color='red'>---------</font>");

            ui->label_v->setGeometry(115, spinbox_vertical100-8,81,16);
            ui->label_v->setText(QString::number(ui->doubleSpinBox_vertical->value(), 'f',2 )+"g");
            ui->label_h->setGeometry(325, spinbox_horizontal100-8,81,16);
            ui->label_h->setText(QString::number(ui->doubleSpinBox_horizontal->value(), 'f',2 )+"g");
        }
}

void ShoulderControl::realtimeDataSlot(double axS, double ayS, double azS, double aV, double aH, double aP)
{

    static QTime time(QTime::currentTime());
    // calculate two new data points:
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds

    double a_horizontal = aH;
    double a_vertical = aV;

    ui->progressBar_vertical->setValue(a_vertical*100);
    ui->progressBar_horizontal->setValue(a_horizontal*100);

    QString StyleSheetOn1("QRadioButton::indicator {width: 25px; height: 25px; border-radius: 12px;} QRadioButton::indicator:unchecked { background-color: lime; border: 2px solid gray;}");
    QString StyleSheetOff1("QRadioButton::indicator {width: 25px; height: 25px; border-radius: 12px;} QRadioButton::indicator:unchecked { background-color: red; border: 2px solid gray;}");
    QString StyleSheetOn2("QRadioButton::indicator {width: 25px; height: 25px; border-radius: 12px;} QRadioButton::indicator:unchecked { background-color: lime; border: 2px solid gray;}");
    QString StyleSheetOff2("QRadioButton::indicator {width: 25px; height: 25px; border-radius: 12px;} QRadioButton::indicator:unchecked { background-color: red; border: 2px solid gray;}");

    if(a_vertical> ui->doubleSpinBox_vertical->value() &&  a_vertical > 0)
    {
        ui->rdo_btn_vertical->show();
        ui->rdo_btn_vertical->setStyleSheet(StyleSheetOn1);
    }
    else
    {
        ui->rdo_btn_vertical->setStyleSheet(StyleSheetOff1);
    }

    if(a_horizontal> ui->doubleSpinBox_vertical->value() &&  a_horizontal > 0)
    {
        ui->rdo_btn_horizontal->show();
        ui->rdo_btn_horizontal->setStyleSheet(StyleSheetOn2);
    }
    else
    {
        ui->rdo_btn_horizontal->setStyleSheet(StyleSheetOff2);
    }

  ui->label_time->setText("Time: "+QString::number(key, 'f', 2));

}

void ShoulderControl::on_pushButton_back_clicked()
{
//    stageProgram *back;
//    this->close();
//    back = new stageProgram(pLabel,this);
//    back -> show();

    StageOneMain *back;
    this->close();
    back = new StageOneMain(pLabel,this);
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

void ShoulderControl::on_pushButton_clicked()
{
    if(ui->radioButton->isChecked())
    {
        qDebug()<<"Low pass filter";
        filter_type = 0;
    }
    else if(ui->radioButton_2->isChecked())
    {
        qDebug()<<"High pass filter";
        filter_type = 1;
    }
    else if(ui->radioButton_3->isChecked())
    {
        qDebug()<<"Band Pass Peak gain Q";
        filter_type = 2;
    }
    else if(ui->radioButton_4->isChecked())
    {
        qDebug()<<"Peaking EQ Filter";
        filter_type = 6;
    }
    else if(ui->radioButton_5->isChecked())
    {
        qDebug()<<"Notch filter";
       filter_type = 4;
    }

    FC = ui->lineEdit_fc->text().toDouble();   //Fc
    FS = ui->lineEdit_fs->text().toDouble(); //Fs
    Q = ui->lineEdit_q->text().toDouble(); //Q
    dB = ui->lineEdit_db->text().toDouble(); // Db

    if(ui->lineEdit_fc->text().isEmpty() || ui->lineEdit_fs->text().isEmpty() || ui->lineEdit_q->text().isEmpty() ||ui->lineEdit_db->text().isEmpty())
    {
        QMessageBox::warning(this,"TetraGrip","Warning: Enter valid filter parameters"
                             ,QMessageBox::Ok	,QMessageBox::NoButton);

        return;
    }

    biquadratic_filter_coefficient(Q, dB, FS, FC, filter_type,coeff);
    qDebug()<<coeff[0]<< coeff[1]<<coeff[2]<<coeff[3]<<coeff[4]; //0:b2, 1:b1, 2:b0, 3:a1, 4:a2

//    ui->textBrowser->setText(
//                 "a1: " + QString::number(coeff[3]) + "\n" +
//                 "a2: " + QString::number(coeff[4]) + "\n" +
//                 "b0: " + QString::number(coeff[0]) + "\n" +
//                 "b1: " + QString::number(coeff[1]) + "\n" +
//                 "b2: " + QString::number(coeff[2]));

   ui->pushButton_2->setEnabled(true);



   saveToXMLFile();

   qDebug()<<"Filter Parameters saved to the xml file";

   ManageConfigFile configFile;
   configFile.filterTest(pLabel);

   qDebug()<<"Filter settings config file created";
}

void ShoulderControl::on_pushButton_2_clicked()
{
    QString configfilename = "config_filterSettiings_"+pLabel;
    QString xmlName = pLabel;

    QString xmlReadPath = QCoreApplication::applicationDirPath()+"/data/"+xmlName+".xml";
   // QString txtWritePath = ":/resources/"+configfilename+".txt";
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

    tetra_grip_api::stimulation_start(true);
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

