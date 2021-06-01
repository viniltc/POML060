#ifndef STAGEONEMAIN_H
#define STAGEONEMAIN_H

#include <QMainWindow>
#include "stagetwopatients.h"
#include "stageprogram.h"
#include <QtSerialPort/QSerialPort>
#include "tetra_grip_api.h"
#include <QDebug>
#include "shouldercontrol.h"
#include "stimulatorlogwindow.h"



QT_BEGIN_NAMESPACE
namespace Ui { class StageOneMain; }
QT_END_NAMESPACE

class StageOneMain : public QMainWindow
{
    Q_OBJECT

public:
     StageOneMain(QString,QWidget *parent = nullptr);
    ~StageOneMain();
     StageTwoPatients *stagetwopatients;
     stageProgram *stageprogram;
     ShoulderControl *window;
     StimulatorLogWindow *logwindow;
     QString pLabel;


signals:

    void textToChange(QString);
    void setPushButton(bool);
    void batteryValue(uint32_t);


private slots:

    void on_pushButton_exit_clicked();
    void on_pushButton_patients_clicked();
    void on_pushButton_programs_clicked();
    void on_pushButton_help_clicked();
    void connectionError(bool);

    void on_pushButton_logs_clicked();

public slots:

    void eventHandler(STIM_GUI_TOPIC_T topic,uint8_t index, uint8_t reg, uint32_t value);
    //


private:
    Ui::StageOneMain *ui;
    QLabel *statusBat, *statusConfig;
    void closeEvent (QCloseEvent *event);
};
#endif // STAGEONEMAIN_H
