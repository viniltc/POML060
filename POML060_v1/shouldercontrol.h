#ifndef SHOULDERCONTROL_H
#define SHOULDERCONTROL_H

#include <QWidget>
#include "tetra_grip_api.h"
#include "filtersettings.h"

namespace Ui {
class ShoulderControl;
}

class ShoulderControl : public QWidget
{
    Q_OBJECT

public:
    explicit ShoulderControl(QString,QWidget *parent = nullptr);
    ~ShoulderControl();
     QString pLabel;
    // FilterSettings *window;

public slots:

// void eventHandler(STIM_GUI_TOPIC_T topic, uint8_t reg, uint32_t value);
// void sensorEventHandler(uint8_t index, SENSOR_DATA_T *sample);
 void sensorFilteredEventHandler(int16_t sensor_role, int16_t filter_output);
 void realtimeDataSlot(double axS, double ayS, double azS, double aV, double aH, double aP);
 void startTimer();
 void startTwoTwitchTimer();
 void noSoundBtn();

private slots:
 void on_pushButton_back_clicked();
 void biquadratic_filter_coefficient(double Q, double dB, double FS, double FC,
         int type, float *coeff);


 void on_pushButton_2_clicked();
 void saveToXMLFile();

 void on_pushButton_4_clicked();



 void on_pushButton_settings_clicked();

private:
    Ui::ShoulderControl *ui;
    double threshold_protraction = 0;
    double threshold_vertical = 0;
    double threshold_retraction = 0;
    double spinbox_vertical100 = 0;
    double spinbox_protraction100 = 0;
    double spinbox_retraction100 = 0;
    const int16_t ACCELEROMETER_1G_COUNT = 16384;
    QTimer twitchtimer;
    QTimer twoTwitchtimer;
    int doubleTimer = 0;

    QString StyleSheetOn1;
    QString StyleSheetOn2;
    QString StyleSheetOff;
    QString StyleSheetTwoTwitchOn;


    bool startBtnStatus = false;
    bool soundBtnStatus = false;


    //biquadratic filter parameters below
    float coeff[5];
    double Q = 0;
    double dB = 0; // peak gain
    double FS = 0; // sample frequency
    double FC = 0; // cutoff frequency
    int filter_type = 0; // 0:Low Pass, 1:High Pass, 2:Band Pass Peak gain Q..(check function def)
};

#endif // SHOULDERCONTROL_H
