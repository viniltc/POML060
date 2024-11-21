#ifndef STAGEPROGRAM_H
#define STAGEPROGRAM_H

#include <QMainWindow>
#include "tetra_grip_api.h"
#include <QDebug>
#include <QCloseEvent>
#include "programkeygripv2.h"
#include "programpalmergrasp.h"
#include "programopenhand.h"
#include "programswitchgrasp.h"
#include "qcustomplot.h"
#include "stopwatch.h"
#include "shouldercontrol.h"
#include "statetestwindow.h"
#include "stroketrainingwindow.h"
#include "stroketrainingwindowtwo.h"
#include "stroketrainingwindowthree.h"
#include "stroketrainingwindowfour.h"
#include "stroketrainingwindowfive.h"
#include "stroketrainingwindowsix.h"



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
     StateTestWindow *statewindow;
     StrokeTrainingWindow *strokewindow;
     StrokeTrainingWindowTwo *strokewindowtwo;
     StrokeTrainingWindowThree *strokewindowthree;
     StrokeTrainingWindowFour *strokewindowfour;
     StrokeTrainingWindowFive *strokewindowfive;
     StrokeTrainingWindowSix *strokewindowsix;
     //StageOneMain *backToMain;
     QString pLabel;
      void onAdvancedSettingsToggledCh1(bool checked);
      void onAdvancedSettingsToggledCh2(bool checked);
      void onAdvancedSettingsToggledCh3(bool checked);
      void onAdvancedSettingsToggledCh4(bool checked);
      void onAdvancedSettingsToggledCh5(bool checked);
 //

public slots:


    void setCurrOnChannelOne(unsigned int current_uA);
    void setCurrOnChannelTwo(unsigned int current_uA);
    void setCurrOnChannelThree(unsigned int current_uA);
    void setCurrOnChannelFour(unsigned int current_uA);
    void setCurrOnChannelFive(unsigned int current_uA);
    void setZeroCurrOnChannelOne();
    void setZeroCurrOnChannelTwo();
    void setZeroCurrOnChannelThree();
    void setZeroCurrOnChannelFour();
    void setZeroCurrOnChannelFive();
    void stimStatusEventHandler(STIM_GUI_TOPIC_T topic,uint8_t index, uint8_t reg, uint32_t value);
    void onTimeout();
    void startStopTimer();
    void resetTimer();

   // void closeEvent (QCloseEvent *event);

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
    void on_tabWidget_currentChanged(int index);
    void on_tabWidget_tabCloseRequested(int index);

    void on_pushButton_shoulderControl_clicked();

    void on_pushButton_5_clicked();



    void on_radioButton_3_clicked();

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();



    void on_comboBox_frequency_1_currentIndexChanged(int index);

    void on_comboBox_frequency_2_currentIndexChanged(int index);

    void on_comboBox_frequency_3_currentIndexChanged(int index);

    void on_comboBox_frequency_4_currentIndexChanged(int index);

    void on_comboBox_frequency_5_currentIndexChanged(int index);

    void on_comboBox_waveform_1_currentIndexChanged(int index);

    void on_comboBox_waveform_2_currentIndexChanged(int index);

    void on_comboBox_waveform_3_currentIndexChanged(int index);

    void on_comboBox_waveform_4_currentIndexChanged(int index);

    void on_comboBox_waveform_5_currentIndexChanged(int index);

    void on_pushButton_stimSave_2_clicked();

    void on_pushButton_programStroke2_clicked();

    void on_pushButton_programStroke1_clicked();

    void on_pushButton_programStroke_3_clicked();

    void on_pushButton_programStroke4_clicked();

    void on_pushButton_programStroke5_clicked();

    void on_pushButton_IMUcontrol_clicked();

    void on_pushButton_programStroke6_clicked();

    //void on_pushButton_currOnOne_clicked();

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
    int m_freqDiv = 10; // to get actual frequency of stimulation


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

    float currOneStored = 0;
    float currTwoStored = 0;
    float currThreeStored = 0;
    float currFourStored = 0;
    float currFiveStored = 0;
    bool timerLabel = false;

    unsigned int freqStim = 0;
    unsigned int oneFreqStim = 0;
    unsigned int twoFreqStim = 0;
    unsigned int threeFreqStim = 0;
    unsigned int fourFreqStim = 0;
    unsigned int fiveFreqStim = 0;

     int oneFreqIndex = 0;
     int twoFreqIndex = 0;
     int threeFreqIndex = 0;
     int fourFreqIndex = 0;
     int fiveFreqIndex = 0;

     uint8_t oneWaveStim = 0;
     uint8_t twoWaveStim = 0;
     uint8_t threeWaveStim = 0;
     uint8_t fourWaveStim = 0;
     uint8_t fiveWaveStim = 0;

      int oneWaveIndex = 0;
      int twoWaveIndex = 0;
      int threeWaveIndex = 0;
      int fourWaveIndex = 0;
      int fiveWaveIndex = 0;

      int oneMusIndex = 0;
      int twoMusIndex = 0;
      int threeMusIndex = 0;
      int fourMusIndex = 0;
      int fiveMusIndex = 0;

    QString StyleSheetOn;
    QString StyleSheetOff;

    QString config_file_name;

    Stopwatch* watch;
    QSize originalSize;
    QMap<QWidget*, QRect> originalGeometries;
    QPixmap originalPixmap;
    QMap<QWidget*, int> originalFontSizes;
    void scaleWidgets(QWidget *parent, double scaleFactorWidth, double scaleFactorHeight);

protected:
     void closeEvent (QCloseEvent *event) override;
     void resizeEvent(QResizeEvent* event) override;
};

#endif // STAGEPROGRAM_H
