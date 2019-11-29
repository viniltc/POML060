#ifndef STAGETWOPATIENTS_H
#define STAGETWOPATIENTS_H

#include <QMainWindow>


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

    void on_pushButton_Open_clicked();

    void on_pushButton_New_clicked();

    void on_pushButton_Modify_clicked();

    void on_pushButton_Remove_clicked();

private:
    Ui::StageTwoPatients *ui;
    void parseDataEntry(const QString dataPath);
};

#endif // STAGETWOPATIENTS_H
