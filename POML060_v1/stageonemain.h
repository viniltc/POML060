#ifndef STAGEONEMAIN_H
#define STAGEONEMAIN_H

#include <QMainWindow>
#include "stagetwopatients.h"
#include "stageprogram.h"
#include <QtSerialPort/QSerialPort>
#include "tetra_grip_api.h"




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

size_t  send_using_qtserial(uint8_t *data, size_t len);  // global definition to access from "stim_gui_protocol_tx"

private slots:


    void on_pushButton_exit_clicked();
    void on_pushButton_patients_clicked();
    void on_pushButton_programs_clicked();

private:
    Ui::StageOneMain *ui;
};
#endif // STAGEONEMAIN_H
