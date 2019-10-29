#ifndef STAGEONEMAIN_H
#define STAGEONEMAIN_H

#include <QMainWindow>
#include "stagetwopatients.h"
#include "stageprogram.h"
#include <QtSerialPort/QSerialPort>
#include "tetra_grip_api.h"
#include <QDebug>




QT_BEGIN_NAMESPACE
namespace Ui { class StageOneMain; }
QT_END_NAMESPACE

class StageOneMain : public QMainWindow
{
    Q_OBJECT

public:
     StageOneMain(QWidget *parent = nullptr);
    ~StageOneMain();
     StageTwoPatients *stagetwopatients;
     stageProgram *stageprogram;


private slots:


    void on_pushButton_exit_clicked();
    void on_pushButton_patients_clicked();
    void on_pushButton_programs_clicked();
    void eventHandler(STIM_GUI_TOPIC_T topic, uint8_t reg, uint32_t value);

private:
    Ui::StageOneMain *ui;
    QLabel *statusBat, *statusConfig;
};
#endif // STAGEONEMAIN_H
