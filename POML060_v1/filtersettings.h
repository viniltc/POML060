#ifndef FILTERSETTINGS_H
#define FILTERSETTINGS_H

#include <QWidget>
#include <QtMath> // for PI
#include "shouldercontrol.h"
#include "manageconfigfile.h"
//#include "stageonemain.h"

namespace Ui {
class FilterSettings;
}

class FilterSettings : public QWidget
{
    Q_OBJECT

public:
    explicit FilterSettings(QString, QWidget *parent = nullptr);
    ~FilterSettings();
    QString pLabel;
    //ShoulderControl *newWindow;
public slots:

 void eventHandler(STIM_GUI_TOPIC_T topic, uint8_t reg, uint32_t value);

private slots:
    void on_pushButton_save_clicked();

    void on_pushButton_tune_clicked();

    void saveToXMLFile();

    void loadNewConfigFile();

private:
    Ui::FilterSettings *ui;
    void biquadratic_filter_coefficient(double Q, double dB, double FS, double FC,
            int type, float *coeff);

    //biquadratic filter parameters below
    float coeff_lpf[5];
    float coeff_hpf[5];
    double Q_lpf = 0;
    double dB_lpf = 0; // peak gain
    double FS = 0; // sample frequency
    double FC_lpf = 0; // cutoff frequency

    double Q_hpf = 0;
    double dB_hpf = 0; // peak gain
    double FC_hpf = 0; // cutoff frequency
    int filter_type = 0; // 0:Low Pass, 1:High Pass, 2:Band Pass Peak gain Q..(check function def)
};

#endif // FILTERSETTINGS_H
