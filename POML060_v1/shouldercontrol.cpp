#include "shouldercontrol.h"
#include "ui_shouldercontrol.h"
#include "stageprogram.h"
#include "stageonemain.h"
#include <QQuaternion>

ShoulderControl::ShoulderControl(QString patientLabel,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShoulderControl)
{
    ui->setupUi(this);
    pLabel = patientLabel;
    connect(&api, &tetra_grip_api::tetraGripEvent,this, &ShoulderControl::eventHandler);
    connect(&api, &tetra_grip_api::tetraGripSensorEvent,this, &ShoulderControl::sensorEventHandler);

    ui->doubleSpinBox_vertical->setSingleStep(0.05);
    ui->doubleSpinBox_horizontal->setSingleStep(0.05);

    //tetra_grip_api::get_battery_percentage();
    tetra_grip_api::set_sensor_data_rate(SENSOR_ADDRESS_BROADCAST, 100);
    ui->progressBar_horizontal->setRange(0, 0.5*100);
    ui->progressBar_vertical->setRange(0, 0.5*100);
}

ShoulderControl::~ShoulderControl()
{
    delete ui;
}

void ShoulderControl::eventHandler(STIM_GUI_TOPIC_T topic, uint8_t reg, uint32_t value)
{

}

void ShoulderControl::sensorEventHandler(uint8_t index, SENSOR_DATA_T *sample)
{
    static QTime time(QTime::currentTime());
    // calculate two new data points:
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds

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

    float a0 = sample->acceleration_g[0];
    double a1 = sample->acceleration_g[1];
    double a2 = sample->acceleration_g[2];

    //ui->label->setText(QString::number(ui->doubleSpinBox->value(), 'f', 3));
    spinbox_vertical100 = 300-(2.2*ui->doubleSpinBox_vertical->value()*100);
    spinbox_horizontal100 = 300-(2.2*ui->doubleSpinBox_horizontal->value()*100);
    ui->label_vertical->setGeometry(100, spinbox_vertical100,81,16);
    ui->label_horizontal->setGeometry(310,spinbox_horizontal100,81,16);
    ui->label_time->setText("Time: "+QString::number(key, 'f', 2));
    //ui->label_vertical->setStyleSheet("{color: #C0BBFE}");
    //ui->label_vertical->setText("<font color='red'>---------</font>");

    ui->label_v->setGeometry(100, spinbox_vertical100-8,81,16);
    ui->label_v->setText(QString::number(ui->doubleSpinBox_vertical->value(), 'f',2 )+"g");
    ui->label_h->setGeometry(310, spinbox_horizontal100-8,81,16);
    ui->label_h->setText(QString::number(ui->doubleSpinBox_horizontal->value(), 'f',2 )+"g");

    QString StyleSheetOn1("QRadioButton::indicator {width: 25px; height: 25px; border-radius: 12px;} QRadioButton::indicator:unchecked { background-color: lime; border: 2px solid gray;}");
    QString StyleSheetOff1("QRadioButton::indicator {width: 25px; height: 25px; border-radius: 12px;} QRadioButton::indicator:unchecked { background-color: red; border: 2px solid gray;}");
    QString StyleSheetOn2("QRadioButton::indicator {width: 25px; height: 25px; border-radius: 12px;} QRadioButton::indicator:unchecked { background-color: lime; border: 2px solid gray;}");
    QString StyleSheetOff2("QRadioButton::indicator {width: 25px; height: 25px; border-radius: 12px;} QRadioButton::indicator:unchecked { background-color: red; border: 2px solid gray;}");




     QVector3D acc(a0,a1,a2);
     QVector3D gg(0,0,1);

     //QVector3D vpost = qNormalized.rotatedVector(acc);
     QVector3D res = qNormalized.rotatedVector(acc)-gg;
    //QVector3D res = acc-gg;

     QVector3D rotRes = qNormalized.inverted().rotatedVector(res);
    //qDebug()<< "The index" << res.x();


    //threshold = ui->doubleSpinBox->value();

    double a_horizontal = sqrt(rotRes.x()*rotRes.x()+rotRes.z()*rotRes.z());
    double a_vertical = rotRes.y();

    ui->progressBar_vertical->setValue(a_vertical*100);
    ui->progressBar_horizontal->setValue(a_horizontal*100);


    //twitchDetection
    static double lastTwitchKey_v;
   // if(a_vertical> ui->doubleSpinBox_vertical->value() &&  a_vertical > 0 && key-lastTwitchKey_v > 0.3)
    if(a_vertical> ui->doubleSpinBox_vertical->value() &&  a_vertical > 0)
      {

        lastTwitchKey_v = key;
        ui->rdo_btn_vertical->show();
        ui->rdo_btn_vertical->setStyleSheet(StyleSheetOn1);


      }
    else
      {
        //ui->btn_twitch->setStyleSheet("");
        ui->rdo_btn_vertical->setStyleSheet(StyleSheetOff1);
      }

    if(a_horizontal> ui->doubleSpinBox_vertical->value() &&  a_horizontal > 0)
      {

        ui->rdo_btn_horizontal->show();
        ui->rdo_btn_horizontal->setStyleSheet(StyleSheetOn2);

      }
    else
      {
        //ui->btn_twitch->setStyleSheet("");
        ui->rdo_btn_horizontal->setStyleSheet(StyleSheetOff2);

      }

  ui->label_vtime->setText("V Time: "+QString::number(key-lastTwitchKey_v, 'f', 2));

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
