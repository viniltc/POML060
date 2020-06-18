#ifndef SHOULDERCONTROL_H
#define SHOULDERCONTROL_H

#include <QWidget>
#include "tetra_grip_api.h"

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

public slots:

 void eventHandler(STIM_GUI_TOPIC_T topic, uint8_t reg, uint32_t value);
 void sensorEventHandler(uint8_t index, SENSOR_DATA_T *sample);

private slots:
 void on_pushButton_back_clicked();

private:
    Ui::ShoulderControl *ui;
    double threshold_horizontal = 0;
    double threshold_vertical = 0;
    double spinbox_vertical100 = 0;
    double spinbox_horizontal100 = 0;
};

#endif // SHOULDERCONTROL_H
