#ifndef STAGETWONEW_H
#define STAGETWONEW_H

#include <QMainWindow>


namespace Ui {
class StageTwoNew;
}

class StageTwoNew : public QMainWindow
{
    Q_OBJECT

public:
    explicit StageTwoNew(QWidget *parent = nullptr);
    ~StageTwoNew();

private slots:
    void on_pushButton_2_clicked();

private:
    Ui::StageTwoNew *ui;
};

#endif // STAGETWONEW_H
