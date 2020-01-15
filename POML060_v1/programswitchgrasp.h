#ifndef PROGRAMSWITCHGRASP_H
#define PROGRAMSWITCHGRASP_H

#include <QMainWindow>

namespace Ui {
class ProgramSwitchGrasp;
}

class ProgramSwitchGrasp : public QMainWindow
{
    Q_OBJECT

public:
    explicit ProgramSwitchGrasp(QWidget *parent = nullptr);
    ~ProgramSwitchGrasp();

private slots:
    void on_pushButton_back_keypro_clicked();

private:
    Ui::ProgramSwitchGrasp *ui;
};

#endif // PROGRAMSWITCHGRASP_H
