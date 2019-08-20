#ifndef STAGETWOPATIENTS_H
#define STAGETWOPATIENTS_H

#include <QMainWindow>
#include "stagetwonew.h"


namespace Ui {
class StageTwoPatients;
}

class StageTwoPatients : public QMainWindow
{
    Q_OBJECT

public:
    explicit StageTwoPatients(QWidget *parent = nullptr);
    ~StageTwoPatients();

private slots:
    void on_pushButton_bmain_clicked();
    void on_pushButton_new_clicked();

private:
    Ui::StageTwoPatients *ui;
};

#endif // STAGETWOPATIENTS_H
