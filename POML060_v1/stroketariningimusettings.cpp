#include "stroketariningimusettings.h"
#include "ui_stroketariningimusettings.h"

StrokeTariningIMUSettings::StrokeTariningIMUSettings(QString patientLabel, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StrokeTariningIMUSettings)
{
    ui->setupUi(this);
    pLabel = patientLabel;
    connect(&api, &tetra_grip_api::tetraGripSensorFilteredEvent,this, &StrokeTariningIMUSettings::sensorFilteredEventHandler);
    ui->doubleSpinBox_vertical->setSingleStep(0.01);
}

StrokeTariningIMUSettings::~StrokeTariningIMUSettings()
{
    delete ui;
}

void StrokeTariningIMUSettings::sensorFilteredEventHandler(int16_t sensor_role, int16_t filter_output)
{
////    static QTime time(QTime::currentTime());
////    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds


//    static int16_t filter_outputs[6] = {0,0,0,0,0,0};

//        filter_outputs[sensor_role] = filter_output;

//        if(sensor_role == 5 && startBtnStatus == true)
//        {
//            realtimeDataSlot((double)filter_outputs[0]/ACCELEROMETER_1G_COUNT,
//                    (double)filter_outputs[1]/ACCELEROMETER_1G_COUNT,
//                    (double)filter_outputs[2]/ACCELEROMETER_1G_COUNT,
//                    (double)filter_outputs[3]/ACCELEROMETER_1G_COUNT,
//                    (double)filter_outputs[4]/ACCELEROMETER_1G_COUNT,
//                    (double)filter_outputs[5]/ACCELEROMETER_1G_COUNT);

//            threshold_vertical = ui->doubleSpinBox_vertical->value();


//            spinbox_vertical100 = 420-(threshold_vertical*100);

//            double spinValueVer = (ui->doubleSpinBox_vertical->value()*100.0);
//            double minPVer = ui->progressBar_vertical->minimum();
//            double maxPVer = ui->progressBar_vertical->maximum();
//            double percentagePVer = (spinValueVer - minPVer)/(maxPVer - minPVer);
//            double yCoorVer = 420 - (420-220)*percentagePVer;
//            ui->label_vertical->setGeometry(110,yCoorVer,61,16);
//            ui->label_v->setGeometry(115, yCoorVer-8,61,16);
//            ui->label_v->setText(QString::number(threshold_vertical, 'f',2 )+"g");
//        }

}
