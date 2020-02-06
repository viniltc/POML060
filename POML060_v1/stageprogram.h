#ifndef STAGEPROGRAM_H
#define STAGEPROGRAM_H

#include <QMainWindow>
#include "tetra_grip_api.h"
#include <QDebug>
#include "programkeygripv2.h"
#include "programpalmergrasp.h"
#include "programopenhand.h"
#include "programswitchgrasp.h"

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
     QString pLabel;


public slots:

//    void setCurrOnChannelOne(QString);
//    void setCurrOnChannelTwo(QString);
//    void setCurrOnChannelThree(QString);
//    void setCurrOnChannelFour(QString);
//    void setCurrOnChannelFive(QString);
    void setCurrOnChannelOne(unsigned int current_uA);
    void setCurrOnChannelTwo(unsigned int current_uA);
    void setCurrOnChannelThree(unsigned int current_uA);
    void setCurrOnChannelFour(unsigned int current_uA);
    void setCurrOnChannelFive(unsigned int current_uA);
    void stimStatusEventHandler(STIM_GUI_TOPIC_T topic,uint8_t index, uint8_t reg, uint32_t value);
   // void stimTargetPulseWidthEventHandler(STIM_GUI_TOPIC_T topic, uint8_t reg, uint32_t value);


signals:

     void targetCurrentValue(QString);


private slots:
    void on_pushButton_stimStart_clicked();

    void on_pushButton_stimStop_clicked();


    void on_pushButton_programKeyGrip_clicked();

    void on_pushButton_programPalmerGrasp_clicked();

    void on_pushButton_programOpenHand_clicked();

    void on_pushButton_programSwitchGrasp_clicked();

private:
    Ui::stageProgram *ui;
    unsigned int currentOneSetVal,currentTwoSetVal, currentThreeSetVal,currentFourSetVal, currentFiveSetVal;
    float m_currentmA = 100.0;
    uint8_t m_channelOne= 0;
    uint8_t m_channelTwo= 1;
    uint8_t m_channelThree= 2;
    uint8_t m_channelFour= 3;
    uint8_t m_channelFive= 4;
};

#endif // STAGEPROGRAM_H
