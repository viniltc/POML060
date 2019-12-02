#ifndef STAGETWOPATIENTS_H
#define STAGETWOPATIENTS_H

#include <QMainWindow>
#include "tetra_grip_api.h"


namespace Ui {
class StageTwoPatients;
}

class StageTwoPatients : public QMainWindow
{
    Q_OBJECT

public:
    explicit StageTwoPatients(QWidget *parent = nullptr);
    ~StageTwoPatients();
    void parseDataEntry(const QString dataPath);

public slots:

    void eventHandlerTwo(STIM_GUI_TOPIC_T topic, uint8_t reg, uint32_t value);

    void on_pushButton_bmain_clicked();

    void on_pushButton_Open_clicked();

    void on_pushButton_New_clicked();

    void on_pushButton_Modify_clicked();

    void on_pushButton_Remove_clicked();



private:
    Ui::StageTwoPatients *ui;

};

#endif // STAGETWOPATIENTS_H
