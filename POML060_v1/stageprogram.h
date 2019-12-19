#ifndef STAGEPROGRAM_H
#define STAGEPROGRAM_H

#include <QMainWindow>
#include "tetra_grip_api.h"
#include <QDebug>
#include "programkeygrip.h"

namespace Ui {
class stageProgram;
}

class stageProgram : public QMainWindow
{
    Q_OBJECT

public:
    explicit stageProgram(QWidget *parent = nullptr);
    ~stageProgram();
     bool clickedCurrOne = false;
     ProgramKeyGrip *keygrip;


public slots:

    void setCurrOnChannelOne(QString);
    void setCurrOnChannelTwo(QString);
    void setCurrOnChannelThree(QString);
    void setCurrOnChannelFour(QString);
    void setCurrOnChannelFive(QString);
    void stimStatusEventHandler(STIM_GUI_TOPIC_T topic, uint8_t reg, uint32_t value);





private slots:
    void on_pushButton_stimStart_clicked();

    void on_pushButton_stimStop_clicked();


    void on_pushButton_programKeyGrip_clicked();

private:
    Ui::stageProgram *ui;
};

#endif // STAGEPROGRAM_H
