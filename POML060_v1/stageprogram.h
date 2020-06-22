#ifndef STAGEPROGRAM_H
#define STAGEPROGRAM_H

#include <QMainWindow>
#include "tetra_grip_api.h"
#include <QDebug>
#include "programkeygripv2.h"
#include "programpalmergrasp.h"
#include "programopenhand.h"
#include "programswitchgrasp.h"
#include "qcustomplot.h"
#include "stopwatch.h"
#include "shouldercontrol.h"


namespace Ui {
class stageProgram;
}

class stageProgram : public QMainWindow
{
    Q_OBJECT

public:
    explicit stageProgram(QString,QWidget *parent = nullptr);
    ~stageProgram();
     bool clickedCurrOne = false;
    // ProgramKeyGrip *keygrip;
     ProgramKeyGripV2 *keygripv2;
     ProgramPalmerGrasp *palmergrasp;
     ProgramOpenHand *openhand;
     ProgramSwitchGrasp *switchgrasp;
     ShoulderControl *window;
     //StageOneMain *backToMain;
     QString pLabel;


public slots:


    void setCurrOnChannelOne(unsigned int current_uA);
    void setCurrOnChannelTwo(unsigned int current_uA);
    void setCurrOnChannelThree(unsigned int current_uA);
    void setCurrOnChannelFour(unsigned int current_uA);
    void setCurrOnChannelFive(unsigned int current_uA);
    void stimStatusEventHandler(STIM_GUI_TOPIC_T topic,uint8_t index, uint8_t reg, uint32_t value);
    void sensorEventHandler(uint8_t index, SENSOR_DATA_T *sample);
    void realtimeDataSlot(double x_acceleration_g, double y_acceleration_g, double z_acceleration_g, double threshold);
    void onTimeout();
    void startStopTimer();
    void resetTimer();
    void setZeroCurrOnChannelOne();

signals:

     void targetCurrentValue(QString);


private slots:
    void on_pushButton_stimStart_clicked();
    void on_pushButton_stimStop_clicked();
    void on_pushButton_programKeyGrip_clicked();
    void on_pushButton_programPalmerGrasp_clicked();
    void on_pushButton_programOpenHand_clicked();
    void on_pushButton_programSwitchGrasp_clicked();
    void on_pushButton_stimSave_clicked();
    void saveToXMLFile();
    void on_pushButton_setThreshold_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_tabWidget_tabCloseRequested(int index);

    void on_pushButton_shoulderControl_clicked();

private:
    Ui::stageProgram *ui;
    float currentOneSetVal;
    float currentTwoSetVal;
    float currentThreeSetVal;
    float currentFourSetVal;
    float currentFiveSetVal;

    float m_currentmA = 100.0;
    float m_currentDiv = 1000.0;
    int zeroCurrent = 0;
    int smallCurrent = 7000; // 7mA -> minimum set to stim engine

    uint8_t m_channelOne = 0;
    uint8_t m_channelTwo = 1;
    uint8_t m_channelThree = 2;
    uint8_t m_channelFour = 3;
    uint8_t m_channelFive = 4;

    QTimer dataTimer;
    QCPItemTracer *itemDemoPhaseTracer;

    int pCount=0;

    double accThreshold = 0;
    bool setThreshold = false;
    bool saveClicked = false;

    float currOneStored;
    float currTwoStored;
    float currThreeStored;
    float currFourStored;
    float currFiveStored;
    bool timerLabel = false;

    Stopwatch* watch;
};

#endif // STAGEPROGRAM_H
