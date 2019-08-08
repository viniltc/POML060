#ifndef STAGEONEMAIN_H
#define STAGEONEMAIN_H

#include <QMainWindow>
#include "stagetwopatients.h"


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


private slots:


    void on_pushButton_exit_clicked();
    void on_pushButton_patients_clicked();

private:
    Ui::StageOneMain *ui;
};
#endif // STAGEONEMAIN_H
